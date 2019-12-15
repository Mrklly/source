#ifndef _BSP_TIMER_H_
#define _BSP_TIMER_H_


#include "stm8s_tim1.h"
#include "stm8s_tim2.h"


//void TIMER1_Init(void);
void TimingDelay_Decrement(void);
void timer2_init(void);
void timer4_init(void);
void time4_delay_ms(uint32_t ms);


#endif

