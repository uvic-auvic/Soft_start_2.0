/*
 * INA226.c
 *
 *  Created on: Aug 17, 2016
 *      Author: auvic
 */


#include "INA226.h"
#include "I2C_controller.h"
#include <stdbool.h>

#define INT_TO_VOLT_RATIO_USING_FLOAT	(0.00125)

float most_recent_voltage;
float most_recent_curr_1;
float most_recent_curr_2;

uint8_t MSB;
uint8_t LSB;
bool MSB_written_to;
bool LSB_written_to;

static void empty_INA226_buffer(){
	MSB = 0;
	LSB = 0;
	MSB_written_to = false;
	LSB_written_to = false;
}

static void append_INA226_buffer(uint8_t data){
	//If MSB has already been written to then the data should be written to LSB
	if(MSB_written_to){
		LSB = data;
		LSB_written_to = true;
	}else{
		MSB = data;
		MSB_written_to = true;
	}
}

static bool is_INA226_buffer_full(){
	if(MSB_written_to == false || LSB_written_to == false)
		return false;
	return true;
}

static void do_volt_update(){
	int volt = MSB<<8 | LSB;
	most_recent_voltage = (float)volt * INT_TO_VOLT_RATIO_USING_FLOAT;
}

extern void volt_update(uint8_t volt){
	append_INA226_buffer(volt);
	if(is_INA226_buffer_full() == true){
		do_volt_update();
	}
}

extern void set_volt_ptr(){
	empty_INA226_buffer();
	I2C2_send_message_no_cb(INA226_VOLT_PTR_ADDR, Voltage_sensor, INA226_VOLT_PTR_ADDR_BYTES);
}

extern void exec_volt_read(){
	I2C2_recv_message_with_cb(Voltage_sensor, INA226_VOLT_READ_DATA_BYTES, volt_update);
}

extern void init_INA226(){
	empty_INA226_buffer();
	most_recent_voltage = 0;
	most_recent_curr_1 = 0;
	most_recent_curr_2 = 0;
}

extern float INA226_get_volt(){
	return most_recent_voltage;
}
