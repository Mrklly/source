#ifndef _LED_H_
#define _LED_H_

#include "global_variables.h"

#define LED_L_PORT		GPIOD
#define LED_L_PIN		GPIO_PIN_3

#define LED_L_ON        LED_L_PORT->ODR |= LED_L_PIN
#define LED_L_OFF       LED_L_PORT->ODR &=~LED_L_PIN
#define LED_L_TOGGLE    LED_L_PORT->ODR ^= LED_L_PIN

#define LED_R_PORT		GPIOD
#define LED_R_PIN		GPIO_PIN_2

#define LED_R_ON        LED_R_PORT->ODR |= LED_R_PIN
#define LED_R_OFF       LED_R_PORT->ODR &=~LED_R_PIN
#define LED_R_TOGGLE    LED_R_PORT->ODR ^= LED_R_PIN

#define LED_RF_PORT		GPIOC
#define LED_RF_PIN		GPIO_PIN_3

#define LED_RF_OFF      LED_RF_PORT->ODR |= LED_RF_PIN
#define LED_RF_ON       LED_RF_PORT->ODR &=~LED_RF_PIN
#define LED_RF_TOGGLE   LED_RF_PORT->ODR ^= LED_RF_PIN



void led_init();

#endif

