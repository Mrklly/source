#include "system_timer.h"

#include "stm8s_tim1.h"

void timer1_init()
{
    TIM1->PSCRH=0;//一定要先写高八位
    TIM1->PSCRL=4;//1分频，定时器时钟等于系统时钟=16m

    TIM1->ARRH=0X3E;//一定要先装高八位，再装低八位
    TIM1->ARRL=0X80;//5ms重装值8000，

    TIM1->CNTRH=0;
    TIM1->CNTRL=0;//有必要清除下计数器

    TIM1->IER |= 1<<0;//使能tim1更新中断
    TIM1->SR1 |= 1<<0;//清除tim1更新中断标志

    TIM1->CR1 |= 1<<7;//允许重装，使能定时器
    TIM1->CR1 |= 1<<4;//选择向下计数模式
    TIM1->CR1 |= 1<<0;//使能计数器
}
