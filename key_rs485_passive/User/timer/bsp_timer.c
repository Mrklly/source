#include "bsp_timer.h"
#include "bsp_wwdg.h"


static u32 TimingDelay;


//void TIMER1_Init(void)
//{
//  TIM1_DeInit();                                        //��ʼ������λ״̬��
//   /* ��ʼ��TIM1Ϊ�ⲿʱ��16��Ƶ ���ϼ���ģʽ ��������1000 ������ʼֵ0 -- 1ms��һ���ж�*/
//  TIM1_TimeBaseInit(16, TIM1_COUNTERMODE_UP, 1000, 0);
//  TIM1_SetCounter(0);                                   // ����������ֵ��Ϊ0��
//  TIM1_ARRPreloadConfig(ENABLE);	                //Ԥװ��ʹ�� ��
//  TIM1_ITConfig(TIM1_IT_UPDATE ,ENABLE);	        // ���������ϼ���/���¼�����������ж� 
//  TIM1_Cmd(ENABLE);	                                //ʹ�ܼ�������
//}

void TimingDelay_Decrement(void)
{
    if(TimingDelay != 0x00) {
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

void timer4_init(void)
{
    TIM4_DeInit();
    TIM4_TimeBaseInit(TIM4_PRESCALER_64, 250);              // period = T*PCLK/DIV = 10^-3 * 16 * 10^6 / 64 = 250
    TIM4_SetCounter(0);
    TIM4_ARRPreloadConfig(ENABLE);
    TIM4_ITConfig(TIM4_IT_UPDATE ,ENABLE);
    TIM4_Cmd(ENABLE);
}

void time4_delay_ms(uint32_t ms)
{
    TimingDelay = ms;
    while(TimingDelay != 0){
        Refresh_WWDG_Window();
    }
}
