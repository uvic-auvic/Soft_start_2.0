/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "main.h"

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "slew.h"
#include "ADC_example.h"
#include "I2C_controller.h"
#include "Si7006.h"
#include "FSM.h"
#include "status_leds.h"
#include "INA226.h"
#include "PWM_out.h"
#include "stm32f0xx_tim.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/*
 * I2C2_semaphore_control is used when a task want to take control of the I2C2 bus
 * This does not necessarily mean that the task is using the I2C2 bus it just
 * means that it has a use for it and needs to block it from other tasks
 */
static SemaphoreHandle_t I2C2_semaphore_control;

/*
 * I2C2_semaphore_using is used by tasks to keep track of when they are done with
 * bus. The I2C2 controller can be configured with a call back that can be
 * configured to give signals to tasks that are waiting or using the I2C2 bus
 */
static SemaphoreHandle_t I2C2_semaphore_using;

void vApplicationTickHook( void )
{
	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */
}
/*-----------------------------------------------------------*/

void CreateSemaphores(void){
	//The I2C2 Semaphore to safeguard tasks from overlapping usage of the bus
	I2C2_semaphore_control = xSemaphoreCreateBinary();
	//Just double check that initialized successfully
	configASSERT(I2C2_semaphore_control);
	//For some reason the semaphore start empty so we must free it
	xSemaphoreGive(I2C2_semaphore_control);
}

int main(void)
{

	//initialize stuff

	blink_led_C8_C9_init();
	//timer16_it_config_48MHz_to_1Hz();


	DAC_init();

	/*
	SetClockForADC();
	SetClockForADC();
	CalibrateADC();
	EnableADC();
	ConfigureADC();
	ConfigureTIM15();
	ADC1->CR |= ADC_CR_ADSTART;
	*/



	Configure_GPIO_I2C2();
	Configure_I2C2_Master();

	init_Si7006();

	//init_FSM();

	//Configure_GPIO_USART1();
	//Configure_USART1();

	//timer1_A7_A8_config();


	//safety checks

	vTaskInit();
	CreateSemaphores();

	//System boot
	/*
	slew_start(500);//input is in microseconds

	set_volt_ptr();

	int temp_timer = 0;
	while(temp_timer < 48000000){
		temp_timer++;
	}

	exec_volt_read();
	*/

	//reconfigure


	//System go

	/* Start the kernel.  From here on, only tasks and interrupts will run. */
	vTaskStartScheduler();

	//This should never happen
	while(1);
}



void updateTemperature(void *dummy){
	while(1){
		// Wait until we are able to take control of the I2C2 bus
		while( xSemaphoreTake(I2C2_semaphore_control, (TickType_t) 1000) == pdFALSE);
		//Delay to ensure that the I2C2 bus will not cause error
		vTaskDelay(TEMPERATURE_DELAY_TIME_MS/10);
		//Check to make sure we are configured to read the temperature
		if(Si7006_check_ready_for(Si7006_temp_read) == false){
			//Set the temperature read
			Si700X_set_temp_read_over_I2C();
			/* Block to wait for I2C2 to notify this task. */
			ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
			vTaskDelay(TEMPERATURE_DELAY_TIME_MS/10);
		}
		//Now that we know the system is configured correctly execute temperature read
		Si700X_exec_temp_read_over_I2C();
		//wait until this blocked tasks is released
		ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

		//We are now done with the I2C2 bus
		//Release the I2C2 bus before sleeping
		xSemaphoreGive(I2C2_semaphore_control);
		//Delay the task until it's time to read again
		vTaskDelay(TEMPERATURE_DELAY_TIME_MS*10);
	}
}

void updateHumidity(void *dummy){
	while(1){
		// Wait until we are able to take control of the I2C2 bus
		while(xSemaphoreTake(I2C2_semaphore_control, (TickType_t) 1000) == pdFALSE);
		//Delay to ensure that the I2C2 bus will not cause error
		vTaskDelay(TEMPERATURE_DELAY_TIME_MS/10);
		//Check to make sure we are configured to read the temperature
		if(Si7006_check_ready_for(SI7006_humidity_read) == false){
			//Set the temperature read
			Si700X_set_humidity_read_over_I2C();
			//Si700X_set_temp_read_over_I2C();
			/* Block to wait for prvTask2() to notify this task. */
			ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
			vTaskDelay(TEMPERATURE_DELAY_TIME_MS/10);
		}
		//Now that we know the system is configured correctly execute temperature read
		Si700X_exec_humidty_read_over_I2C();
		//wait until this blocked tasks is released
		ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

		//We are now done with the I2C2 bus
		//Release the I2C2 bus before sleeping
		xSemaphoreGive(I2C2_semaphore_control);
		//Delay the task until it's time to read again
		vTaskDelay(TEMPERATURE_DELAY_TIME_MS*10);
	}
}

void humidityAndTemperatureUpdating(void *dummy){
	static uint8_t humidity = 0;
	while(1){
		// Wait until we are able to take control of the I2C2 bus
		while(xSemaphoreTake(I2C2_semaphore_control, (TickType_t) 1000) == pdFALSE);
		//Delay to ensure that the I2C2 bus will not cause error
		vTaskDelay(TEMPERATURE_DELAY_TIME_MS/10);

		if(humidity){
			//Set the temperature read
			Si700X_set_humidity_read_over_I2C();
		}else{
			//Set the temperature read
			Si700X_set_temp_read_over_I2C();
		}

		/* Block to wait for prvTask2() to notify this task. */
		ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
		vTaskDelay(TEMPERATURE_DELAY_TIME_MS/10);

		if(humidity){
			//Set the temperature read
			Si700X_exec_humidty_read_over_I2C();
		}else{
			//Set the temperature read
			Si700X_exec_temp_read_over_I2C();
		}

		//wait until this blocked tasks is released
		ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

		humidity = !humidity;
		//We are now done with the I2C2 bus
		//Release the I2C2 bus before sleeping
		xSemaphoreGive(I2C2_semaphore_control);
		//Delay the task until it's time to read again
		vTaskDelay(TEMPERATURE_DELAY_TIME_MS*10);
	}
}

void blinkyTask(void *dummy){
	while(1){
		GPIOC->ODR ^= GPIO_ODR_9;
		/* maintain LED3 status for 200ms */
		vTaskDelay(500);
	}
}

void vTaskInit(void){
    xTaskCreate(blinkyTask,
		(const signed char *)"blinkyTask",
		configMINIMAL_STACK_SIZE,
		NULL,                 // pvParameters
		tskIDLE_PRIORITY + 1, // uxPriority
		NULL              ); // pvCreatedTask */
    /*xTaskCreate(updateTemperature,
    		(const signed char *)"temperature",
    		configMINIMAL_STACK_SIZE * 2,
    		NULL,                 // pvParameters
    		tskIDLE_PRIORITY + 1, // uxPriority
    		NULL               );///* pvCreatedTask */
    /*xTaskCreate(updateHumidity,
			(const signed char *)"humidity",
			configMINIMAL_STACK_SIZE * 2,
			NULL,                 // pvParameters
			tskIDLE_PRIORITY + 1, // uxPriority
			NULL              );  // pvCreatedTask*/
    xTaskCreate(humidityAndTemperatureUpdating,
		(const signed char *)"humAndTemp",
		configMINIMAL_STACK_SIZE * 2,
		NULL,                 // pvParameters
		tskIDLE_PRIORITY + 1, // uxPriority
		NULL              );  // pvCreatedTask*/
}

void TIM16_IRQHandler(void)//Once per second
{
	if (TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET){
		GPIOC->ODR ^= GPIO_ODR_9;
		TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
	}
}
