//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#include "status_leds.h"
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_tim.h"
// ----------------------------------------------------------------------------

extern void blink_led_C8_C9_init()
{
  GPIO_InitTypeDef GPIO_InitStructure;

  //Andy's attempt at blinking an LED

  //Enable the C port to be used
  RCC->AHBENR |= RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOA;

  //For now use the GPIO_Init function, to save time
  //Be careful because not erasing any of the past settings so failure to overwrite will keep them

  //Select port 3 to be our toggle port
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //Select port 3 to be our toggle port
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);
  GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);
}

extern void timer16_it_config_48MHz_to_1Hz(){
	//Enable the interrupt this is needed to flicker an LED A#
	NVIC_InitTypeDef NVIC_InitStructure;
	//Enable Tim16 global interrupt
	NVIC_InitStructure.NVIC_IRQChannel=TIM16_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIM6 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
	/* Time base configuration */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Period = 50000 - 1; // 50kHz down to 1Hz (1000 ms)
	TIM_TimeBaseStructure.TIM_Prescaler = 960 - 1; // 48 MHz Clock down to 50 kHz (adjust per your clock)
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);
	/* TIM IT enable */
	TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);

	/* TIM16 enable counter */
	TIM_Cmd(TIM16, ENABLE);
}

extern void timer_1_charge_pump(){
	/* TIM3 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	/* Time base configuration */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Period = 20 - 1; // 600 kHz down to 60Hz (2 ms)
	TIM_TimeBaseStructure.TIM_Prescaler = 48 - 1; // 48 MHz Clock down to 600 kHz (adjust per your clock)
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	/* TIM IT enable */
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

	RCC->AHBENR |= RCC_AHBPeriph_GPIOA;

	//initialize structs
	GPIO_InitTypeDef GPIO_struct;

	//initialize the LEDs
	GPIO_struct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_struct.GPIO_OType = GPIO_OType_PP;
	GPIO_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_struct.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_Init(GPIOA, &GPIO_struct);

	TIM_OCInitTypeDef outputChannelInit = {0,};
	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
	outputChannelInit.TIM_Pulse = 10 - 1; //50%
	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM1, &outputChannelInit);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);

	/* TIM3 enable counter */
	TIM_Cmd(TIM1, ENABLE);
}
