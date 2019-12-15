#include "control.h"
#include "global_variables.h"

void control_init(void)         // vavle and fan gpio init
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stc_gpio_config_t stcLEDPortCfg;

    DDL_ZERO_STRUCT(stcLEDPortCfg);

    stcLEDPortCfg.enDir = GpioDirOut;

    Gpio_Init(CTRL_VAVLE_PORT,    CTRL_VAVLE_PIN,    &stcLEDPortCfg);
    Gpio_Init(CTRL_FAN_HIGH_PORT, CTRL_FAN_HIGH_PIN, &stcLEDPortCfg);
    Gpio_Init(CTRL_FAN_MID_PORT,  CTRL_FAN_MID_PIN,  &stcLEDPortCfg);
    Gpio_Init(CTRL_FAN_LOW_PORT,  CTRL_FAN_LOW_PIN,  &stcLEDPortCfg);
    Gpio_Init(CTRL_JK5_PORT,      CTRL_JK5_PIN,      &stcLEDPortCfg);
}

void control_valve(void)
{
    if((g_dev.mode.power) && (g_dev.mode.valve)) {
        CTRL_VAVLE_OPEN();
    } else {
        CTRL_VAVLE_CLOSE();
    }
}

void control_fan(void)
{
    if(!g_dev.mode.power) {
        CTRL_FAN_CLZ();
        return ;
    }

    switch( (fan_status)g_dev.mode.wind ) {
    case FAN_MAX:
        CTRL_FAN_HIGH();
        break;
    case FAN_MID:
        CTRL_FAN_MID();
        break;
    case FAN_MIN:
        CTRL_FAN_LOW();
        break;
    case FAN_CLZ:
        CTRL_FAN_CLZ();
        break;
    }
}


