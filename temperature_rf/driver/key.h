#ifndef _KEY_H_
#define	_KEY_H_

#include "global_variables.h"


#define KEY_PIN_MODE		GpioPin8
#define KEY_PIN_FAN			GpioPin2
#define KEY_PIN_UP			GpioPin6
#define KEY_PIN_DOWN		GpioPin7
#define KEY_PIN_ON_OFF		GpioPin7

#define KEY_PORT_MODE		GpioPortB   // TC5  PB08
#define KEY_PORT_FAN		GpioPortD   // TC3  PD02
#define KEY_PORT_UP			GpioPortD   // TC1  PD06
#define KEY_PORT_DOWN		GpioPortD   // TC2  PD07
#define KEY_PORT_ON_OFF		GpioPortB   // TC4  PB07

#define KEY_PRESSED_MODE    !Gpio_GetInputIO(KEY_PORT_MODE,   KEY_PIN_MODE)
#define KEY_PRESSED_FAN     !Gpio_GetInputIO(KEY_PORT_FAN,    KEY_PIN_FAN)
#define KEY_PRESSED_UP      !Gpio_GetInputIO(KEY_PORT_UP,     KEY_PIN_UP)
#define KEY_PRESSED_DOWN    !Gpio_GetInputIO(KEY_PORT_DOWN,   KEY_PIN_DOWN)
#define KEY_PRESSED_POWER   !Gpio_GetInputIO(KEY_PORT_ON_OFF, KEY_PIN_ON_OFF)


extern const com_io g_io[5];


void key_init(void);
uint8_t key_scan(en_gpio_port_t enPort, en_gpio_pin_t enPin, uint8_t *press_counter);
void key_check_all(void);
void key_handler(void);
void AddrMod_Set(void);

extern void btn_hander_mode(uint8_t	shirt);
extern void btn_hander_fan(uint8_t	shirt);
extern void btn_hander_pwn(uint8_t	shirt);
extern void btn_hander_up(uint8_t	shirt);
extern void btn_hander_down(uint8_t	shirt);

#endif

