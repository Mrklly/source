#ifndef _KEY_HANDLER_H_
#define _KEY_HANDLER_H_


#include "global_variables.h"


//typedef void (*btn_callback)(void);


extern void btn_callback(uint8_t key);


void btn_hander_mode(uint8_t shirt);
void btn_hander_fan(uint8_t shirt);
void btn_hander_pwn(uint8_t shirt);
void btn_hander_up(uint8_t shirt);
void btn_hander_down(uint8_t shirt);
void set_lcd_backlight(void);
void set_led_indicator(void);
void device_setting(void);


#endif

