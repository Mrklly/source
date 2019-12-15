#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_


#include "hc32_gpio.h"


#define PORT_LED_RED        GpioPortB
#define PIN_LED_RED         GpioPin4

#define PORT_LED_GREEN      GpioPortB
#define PIN_LED_GREEN       GpioPin5

#define TEST_PORT           GpioPortB
#define TEST_PIN            GpioPin14


#define GPIO_HIGH(PORT, PIN)    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) |=  (1 << PIN)
#define GPIO_LOW(PORT, PIN)     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) &= ~(1 << PIN)
#define GPIO_TOG(PORT, PIN)     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) ^=  (1 << PIN)

#define LED_RED_ON     GPIO_LOW (PORT_LED_RED, PIN_LED_RED)
#define LED_RED_OFF    GPIO_HIGH(PORT_LED_RED, PIN_LED_RED)
#define LED_RED_TOG    GPIO_TOG (PORT_LED_RED, PIN_LED_RED)

#define LED_GREEN_ON   GPIO_LOW (PORT_LED_GREEN, PIN_LED_GREEN)
#define LED_GREEN_OFF  GPIO_HIGH(PORT_LED_GREEN, PIN_LED_GREEN)
#define LED_GREEN_TOG  GPIO_TOG (PORT_LED_GREEN, PIN_LED_GREEN)


void LED_Init(void);


#endif

