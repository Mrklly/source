#ifndef _BSP_PWM_H_
#define _BSP_PWM_H_


#include "hc32_timer3.h"

#include "bsp_timer.h"
#include "bsp_gpio.h"
#include "bsp_it.h"


#define PWM_MAX         1800
#define PWM_MIN         0

#define PWM_PORT_CH1    GpioPortA
#define PWM_PORT_CH2    GpioPortA
#define PWM_PORT_CH3    GpioPortB
#define PWM_PORT_CH4    GpioPortB

#define PWM_PIN_CH1     GpioPin6
#define PWM_PIN_CH2     GpioPin7
#define PWM_PIN_CH3     GpioPin0
#define PWM_PIN_CH4     GpioPin1


#define PWM_HIGH(PORT, PIN)    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) |=  (1 << PIN)
#define PWM_LOW(PORT, PIN)     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) &= ~(1 << PIN)
#define PWM_TOG(PORT, PIN)     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + PORT))) ^=  (1 << PIN)



void PWM_timer3(void);
void SetTIM1_PWM_Frequency(uint16_t TIM2_Period);
void SetTIM1_PWM_DutyCycle( uint16_t TIM1_Pulse,uint8_t ChannelNumber);
void PWM_Contorl_LED(uint8_t ch,uint8_t ch_state);
void Tim3Int(void);
void Tim1Int(void);
void PWM_timer1(void);
void pwm_to_io(void);
void io_to_pwm(void);



#endif

