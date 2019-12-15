#include "drv_led.h"


void hal_led_init(void)
{
    M0P_CLOCK->SYSCTRL2 = 0x5A5A;
    M0P_CLOCK->SYSCTRL2 = 0xA5A5;

    Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE);

    Gpio_InitIO(PORT_LED_1, PIN_LED_1,  GpioDirOut);
    Gpio_InitIO(PORT_LED_2, PIN_LED_2,  GpioDirOut);
    Gpio_InitIO(PORT_LED_3, PIN_LED_3,  GpioDirOut);
    Gpio_InitIO(PORT_LED_4, PIN_LED_4,  GpioDirOut);
    Gpio_InitIO(PORT_LED_NET, PIN_LED_NET,  GpioDirOut);

    LED_1_OFF;
    LED_2_OFF;
    LED_3_OFF;
    LED_4_OFF;
    LED_NET_OFF;
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
