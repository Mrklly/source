#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_

#include "base_types.h"
#include "hc32l136.h"
#include "system_hc32l136.h"
#include "hc32_gpio.h"

float Get_Temp(uint16_t adc);
void temp_init(void);
void get_room_temp();
void temperature_control(void);
void power_control(void);
void mod_cool(void);
void mod_warm(void);
void mod_fan(void);
void mod_dry(void);
void mod_off(void);
void mod_auto(void);

#endif
