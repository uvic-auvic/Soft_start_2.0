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
#define INT_TO_CURR_RATIO_USING_FLOAT	(0.00833)

float most_recent_voltage_batteries;
float most_recent_voltage_fets;
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

static float do_volt_update(){
	int volt = MSB<<8 | LSB;
	return (float)volt * INT_TO_VOLT_RATIO_USING_FLOAT;
}

static float do_curr_update(){
	uint16_t val = MSB<<8 | LSB;
	return ((val&0x7F) * INT_TO_CURR_RATIO_USING_FLOAT) * ((val&0x80)>>8);
}

extern void fet_volt_update(uint8_t volt){
	append_INA226_buffer(volt);
	if(is_INA226_buffer_full() == true){
		most_recent_voltage_fets = do_volt_update();
	}
}

extern void bat_volt_update(uint8_t volt){
	append_INA226_buffer(volt);
	if(is_INA226_buffer_full() == true){
		most_recent_voltage_batteries = do_volt_update();
	}
}

extern void curr1_update(uint8_t curr){
	append_INA226_buffer(curr);
	if(is_INA226_buffer_full() == true){
		most_recent_curr_1 = do_curr_update();
	}
}

extern void curr2_update(uint8_t curr){
	append_INA226_buffer(curr);
	if(is_INA226_buffer_full() == true){
		most_recent_curr_2 = do_curr_update();
	}
}

extern void INA226_set_volt_ptr(uint8_t address){
	empty_INA226_buffer();
	I2C2_send_message_no_cb(INA226_volt_read, address, INA226_VOLT_PTR_ADDR_BYTES);
}

extern void INA226_set_curr_ptr(uint8_t address){
	empty_INA226_buffer();
	I2C2_send_message_no_cb(INA226_curr_read, address, INA226_CURR_PTR_ADDR_BYTES);
}

extern void INA226_exec_volt_read(uint8_t address){
	if(address == FET_Voltage_sensor)
		I2C2_recv_message_with_cb(FET_Voltage_sensor, INA226_VOLT_READ_DATA_BYTES, fet_volt_update);
	else if(address == Bat_Voltage_sensor)
		I2C2_recv_message_with_cb(Bat_Voltage_sensor, INA226_VOLT_READ_DATA_BYTES, bat_volt_update);
	else{
		while(1);
	}
}

extern void INA226_exec_curr_read(uint8_t address){
	if(address == FET_Voltage_sensor)
		I2C2_recv_message_with_cb(FET_Voltage_sensor, INA226_CURR_READ_DATA_BYTES, curr1_update);
	else if(address == Bat_Voltage_sensor)
		I2C2_recv_message_with_cb(Bat_Voltage_sensor, INA226_CURR_READ_DATA_BYTES, curr2_update);
	else{
		while(1);
	}
}

extern void init_INA226(){
	empty_INA226_buffer();
	most_recent_voltage_batteries = 0;
	most_recent_voltage_fets = 0;
	most_recent_curr_1 = 0;
	most_recent_curr_2 = 0;
}

extern float INA226_get_battery_voltage(){
	return most_recent_voltage_batteries;
}
