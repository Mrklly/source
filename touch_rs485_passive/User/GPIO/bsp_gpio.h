#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

#include "stm8s.h"
#include "stm8s_gpio.h"
#include "bsp_flash.h"


#define LED1_PIN        GPIO_PIN_3              //PC3_LED1
#define LED2_PIN        GPIO_PIN_1              //PC2_LED2
#define LED3_PIN        GPIO_PIN_3              //PC4_LED3
#define LED4_PIN        GPIO_PIN_2              //PC3_LED4

#define LED1_PORT       GPIOC
#define LED2_PORT       GPIOA
#define LED3_PORT       GPIOD
#define LED4_PORT       GPIOD

#define CH1_PIN        GPIO_PIN_3               //PB3_�̵���1
#define CH2_PIN        GPIO_PIN_2               //PB2_�̵���2
#define CH3_PIN        GPIO_PIN_0               //PB0_�̵���3
#define CH4_PIN        GPIO_PIN_1               //PB1_�̵���4

#define CH1_PORT       GPIOB
#define CH2_PORT       GPIOB
#define CH3_PORT       GPIOB
#define CH4_PORT       GPIOB

#define GPIO_HIGH(PORT, PIN)    PORT->ODR |=  PIN
#define GPIO_LOW(PORT, PIN)     PORT->ODR &= ~PIN
#define GPIO_TOG(PORT, PIN)     PORT->ODR ^=  PIN



/**********************************************��������***********************************************/

void LED_Init(void);//LED��ʼ����
void LED_open_Close_Config(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins,uint8_t EffectiVelevel);
void gpio_set_value(GPIO_TypeDef* port, GPIO_Pin_TypeDef pin,uint8_t value);
void LED_Toggle(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins);
void Relay_Init(void);//�̵�����ʼ��������͵�ƽ��
void Relay_open_Close_Config(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins,uint8_t EffectiVelevel);
void LED_field_Init(void);


#endif

