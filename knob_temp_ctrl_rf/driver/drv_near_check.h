#ifndef _DRV_NEAR_CHECK_H_
#define _DRV_NEAR_CHECK_H_


#include "hc32_gpio.h"


#define PORT_NEAR_CHECK       GpioPortC
#define PIN_NEAR_CHECK        GpioPin10


#define NEAR_CHECK_STA        M0P_GPIO->PCIN_f.PC10
//#define NEAR_CHECK_STA    Gpio_GetInputIO(PORT_NEAR_CHECK, PIN_NEAR_CHECK)



void near_check_init(void);


#endif

