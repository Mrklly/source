#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "eeprom.h"
#include "global_variables.h"


void led_init(void)
{
    stc_gpio_config_t stcGpioCfg;

    DDL_ZERO_STRUCT(stcGpioCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    stcGpioCfg.enDir   = GpioDirOut;
    stcGpioCfg.enDrv   = GpioDrvH;
    stcGpioCfg.enPuPd  = GpioNoPuPd;
    stcGpioCfg.enOD    = GpioOdDisable;

    Gpio_Init(PORT_LED1, PIN_LED1,  &stcGpioCfg);
    Gpio_Init(PORT_LED2, PIN_LED2,  &stcGpioCfg);
    Gpio_Init(PORT_LED3, PIN_LED3,  &stcGpioCfg);
    Gpio_Init(PORT_LED4, PIN_LED4,  &stcGpioCfg);
    Gpio_Init(PORT_LED5, PIN_LED5,  &stcGpioCfg);
    Gpio_Init(PORT_LED6, PIN_LED6,  &stcGpioCfg);

    for(uint8_t i=0; i<g_dev.key_num; i++) {
        GPIO_LOW(gpio_led[i].port, gpio_led[i].pin);
    }

}

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
        GPIO_HIGH(gpio_backlight[i].port, gpio_backlight[i].pin);
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

void test_io_init(void)
{
    stc_gpio_config_t gpio;

    DDL_ZERO_STRUCT(gpio);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    gpio.enDir   = GpioDirOut;
    gpio.enDrv   = GpioDrvL;
    gpio.enPuPd  = GpioPu;
    gpio.enOD    = GpioOdDisable;

    Gpio_Init(PORT_TEST, PIN_TEST, &gpio);

    GPIO_LOW(PORT_TEST, PIN_TEST);
}

