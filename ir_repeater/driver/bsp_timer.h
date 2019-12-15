#ifndef _BSP_TIMER_H_
#define _BSP_TIMER_H_


#include "bsp_it.h"


void timer2_init(void);
void time2_xms(uint32_t MStime);
void TimingDelay_Decrement(void);
void Delay(uint32_t nCount);


#endif

