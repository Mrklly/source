#include "dimmer.h"
#include "global_variables.h"


void dimmer_run(void)
{
    pwm_share_cont++;

    if(adjust1_flag == 1) {
        adjust1_cont++;

        if(adjust1_cont == Chopping1_channel_cont) {
            GPIO_DIMMER_1_ON;
        }
    }

    if(adjust2_flag == 1) {
        adjust2_cont++;

        if(adjust2_cont == Chopping2_channel_cont) {
            GPIO_DIMMER_2_ON;
        }
    }

    if(pwm_share_cont >= STEP) {
        DIMMER_TIMER_STOP;
        pwm_share_cont = 0;
        adjust1_flag = 0;
        adjust1_cont = 0;
        adjust2_flag = 0;
        adjust2_cont = 0;
        GPIO_DIMMER_1_OFF;
        GPIO_DIMMER_2_OFF;
    }
}

void led_run(void)
{
    static uint8_t led_sta_last_1 = 0, led_sta_last_4 = 0;

    LED_1_OUT = led_sta[0];
    LED_2_OUT = led_sta[1];
    LED_3_OUT = led_sta[2];
    LED_4_OUT = led_sta[3];

    if(led_sta[1] != led_sta_last_1) {

        if(led_sta[1]) {

            Chopping1_channel_cont = 13;

        } else {

            Chopping1_channel_cont = STEP;

        }

        led_sta_last_1 = led_sta[1];
    }

    if(led_sta[4] != led_sta_last_4) {

        if(led_sta[4]) {

            Chopping2_channel_cont = 13;

        } else {

            Chopping2_channel_cont = STEP;

        }

        led_sta_last_4 = led_sta[4];
    }
}

