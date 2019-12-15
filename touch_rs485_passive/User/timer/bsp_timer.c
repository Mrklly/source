#include "bsp_timer.h"
#include "bsp_wwdg.h"
static u32 TimingDelay; 


void TIMER1_Init(void)
{
  TIM1_DeInit();                                        //初始化到复位状态。
   /* 初始化TIM1为外部时钟16分频 向上计数模式 计数周期1000 计数初始值0 -- 1ms进一次中断*/
  TIM1_TimeBaseInit(16, TIM1_COUNTERMODE_UP, 10000, 0);
  TIM1_SetCounter(0);                                   // 将计数器初值设为0。
  TIM1_ARRPreloadConfig(ENABLE);	                //预装载使能 。
  TIM1_ITConfig(TIM1_IT_UPDATE ,ENABLE);	        // 计数器向上计数/向下计数溢出更新中断 
  TIM1_Cmd(ENABLE);	                                //使能计数器。
}

void time1_10ms(uint32_t MStime)
{
  TimingDelay=MStime;
  while(TimingDelay != 0)
  {
    Refresh_WWDG_Window();
  }
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {  
    TimingDelay--;
  }
}

void timer2_init(void)                                    // 2000ms irq
{
    TIM2_DeInit();
    TIM2_ITConfig(TIM2_IT_UPDATE ,DISABLE);
    TIM2_TimeBaseInit(TIM2_PRESCALER_16384, 1953);          // period = T*PCLK/DIV
    TIM2_UpdateRequestConfig(TIM2_UPDATESOURCE_REGULAR);
    TIM2_ARRPreloadConfig(ENABLE);
    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
    TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
    TIM2_ITConfig(TIM2_IT_UPDATE ,ENABLE);
    TIM2_SetCounter(0);
    TIM2_Cmd(ENABLE);
}

