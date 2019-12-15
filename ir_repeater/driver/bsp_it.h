#ifndef _BSP_IT_H_
#define _BSP_IT_H_


#include "hc32_bt.h"
#include "bsp_key.h"
#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "bsp_pwm.h"
#include "hc32_ddl.h"
#include "hc32_uart.h"
#include "hc32_gpio.h"
#include "hc32_sysctrl.h"



void timer2_int(void);

void Gpio_IRQHandler(uint8_t u8Param);
void RxIntCallback(void);


#endif

