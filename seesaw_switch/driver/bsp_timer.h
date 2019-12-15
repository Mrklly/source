#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H
#include "bsp_it.h"
/**********************************************函数声明***********************************************/
void TIMER2_Init(void);                 //定时器初始化函数。
void time2_xms(uint32_t MStime);        //延时1m函数。
void TimingDelay_Decrement(void);       //递减函数，中断调用。
void Delay(uint32_t nCount);
#endif
