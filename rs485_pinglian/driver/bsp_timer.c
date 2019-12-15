#include "bsp_timer.h"

static uint32_t TimingDelay;  

void TIMER2_Init(void)
{
  uint16_t                  u16ArrValue;
  uint16_t                  u16CntValue;
  stc_bt_mode0_config_t     stcBtBaseCfg;
  
  
  DDL_ZERO_STRUCT(stcBtBaseCfg);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE); //Base Timer外设时钟使能
  
  
  stcBtBaseCfg.enWorkMode = BtWorkMode0;                  //定时器模式
  stcBtBaseCfg.enCT       = BtTimer;                      //定时器功能，计数时钟为内部PCLK
  stcBtBaseCfg.enPRS      = BtPCLKDiv256;                 //PCLK/256
  stcBtBaseCfg.enCntMode  = Bt16bitArrMode;               //自动重载16位计数器/定时器
  stcBtBaseCfg.bEnTog     = FALSE;
  stcBtBaseCfg.bEnGate    = FALSE;
  stcBtBaseCfg.enGateP    = BtGatePositive;
  
  stcBtBaseCfg.pfnTim2Cb  = Tim2Int;                      //中断函数入口

  u16ArrValue = 65348u;                                   // period = (0xffff + 1 - ARR)/(PCLK / DIV)
  Bt_M0_ARRSet(TIM2, u16ArrValue);                        //设置重载值
  
  u16CntValue = 65348u;
  
  Bt_M0_Cnt16Set(TIM2, u16CntValue);                      //设置计数初值

  EnableNvic(TIM2_IRQn, IrqLevel3, TRUE);                 //TIM0中断使能
  
  Bt_Mode0_EnableIrq(TIM2);                               //使能TIM0中断(模式0时只有一个中断)

  Bt_Mode0_Init(TIM2, &stcBtBaseCfg);                     //TIM0 的模式0功能初始化
  
  Bt_ClearIntFlag(TIM2,BtUevIrq);                         //清中断标志

  Bt_M0_Run(TIM2);                                        //TIM0 运行。
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
