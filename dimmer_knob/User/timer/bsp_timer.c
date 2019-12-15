#include "bsp_timer.h"
static uint32_t TimingDelay;  
/*****************************************************************************************************
程序名：TIMER2_Init
功能：定时器2初始化
参数：无参数。
返回值：无返回值。
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
程序名：time2_xms
功能：实现定时1ms
参数：参数1：传入要定的时间大小，时间单位MS。
返回值：无返回值。
*****************************************************************************************************/
void time2_xms(uint32_t MStime)
{
  TimingDelay=MStime;
  while(TimingDelay != 0)
  {
    
  }
}
/*****************************************************************************************************
程序名：TimingDelay_Decrement
功能：延时时间递减函数。函数被定时器中断调用。实现递减。
参数：无参数。
返回值：无返回值。
*****************************************************************************************************/
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
/*****************************************************************************************************
程序名：Delay
功能：延时时间递减函数。实现递减。
参数：参数1：要延时的递减值。
返回值：无返回值。
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

