#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

#include "stm8s.h"
#include "stm8s_gpio.h"
#include "bsp_flash.h"

#define LED1_PORT       GPIOC
#define LED2_PORT       GPIOC
#define LED3_PORT       GPIOC
#define LED4_PORT       GPIOC

#define LED1_PIN        GPIO_PIN_1
#define LED2_PIN        GPIO_PIN_2
#define LED3_PIN        GPIO_PIN_3
#define LED4_PIN        GPIO_PIN_4

#define TEST_PORT       GPIOF
#define TEST_PIN        GPIO_PIN_4

#define PORT_RELAY_CH1       GPIOB
#define PORT_RELAY_CH2       GPIOB
#define PORT_RELAY_CH3       GPIOB
#define PORT_RELAY_CH4       GPIOB

#define PIN_RELAY_CH1        GPIO_PIN_3
#define PIN_RELAY_CH2        GPIO_PIN_2
#define PIN_RELAY_CH3        GPIO_PIN_0
#define PIN_RELAY_CH4        GPIO_PIN_1


// Panel name = PANEL_TOUCH
#define PIN_LED1_TOUCH        GPIO_PIN_3              //PC3_LED1
#define PIN_LED2_TOUCH        GPIO_PIN_1              //PC2_LED2
#define PIN_LED3_TOUCH        GPIO_PIN_3              //PC4_LED3
#define PIN_LED4_TOUCH        GPIO_PIN_2              //PC3_LED4

#define PORT_LED1_TOUCH       GPIOC
#define PORT_LED2_TOUCH       GPIOA
#define PORT_LED3_TOUCH       GPIOD
#define PORT_LED4_TOUCH       GPIOD


#define GPIO_HIGH(PORT, PIN)    PORT->ODR |=  PIN
#define GPIO_LOW(PORT, PIN)     PORT->ODR &= ~PIN
#define GPIO_TOG(PORT, PIN)     PORT->ODR ^=  PIN

typedef struct _gpio_t_ {
    GPIO_TypeDef     *key_port;
    GPIO_Pin_TypeDef key_pin;
} gpio_t;


extern const gpio_t gpio_led_key[4];
extern const gpio_t gpio_led_touch[4];
extern const gpio_t *p_gpio_led;


void LED_Init(void);//LED初始化。
void LED_open_Close_Config(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins,uint8_t EffectiVelevel);
void gpio_set_value(GPIO_TypeDef* port, GPIO_Pin_TypeDef pin,uint8_t value);
void LED_Toggle(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins);
void Relay_Init(void);//继电器初始化，输出低电平。
void Relay_open_Close_Config(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins,uint8_t EffectiVelevel);
void LED_field_Init(void);


#endif

