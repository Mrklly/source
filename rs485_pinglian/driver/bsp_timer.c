#include "bsp_timer.h"

static uint32_t TimingDelay;  

void TIMER2_Init(void)
{
  uint16_t                  u16ArrValue;
  uint16_t                  u16CntValue;
  stc_bt_mode0_config_t     stcBtBaseCfg;
  
  
  DDL_ZERO_STRUCT(stcBtBaseCfg);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE); //Base Timer����ʱ��ʹ��
  
  
  stcBtBaseCfg.enWorkMode = BtWorkMode0;                  //��ʱ��ģʽ
  stcBtBaseCfg.enCT       = BtTimer;                      //��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK
  stcBtBaseCfg.enPRS      = BtPCLKDiv256;                 //PCLK/256
  stcBtBaseCfg.enCntMode  = Bt16bitArrMode;               //�Զ�����16λ������/��ʱ��
  stcBtBaseCfg.bEnTog     = FALSE;
  stcBtBaseCfg.bEnGate    = FALSE;
  stcBtBaseCfg.enGateP    = BtGatePositive;
  
  stcBtBaseCfg.pfnTim2Cb  = Tim2Int;                      //�жϺ������

  u16ArrValue = 65348u;                                   // period = (0xffff + 1 - ARR)/(PCLK / DIV)
  Bt_M0_ARRSet(TIM2, u16ArrValue);                        //��������ֵ
  
  u16CntValue = 65348u;
  
  Bt_M0_Cnt16Set(TIM2, u16CntValue);                      //���ü�����ֵ

  EnableNvic(TIM2_IRQn, IrqLevel3, TRUE);                 //TIM0�ж�ʹ��
  
  Bt_Mode0_EnableIrq(TIM2);                               //ʹ��TIM0�ж�(ģʽ0ʱֻ��һ���ж�)

  Bt_Mode0_Init(TIM2, &stcBtBaseCfg);                     //TIM0 ��ģʽ0���ܳ�ʼ��
  
  Bt_ClearIntFlag(TIM2,BtUevIrq);                         //���жϱ�־

  Bt_M0_Run(TIM2);                                        //TIM0 ���С�
}

void time2_xms(uint32_t MStime)
{
  TimingDelay=MStime;
  while(TimingDelay != 0)
  {
    
  }
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

void Delay(uint32_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}
