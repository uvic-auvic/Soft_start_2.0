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
#define INA226_CURRENT_PTR_ADDR		(0x04)
#define INA226_VOLT_PTR_ADDR_BYTES 	(1)
#define INA226_VOLT_READ_DATA_BYTES (2)

//Public functions

extern void volt_update(uint8_t temp);
extern void set_volt_ptr();
extern void exec_volt_read();
extern void init_INA226();
extern float INA226_get_volt();

#endif /* INA226_H_ */
