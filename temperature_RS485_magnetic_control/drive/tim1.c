#include "tim1.h"
#include "led.h"

void Tim1_Init(void)
{
    TIM1_TimeBaseInit(16,TIM1_COUNTERMODE_UP,500,0);
    TIM1_ARRPreloadConfig(ENABLE);
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
    TIM1_Cmd(ENABLE);
}

