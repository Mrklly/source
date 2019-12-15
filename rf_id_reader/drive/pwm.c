#include "pwm.h"
#include "global_variables.h"

#define CCR1_Val  ((u16)DIM_DEVICE)
#define CCR2_Val  ((u16)DIM_DEVICE)
#define CCR3_Val  ((u16)DIM_DEVICE)
#define CCR4_Val  ((u16)DIM_DEVICE)

gradual		gra;

void PWM_Init(void)
{
    gra.zkb = DIM_DEVICE;
    TIM1_DeInit();
    TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, DIM_DEVICE, 0);
    TIM1_OC1Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE, CCR1_Val, TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET, TIM1_OCNIDLESTATE_RESET);
    TIM1_Cmd(ENABLE);
    TIM1_CtrlPWMOutputs(ENABLE);
}

void gradual_hander(void)
{
    if(gra.v < gra.zkb) {
        gra.zkb--;
    } else if(gra.v > gra.zkb) {
        gra.zkb++;
    }

    TIM1_SetCompare1(gra.zkb);
}
