#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H
#include "bsp_it.h"
/**********************************************��������***********************************************/
void TIMER2_Init(void);                 //��ʱ����ʼ��������
void time2_xms(uint32_t MStime);        //��ʱ1m������
void TimingDelay_Decrement(void);       //�ݼ��������жϵ��á�
void Delay(uint32_t nCount);
#endif
