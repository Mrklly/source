#ifndef _BSP_PWM_H_
#define _BSP_PWM_H_


#include "stm8s.h"


#define PWM_DUTY_H(i) (TIM1->CCR##i##H)
#define PWM_DUTY_L(i) (TIM1->CCR##i##L)


extern volatile uint16_t count[4];
extern uint16_t pwm[4];

void pwm_init(void);
void SetTIM1_PWM_Frequency(uint16_t TIM2_Period);
void SetTIM1_PWM_DutyCycle( uint16_t TIM1_Pulse,uint8_t ChannelNumber);
void PWM_Contorl_LED(uint8_t ch,uint8_t ch_state);
void PWM_channel(uint8_t ch,uint8_t ch_state);


#endif
