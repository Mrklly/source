#ifndef _DRV_TIMER_H_
#define _DRV_TIMER_H_


#include "hc32_bt.h"


extern volatile uint16_t systick_counter;


void timer0_interrupt(void);
void timer0_init(void);


#endif
