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

#define SI7006_TEMP_READ_COMM  		(0xF3)
#define SI7006_TEMP_READ_COMM_BYTES (1)
#define SI7006_TEMP_READ_DATA_BYTES (2)

#define TEMP_CONVERSION(x) ((175.72*(x)/65536)-46.85)

typedef enum{
	Si7006_temp_read
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
extern void set_temp_read();


extern void exec_temp_read();
extern void init_Si7006();
extern float Si7006_get_temp();
extern bool Si7006_in_progress();
extern bool Si7006_is_task_done_and_offer_resources(Si7006_tasks task);

#endif /* SI7006_H_ */
