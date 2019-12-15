#include "drv_sys.h"
#include "drv_wdt.h"
#include "timer.h"
#include "uart.h"
#include "key.h"
#include "key_handler.h"
#include "adc.h"
#include "temperature.h"
#include "spi_soft.h"
#include "spi.h"
#include "cmt2300a.h"
#include "led.h"
#include "lcd.h"
#include "eeprom.h"
#include "modbus_rtu.h"
#include "control.h"
#include "wireless.h"
#include "ir_study.h"
#include "knob_handler.h"
#include "timing_task.h"
#include "mcu_api.h"
#include "protocol.h"


int32_t main(void)
{
    uint16_t temp_systick_counter;

    check_device_type();

    clock_init();

    zigbee_protocol_init();
    uart_zigbee_init(BAUD9600);

    led_init();
    seglcd_init();
    near_check_init();
    eeprom_init();
    uart_init(BAUD19200);       // first initialization for uart get settings
//    modbus_get_mac();         // set rf433 mac
    eeprom_get_all();
//    __disable_irq();
    uart_init(g_dev.baud);      // second initialization for the whole system

    key_init();

#if TEMP_CTRL_KNOB
    knob_io_init();
#endif

    get_dev_type();             // get device type, rs485 or rf433?

    hal_wdt_init();

    systick_init(); // 5ms
    timer3_init();  // 100ms
    Tim3_M0_Run();
    adc_init();
    control_init();         // vavle and fan gpio init

    if(g_dev_type == DEV_TYPE_RF433) {
        
#if SPI_SOFT
        spi_soft_io_init();
#else
        spi_init();
#endif
        cmt2300_init();
    }

    ir_init();

//    __enable_irq();

    while(1) {

#if TEMP_CTRL_KNOB
        knob_handler();
#endif

        zigbee_uart_service();

        modbus_recv_hander();

        if(g_dev_type == DEV_TYPE_RF433) {
            rf_recv_check();
            wireless_handler();
        }

        if(temp_systick_counter != systick_counter) {

            temp_systick_counter = systick_counter;

            if(systick_counter % 2 == 0) {   // 10 ms

            }

            if(systick_counter % 4 == 0) { // 20 ms
                get_room_temp();
                hal_wdt_feed();
            }

            if(systick_counter % 10 == 0) { // 50 ms
                key_check_all();
                key_handler();
                device_setting();
            }

            if(systick_counter % 20 == 0) { // 100 ms

                if(g_knob_changed) {
                    if(g_set_type == SET_NONE) {
                        if(g_dev_type == DEV_TYPE_RF433) {
//                            btn_callback(0xff);
                        }
                    }

//                    printf("temp_offset = %d = 0x%02hhx\n", g_dev.temp_offset, g_dev.temp_offset);
                    g_knob_changed = 0;
                }

                temperature_control();
                led_update();
                set_lcd_backlight();
                lcd_update();
                lcd_screen_update();
            }

            if(systick_counter % 100 == 0) { // 500 ms
                control_fan();
                control_valve();
            }

            if(systick_counter % 200 == 0) { // 1 s

                zigbee_auto_send();

//                ir_debug();
                if(g_dev_type == DEV_TYPE_RF433) {
                    ir_study_run();
                    ir_eep_send();
                    ir_eep_recv();
                }

                if(g_counter_sec == 60) {
                    g_counter_min++;
                    g_counter_sec = 0;
                }

                g_counter_sec++;

                timing_task_run();

            }
        }
    }
}

