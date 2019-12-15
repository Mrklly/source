#ifndef _DRV_IR_H_
#define _DRV_IR_H_

#include "hc32_gpio.h"
#include "hc32_sysctrl.h"
#include "hc32_uart.h"
#include "hc32_bt.h"


#define CAPTURE_EDGE(EDGE) ((M0P_TIM0_MODE23_TypeDef *)((uint32_t)M0P_TIM0_MODE23+0x100*TIM0))->CRCH0_f.CFB_CRB_BKSB = EDGE

#define CLK_DIV              (3 / 4)

#define PULSE_START          ((9000*3) >> 2)
#define PULSE_START_ZONE     (( 900*3) >> 2)

#define PULSE_SPACE          ((4500*3) >> 2)
#define PULSE_SPACE_ZONE     (( 450*3) >> 2)

#define PULSE_H              ((1684*3) >> 2)
#define PULSE_H_ZONE         (( 168*3) >> 2)

#define PULSE_L              (( 562*3) >> 2)
#define PULSE_L_ZONE         ((  56*3) >> 2)

#define PULSE_REPEAT         ((2250*3) >> 2)
#define PULSE_REPEAT_ZONE    (( 225*3) >> 2)

#define PULSE_CNT_LG         ((40000*3) >> 2)
#define PULSE_CNT_LG_ZONE    (( 4000*3) >> 2)

#define PULSE_CNT_ST         ((20000*3) >> 2)
#define PULSE_CNT_ST_ZONE    (( 2000*3) >> 2)



#define IR_PORT        GpioPortA
#define IR_PIN         GpioPin15

#define IR_PIN_DATA    Gpio_GetInputIO(IR_PORT, IR_PIN)


#define IR_STA_START   0x01
#define IR_STA_SPACE   0x02
#define IR_STA_DATA    0x04
#define IR_STA_END     0x08


extern volatile uint8_t ir_sta;



void ir_init(void);
void ir_io_init(void);
void ir_timer0_init(void);
void timer0_interrupt(void);
void ir_debug(void);


#endif
