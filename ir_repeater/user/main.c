#include "bsp_system.h"
#include "bsp_flash.h"
#include "bsp_it.h"
#include "bsp_uart.h"

#include "drv_spi_soft.h"

#include "global_variables.h"

#include "eeprom.h"
#include "key_handler.h"

#include "rf433.h"
#include "drv_infrared.h"
#include "drv_w25qxx.h"
#include "drv_ir.h"
#include "ir_study.h"


int main()
{
    uint16_t temp_systick_counter;

    system_init();
    crc_init();
    systick_init();     // no irq

    flash_init();
    timer2_init();
    RS485_Init();

//    modbus_set_mac();
    eeprom_get_all();

    spi_soft_io_init();
    cmt2300_init();

    LED_Init();

    KEY_Init();

    hal_ir_init();

    spi_init();

    ir_init();

    W25QXX_Init();

    g_dev.set_temp  = 17;
    g_dev.sta.mode = TEMP_MODE_COOL;
    g_dev.sta.wind = FAN_MIN;
    g_dev.sta.power = 1;

    gree_data.data1_s.reserve0 = 0x28;
    gree_data.data1_s.led      = 1;

//    for(uint8_t i=0; i<NUM_IR_ARRAY; i++) {
//        W25QXX_Read(buf_flash_read, i<<10, 1024);
//        uart_send_buf(buf_flash_read, 1024);
//    }

    if(KEY_IS_DOWN) {

        W25QXX_Erase_Chip();

        while(W25QXX_ReadSR() & 0x01) {
            LED_RED_TOG;
            LED_GREEN_TOG;
            delay_ms(100);
        }

        LED_RED_OFF;
        LED_GREEN_OFF;

        g_dev.sta.power = 0;
        g_dev.sta.mode = TEMP_MODE_COOL;
        g_dev.sta.wind = FAN_MIN;
        g_dev.set_temp = 16;

        while(1) {
            ir_study_run();

            if(temp_systick_counter != systick_counter) {

                temp_systick_counter = systick_counter;

                if(systick_counter % 100 == 0) {
                    LED_RED_TOG;
                }
            }
        }
    }

    while(1) {

        modbus_recv_hander();

        g_rf_flag = rf433_rx(g_rf_buf, 31);
        if(g_rf_flag) {
            uart_send_buf(g_rf_buf, 31);
            rf433_handler();

            if(g_rf_buf[27] == 'I' && g_rf_buf[28] == 'R') {

//                memset((uint8_t *)(&gree_data), 0, sizeof(gree_data));

//                memcpy((uint8_t *)(&gree_data.data1), &g_rf_buf[12], 4);
//                memcpy((uint8_t *)(&gree_data.data2), &g_rf_buf[16], 4);

//                gree_data.data3                      = g_rf_buf[20];

//                memcpy((uint8_t *)(&gree_data.data4), &g_rf_buf[21], 2);

                gree_data.data1 = (g_rf_buf[12] << 24) | (g_rf_buf[13] << 16) | (g_rf_buf[14] << 8) | g_rf_buf[15];
                gree_data.data2 = (g_rf_buf[16] << 24) | (g_rf_buf[17] << 16) | (g_rf_buf[18] << 8) | g_rf_buf[19];
                gree_data.data3  = g_rf_buf[20];
                gree_data.data4 = (g_rf_buf[21] << 8)   | g_rf_buf[22];

                ir_send_gree();
            }

            g_rf_flag = 0;
        }

        if(temp_systick_counter != systick_counter) {

            temp_systick_counter = systick_counter;

            if(systick_counter % 2 == 0) {   // 10 ms

            }

            if(systick_counter % 4 == 0) { // 20 ms

            }

            if(systick_counter % 10 == 0) { // 50 ms
                key_check_all();
                key_handler();
            }

            if(systick_counter % 20 == 0) { // 100 ms
 
            }

            if(systick_counter % 100 == 0) { // 500 ms
                ir_repeater_run();
            }

            if(systick_counter % 200 == 0) { // 1 s
                LED_RED_TOG;
                LED_GREEN_TOG;
            }
        }
    }
}

