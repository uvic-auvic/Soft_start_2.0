/*
 * PWM_out.c
 *
 *  Created on: Aug 18, 2016
 *      Author: auvic
 */

#include "stm32f0xx.h"
#include "PWM_out.h"

#define TIM_PERIOD_CHN_1 			(960)
#define TIM_PERIOD_CHN_1_DIV_2 		((TIM_PERIOD_CHN_1) / (2))

/**
  * @brief  This function configures the TIMx as PWM mode 1 and center-aligned
  *         and enables the peripheral clock on TIMx and on GPIOA.
  *         It configures GPIO PA8 as Alternate function for TIM1_CH1
  *         To use another timer, channel or GPIO, the RCC and GPIO configuration
  *         must be adapted according to the datasheet.
  *         In case of other timer, the interrupt sub-routine must also be renamed
  *         with the right handler and the NVIC configured correctly.
  * @param  None
  * @retval None
  */
extern void timer1_A7_A8_config(void)
{
  /* (1) Enable the peripheral clock of Timer x */
  /* (2) Enable the peripheral clock of GPIOA */
  /* (3) Select alternate function mode on GPIOA pin 8 */
  /* (4) Select AF2 on PA8 in AFRH for TIM1_CH1 */

  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; /* (1) */
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN; /* (2) */
  GPIOA->MODER = GPIOA->MODER & ((~(GPIO_MODER_MODER8)) | (~(GPIO_MODER_MODER7))) | (GPIO_MODER_MODER8_1 | GPIO_MODER_MODER7_1); /* (3) */
  GPIOA->AFR[1] |= 0x02; /* (4) */
  GPIOA->AFR[0] |= 0x02<<(4*7);

  /* (1) Set prescaler to 47, so APBCLK/48 i.e 1MHz */
  /* (2) Set ARR = 8, as timer clock is 1MHz and center-aligned counting,
         the period is 16 us */
  /* (3) Set CCRx = 7, the signal will be high during 14 us */
  /* (4) Select PWM mode 1 on OC1  (OC1M = 110),
         enable preload register on OC1 (OC1PE = 1, reset value) */
  /* (5) Select active high polarity on OC1 (CC1P = 0, reset value),
         enable the output on OC1 (CC1E = 1)*/
  /* (6) Enable output (MOE = 1)*/
  /* (7) Enable counter (CEN = 1)
         select center-aligned mode 1 (CMS = 01) */
  /* (8) Force update generation (UG = 1) */

  TIM1->PSC = 0; /* (1) */
  TIM1->ARR = TIM_PERIOD_CHN_1; /* (2) */
  TIM1->CCR1 = TIM_PERIOD_CHN_1_DIV_2; /* (3) */
  TIM1->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE; /* (4) */
  TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1NE; /* (5) */
  TIM1->BDTR |= TIM_BDTR_MOE; /* (6) */
  TIM1->CR1 |= TIM_CR1_CMS_0 | TIM_CR1_CEN; /* (7) */
  TIM1->EGR |= TIM_EGR_UG; /* (8) */
}
