//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef ADC_example_H_
#define ADC_example_H_

#include "stm32f0xx.h"


/* Error codes used to make the orange led blinking */
#define WARNING_MEASURE 0x01

/* Internal voltage reference calibration value address */
#define VREFINT_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FFFF7BA))
#define VDD_CALIB ((uint16_t) (330))
#define VDD_APPLI ((uint16_t) (300))
/* Private function prototypes -----------------------------------------------*/
void ConfigureGPIO(void);
void SetClockForADC(void);
void CalibrateADC(void);
void ConfigureADC(void);
void EnableADC(void);
void DisableADC(void);
void ConfigureTIM15(void);
// ----------------------------------------------------------------------------


//Used to read the millivolts off of the currently selected ADC
//unknown performance if there is no currently selected ADC
extern int voltage_in_millivolts();

#endif // ADC_example_H_
