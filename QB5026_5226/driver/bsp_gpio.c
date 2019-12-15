#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "eeprom.h"
#include "global_variables.h"

void backlight_init(void)
{
    stc_gpio_config_t     stcBacklightPortCfg;

    DDL_ZERO_STRUCT(stcBacklightPortCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    stcBacklightPortCfg.enDir   = GpioDirOut;
    stcBacklightPortCfg.enDrv   = GpioDrvH;
    stcBacklightPortCfg.enPuPd  = GpioNoPuPd;
    stcBacklightPortCfg.enOD    = GpioOdDisable;

    Gpio_Init(BACK_LED1_PORT, BACK_LED1_PIN,  &stcBacklightPortCfg);
    Gpio_Init(BACK_LED2_PORT, BACK_LED2_PIN,  &stcBacklightPortCfg);
    Gpio_Init(BACK_LED3_PORT, BACK_LED3_PIN,  &stcBacklightPortCfg);
    Gpio_Init(BACK_LED4_PORT, BACK_LED4_PIN,  &stcBacklightPortCfg);
    Gpio_Init(BACK_LED5_PORT, BACK_LED5_PIN,  &stcBacklightPortCfg);
    Gpio_Init(BACK_LED6_PORT, BACK_LED6_PIN,  &stcBacklightPortCfg);

    for(uint8_t i=0; i<g_dev.key_num; i++) {
        GPIO_HIGH(gpio_led[i].port, gpio_led[i].pin);
    }
}

void Relay_Init(void)
{
    stc_gpio_config_t     stcRelayPortCfg;

    DDL_ZERO_STRUCT(stcRelayPortCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    stcRelayPortCfg.enDir   = GpioDirOut;
    stcRelayPortCfg.enDrv   = GpioDrvL;
    stcRelayPortCfg.enPuPd  = GpioPu;
    stcRelayPortCfg.enOD    = GpioOdDisable;

    Gpio_Init(RELAY_CH1_PORT, RELAY_CH1_PIN,  &stcRelayPortCfg);
    Gpio_Init(RELAY_CH2_PORT, RELAY_CH2_PIN,  &stcRelayPortCfg);
    Gpio_Init(RELAY_CH3_PORT, RELAY_CH3_PIN,  &stcRelayPortCfg);
    Gpio_Init(RELAY_CH4_PORT, RELAY_CH4_PIN,  &stcRelayPortCfg);

    GPIO_LOW(RELAY_CH1_PORT, RELAY_CH1_PIN);
    GPIO_LOW(RELAY_CH2_PORT, RELAY_CH2_PIN);
    GPIO_LOW(RELAY_CH3_PORT, RELAY_CH3_PIN);
    GPIO_LOW(RELAY_CH4_PORT, RELAY_CH4_PIN);
}

void near_sensor_init(void)
{
    stc_gpio_config_t stcPortCfg;

    DDL_ZERO_STRUCT(stcPortCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);

    stcPortCfg.enDir  = GpioDirIn;
    stcPortCfg.enDrv  = GpioDrvH;
    stcPortCfg.enPuPd = GpioNoPuPd;

    Gpio_Init(NEAR_SENSOR_PORT, NEAR_SENSOR_PIN, &stcPortCfg);
}

