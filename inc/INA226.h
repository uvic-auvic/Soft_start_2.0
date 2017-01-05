/*
 * INA226.h
 *
 *  Created on: Aug 17, 2016
 *      Author: auvic
 */

#ifndef INA226_H_
#define INA226_H_

#include "stm32f0xx.h"

#define INA226_VOLT_PTR_ADDR  		(0x02)
#define INA226_VOLT_PTR_ADDR_BYTES 	(1)
#define INA226_CURR_PTR_ADDR_BYTES 	(1)
#define INA226_PTR_ADDR_BYTES 		(1)
#define INA226_VOLT_READ_DATA_BYTES (2)
#define INA226_CURR_READ_DATA_BYTES (2)

typedef enum{
	INA226_volt_read = 0x02,
	INA226_curr_read = 0x01
} INA226_tasks;

//Public functions

extern void fet_volt_update(uint8_t temp);
extern void INA226_set_volt_ptr(uint8_t address);
extern void INA226_set_curr_ptr(uint8_t address);
extern void INA226_exec_volt_read(uint8_t address);
extern void init_INA226();
extern float INA226_get_volt();

extern void fet_volt_update(uint8_t volt);
extern void bat_volt_update(uint8_t volt);

#endif /* INA226_H_ */
