#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H

#include "stm8s_tim1.h"
#include "stm8s_tim2.h"

/**********************************************��������***********************************************/
void TIMER1_Init(void);                 //��ʱ����ʼ��������
void time1_10ms(uint32_t MStime);        //��ʱ1m������
void TimingDelay_Decrement(void);       //�ݼ��������жϵ��á�
void timer2_init(void);

#endif

