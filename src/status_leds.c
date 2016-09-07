//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#include "status_leds.h"
#include "stm32f0xx.h"

// ----------------------------------------------------------------------------

extern void blink_led_C8_C9_init()
{
  GPIO_InitTypeDef GPIO_InitStructure;

  //Andy's attempt at blinking an LED

  //Enable the C port to be used
  RCC->AHBENR |= RCC_AHBPeriph_GPIOC;

  //For now use the GPIO_Init function, to save time
  //Be careful because not erasing any of the past settings so failure to overwrite will keep them

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
