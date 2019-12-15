#include "drv_knob.h"
#include "global_variables.h"


void knob_io_init(void)
{
    stc_gpio_config_t stcLEDPortCfg;

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    DDL_ZERO_STRUCT(stcLEDPortCfg);

    stcLEDPortCfg.enDir  = GpioDirIn;
    stcLEDPortCfg.enDrv = GpioDrvL;
    stcLEDPortCfg.enPuPd = GpioNoPuPd;
    stcLEDPortCfg.enOD = GpioOdDisable;
    stcLEDPortCfg.enCtrlMode = GpioAHB;

    Gpio_Init(PORT_KNOB_CHA, PIN_KNOB_CHA, &stcLEDPortCfg);
    Gpio_Init(PORT_KNOB_CHB, PIN_KNOB_CHB, &stcLEDPortCfg);
    
    Gpio_ClearIrq(PORT_KNOB_CHA, PIN_KNOB_CHA);
    Gpio_ClearIrq(PORT_KNOB_CHB, PIN_KNOB_CHB);

    Gpio_EnableIrq(PORT_KNOB_CHA, PIN_KNOB_CHA, GpioIrqRising | GpioIrqFalling);
    Gpio_EnableIrq(PORT_KNOB_CHB, PIN_KNOB_CHB, GpioIrqRising | GpioIrqFalling);

    EnableNvic(IRQ_SOURCE_KNOB, IrqLevel3, TRUE);

    if(Gpio_GetInputIO(PORT_KNOB_CHA, PIN_KNOB_CHA) != 
       Gpio_GetInputIO(PORT_KNOB_CHB, PIN_KNOB_CHB)) {

//        g_knob_direction = DIRECTION_JITTER;
    }
}

