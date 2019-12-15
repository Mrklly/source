#include "timing_task.h"


void timing_task_run(void)
{
    if(g_timing_task) {

        if(g_counter_min == 30) {

            g_timing_task--;
            g_counter_min = 0;
            g_counter_sec = 0;

            if(g_timing_task == 0) {

                g_dev.sta.power = 0;

                g_lcd_on = 0;
                g_led_on = 0;

            }
        }
    }
}
