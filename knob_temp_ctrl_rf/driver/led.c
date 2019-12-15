#include "led.h"
#include "global_variables.h"


void led_update(void)             // run at every 100ms
{
    static uint8_t led_counter = 0, led_status = 0;
    static uint8_t zig_cnt = 0;

    if(g_saving_flag || g_erasing_flag) {

        if(led_counter % 2 == 0) {

#if TEMP_CTRL_KNOB

            RGB_COLOR_BLACK;
        } else {
            RGB_COLOR_WHITE;
        }

#elif TEMP_CTRL_TOUCH

            LED_INDICATOR_OFF;
        } else {
            LED_INDICATOR_ON;
        }

#endif

        led_counter++;

        if(led_counter >= 6) {
//            g_saving_flag = 0;
            g_erasing_flag = 0;
            led_counter = 0;
        }

        return ;
    }

    if((g_led_on)) {
        led_status = 1;
    } else {
        led_status = 0;
    }

    if(g_led_delay == 0) {
        g_led_delay = g_dev.led_timeout * 10;
        g_led_on = 0;
        led_status = 0;
    }

    if(g_set_type) {

        if(g_set_type == SET_ZIGBEE) {

            if(zig_cnt == 9) {

                zig_cnt = 0;

                if(g_zig_net_sta == ZIG_NET_STA_NONE) {

#if TEMP_CTRL_KNOB

                    RGB_COLOR_RED;
                } else {
                    RGB_COLOR_BLUE;
                }

#elif TEMP_CTRL_TOUCH

                    LED_INDICATOR_ON;
                } else {
                    LED_INDICATOR_OFF;
                }
#endif

            } else {

#if TEMP_CTRL_KNOB

                RGB_COLOR_BLACK;

#elif TEMP_CTRL_TOUCH

                LED_INDICATOR_OFF;
#endif
            }

            zig_cnt++;

            return ;
        }

        led_status = 1;
        g_lcd_delay = g_dev.led_timeout * 10;

#if TEMP_CTRL_KNOB
        RGB_COLOR_WHITE;
#elif TEMP_CTRL_TOUCH
        LED_INDICATOR_ON;
#endif

        return ;
    }

    if(led_status) {
        if(0 < g_led_delay && g_led_delay < 250*10) {
            g_led_delay--;
        }

#if TEMP_CTRL_KNOB
        switch(g_dev.sta.mode) {
        case TEMP_MODE_COOL:
            RGB_COLOR_BLUE;
            break;
        case TEMP_MODE_WARM:
            RGB_COLOR_RED;
            break;
        case TEMP_MODE_FAN:
            RGB_COLOR_GREEN;
            break;
        case TEMP_MODE_DRY:
            break;
        case TEMP_MODE_AUTO:
            break;
        default :
            break;
        }
#elif TEMP_CTRL_TOUCH
        LED_INDICATOR_ON;
#endif

    } else {
        g_led_delay = g_dev.led_timeout * 10;

#if TEMP_CTRL_KNOB
        RGB_COLOR_BLACK;
#elif TEMP_CTRL_TOUCH
        LED_INDICATOR_OFF;
#endif
    }
}

