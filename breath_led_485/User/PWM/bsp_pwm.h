#ifndef _BSP_PWM_H
#define _BSP_PWM_H

#include "stm8s.h"

extern volatile uint16_t count[4];


/**********************************************º¯ÊýÉùÃ÷***********************************************/
void PWM_Init(void);
void SetTIM1_PWM_Frequency(uint16_t TIM2_Period);
void SetTIM1_PWM_DutyCycle( uint16_t TIM1_Pulse,uint8_t ChannelNumber);
void PWM_Contorl_LED(uint8_t ch,uint8_t ch_state);
void PWM_channel(uint8_t ch,uint8_t ch_state);
#endif