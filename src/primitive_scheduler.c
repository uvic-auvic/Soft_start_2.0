/*
 * primitive_scheduler.c
 *
 *  Created on: Aug 19, 2016
 *      Author: auvic
 */

#include "primitive_scheduler.h"
#include "stm32f0xx.h"
#include "Si7006.h"

#define INTERVAL_TIME_MS			(100)

#define TEMP_UPDATE_INTERVAL		(8)

extern void very_primitive_scheduler(){
	//The counters which are used to keep track of the specific tasks
	static uint32_t temperature_idx = 0;

	if(temperature_idx % TEMP_UPDATE_INTERVAL == 0){
		if(SI7006_is_task_executing(Si7006_temp_read))
				temperature_idx = 0;
	}
}
