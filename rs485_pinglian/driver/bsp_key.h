#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_


#include "bsp_it.h"


#define PORT_KEY1       GpioPortA
#define PORT_KEY2       GpioPortB
#define PORT_KEY3       GpioPortB
#define PORT_KEY4       GpioPortB
#define PORT_KEY5       GpioPortC
#define PORT_KEY6       GpioPortC

#define PIN_KEY1        GpioPin15
#define PIN_KEY2        GpioPin3
#define PIN_KEY3        GpioPin4
#define PIN_KEY4        GpioPin5
#define PIN_KEY5        GpioPin14
#define PIN_KEY6        GpioPin15

#define KEY_COUNT       6

#define SHORT_PRESS     5                            //短按：20ms*5=100ms
#define LONG_PRESS      100                          //长按按:20ms*100=2000ms
#define REPEAT_PRESS    50                           //连按：20ms*50=1000ms

#define NO_KEY          0x00
#define KEY1_VALUE      0x01
#define KEY2_VALUE      0x02
#define KEY3_VALUE      0x04
#define KEY4_VALUE      0x08
#define KEY5_VALUE      0x10
#define KEY6_VALUE      0x20

#define KEY_VALUE(i)    (1 << i)

#define KEY         1                           //KEY总开关
#define KEY_ON      1                           //有按键按下。
#define KEY_OFF     0                           //无按键按下。

#define STATE_KEY_UP            0               //初始状态,未按键
#define STATE_KEY_DOWM          1               //键被按下
#define STATE_KEY_SHORT         2               //按键计时态
#define STATE_KEY_LONG          3               //长按
#define STATE_KEY_LONG_UP       4               //长按抬起。

#define KEY_SHORT               0x10            //短按状态。
#define KEY_SHORT_UP            0x20            //短按抬起。
#define KEY_LONG                0x40            //长按状态。
#define KEY_LONG_UP             0x80            //长按抬起。

#define KEY_STATUS(i)           (0x10 << i)

#define KEY_CHECK(KEY_PORT, KEY_PIN) ((*((volatile uint32_t *)((uint32_t)&M0P_GPIO->PAIN + KEY_PORT))) & (1 << KEY_PIN))


extern const uint8_t gpio_key[6][2];

void key_init_all(void);
uint8_t Key_Scan(void);
uint8_t key_check_all(void);


#endif

