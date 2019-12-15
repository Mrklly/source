#include "led.h"

void led_init(void)
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stc_gpio_config_t           stcLEDPortCfg;

    DDL_ZERO_STRUCT(stcLEDPortCfg);

    stcLEDPortCfg.enDir  = GpioDirOut;
    Gpio_Init(LED_INDICATOR_PORT, LED_INDICATOR_PIN, &stcLEDPortCfg);
    Gpio_Init(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN, &stcLEDPortCfg);
    Gpio_Init(TEST_PORT, TEST_PIN, &stcLEDPortCfg);
}