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

//Management Variables to help with the movement of data
uint8_t MSB;
uint8_t LSB;
bool MSB_written_to;
bool LSB_written_to;

bool Si7006_curr_running_task;
bool Si7006_recently_finish_task;

static void reset_Si7006_managment_vars(){
	MSB = 0;
	LSB = 0;
	MSB_written_to = false;
	LSB_written_to = false;

	Si7006_curr_running_task = false;
	Si7006_recently_finish_task  = false;
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

extern void temp_update(uint8_t temp){
	append_Si7006_buffer(temp);
	if(is_Si7006_buffer_full() == true){
		do_temp_update();
	}
}

extern void set_temp_read(){
	reset_Si7006_managment_vars();
	I2C2_send_message_no_cb(SI7006_TEMP_READ_COMM, Humidty_sensor, SI7006_TEMP_READ_COMM_BYTES);
}

extern void exec_temp_read(){
	I2C2_recv_message_with_cb(Humidty_sensor, SI7006_TEMP_READ_DATA_BYTES, temp_update);
}

extern void init_Si7006(){
	reset_Si7006_managment_vars();
	current_temperature = 0;
}

extern float Si7006_get_temp(){
	return current_temperature;
}

extern bool Si7006_in_progress(){
	return false;
}

extern bool Si7006_is_task_done_and_offer_resources(Si7006_tasks task){
	switch(task){
	case Si7006_temp_read:
		break;
	}
	return false;
}

extern void Si7006_do_temp_read(){

}
