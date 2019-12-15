#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_


#include "hc32_gpio.h"


// PWM1 -> PA06 -> TIM1_CHA -> FUNC(4)
// PWM2 -> PA07 -> TIM3_CH0B -> FUNC(4)
// PWM3 -> PB00 -> TIM3_CH1B -> FUNC(2)
// PWM4 -> PB01 -> TIM3_CH2B -> FUNC(3)
// PWM5 -> PA10 -> TIM3_CH2A -> FUNC(2)
// PWM6 -> PA11 -> TIM4_CHB -> FUNC(7)
#define PORT_LED1       GpioPortA
#define PORT_LED2       GpioPortA
#define PORT_LED3       GpioPortB
#define PORT_LED4       GpioPortB
#define PORT_LED5       GpioPortA
#define PORT_LED6       GpioPortA

#define PIN_LED1        GpioPin6
#define PIN_LED2        GpioPin7
#define PIN_LED3        GpioPin0
#define PIN_LED4        GpioPin1
#define PIN_LED5        GpioPin10
#define PIN_LED6        GpioPin11


// BL1 -> PA05 -> TIM5_CHB -> FUNC(5)
// BL2 -> PB02 -> TIM4_CHA -> FUNC(4)
// BL3 -> PB10 -> TIM3_CH1A -> FUNC(5)
// BL4 -> PB11 -> TIM6_CHA -> FUNC(5)
// BL5 -> PA04 -> TIM5_CHA -> FUNC(5)
// BL6 -> PA00 -> TIM0_CHA -> FUNC(7)
#define BACK_LED1_PORT       GpioPortA
#define BACK_LED2_PORT       GpioPortB
#define BACK_LED3_PORT       GpioPortB
#define BACK_LED4_PORT       GpioPortB
#define BACK_LED5_PORT       GpioPortA
#define BACK_LED6_PORT       GpioPortA

#define BACK_LED1_PIN        GpioPin5
#define BACK_LED2_PIN        GpioPin2
#define BACK_LED3_PIN        GpioPin10
#define BACK_LED4_PIN        GpioPin11
#define BACK_LED5_PIN        GpioPin4
#define BACK_LED6_PIN        GpioPin0

#define RELAY_CH1_PIN        GpioPin6               
#define RELAY_CH2_PIN        GpioPin7               
#define RELAY_CH3_PIN        GpioPin8               
#define RELAY_CH4_PIN        GpioPin9               

#define RELAY_CH1_PORT       GpioPortB
#define RELAY_CH2_PORT       GpioPortB
#define RELAY_CH3_PORT       GpioPortB
#define RELAY_CH4_PORT       GpioPortB

#define NEAR_SENSOR_PORT     GpioPortC
#define NEAR_SENSOR_PIN      GpioPin13

#define PORT_TEST            GpioPortB
#define PIN_TEST             GpioPin12

#define TEST_IO_TOGGLE       M0P_GPIO->PBOUT_f.PB12 ^= 1

#define GPIO_HIGH(PORT, PIN)    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) |=  (1 << PIN)
#define GPIO_LOW(PORT, PIN)     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) &= ~(1 << PIN)
#define GPIO_TOG(PORT, PIN)     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) ^=  (1 << PIN)


#define NEAR_SENSOR_STA     M0P_GPIO->PCIN_f.PC13


void led_init(void);
void backlight_init(void);
void Relay_Init(void);
void near_sensor_init(void);
void test_io_init(void);


#endif

