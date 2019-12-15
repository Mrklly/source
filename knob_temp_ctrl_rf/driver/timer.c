#include "timer.h"
#include "drv_led.h"

void timer3_init(void)
{
    uint16_t                    u16ArrValue;
    uint16_t                    u16CntValue;
    stc_tim3_mode0_config_t     stcTim3BaseCfg;

    DDL_ZERO_STRUCT(stcTim3BaseCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE); //Base Timer����ʱ��ʹ��

    stcTim3BaseCfg.enWorkMode = Tim3WorkMode0;              //��ʱ��ģʽ
    stcTim3BaseCfg.enCT       = Tim3Timer;                  //��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK
    stcTim3BaseCfg.enPRS      = Tim3PCLKDiv256;              //PCLK/256
    stcTim3BaseCfg.enCntMode  = Tim316bitArrMode;           //�Զ�����16λ������/��ʱ��
    stcTim3BaseCfg.bEnTog     = FALSE;
    stcTim3BaseCfg.bEnGate    = FALSE;
    stcTim3BaseCfg.enGateP    = Tim3GatePositive;

    stcTim3BaseCfg.pfnTim3Cb  = timer3_interrupt;                    //�жϺ������

    Tim3_Mode0_Init(&stcTim3BaseCfg);                       //TIM3 ��ģʽ0���ܳ�ʼ��

    u16ArrValue = 46768u;

    Tim3_M0_ARRSet(u16ArrValue);                            // period = (0xffff + 1 - ARR)/(PCLK / DIV)

    u16CntValue = 0;

    Tim3_M0_Cnt16Set(u16CntValue);                          //���ü�����ֵ

    Tim3_ClearIntFlag(Tim3UevIrq);                          //���жϱ�־
    Tim3_Mode0_EnableIrq();                                 //ʹ��TIM3�ж�(ģʽ0ʱֻ��һ���ж�)
    EnableNvic(TIM3_IRQn, IrqLevel3, TRUE);                 //TIM3 ���ж�
}

void timer3_interrupt(void)
{
    static uint16_t i;

    if(TRUE == Tim3_GetIntFlag(Tim3UevIrq)) {

//        GPIO_TOG(TEST_PORT, TEST_PIN);

        i++;

        Tim3_ClearIntFlag(Tim3UevIrq);
    }
}

