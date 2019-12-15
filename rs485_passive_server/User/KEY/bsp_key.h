#ifndef _BSP_KYE_H
#define _BSP_KEY_H
#include "stm8s.h"
#include "stm8s_clk.h"
#include "bsp_gpio.h"
#include "checksum.h"
/*****************************************************************************************************/
/*按键引脚定义*/
#define KEY1_PIN        GPIO_PIN_5
#define KEY2_PIN        GPIO_PIN_5
#define KEY3_PIN        GPIO_PIN_6
#define KEY4_PIN        GPIO_PIN_7
/*按键端口定义*/
#define KEY1_PORT       GPIOE
#define KEY2_PORT       GPIOC
#define KEY3_PORT       GPIOB
#define KEY4_PORT       GPIOB
/*按键模式*/
#define SHORT_PRESS     5                            //短按：20ms*5=100ms
#define LONG_PRESS      100                          //长按按:20ms*100=2000ms
#define REPEAT_PRESS    50                           //连按：20ms*50=1000ms
/*定义键值*/
#define NO_KEY          0x0
#define KEY1_VALUE      0x01
#define KEY2_VALUE      0x02
#define KEY3_VALUE      0x03
#define KEY4_VALUE      0x04
//定义按键编译开关
#define KEY         1                   //KEY总开关
#define KEY_ON      1                   //有按键按下。
#define KEY_OFF     0                   //无按键按下。
typedef struct _com_io_t_{
	GPIO_TypeDef 		*key_port;//按键端口
	GPIO_Pin_TypeDef	 key_pin;//按键引脚。
        uint8_t                  key_value;
}port_io;
/**********************************************函数声明***********************************************/
void KEY_Init(void);
uint8_t Key_Scan(void);
void KEY_LED_Control(void);
void KEY_value_Loading_Data(uint8_t KeyVelue);
#endif
