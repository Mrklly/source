#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_


#include "hc32_gpio.h"
#include "bsp_it.h"


#define KEY1_PORT       GpioPortA
#define KEY2_PORT       GpioPortB
#define KEY3_PORT       GpioPortB
#define KEY4_PORT       GpioPortB

#define KEY1_PIN        GpioPin15
#define KEY2_PIN        GpioPin3
#define KEY3_PIN        GpioPin4
#define KEY4_PIN        GpioPin5

#define KEY_COUNT       4

#define SHORT_PRESS     5                            //短按：20ms*5=100ms
#define LONG_PRESS      100                          //长按按:20ms*100=2000ms
#define REPEAT_PRESS    50                           //连按：20ms*50=1000ms

#define NO_KEY          0x0
#define KEY1_VALUE      0x01
#define KEY2_VALUE      0x02
#define KEY3_VALUE      0x04
#define KEY4_VALUE      0x08

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


typedef void (*btn_hander)(uint8_t key, uint16_t shirt);
typedef struct _com_io_t_ {
    en_gpio_port_t  key_port;
    en_gpio_pin_t	key_pin;
    btn_hander      hander;
} com_io_t;


extern const uint8_t key_define[4][2];
extern const com_io_t key_io[4];

void KEY_Init(void);
uint8_t Key_Scan(void);
uint8_t Key_value_detection (void);

void key_handler_callback(uint8_t key, uint16_t shirt);



#endif

