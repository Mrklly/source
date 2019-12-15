#ifndef _DRV_LED_H_
#define _DRV_LED_H_

#include "hc32_gpio.h"


#define LED_RF_PORT     0
#define LED_RF_PIN      3

#define LED_RF_OFF      *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + LED_RF_PORT * GPIO_GPSZ)) |= (1UL << LED_RF_PIN)
#define LED_RF_ON       *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + LED_RF_PORT * GPIO_GPSZ)) &=~(1UL << LED_RF_PIN)
#define LED_RF_TOGGLE   *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + LED_RF_PORT * GPIO_GPSZ)) ^= (1UL << LED_RF_PIN)


#define LED_YEL_PORT     1
#define LED_YEL_PIN      4

#define LED_YEL_ON       *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + LED_YEL_PORT * GPIO_GPSZ)) |= (1UL << LED_YEL_PIN)
#define LED_YEL_OFF      *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + LED_YEL_PORT * GPIO_GPSZ)) &=~(1UL << LED_YEL_PIN)
#define LED_YEL_TOGGLE   *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + LED_YEL_PORT * GPIO_GPSZ)) ^= (1UL << LED_YEL_PIN)


#define LED_RED_PORT     1
#define LED_RED_PIN      5

#define LED_RED_ON       *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + LED_RED_PORT * GPIO_GPSZ)) |= (1UL << LED_RED_PIN)
#define LED_RED_OFF      *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + LED_RED_PORT * GPIO_GPSZ)) &=~(1UL << LED_RED_PIN)
#define LED_RED_TOGGLE   *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + LED_RED_PORT * GPIO_GPSZ)) ^= (1UL << LED_RED_PIN)


void hal_led_init(void);
void hal_swd_to_io(void);


#endif
