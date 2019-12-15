#ifndef _BSP_PWM_H
#define _BSP_PWM_H
#include "timer3.h"
#include "bsp_timer.h"
#include "bsp_gpio.h"
#include "bsp_it.h"
#define PWM_MAX         1600
#define PWM_MIN         0
/**********************************************º¯ÊýÉùÃ÷***********************************************/
void PWM_timer3(void);
void PWM_timer4(void);
void SetTIM1_PWM_Frequency(uint16_t TIM2_Period);
void SetTIM1_PWM_DutyCycle( uint16_t TIM1_Pulse,uint8_t ChannelNumber);
void PWM_Contorl_LED(uint8_t ch,uint8_t ch_state);
void Tim3Int(void);
void Tim1Int(void);
void PWM_timer1(void);
#endif