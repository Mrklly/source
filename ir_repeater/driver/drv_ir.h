#ifndef _DRV_IR_H_
#define _DRV_IR_H_


#include "hc32_gpio.h"
#include "hc32_sysctrl.h"
#include "hc32_uart.h"
#include "hc32_bt.h"


#define IR_PORT        GpioPortA
#define IR_PIN         GpioPin15

#define IR_PIN_DATA    M0P_GPIO->PAIN_f.PA15


void ir_init(void);
void ir_io_init(void);
void ir_timer0_init(void);
void timer0_interrupt(void);
void ir_debug(void);


#endif

