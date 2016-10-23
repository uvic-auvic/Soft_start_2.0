/*
 * Si7006.c
 *
 *  Created on: Aug 15, 2016
 *      Author: auvic
 */
#include "Si7006.h"
#include "I2C_controller.h"
#include <string.h>

// Data Variables
float current_temperature;
float current_humidity;

//Management Variables to help with the movement of data
uint8_t MSB;
uint8_t LSB;
bool MSB_written_to;
bool LSB_written_to;

static uint8_t Si7006_ptr;

static void reset_Si7006_managment_vars(){
	MSB = 0;
	LSB = 0;
	MSB_written_to = false;
	LSB_written_to = false;
}

static void append_Si7006_buffer(uint8_t data){
	//If MSB has already been written to then the data should be written to LSB
	if(MSB_written_to){
		LSB = data;
		LSB_written_to = true;
	}else{
		MSB = data;
		MSB_written_to = true;
	}
}

static bool is_Si7006_buffer_full(){
	if(MSB_written_to == false || LSB_written_to == false)
		return false;
	return true;
}

static void do_temp_update(){
	//needs to tested
	int temp = MSB<<8 | LSB;
	current_temperature = TEMP_CONVERSION(temp);
}

static void do_humidity_update(){
	uint16_t humidity = MSB << 8 | LSB;
	current_humidity = HUMIDTY_CONVERSION(humidity);
}

extern void temp_update(uint8_t temp){
	append_Si7006_buffer(temp);
	if(is_Si7006_buffer_full() == true){
		do_temp_update();
	}
}

extern void humidty_update(uint8_t humidity){
	append_Si7006_buffer(humidity);
	if(is_Si7006_buffer_full() == true){
		do_humidity_update();
	}
}

extern void Si700X_set_temp_read_over_I2C(){
	reset_Si7006_managment_vars();
	Si7006_ptr = Si7006_temp_read;
	I2C2_send_message_no_cb(Si7006_ptr, Humidty_sensor, SI7006_TEMP_READ_COMM_BYTES);
}

extern void Si700X_exec_temp_read_over_I2C(){
	reset_Si7006_managment_vars();
	I2C2_recv_message_with_cb(Humidty_sensor, SI7006_TEMP_READ_DATA_BYTES, temp_update);
}

extern void Si700X_set_humidity_read_over_I2C(){
	reset_Si7006_managment_vars();
	Si7006_ptr = SI7006_humidity_read;
	I2C2_send_message_no_cb(Si7006_ptr, Humidty_sensor, SI7006_HUMIDTY_READ_COMM_BYTES);
}

extern void Si700X_exec_humidty_read_over_I2C(){
	reset_Si7006_managment_vars();
	I2C2_recv_message_with_cb(Humidty_sensor, SI7006_HUMIDTY_DATA_COMM_BYTES, humidty_update);
}

extern void init_Si700X(){
	reset_Si7006_managment_vars();
	current_temperature = 0;
	Si7006_ptr = 0;
}

extern float Si700X_get_temp(){
	return current_temperature;
}

extern int Si700X_get_humidty(){
	return (int)current_humidity;
}

extern bool Si7006_check_ready_for(Si7006_tasks task){
	return (Si7006_ptr == task);
}
