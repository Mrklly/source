#ifndef _DRV_LED_H_
#define _DRV_LED_H_

#include "hc32_gpio.h"


#define PORT_LED_1     0
#define PIN_LED_1      2

#define PORT_LED_2     2
#define PIN_LED_2      6

#define PORT_LED_3     2
#define PIN_LED_3      5

#define PORT_LED_4     2
#define PIN_LED_4      4

#define PORT_LED_NET   0
#define PIN_LED_NET    1


#define LED_1_OUT      M0P_GPIO->P0OUT_f.P02
#define LED_1_ON       M0P_GPIO->P0OUT_f.P02 = 1
#define LED_1_OFF      M0P_GPIO->P0OUT_f.P02 = 0

#define LED_2_OUT      M0P_GPIO->P2OUT_f.P26
#define LED_2_ON       M0P_GPIO->P2OUT_f.P26 = 1
#define LED_2_OFF      M0P_GPIO->P2OUT_f.P26 = 0

#define LED_3_OUT      M0P_GPIO->P2OUT_f.P25
#define LED_3_ON       M0P_GPIO->P2OUT_f.P25 = 1
#define LED_3_OFF      M0P_GPIO->P2OUT_f.P25 = 0

#define LED_4_OUT      M0P_GPIO->P2OUT_f.P24
#define LED_4_ON       M0P_GPIO->P2OUT_f.P24 = 1
#define LED_4_OFF      M0P_GPIO->P2OUT_f.P24 = 0

#define LED_NET_OUT    M0P_GPIO->P0OUT_f.P01
#define LED_NET_ON     M0P_GPIO->P0OUT_f.P01 = 0
#define LED_NET_OFF    M0P_GPIO->P0OUT_f.P01 = 1
#define LED_NET_TOG    M0P_GPIO->P0OUT_f.P01 ^= 1



#define GPIO_HIG(PORT, PIN)        *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + PORT * GPIO_GPSZ)) |= (1UL << PIN)
#define GPIO_LOW(PORT, PIN)        *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + PORT * GPIO_GPSZ)) &=~(1UL << PIN)
#define GPIO_TOG(PORT, PIN)        *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + PORT * GPIO_GPSZ)) ^= (1UL << PIN)


void hal_led_init(void);
void hal_swd_to_io(void);


#endif

