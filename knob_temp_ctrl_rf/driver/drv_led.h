#ifndef _DRV_LED_H_
#define _DRV_LED_H_


#include "hc32_gpio.h"


#define GPIO_HIGH(PORT, PIN)    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) |=   1 << PIN
#define GPIO_LOW(PORT, PIN)     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) &= ~(1 << PIN)
#define GPIO_TOG(PORT, PIN)     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) ^=   1 << PIN


#if TEMP_CTRL_KNOB
#define LED_PORT_R              GpioPortB
#define LED_PORT_G              GpioPortA
#define LED_PORT_B              GpioPortB
#define LED_PIN_R               GpioPin7
#define LED_PIN_G               GpioPin3
#define LED_PIN_B               GpioPin8
#elif TEMP_CTRL_TOUCH
#define LED_INDICATOR_PORT		GpioPortD
#define LED_INDICATOR_PIN       GpioPin4
#endif


#if TEMP_CTRL_KNOB
#define LCD_BACKLIGHT_PORT	    GpioPortC
#define LCD_BACKLIGHT_PIN       GpioPin13
#elif TEMP_CTRL_TOUCH
#define LCD_BACKLIGHT_PORT	    GpioPortD
#define LCD_BACKLIGHT_PIN       GpioPin5
#endif


#define TEST_PORT	            GpioPortC
#define TEST_PIN                GpioPin14


#define LCD_BACKLIGHT_ON        *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + LCD_BACKLIGHT_PORT))) |=   1UL << LCD_BACKLIGHT_PIN
#define LCD_BACKLIGHT_OFF       *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + LCD_BACKLIGHT_PORT))) &= ~(1UL << LCD_BACKLIGHT_PIN)
#define LCD_BACKLIGHT_TOGGLE    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + LCD_BACKLIGHT_PORT))) ^=   1UL << LCD_BACKLIGHT_PIN


#if TEMP_CTRL_KNOB
#define RGB_COLOR_RED           GPIO_HIGH(LED_PORT_R, LED_PIN_R); \
                                GPIO_LOW (LED_PORT_G, LED_PIN_G); \
                                GPIO_LOW (LED_PORT_B, LED_PIN_B)

#define RGB_COLOR_GREEN         GPIO_LOW (LED_PORT_R, LED_PIN_R); \
                                GPIO_HIGH(LED_PORT_G, LED_PIN_G); \
                                GPIO_LOW (LED_PORT_B, LED_PIN_B)

#define RGB_COLOR_BLUE          GPIO_LOW (LED_PORT_R, LED_PIN_R); \
                                GPIO_LOW (LED_PORT_G, LED_PIN_G); \
                                GPIO_HIGH(LED_PORT_B, LED_PIN_B)

#define RGB_COLOR_CYAN          GPIO_LOW (LED_PORT_R, LED_PIN_R); \
                                GPIO_HIGH(LED_PORT_G, LED_PIN_G); \
                                GPIO_HIGH (LED_PORT_B, LED_PIN_B)

#define RGB_COLOR_YELLOW        GPIO_HIGH(LED_PORT_R, LED_PIN_R); \
                                GPIO_HIGH(LED_PORT_G, LED_PIN_G); \
                                GPIO_LOW (LED_PORT_B, LED_PIN_B)

#define RGB_COLOR_MAGENTA       GPIO_HIGH(LED_PORT_R, LED_PIN_R); \
                                GPIO_LOW (LED_PORT_G, LED_PIN_G); \
                                GPIO_HIGH(LED_PORT_B, LED_PIN_B)

#define RGB_COLOR_WHITE         GPIO_HIGH(LED_PORT_R, LED_PIN_R); \
                                GPIO_HIGH(LED_PORT_G, LED_PIN_G); \
                                GPIO_HIGH(LED_PORT_B, LED_PIN_B)

#define RGB_COLOR_BLACK         GPIO_LOW(LED_PORT_R, LED_PIN_R); \
                                GPIO_LOW(LED_PORT_G, LED_PIN_G); \
                                GPIO_LOW(LED_PORT_B, LED_PIN_B)
#elif TEMP_CTRL_TOUCH

#define LED_INDICATOR_ON        GPIO_HIGH(LED_INDICATOR_PORT, LED_INDICATOR_PIN)
#define LED_INDICATOR_OFF       GPIO_LOW(LED_INDICATOR_PORT, LED_INDICATOR_PIN)

#endif


void led_init(void);

#if TEMP_CTRL_KNOB
void rgb_led_test(void);
#endif


#endif

