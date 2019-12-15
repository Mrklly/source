#ifndef _KEY_HANDLER_H_
#define _KEY_HANDLER_H_

#include "stm8s.h"
#include "global_variables.h"

/* °´¼ü */
#define KEY_PIN_MODE		GPIO_PIN_6
#define KEY_PIN_FAN			GPIO_PIN_4
#define KEY_PIN_UP			GPIO_PIN_7
#define KEY_PIN_DOWN		GPIO_PIN_3
#define KEY_PIN_ON_OFF		GPIO_PIN_5

#define KEY_PORT_UP			GPIOB
#define KEY_PORT_DOWN		GPIOB
#define KEY_PORT_ON_OFF		GPIOB
#define KEY_PORT_MODE		GPIOB
#define KEY_PORT_FAN		GPIOB


void btn_hander_mode(u8	shirt);
void btn_hander_fan(u8	shirt);
void btn_hander_pwn(u8	shirt);
void btn_hander_up(u8	shirt);
void btn_hander_down(u8	shirt);
void valve_control(void);
void set_fan(void);
void set_lcd_backlight(void);
void set_led_indicator(void);






#endif
