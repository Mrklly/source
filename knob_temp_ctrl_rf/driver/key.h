#ifndef _KEY_H_
#define	_KEY_H_


#include "global_variables.h"


#if TEMP_CTRL_KNOB

#define KEY_PORT_MODE       GpioPortD   // TC5  PD07
#define KEY_PORT_FAN        GpioPortD   // TC3  PD02
#define KEY_PORT_FUNC       GpioPortD   // SET  PD04
#define KEY_PORT_PWR        GpioPortD   // TC4  PD06
#define KEY_PIN_MODE        GpioPin7
#define KEY_PIN_FAN         GpioPin2
#define KEY_PIN_FUNC        GpioPin4
#define KEY_PIN_PWR         GpioPin6

#elif TEMP_CTRL_TOUCH

#define KEY_PORT_MODE		GpioPortB   // TC5  PB08
#define KEY_PORT_FAN		GpioPortD   // TC3  PD02
#define KEY_PORT_UP			GpioPortD   // TC1  PD06
#define KEY_PORT_DOWN		GpioPortD   // TC2  PD07
#define KEY_PORT_PWR		GpioPortB   // TC4  PB07
#define KEY_PIN_MODE		GpioPin8
#define KEY_PIN_FAN			GpioPin2
#define KEY_PIN_UP			GpioPin6
#define KEY_PIN_DOWN		GpioPin7
#define KEY_PIN_PWR		    GpioPin7

#endif


#define PORT_DEV_TYPE       GpioPortB
#define PIN_DEV_TYPE        GpioPin1


#define GPIO_PIN_STA(PORT,PIN)  ((*((volatile uint32_t *)((uint32_t)&M0P_GPIO->PAIN + PORT)))  & (1 << PIN))


#define KEY_PRESSED_MODE    !Gpio_GetInputIO(KEY_PORT_MODE, KEY_PIN_MODE)
#define KEY_PRESSED_FAN     !Gpio_GetInputIO(KEY_PORT_FAN,  KEY_PIN_FAN)
#define KEY_PRESSED_FUNC    !Gpio_GetInputIO(KEY_PORT_FUNC, KEY_PIN_FUNC)
#define KEY_PRESSED_PWR     !Gpio_GetInputIO(KEY_PORT_PWR,  KEY_PIN_PWR)


#if TEMP_CTRL_KNOB

extern const com_io g_io[4];

#elif TEMP_CTRL_TOUCH

extern const com_io g_io[5];

#endif


void key_init(void);
uint8_t key_scan(en_gpio_port_t enPort, en_gpio_pin_t enPin, uint8_t *press_counter);
void key_check_all(void);
void key_handler(void);
void AddrMod_Set(void);

void get_dev_type(void);


extern void btn_hander_mode(uint8_t shirt);
extern void btn_hander_fan(uint8_t shirt);
extern void btn_hander_func(uint8_t shirt);
extern void btn_hander_pwr(uint8_t shirt);
extern void btn_hander_up(uint8_t shirt);
extern void btn_hander_down(uint8_t shirt);


#endif

