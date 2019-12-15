#ifndef _BSP_KYE_H
#define _BSP_KEY_H
#include "bsp_it.h"
/*****************************************************************************************************/
/*按键引脚定义*/
#define KEY1_PIN        GpioPin15     //PA15_按键1
#define KEY2_PIN        GpioPin3      //PB3_按键2
#define KEY3_PIN        GpioPin4      //PB4_按键3
#define KEY4_PIN        GpioPin5      //PB5_按键4
#define KEY5_PIN        GpioPin14     //PC14_按键5
#define KEY6_PIN        GpioPin15     //PC15_按键6
/*步长与总共步数*/
#define STEP            25             //总共多少步。既然25个等级。
/*按键端口定义*/
#define KEY1_PORT       GpioPortA
#define KEY2_PORT       GpioPortB
#define KEY3_PORT       GpioPortB
#define KEY4_PORT       GpioPortB
#define KEY5_PORT       GpioPortC
#define KEY6_PORT       GpioPortC
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
#define KEY5_VALUE      0x05
#define KEY6_VALUE      0x06
/*定义状态机5种状态*/
#define STATE_KEY_UP            0               //初始状态,未按键
#define STATE_KEY_DOWM          1               //键被按下
#define STATE_KEY_SHORT         2               //按键计时态
#define STATE_KEY_KEEP          3               //短按维持。
#define STATE_KEY_LONG          4               //长按
#define STATE_KEY_LONG_UP       5               //长按抬起。

/*定义按键状态*/
#define KEY_SHORT               0x10            //短按状态。
#define KEY_SHORT_KEEP          0x20            //短按维持。
#define KEY_SHORT_UP            0x30            //短按抬起。
#define KEY_LONG                0x40            //长按状态。
#define KEY_LONG_UP             0x80            //长按抬起。
//定义按键编译开关
#define KEY         1                   //KEY总开关
#define KEY_ON      1                   //有按键按下。
#define KEY_OFF     0                   //无按键按下。
/**********************************************函数声明***********************************************/
void KEY_Init(void);
uint8_t Key_Scan(void);
void KEY_LED_Control(void);
void KEY_value_Loading_Data(uint8_t KeyVelue);
void KEY_SEND_Code(uint8_t keyfun,uint8_t keynumber);
void Uploading_data(void);
//void KEY_Modify_LED_state(uint8_t key);
void Local_control(void);
void Interlocking_control(uint8_t *buff,uint8_t key);
void Point_movement(void);
void Dismantling_function(uint8_t function,uint8_t key_number,uint8_t state);
uint8_t Key_value_detection (void);
void KEY_Modify_LED_state(uint8_t key,uint8_t state);
void Ligth_PowerOn(uint8_t number);
void Ligth_PowerOff(uint8_t number);
void Modify_Ligth_state(uint8_t number);
void LigthLED_ON(uint8_t key);
void LigthLED_OFF(uint8_t key);

#endif
