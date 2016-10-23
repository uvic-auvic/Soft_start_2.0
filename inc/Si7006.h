/*
 * Si7006.h
 *
 *  Created on: Aug 15, 2016
 *      Author: auvic
 */

#ifndef SI7006_H_
#define SI7006_H_

#include "stm32f0xx.h"
#include <stdbool.h>

#define SI7006_TEMP_READ_COMM  				(0xF3)
#define SI7006_TEMP_READ_COMM_BYTES 		(1)
#define SI7006_TEMP_READ_DATA_BYTES 		(2)
#define SI7006_HUMIDTY_READ_COMM_BYTES 		(1)
#define SI7006_HUMIDTY_DATA_COMM_BYTES		(2)

#define TEMP_CONVERSION(x) 		((175.72*(x)/65536)-46.85)
#define HUMIDTY_CONVERSION(x) 	((125*(x)/65536)-6)

typedef enum{
	Si7006_temp_read = 0xF3,
	SI7006_humidity_read = 0xF5
} Si7006_tasks;

//Public functions used by driver

/**
  * @brief  A callback method that is handed to I2C interrupt servicer in order to relay the data back
  * 		to the driver. Once at the driver a buffer is implemented to manage the data
  * @param  temperature is used to pass back one of the two bytes that represents the data received
  * @note   This method is mount to be passed by reference and is not attended to be called by anything
  * 		other than the I2C driver
  * @retval None
  */
extern void temp_update(uint8_t temp);

/**
  * @brief  Responsible for sending over the bytes that are necessary to get the chip to prepare to
  * 		transmit the temperature
  * @note   This function is only used as a convenient way to interact with the I2C driver
  * @retval None
  */

extern void init_Si700X();
extern float Si700X_get_temp();
extern int Si700X_get_humidty();

extern void Si700X_set_temp_read_over_I2C();
extern void Si700X_exec_temp_read_over_I2C();

extern void Si700X_set_humidity_read_over_I2C();
extern void Si700X_exec_humidty_read_over_I2C();

#endif /* SI7006_H_ */
