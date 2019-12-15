#include "drv_sys.h"
#include "hc32_gpio.h"
#include "hc32_flash.h"
#include "global_variables.h"

#define USE_RCH 1

void clock_init(void)
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralFlash, TRUE);
    Flash_WaitCycle(FlashWaitCycle2);   // when PLL=48Mhz, must set FlashWaitCycle1

    stc_sysctrl_clk_config_t stcCfg;
    stc_sysctrl_pll_config_t stcPLLCfg;

#ifdef USE_RCH
    Sysctrl_SetRCHTrim(SysctrlRchFreq24MHz);            // using RCH 24MHz

    stcPLLCfg.enInFreq    = SysctrlPllInFreq20_24MHz;   // RCH = 4MHz
    stcPLLCfg.enOutFreq   = SysctrlPllOutFreq36_48MHz;  // PLL = 48MHz
    stcPLLCfg.enPllClkSrc = SysctrlPllRch;              // using RCH
    stcPLLCfg.enPllMul    = SysctrlPllMul2;             // 4MHz x 12 = 48MHz

#else
    Sysctrl_ClkSourceEnable(SysctrlClkXTH,TRUE);        // using XTH 24MHz

    Sysctrl_XTHDriverConfig(SysctrlXtalDriver2);
    Sysctrl_SetXTHStableTime(SysctrlXthStableCycle4096);
    Sysctrl_SetPLLStableTime(SysctrlPllStableCycle4096);
    Sysctrl_SetXTHFreq(SysctrlXthFreq20_32MHz);

    stcPLLCfg.enInFreq    = SysctrlPllInFreq20_24MHz;   // XTH = 24MHz
    stcPLLCfg.enOutFreq   = SysctrlPllOutFreq36_48MHz;  // PLL = 48MHz
    stcPLLCfg.enPllClkSrc = SysctrlPllXthXtal;          // using XTH
    stcPLLCfg.enPllMul    = SysctrlPllMul2;             // 24MHz x 2 = 48MHz
#endif

    Sysctrl_SetPLLFreq(&stcPLLCfg);

    stcCfg.enClkSrc  = SysctrlClkPLL;
    stcCfg.enHClkDiv = SysctrlHclkDiv1;
    stcCfg.enPClkDiv = SysctrlPclkDiv1;
    Sysctrl_ClkInit(&stcCfg);

//    clock_output();
}

void clock_output(void)
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stc_gpio_config_t        stcGpioCfg;

    stcGpioCfg.enDir = GpioDirOut;
    stcGpioCfg.enDrv = GpioDrvH;
    stcGpioCfg.enPuPd = GpioNoPuPd;
    stcGpioCfg.enOD = GpioOdDisable;
    Gpio_Init(GpioPortB, GpioPin0, &stcGpioCfg);
    Gpio_Init(GpioPortA, GpioPin1, &stcGpioCfg);

    Gpio_SetAfMode(GpioPortB, GpioPin0, GpioAf7);

    Gpio_SfHClkOutputConfig(GpioSfHclkOutEnable, GpioSfHclkOutDiv1);
    Gpio_SetAfMode(GpioPortA, GpioPin1, GpioAf6);
}

void systick_init(void)
{
    SysTick_Config(Sysctrl_GetHClkFreq() / 200);
}

void SysTick_Handler(void)
{
    systick_counter++;
}