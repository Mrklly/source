#include "system_timer.h"

#include "stm8s_tim1.h"

void timer1_init()
{
    TIM1->PSCRH=0;//һ��Ҫ��д�߰�λ
    TIM1->PSCRL=4;//1��Ƶ����ʱ��ʱ�ӵ���ϵͳʱ��=16m

    TIM1->ARRH=0X3E;//һ��Ҫ��װ�߰�λ����װ�Ͱ�λ
    TIM1->ARRL=0X80;//5ms��װֵ8000��

    TIM1->CNTRH=0;
    TIM1->CNTRL=0;//�б�Ҫ����¼�����

    TIM1->IER |= 1<<0;//ʹ��tim1�����ж�
    TIM1->SR1 |= 1<<0;//���tim1�����жϱ�־

    TIM1->CR1 |= 1<<7;//������װ��ʹ�ܶ�ʱ��
    TIM1->CR1 |= 1<<4;//ѡ�����¼���ģʽ
    TIM1->CR1 |= 1<<0;//ʹ�ܼ�����
}
