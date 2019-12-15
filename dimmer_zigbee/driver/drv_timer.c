#include "drv_timer.h"
#include "drv_led.h"
#include "dimmer.h"


volatile uint16_t systick_counter = 0;


void timer0_interrupt(void)
{
    if (M0P_BT0->IFR_f.TF) {

        M0P_BT0->ICLR_f.TFC = FALSE;

        systick_counter++;
    }
}

void timer0_init(void)
{
    stc_bt_config_t stcConfig;

    uint16_t u16ArrData     = 50535ul;      // 5ms
    uint16_t u16InitCntData = 0;

    Clk_SetPeripheralGate(ClkPeripheralBt, TRUE);

    stcConfig.pfnTim0Cb = timer0_interrupt;

    stcConfig.enGate  = BtGateDisable;
    stcConfig.enCT    = BtTimer;
    stcConfig.enPRS   = BtPCLKDiv8;
    stcConfig.enTog   = BtTogDisable;
    stcConfig.enCT    = BtTimer;
    stcConfig.enMD    = BtMode2;

    Bt_Init(TIM0, &stcConfig);

    Bt_ClearIntFlag(TIM0);
    Bt_EnableIrq(TIM0);
    EnableNvic(TIM0_IRQn, 3, TRUE);

    Bt_ARRSet(TIM0,   u16ArrData);
    Bt_Cnt16Set(TIM0, u16InitCntData);
    Bt_Run(TIM0);
}


void timer2_interrupt(void)
{
    if (M0P_BT2->IFR_f.TF) {

        dimmer_run();

        M0P_BT2->ICLR_f.TFC = 0;
    }
}

void timer2_init(void)
{
    stc_bt_config_t stcConfig;

    uint16_t u16ArrData     = 57856;      // 0.32ms, 0.32 * 25 = 8ms
    uint16_t u16InitCntData = 0;

    Clk_SetPeripheralGate(ClkPeripheralBt, TRUE);

    stcConfig.pfnTim2Cb = timer2_interrupt;

    stcConfig.enGate  = BtGateDisable;
    stcConfig.enCT    = BtTimer;
    stcConfig.enPRS   = BtPCLKDiv1;
    stcConfig.enTog   = BtTogDisable;
    stcConfig.enCT    = BtTimer;
    stcConfig.enMD    = BtMode2;

    Bt_Init(TIM2, &stcConfig);

    Bt_ClearIntFlag(TIM2);
    Bt_EnableIrq(TIM2);
    EnableNvic(TIM2_IRQn, 3, TRUE);

    Bt_ARRSet(TIM2,   u16ArrData);
    Bt_Cnt16Set(TIM2, u16InitCntData);
    Bt_Stop(TIM2);
}

