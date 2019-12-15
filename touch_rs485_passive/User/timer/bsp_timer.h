#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H

#include "stm8s_tim1.h"
#include "stm8s_tim2.h"

/**********************************************函数声明***********************************************/
void TIMER1_Init(void);                 //定时器初始化函数。
void time1_10ms(uint32_t MStime);        //延时1m函数。
void TimingDelay_Decrement(void);       //递减函数，中断调用。
void timer2_init(void);

#endif

