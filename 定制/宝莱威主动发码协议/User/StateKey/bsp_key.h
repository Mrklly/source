#ifndef _BSP_KYE_H
#define _BSP_KEY_H


#include "stm8s.h"
#include "stm8s_clk.h"
#include "bsp_gpio.h"


#define KEY1_PIN        GPIO_PIN_5
#define KEY2_PIN        GPIO_PIN_5
#define KEY3_PIN        GPIO_PIN_6
#define KEY4_PIN        GPIO_PIN_7

#define KEY1_PORT       GPIOE
#define KEY2_PORT       GPIOC
#define KEY3_PORT       GPIOB
#define KEY4_PORT       GPIOB

#define NO_KEY                  0x0
#define KEY1_VALUE              0x01
#define KEY2_VALUE              0x02
#define KEY3_VALUE              0x04
#define KEY4_VALUE              0x08

#define STATE_KEY_UP            0               //初始状态,未按键
#define STATE_KEY_DOWM          1               //键被按下
#define STATE_KEY_SHORT         2               //按键计时态
#define STATE_KEY_LONG          3               //长按
#define STATE_KEY_LONG_UP       4               //长按抬起。

#define KEY_SHORT               0x10            //短按状态。
#define KEY_SHORT_UP            0x20            //短按抬起。
#define KEY_LONG                0x40            //长按状态。
#define KEY_LONG_UP             0x80            //长按抬起。


typedef struct _key_io_t_ {
    GPIO_TypeDef     *key_port;
    GPIO_Pin_TypeDef key_pin;
} key_io_t;


#define KEY_CHECK(KEY_PORT, KEY_PIN) ((KEY_PORT->IDR) & KEY_PIN)


void KEY_Init(void);
void KEY_value_Loading_Data(uint8_t KeyVelue,uint8_t state);
uint8_t Key_value_detection (void);

#endif

