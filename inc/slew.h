#ifndef __DAC_H
#define __DAC_H

#include "stm32f0xx.h"

//Public functions
extern void DAC_init();
extern void slew_start(int stepping_rate_micros);

#endif // __DAC_H
