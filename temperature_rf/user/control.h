#ifndef _CONTROL_H_
#define _CONTROL_H_


#include "hc32_gpio.h"


#define CTRL_VAVLE_PORT     GpioPortC
#define CTRL_VAVLE_PIN      GpioPin3

#define CTRL_FAN_HIGH_PORT  GpioPortC
#define CTRL_FAN_HIGH_PIN   GpioPin0

#define CTRL_FAN_MID_PORT   GpioPortC
#define CTRL_FAN_MID_PIN    GpioPin1

#define CTRL_FAN_LOW_PORT   GpioPortC
#define CTRL_FAN_LOW_PIN    GpioPin2

#define CTRL_JK5_PORT       GpioPortC
#define CTRL_JK5_PIN        GpioPin13


#define CTRL_VAVLE_OPEN()     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + CTRL_VAVLE_PORT)))    |=   1UL << CTRL_VAVLE_PIN
#define CTRL_VAVLE_CLOSE()    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + CTRL_VAVLE_PORT)))    &= ~(1UL << CTRL_VAVLE_PIN)

#define CTRL_JK5_OPEN()     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + CTRL_JK5_PORT)))        |=   1UL << CTRL_JK5_PIN
#define CTRL_JK5_CLOSE()    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + CTRL_JK5_PORT)))        &= ~(1UL << CTRL_JK5_PIN)

#define CTRL_FAN_HIGH()       *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + CTRL_FAN_HIGH_PORT))) |=   1UL << CTRL_FAN_HIGH_PIN;\
                              *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + CTRL_FAN_MID_PORT ))) &= ~(1UL << CTRL_FAN_MID_PIN);\
                              *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + CTRL_FAN_LOW_PORT ))) &= ~(1UL << CTRL_FAN_LOW_PIN)

#define CTRL_FAN_MID()        *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + CTRL_FAN_HIGH_PORT))) &= ~(1UL << CTRL_FAN_HIGH_PIN);\
                              *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + CTRL_FAN_MID_PORT ))) |=   1UL << CTRL_FAN_MID_PIN;\
                              *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + CTRL_FAN_LOW_PORT ))) &= ~(1UL << CTRL_FAN_LOW_PIN)

#define CTRL_FAN_LOW()        *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + CTRL_FAN_HIGH_PORT))) &= ~(1UL << CTRL_FAN_HIGH_PIN);\
                              *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + CTRL_FAN_MID_PORT ))) &= ~(1UL << CTRL_FAN_MID_PIN );\
                              *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + CTRL_FAN_LOW_PORT ))) |=   1UL << CTRL_FAN_LOW_PIN

#define CTRL_FAN_CLZ()        *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + CTRL_FAN_HIGH_PORT))) &= ~(1UL << CTRL_FAN_HIGH_PIN);\
                              *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + CTRL_FAN_MID_PORT ))) &= ~(1UL << CTRL_FAN_MID_PIN );\
                              *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + CTRL_FAN_LOW_PORT ))) &= ~(1UL << CTRL_FAN_LOW_PIN )

#define CTRL_PWR_OFF()        CTRL_FAN_CLZ()


void control_init(void);
void control_valve(void);
void control_fan(void);


#endif

