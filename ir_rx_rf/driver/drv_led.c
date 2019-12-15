#include "drv_led.h"


void hal_led_init(void)
{
    M0P_CLOCK->SYSCTRL2 = 0x5A5A;
    M0P_CLOCK->SYSCTRL2 = 0xA5A5;

    Clk_SetPeripheralGate(ClkPeripheralGpio,TRUE);

    Gpio_InitIO(LED_RF_PORT, LED_RF_PIN,  GpioDirOut);
    Gpio_InitIO(LED_YEL_PORT,LED_YEL_PIN, GpioDirOut);
    Gpio_InitIO(LED_RED_PORT,LED_RED_PIN, GpioDirOut);

    LED_RF_OFF;
    LED_YEL_OFF;
    LED_RED_OFF;
}

void hal_swd_to_io(void)
{
    M0P_CLOCK->PERI_CLKEN_f.GPIO  = 1;

    M0P_CLOCK->SYSCTRL2 = 0x5A5A;
    M0P_CLOCK->SYSCTRL2 = 0xA5A5;
    M0P_CLOCK->SYSCTRL1_f.SWD_UIO = 1;

    M0P_GPIO->P2ADS_f.P27 = 0;
    M0P_GPIO->P3ADS_f.P31 = 0;

    M0P_GPIO->P27_SEL_f.SEL = 0;
    M0P_GPIO->P31_SEL_f.SEL = 0;

//    Gpio_InitIO(2, 7, GpioDirOut);
//    Gpio_InitIO(3, 1, GpioDirOut);

    M0P_GPIO->P2DIR_f.P27 = 0;      // output
    M0P_GPIO->P3DIR_f.P31 = 0;
}
