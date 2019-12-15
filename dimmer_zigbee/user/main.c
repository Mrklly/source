#include "drv_clock.h"
#include "drv_led.h"
#include "drv_uart.h"
#include "drv_timer.h"
#include "drv_wdt.h"
#include "global_variables.h"
#include "key_handler.h"
#include "mcu_api.h"
#include "dimmer.h"
#include "zg_register.h"
#include "protocol.h"


int32_t main(void)
{
    hal_clock_init();

    hal_uart_init();
    zigbee_protocol_init();

    hal_led_init();
    hal_dimmer_init();

    hal_key_init();

    timer0_init();      // using for systick
    timer2_init();      // using for dimmer

    uint16_t temp_systick_counter;

    hal_wdt_init();

    while(1) {

        led_run();

        zigbee_uart_service();

        if(temp_systick_counter != systick_counter) {
            temp_systick_counter = systick_counter;

//            if(systick_counter % 2 == 0) {   // 10 ms

//            }

            if(systick_counter % 4 == 0) {   // 20 ms
                hal_wdt_feed();
                key_Result = Key_value_detection();
                key_handler();
            }

            if(systick_counter % 40 == 0) {   // 100 ms
                zg_reg_run();
            }

            if(systick_counter % 100 == 0) {   // 500 ms
                all_data_update();
            }

//                printf("%s-%s\n", __DATE__, __TIME__);

        }
    }
}

