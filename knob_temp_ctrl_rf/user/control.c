#include "control.h"
#include "global_variables.h"

void control_init(void)         // vavle and fan gpio init
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stc_gpio_config_t stcLEDPortCfg;

    DDL_ZERO_STRUCT(stcLEDPortCfg);

    stcLEDPortCfg.enDir = GpioDirOut;

    Gpio_Init(VAVLE_COLD_PORT,    VAVLE_COLD_PIN,    &stcLEDPortCfg);
    Gpio_Init(VAVLE_WARM_PORT,    VAVLE_WARM_PIN,    &stcLEDPortCfg);   // JK5
    Gpio_Init(CTRL_FAN_HIGH_PORT, CTRL_FAN_HIGH_PIN, &stcLEDPortCfg);
    Gpio_Init(CTRL_FAN_MID_PORT,  CTRL_FAN_MID_PIN,  &stcLEDPortCfg);
    Gpio_Init(CTRL_FAN_LOW_PORT,  CTRL_FAN_LOW_PIN,  &stcLEDPortCfg);

    GPIO_LOW(CTRL_FAN_LOW_PORT,  CTRL_FAN_LOW_PIN);
    GPIO_LOW(CTRL_FAN_MID_PORT,  CTRL_FAN_MID_PIN);
    GPIO_LOW(CTRL_FAN_HIGH_PORT, CTRL_FAN_HIGH_PIN);
    GPIO_LOW(VAVLE_COLD_PORT,    VAVLE_COLD_PIN);
    GPIO_LOW(VAVLE_WARM_PORT,    VAVLE_WARM_PIN);
}

void control_valve(void)
{
    if(!g_dev.sta.power || !g_dev.sta.valve) {
        VAVLE_COLD_CLOSE();
        VAVLE_WARM_CLOSE();
        return ;
    }

    if(g_dev.pipe_type == PIPE_DOUB) {

        VAVLE_COLD_OPEN();
        VAVLE_WARM_CLOSE();

    } else if(g_dev.pipe_type == PIPE_QUAD) {

        if(g_dev.sta.mode == TEMP_MODE_COOL) {
            VAVLE_COLD_OPEN();
            VAVLE_WARM_CLOSE();
        } else if(g_dev.sta.mode == TEMP_MODE_WARM) {
            VAVLE_COLD_CLOSE();
            VAVLE_WARM_OPEN();
        }
    }

//    if((g_dev.sta.power) && (g_dev.sta.valve)) {
//        VAVLE_COLD_OPEN();
//    } else {
//        VAVLE_COLD_CLOSE();
//    }

}

void control_fan(void)
{
    if(!g_dev.sta.power) {
        CTRL_FAN_CLZ();
        return ;
    }

    switch( (fan_status)g_dev.sta.wind ) {
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


