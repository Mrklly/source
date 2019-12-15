#ifndef _KEY_HANDLER_H_
#define _KEY_HANDLER_H_


#include "global_variables.h"


//typedef void (*btn_callback)(void);


extern void btn_callback(uint8_t key);


void btn_hander_mode(uint8_t shirt);
void btn_hander_fan(uint8_t shirt);
void btn_hander_pwr(uint8_t shirt);
void btn_hander_up(uint8_t shirt);
void btn_hander_down(uint8_t shirt);
void set_lcd_backlight(void);
void device_setting(void);

#if TEMP_CTRL_KNOB

void btn_hander_func(uint8_t shirt);

#elif TEMP_CTRL_TOUCH

void key_knob_up(void);
void key_knob_down(void);

#endif

#endif

