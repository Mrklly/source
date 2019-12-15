#ifndef _BSP_PWM_H
#define _BSP_PWM_H


#include "hc32_timer3.h"

#include "bsp_timer.h"
#include "bsp_gpio.h"
#include "bsp_it.h"

#include "global_variables.h"


#define PWM_MAX         1600
#define PWM_MIN         0


#define PWM_LED_0   M0P_TIM1_MODE23->CCR0A_f.CCR0A
#define PWM_LED_1   M0P_TIM3_MODE23->CCR0B_f.CCR0B
#define PWM_LED_2   M0P_TIM3_MODE23->CCR1B_f.CCR1B
#define PWM_LED_3   M0P_TIM3_MODE23->CCR2B_f.CCR2B
#define PWM_LED_4   M0P_TIM3_MODE23->CCR2A_f.CCR2A
#define PWM_LED_5   M0P_TIM4->GCMBR_f.GCMB

#define PWM_BACKLIGHT_0   M0P_TIM5->GCMBR_f.GCMB
#define PWM_BACKLIGHT_1   M0P_TIM4->GCMAR_f.GCMA
#define PWM_BACKLIGHT_2   M0P_TIM3_MODE23->CCR1A_f.CCR1A
#define PWM_BACKLIGHT_3   M0P_TIM6->GCMAR_f.GCMA
#define PWM_BACKLIGHT_4   M0P_TIM5->GCMAR_f.GCMA
#define PWM_BACKLIGHT_5   M0P_TIM0_MODE23->CCR0A_f.CCR0A


void PWM_timer1(void);
void PWM_timer3(void);
void pwm_init_timer4(void);
void backlight_tim5_init(void);
void backlight_tim0_init(void);
void backlight_tim4_init(void);
void backlight_tim3_init(void);
void backlight_tim6_init(void);
void SetTIM1_PWM_Frequency(uint16_t TIM2_Period);
void led_set_pwm(uint8_t ch, uint16_t pulse);
void Tim3Int(void);
void Tim1Int(void);

#if BACKLIGHT_PWM
void backlight_set_pwm(uint8_t ch, uint16_t pulse);
#endif

static inline void backlight_set_all(uint16_t pulse);

void backlight_set_zero(void);
void backlight_set_mid(void);
void backlight_set_full(void);


#endif

