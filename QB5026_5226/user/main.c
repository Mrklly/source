#include "bsp_system.h"
#include "bsp_flash.h"
#include "bsp_it.h"

#include "eeprom.h"
#include "protocol_passive.h"
#include "key_handler.h"
#include "global_variables.h"


int main()
{
    system_init();
    flash_init();
    TIMER2_Init();
    RS485_Init();

    dev_param_get();

    if(g_dev_with_near) {
        near_sensor_init();
    }

    Relay_Init();
    key_init_all();
    PWM_timer1();
    PWM_timer3();

#if BACKLIGHT_PWM
    backlight_tim5_init();
    backlight_tim0_init();
    backlight_tim4_init();
    backlight_tim3_init();
    backlight_tim6_init();
#else
    backlight_init();
#endif

    uint16_t data = 0;
    uint8_t flag = 1;
//    while(1) {
//        if(flag) {
//            backlight_init_set_all(data);
//            flag = 0;
//        }
//        delay_ms(100);
//    }

    while(1) {
//        protocol_run();

        if(g_dev_with_near) {
            near_check();
        }

        protocol_master_check();
        delay_ms(10);
    }
}

