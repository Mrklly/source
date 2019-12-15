#include "drv_led.h"

void led_init(void)
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stc_gpio_config_t           stcLEDPortCfg;

    DDL_ZERO_STRUCT(stcLEDPortCfg);

    stcLEDPortCfg.enDir  = GpioDirOut;

#if TEMP_CTRL_KNOB
    Gpio_Init(LED_PORT_R, LED_PIN_R, &stcLEDPortCfg);
    Gpio_Init(LED_PORT_G, LED_PIN_G, &stcLEDPortCfg);
    Gpio_Init(LED_PORT_B, LED_PIN_B, &stcLEDPortCfg);
    GPIO_LOW(LED_PORT_R, LED_PIN_R);
    GPIO_LOW(LED_PORT_G, LED_PIN_G);
    GPIO_LOW(LED_PORT_B, LED_PIN_B);
#elif TEMP_CTRL_TOUCH
    Gpio_Init(LED_INDICATOR_PORT, LED_INDICATOR_PIN, &stcLEDPortCfg);
    GPIO_LOW(LED_INDICATOR_PORT, LED_INDICATOR_PIN);
#endif

    Gpio_Init(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN, &stcLEDPortCfg);
    GPIO_LOW(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN);
}

#if TEMP_CTRL_KNOB
void rgb_led_test(void)
{
    static uint8_t temp = 0;

    temp++;

    switch(temp) {
    case 1:
        RGB_COLOR_RED;
        break ;
    case 2:
        RGB_COLOR_GREEN;
        break ;
    case 3:
        RGB_COLOR_BLUE;
        break ;
    case 4:
        RGB_COLOR_CYAN;
        break ;
    case 5:
        RGB_COLOR_YELLOW;
        break ;
    case 6:
        RGB_COLOR_MAGENTA;
        break ;
    case 7:
        RGB_COLOR_WHITE;
        temp = 0;
        break ;
    default:
        break ;
    }
}
#endif

