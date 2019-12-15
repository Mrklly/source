#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_


#include "hc32_gpio.h"


#define BACK_LED1_PIN        GpioPin5                   //PA05_LED1
#define BACK_LED2_PIN        GpioPin2                   //PB02_LED2
#define BACK_LED3_PIN        GpioPin10                  //PB10_LED3
#define BACK_LED4_PIN        GpioPin11                  //PB11_LED4
#define BACK_LED5_PIN        GpioPin4
#define BACK_LED6_PIN        GpioPin0

#define BACK_LED1_PORT       GpioPortA
#define BACK_LED2_PORT       GpioPortB
#define BACK_LED3_PORT       GpioPortB
#define BACK_LED4_PORT       GpioPortB
#define BACK_LED5_PORT       GpioPortA
#define BACK_LED6_PORT       GpioPortA

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

#define GPIO_HIGH(PORT, PIN)    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) |=  (1 << PIN)
#define GPIO_LOW(PORT, PIN)     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) &= ~(1 << PIN)
#define GPIO_TOG(PORT, PIN)     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) ^=  (1 << PIN)


#define NEAR_SENSOR_STA     M0P_GPIO->PCIN_f.PC13


void backlight_init(void);
void Relay_Init(void);
void near_sensor_init(void);


#endif

