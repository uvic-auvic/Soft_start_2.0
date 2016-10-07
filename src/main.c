/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


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

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


static SemaphoreHandle_t I2C_semaphore;

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
	vSemaphoreCreateBinary( I2C_semaphore );
	if(I2C_semaphore == NULL)
		while(1);
}

int main(void)
{

	//initialize stuff

	blink_led_C8_C9_init();
	//timer16_it_config_48MHz_to_1Hz();

	DAC_init();

	SetClockForADC();
	SetClockForADC();
	CalibrateADC();
	EnableADC();
	ConfigureADC();
	ConfigureTIM15();
	ADC1->CR |= ADC_CR_ADSTART;



	Configure_GPIO_I2C2();
	Configure_I2C2_Master();

	init_Si7006();

	//init_FSM();

	Configure_GPIO_USART1();
	Configure_USART1();

	timer1_A7_A8_config();


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

	while(1){
		FSM_do();
	}
}

void blinkyTask(void *dummy){
	while(1){
		GPIOC->ODR ^= GPIO_ODR_9;
		/* maintain LED3 status for 200ms */
		vTaskDelay(200);
	}
}

void blinkyTask_2(void *dummy){
	while(1){
		GPIOC->ODR ^= GPIO_ODR_8;
		/* maintain LED3 status for 200ms */
		vTaskDelay(400);
	}
}

void vTaskInit(void){
    xTaskCreate(blinkyTask,
		(const signed char *)"blinkyTask",
		configMINIMAL_STACK_SIZE,
		NULL,                 /* pvParameters */
		tskIDLE_PRIORITY + 1, /* uxPriority */
		NULL                  /* pvCreatedTask */);
    xTaskCreate(blinkyTask_2,
    		(const signed char *)"blinkyTask_2",
    		configMINIMAL_STACK_SIZE,
    		NULL,                 /* pvParameters */
    		tskIDLE_PRIORITY + 1, /* uxPriority */
    		NULL                  /* pvCreatedTask */);
}

void TIM16_IRQHandler(void)//Once per second
{
	if (TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET){
		GPIOC->ODR ^= GPIO_ODR_9;
		TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
	}
}
