#include "bsp_timer.h"
static uint32_t TimingDelay;  
/*****************************************************************************************************
��������TIMER2_Init
���ܣ���ʱ��2��ʼ��
�������޲�����
����ֵ���޷���ֵ��
*****************************************************************************************************/
void TIMER2_Init(void)
{
  uint16_t                  u16ArrValue;
  uint16_t                  u16CntValue;
  stc_bt_mode0_config_t     stcBtBaseCfg;
  
  
  DDL_ZERO_STRUCT(stcBtBaseCfg);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE); 
  
  
  stcBtBaseCfg.enWorkMode = BtWorkMode0;                  
  stcBtBaseCfg.enCT       = BtTimer;                      
  stcBtBaseCfg.enPRS      = BtPCLKDiv256;                 
  stcBtBaseCfg.enCntMode  = Bt16bitArrMode;               
  stcBtBaseCfg.bEnTog     = FALSE;
  stcBtBaseCfg.bEnGate    = FALSE;
  stcBtBaseCfg.enGateP    = BtGatePositive;
  
  stcBtBaseCfg.pfnTim2Cb  = Tim2Int;                     
  
  Bt_Mode0_Init(TIM2, &stcBtBaseCfg);                     
  
  Bt_ClearIntFlag(TIM2,BtUevIrq);                         
  
  EnableNvic(TIM2_IRQn, IrqLevel3, TRUE);                 
  
  Bt_Mode0_EnableIrq(TIM2);                               
  
  u16ArrValue = 65348u;                                  
  Bt_M0_ARRSet(TIM2, u16ArrValue);                       
  
  u16CntValue = 65348u;
  
  Bt_M0_Cnt16Set(TIM2, u16CntValue);                      
  
  Bt_M0_Run(TIM2);                                        
}

void TIMER0_Init(void)
{
  uint16_t                  u16ArrValue;
  uint16_t                  u16CntValue;
  stc_bt_mode0_config_t     stcBtBaseCfg;
  
  
  DDL_ZERO_STRUCT(stcBtBaseCfg);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE); 
  
  
  stcBtBaseCfg.enWorkMode = BtWorkMode0;                  
  stcBtBaseCfg.enCT       = BtTimer;                      
  stcBtBaseCfg.enPRS      = BtPCLKDiv256;                 
  stcBtBaseCfg.enCntMode  = Bt16bitArrMode;              
  stcBtBaseCfg.bEnTog     = FALSE;
  stcBtBaseCfg.bEnGate    = FALSE;
  stcBtBaseCfg.enGateP    = BtGatePositive;
  
  stcBtBaseCfg.pfnTim0Cb  = Tim0Int;                     
  
  Bt_Mode0_Init(TIM0, &stcBtBaseCfg);                     
  
  Bt_ClearIntFlag(TIM0,BtUevIrq);                         
  
  EnableNvic(TIM0_IRQn, IrqLevel3, TRUE);                 
  
  Bt_Mode0_EnableIrq(TIM0);                               
  
  u16ArrValue = 65476u;                                   
  Bt_M0_ARRSet(TIM0, u16ArrValue);                       
  
  u16CntValue = 65476u;
  
  Bt_M0_Cnt16Set(TIM0, u16CntValue);                      
  Bt_M0_Stop(TIM0);    
}
/*****************************************************************************************************
��������time2_xms
���ܣ�ʵ�ֶ�ʱ1ms
����������1������Ҫ����ʱ���С��ʱ�䵥λMS��
����ֵ���޷���ֵ��
*****************************************************************************************************/
void time2_xms(uint32_t MStime)
{
  TimingDelay=MStime;
  while(TimingDelay != 0)
  {
    
  }
}
/*****************************************************************************************************
��������TimingDelay_Decrement
���ܣ���ʱʱ��ݼ���������������ʱ���жϵ��á�ʵ�ֵݼ���
�������޲�����
����ֵ���޷���ֵ��
*****************************************************************************************************/
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
/*****************************************************************************************************
��������Delay
���ܣ���ʱʱ��ݼ�������ʵ�ֵݼ���
����������1��Ҫ��ʱ�ĵݼ�ֵ��
����ֵ���޷���ֵ��
*****************************************************************************************************/
void Delay(uint32_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}
/********************************************timer.C*************************************************/

