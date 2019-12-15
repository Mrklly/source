#include "drv_clock.h"

void hal_clock_init(void)
{
    stc_clk_config_t stcClkCfg;

    Clk_SetPeripheralGate(ClkPeripheralTick,TRUE);
    Clk_SetPeripheralGate(ClkPeripheralFlash,TRUE);

    Clk_SetRCHFreq(ClkFreq24Mhz);
    Clk_Enable(ClkRCH, TRUE);

    stcClkCfg.enClkSrc  = ClkRCH;
    stcClkCfg.enHClkDiv = ClkDiv1;
    stcClkCfg.enPClkDiv = ClkDiv1;

    Clk_Init(&stcClkCfg);

    stc_clk_systickcfg_t stcCfg;

    SystemCoreClockUpdate();

    DDL_ZERO_STRUCT(stcCfg);

    stcCfg.bNoRef = TRUE;
    stcCfg.u32LoadVal = 0xFFFFFF;
    Clk_SysTickConfig(&stcCfg);
}

