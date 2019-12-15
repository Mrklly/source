#include "timer.h"
#include "drv_led.h"

void timer3_init(void)
{
    uint16_t                    u16ArrValue;
    uint16_t                    u16CntValue;
    stc_tim3_mode0_config_t     stcTim3BaseCfg;

    DDL_ZERO_STRUCT(stcTim3BaseCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE); //Base Timer外设时钟使能

    stcTim3BaseCfg.enWorkMode = Tim3WorkMode0;              //定时器模式
    stcTim3BaseCfg.enCT       = Tim3Timer;                  //定时器功能，计数时钟为内部PCLK
    stcTim3BaseCfg.enPRS      = Tim3PCLKDiv256;              //PCLK/256
    stcTim3BaseCfg.enCntMode  = Tim316bitArrMode;           //自动重载16位计数器/定时器
    stcTim3BaseCfg.bEnTog     = FALSE;
    stcTim3BaseCfg.bEnGate    = FALSE;
    stcTim3BaseCfg.enGateP    = Tim3GatePositive;

    stcTim3BaseCfg.pfnTim3Cb  = timer3_interrupt;                    //中断函数入口

    Tim3_Mode0_Init(&stcTim3BaseCfg);                       //TIM3 的模式0功能初始化

    u16ArrValue = 46768u;

    Tim3_M0_ARRSet(u16ArrValue);                            // period = (0xffff + 1 - ARR)/(PCLK / DIV)

    u16CntValue = 0;

    Tim3_M0_Cnt16Set(u16CntValue);                          //设置计数初值

    Tim3_ClearIntFlag(Tim3UevIrq);                          //清中断标志
    Tim3_Mode0_EnableIrq();                                 //使能TIM3中断(模式0时只有一个中断)
    EnableNvic(TIM3_IRQn, IrqLevel3, TRUE);                 //TIM3 开中断
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

