#ifndef _DIMMER_H_
#define _DIMMER_H_


#include "drv_dimmer.h"
#include "drv_timer.h"
#include "drv_led.h"


#define STEP    25


void dimmer_run(void);
void led_run(void);


#endif

