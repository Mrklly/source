#include "drv_dimmer.h"
#include "drv_timer.h"
#include "global_variables.h"


void hal_dimmer_init(void)
{
    Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE);

    Gpio_InitIO(PORT_DIMMER_1, PIN_DIMMER_1,  GpioDirOut);
    Gpio_InitIO(PORT_DIMMER_2, PIN_DIMMER_2,  GpioDirOut);

    GPIO_DIMMER_1_OFF;
    GPIO_DIMMER_2_OFF;

    Gpio_InitIOExt(PORT_ZERO_CROSS, PIN_ZERO_CROSS,  GpioDirIn, TRUE, FALSE, FALSE, FALSE);
    Gpio_ClearIrq(PORT_ZERO_CROSS, PIN_ZERO_CROSS);
    Gpio_EnableIrq(PORT_ZERO_CROSS, PIN_ZERO_CROSS, GpioIrqRising | GpioIrqFalling);
    EnableNvic(PORT2_IRQn, 2, 1);
}


void irq_zero_cross(void)
{
    if(GPIO_IRQ_ZERO_CROSS) {

        if(GPIO_STA_ZERO_CROSS) {

            GPIO_DIMMER_1_OFF;
            GPIO_DIMMER_2_OFF;

        } else {

            if(led_sta[1] == 0) {

                GPIO_DIMMER_1_OFF;
                adjust1_flag = 0;

            } else if(Chopping1_channel_cont == 0) {

                GPIO_DIMMER_1_ON;
                adjust1_flag = 1;

            } else {

                adjust1_flag = 1;
            }

            if(led_sta[4] == 0) {

                GPIO_DIMMER_2_OFF;
                adjust2_flag = 0;

            } else if(Chopping2_channel_cont == 0) {

                GPIO_DIMMER_2_ON;
                adjust2_flag = 1;

            } else {

                adjust2_flag = 1;

            }

            adjust1_cont = 0;
            adjust2_cont = 0;
            pwm_share_cont = 0;
            DIMMER_TIMER_RUN;
        }

        M0P_GPIO->P2ICLR_f.P23 = 0;
    }
}

