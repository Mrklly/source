#include "bsp_timer.h"


void timer2_init(void)
{
    uint16_t                  u16ArrValue;
    uint16_t                  u16CntValue;
    stc_bt_mode0_config_t     stcBtBaseCfg;

    DDL_ZERO_STRUCT(stcBtBaseCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE);

    stcBtBaseCfg.enWorkMode = BtWorkMode0;
    stcBtBaseCfg.enCT       = BtTimer;
    stcBtBaseCfg.enPRS      = BtPCLKDiv8;
    stcBtBaseCfg.enCntMode  = Bt16bitArrMode;
    stcBtBaseCfg.bEnTog     = FALSE;
    stcBtBaseCfg.bEnGate    = FALSE;
    stcBtBaseCfg.enGateP    = BtGatePositive;

    stcBtBaseCfg.pfnTim2Cb  = timer2_int;

    Bt_Mode0_Init(TIM2, &stcBtBaseCfg);

    Bt_ClearIntFlag(TIM2, BtUevIrq);

    EnableNvic(TIM2_IRQn, IrqLevel3, TRUE);

    Bt_Mode0_EnableIrq(TIM2);

    u16ArrValue = 35536;                       // period = (0xffff + 1 - ARR)/(PCLK / DIV)
    Bt_M0_ARRSet(TIM2, u16ArrValue);

    u16CntValue = 35536;

    Bt_M0_Cnt16Set(TIM2, u16CntValue);

    Bt_M0_Run(TIM2);
}


