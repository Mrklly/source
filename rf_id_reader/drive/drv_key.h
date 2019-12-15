#ifndef _DRV_KEY_H_
#define _DRV_KEY_H_


#include "stm8s.h"


#define KEY_COUNT   4

#define KEY_PORT_1  GPIOD
#define KEY_PORT_2  GPIOB
#define KEY_PORT_3  GPIOB
#define KEY_PORT_4  GPIOA

#define KEY_PIN_1   GPIO_PIN_4
#define KEY_PIN_2   GPIO_PIN_4
#define KEY_PIN_3   GPIO_PIN_5
#define KEY_PIN_4   GPIO_PIN_3


#define KEY_DOWN(port, pin)    GPIO_ReadInputPin(port, pin)


typedef void (*btn_hander)(uint8_t shirt);
typedef struct _com_io_t_ {
    GPIO_TypeDef 		*key_port;
    GPIO_Pin_TypeDef	 key_pin;
    btn_hander			 hander;
} com_io_t;


extern const com_io_t g_io[];


void key_init(void);
void key_is_down(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin, uint8_t *press_counter);


#endif
