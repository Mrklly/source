#ifndef _drv_dimmer_h_
#define _drv_dimmer_h_


#include "hc32_gpio.h"


#define PORT_DIMMER_1       3
#define PIN_DIMMER_1        2

#define PORT_DIMMER_2       3
#define PIN_DIMMER_2        3

#define PORT_ZERO_CROSS     2
#define PIN_ZERO_CROSS      3


#define GPIO_DIMMER_1_ON       M0P_GPIO->P3OUT_f.P32 = 1
#define GPIO_DIMMER_1_OFF      M0P_GPIO->P3OUT_f.P32 = 0

#define GPIO_DIMMER_2_ON       M0P_GPIO->P3OUT_f.P33 = 1
#define GPIO_DIMMER_2_OFF      M0P_GPIO->P3OUT_f.P33 = 0


#define GPIO_IRQ_ZERO_CROSS    M0P_GPIO->P2STAT_f.P23
#define GPIO_STA_ZERO_CROSS    M0P_GPIO->P2IN_f.P23


#define DIMMER_TIMER_RUN       M0P_BT2->CR_f.TR = 1
#define DIMMER_TIMER_STOP      M0P_BT2->CR_f.TR = 0


void hal_dimmer_init(void);
void irq_zero_cross(void);


#endif

