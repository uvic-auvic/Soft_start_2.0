#include "slew.h"
#include "stm32f0xx.h"
#include "ADC_example.h"
#include "system_info.h"

#define STARTING_VOLTAGE_IN_MILLIVOLTS 700
#define ENDING_VOLTAGE_IN_MILLIVOLTS 1000

extern void DAC_init(){
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	DAC->CR |= DAC_CR_TSEL1 | DAC_CR_BOFF1 | DAC_CR_EN1;
	DAC->DHR12R1 = 0x000;
}

extern void DAC_change_milli_volt(uint16_t mV){
	//milli volts to 12 bit is mV * 1.365
	mV = (mV * MILLIVOLTS_TO_12BIT_CONVERSION_NUMER) / MILLIVOLTS_TO_12BIT_CONVERSION_DENOM;
	if(mV > 0xFFF){
		mV = 0xFFF;
	}
	DAC->DHR12R1 = mV;
}

extern void DAC_increment_output(){
	DAC->DHR12R1++;
}

extern void slew_start(int stepping_rate_micros){
	DAC_change_milli_volt(STARTING_VOLTAGE_IN_MILLIVOLTS);
	int temp = voltage_in_millivolts();
	while(temp < ENDING_VOLTAGE_IN_MILLIVOLTS){
		DAC_increment_output();
		SysTick_Config(CLOCK_SPEED_IN_MHZ * stepping_rate_micros);
		temp = voltage_in_millivolts();
	}
	DAC_change_milli_volt(MPU_MILLIVOLTS);
}
