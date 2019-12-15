#include "knob_handler.h"
#include "eeprom.h"
#include "key_handler.h"
#include "global_variables.h"


void knob_handler(void)
{
    if(g_set_type == IR_STUDY || g_set_type == IR_SEND || g_set_type == IR_RECV) {
        g_knob_direction = DIRECTION_NONE;
        return ;
    }

    if(!g_dev.sta.power && !g_lcd_on) {
        if(g_knob_direction != DIRECTION_NONE) {    // this could cause a poweron led on issure
            g_led_on = 1;
            g_led_delay = 20;
            g_knob_direction = DIRECTION_NONE;
        }
        return ;
    }

    if(g_knob_direction < DIRECTION_CW) {
        return ;
    }

    g_led_on = 1;
    g_lcd_on = 1;
    g_lcd_delay = g_dev.led_timeout * 10;       // g_dev.led_timeout * 10 = 1 second
    g_led_delay = g_dev.led_timeout * 10;
    g_knob_changed = 1;

    if(g_knob_direction == DIRECTION_CW) {
//        printf("knob: CW\n");

        if(g_lcd_func == SHOW_TIMING_TASK) {

            if(g_timing_task < TIMING_TASK_MAX) {
                g_timing_task++;
            } else {
                g_timing_task = 0;
            }

        } else {

            key_knob_up();
        }
    } else if(g_knob_direction == DIRECTION_CCW) {
//        printf("knob: CCW\n");

        if(g_lcd_func == SHOW_TIMING_TASK) {

            if(g_timing_task > 0) {
                g_timing_task--;
            } else {
                g_timing_task = TIMING_TASK_MAX;
            }

        } else {

            key_knob_down();
        }
    }
    g_lcd_func_delay = LCD_FUNC_DELAY_TIME;
    g_knob_direction = DIRECTION_NONE;
}

