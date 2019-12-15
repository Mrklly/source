#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H
#include "gpio.h"

/*****************************************************************************************************/
/*led灯引脚定义*/
#define BACK_LED1_PIN        GpioPin5                   //PA05_LED1
#define BACK_LED2_PIN        GpioPin2                   //PB02_LED2
#define BACK_LED3_PIN        GpioPin10                  //PB10_LED3
#define BACK_LED4_PIN        GpioPin11                  //PB11_LED4
#define BACK_LED5_PIN        GpioPin4                   //PB11_LED4
#define BACK_LED6_PIN        GpioPin0                   //PB11_LED4
/*led端口PC端口*/
#define BACK_LED1_PORT       GpioPortA
#define BACK_LED2_PORT       GpioPortB
#define BACK_LED3_PORT       GpioPortB
#define BACK_LED4_PORT       GpioPortB
#define BACK_LED5_PORT       GpioPortA
#define BACK_LED6_PORT       GpioPortA
/*继电器引脚定义*/
#define CH1_PIN        GpioPin6               
#define CH2_PIN        GpioPin7               
#define CH3_PIN        GpioPin8               
#define CH4_PIN        GpioPin9               
/*继电器端口PB*/
#define CH1_PORT       GpioPortB
#define CH2_PORT       GpioPortB
#define CH3_PORT       GpioPortB
#define CH4_PORT       GpioPortB
/**********************************************函数声明***********************************************/
void LED_Init(void);
void LED_open_Close_Config(uint8_t channel,uint8_t EffectiVelevel);
void LED_Toggle(en_gpio_port_t enPort, en_gpio_pin_t enPin);
void Relay_Init(void);//继电器初始化，输出低电平。
void Relay_open_Close_Config(en_gpio_port_t enPort, en_gpio_pin_t enPin,uint8_t EffectiVelevel);
void back_LED_open_Close_Config( uint8_t ch,uint8_t EffectiVelevel);
#endif
