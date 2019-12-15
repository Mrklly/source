#include "bsp_gpio.h"


void LED_Init(void)
{
    stc_gpio_config_t     stcBacklightPortCfg;

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);

    DDL_ZERO_STRUCT(stcBacklightPortCfg);

    stcBacklightPortCfg.enDir   = GpioDirOut;
    stcBacklightPortCfg.enDrv   = GpioDrvH;
    stcBacklightPortCfg.enPuPd  = GpioNoPuPd;
    stcBacklightPortCfg.enOD    = GpioOdDisable;

    Gpio_Init(PORT_LED_RED,   PIN_LED_RED,   &stcBacklightPortCfg);
    Gpio_Init(PORT_LED_GREEN, PIN_LED_GREEN, &stcBacklightPortCfg);

    LED_RED_ON;
    LED_GREEN_OFF;
}

