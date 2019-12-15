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

#if PWM_LED
    PWM_timer1();
    PWM_timer3();
    pwm_init_timer4();
#else
    led_init();
#endif

#if PWM_BACKLIGHT
    backlight_tim5_init();
    backlight_tim0_init();
    backlight_tim4_init();
    backlight_tim3_init();
    backlight_tim6_init();
#else
    backlight_init();
    g_panel_backlight2 |= 0x04;
#endif

    test_io_init();

    uint16_t data = 0;
    uint8_t flag = 1;

    uint8_t modbus_data[64], modbus_len = 0;
    memset(modbus_data, 0, 64);

    while(1) {

        modbus_recv_hander(modbus_data, &modbus_len);
        delay_ms(8);
        panel_check(modbus_data, &modbus_len);
        panel_control();
        master_check();
    }
}

