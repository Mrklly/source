#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_


#include "hc32_gpio.h"

/*****************************************************************************************************/
/*led灯引脚定义*/
#define BACK_LED1_PIN        GpioPin5                   //PA05_LED1
#define BACK_LED2_PIN        GpioPin2                   //PB02_LED2
#define BACK_LED3_PIN        GpioPin10                  //PB10_LED3
#define BACK_LED4_PIN        GpioPin11                  //PB11_LED4
/*led端口PC端口*/
#define BACK_LED1_PORT       GpioPortA
#define BACK_LED2_PORT       GpioPortB
#define BACK_LED3_PORT       GpioPortB
#define BACK_LED4_PORT       GpioPortB
/*继电器引脚定义*/
#define RELAY_PIN_CH1        GpioPin6               
#define RELAY_PIN_CH2        GpioPin7               
#define RELAY_PIN_CH3        GpioPin8               
#define RELAY_PIN_CH4        GpioPin9               
/*继电器端口PB*/
#define RELAY_PORT_CH1       GpioPortB
#define RELAY_PORT_CH2       GpioPortB
#define RELAY_PORT_CH3       GpioPortB
#define RELAY_PORT_CH4       GpioPortB

#define TEST_PORT      GpioPortB
#define TEST_PIN       GpioPin14

#define TEST_IO_HIGH     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + TEST_PORT))) |=   1 << TEST_PIN
#define TEST_IO_LOW      *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + TEST_PORT))) &= ~(1 << TEST_PIN)
#define TEST_IO_TOG      *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + TEST_PORT))) ^=  (1 << TEST_PIN)

#define RELAY_ON(PORT, PIN)     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) |=  (1 << PIN)
#define RELAY_OFF(PORT, PIN)    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) &= ~(1 << PIN)
#define RELAY_TOG(PORT, PIN)    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) ^=  (1 << PIN)

#define GPIO_HIGH(PORT, PIN)    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) |=  (1 << PIN)
#define GPIO_LOW(PORT, PIN)     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) &= ~(1 << PIN)
#define GPIO_TOG(PORT, PIN)     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) ^=  (1 << PIN)



/**********************************************函数声明***********************************************/
void LED_Init(void);
void LED_open_Close_Config(uint8_t channel,uint8_t EffectiVelevel);
void LED_Toggle(en_gpio_port_t enPort, en_gpio_pin_t enPin);
void Relay_Init(void);//继电器初始化，输出低电平。
void Relay_open_Close_Config(en_gpio_port_t enPort, en_gpio_pin_t enPin,uint8_t EffectiVelevel);
void back_LED_open_Close_Config( uint8_t ch,uint8_t EffectiVelevel);
void gpio_set(en_gpio_port_t port, en_gpio_pin_t pin, uint8_t status);


#endif

