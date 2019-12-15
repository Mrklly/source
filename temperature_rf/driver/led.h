#ifndef _LED_H_
#define _LED_H_

#include "hc32_gpio.h"

#define LED_INDICATOR_PORT		GpioPortD
#define LED_INDICATOR_PIN       GpioPin4

#define LED_INDICATOR_ON        *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + LED_INDICATOR_PORT))) |=   1UL << LED_INDICATOR_PIN
#define LED_INDICATOR_OFF       *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + LED_INDICATOR_PORT))) &= ~(1UL << LED_INDICATOR_PIN)
#define LED_INDICATOR_TOGGLE    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + LED_INDICATOR_PORT))) ^=   1UL << LED_INDICATOR_PIN

#define LCD_BACKLIGHT_PORT	    GpioPortD
#define LCD_BACKLIGHT_PIN       GpioPin5

#define LCD_BACKLIGHT_ON        *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + LCD_BACKLIGHT_PORT))) |=   1UL << LCD_BACKLIGHT_PIN
#define LCD_BACKLIGHT_OFF       *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + LCD_BACKLIGHT_PORT))) &= ~(1UL << LCD_BACKLIGHT_PIN)
#define LCD_BACKLIGHT_TOGGLE    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + LCD_BACKLIGHT_PORT))) ^=   1UL << LCD_BACKLIGHT_PIN

#define TEST_PORT	   GpioPortB
#define TEST_PIN       GpioPin1

#define TEST_ON        *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + TEST_PORT))) |=   1UL << TEST_PIN
#define TEST_OFF       *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + TEST_PORT))) &= ~(1UL << TEST_PIN)
#define TEST_TOGGLE    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + TEST_PORT))) ^=   1UL << TEST_PIN



void led_init(void);


#endif
