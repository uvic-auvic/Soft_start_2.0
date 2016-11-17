/*
 * I2C_controller.h
 *
 *  Created on: Aug 11, 2016
 *      Author: auvic
 */

#ifndef I2C_CONTROLLER_H_
#define I2C_CONTROLLER_H_

#include "stm32f0xx.h"

//Public Functions
extern void Configure_GPIO_I2C2(void);
extern void Configure_I2C2_Master(void);
extern void I2C2_send_message_no_cb(uint8_t message, uint8_t address, uint8_t bytes_to_send);
extern void I2C2_recv_message_with_cb(int address, int bytes_to_send, void (*callback_func)(uint8_t));
extern void I2C2_send_message_with_cb(uint8_t address, uint8_t bytes_to_send, uint8_t (*callback_func)(void));
extern void Configure_GPIO_I2C2(void);
extern void Configure_I2C2_Master(void);

typedef enum {
	Humidty_sensor = 0b1000000,
	power_IC_1 = 0b1000101,
	power_IC_2 = 0b1001111
} I2C_address;

#endif /* I2C_CONTROLLER_H_ */
