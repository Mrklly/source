#ifndef _LED_DIMMER_H_
#define _LED_DIMMER_H_


#include "bsp_pwm.h"
#include "global_variables.h"


void pwm_set_all(uint16_t *buf);
void pwm_duty_update(uint16_t *buf_duty);
void led_dimmer_run(void);


#endif

