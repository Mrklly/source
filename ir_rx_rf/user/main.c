#include "drv_clock.h"
#include "drv_led.h"
#include "drv_uart.h"
#include "drv_spi_soft.h"
#include "drv_timer.h"

#include "cmt2300.h"
#include "global_variables.h"
#include "modbus_rtu.h"
#include "infrared.h"


int32_t main(void)
{
    hal_ir_init();
    hal_clock_init();
    hal_led_init();

    timer0_init();      // using for systick

    for(uint8_t i=0; i<5; i++) {
        LED_RF_ON;
        LED_YEL_ON;
        LED_RED_ON;
        delay_ms(50);
        LED_RF_OFF;
        LED_YEL_OFF;
        LED_RED_OFF;
        delay_ms(200);
	}

    hal_swd_to_io();        // P2.7 swdio using for spi sdio

    hal_spi_soft_init();

    g_dev.addr      = 0x03;
    g_dev.set_temp  = 20;
    g_dev.room_temp = 0;

    uint8_t dev_mac[]        = {0x07, 0x00, 0x00, 0x00, 0x00, 0x99};
    uint8_t controller_mac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x66};

    g_dev_mac.channel = 101;

    for(uint8_t i=0; i<6; i++) {
        g_dev_mac.name[10+i] = dev_mac[i];
    }

    for(uint8_t i=0; i<6; i++) {
        g_dev_mac.controller_name[i] = controller_mac[i];
    }

    cmt2300_init();

    hal_uart_init();

    uint16_t temp_systick_counter;
    uint8_t i;
    volatile uint8_t rf_cmd, rf_data;

    g_dev.set_temp  = 17;
    g_dev.mode.mod  = TEMP_MODE_COOL;
    g_dev.mode.wind = FAN_MIN;
    g_dev.mode.power = 1;

    gree_data.data1_s.reserve0 = 0x28;
    gree_data.data1_s.led      = 1;

    while(1) {

        modbus_recv_hander();

        if(rf433_flag) {

            LED_RF_ON;

            if(mm[27] == 'I' && mm[28] == 'R') {

                uart_send_buf(mm, 31);

//                memset((uint8_t *)(&gree_data), 0, sizeof(gree_data));

//                memcpy((uint8_t *)(&gree_data.data1), &mm[12], 4);
//                memcpy((uint8_t *)(&gree_data.data2), &mm[16], 4);

//                gree_data.data3                      = mm[20];

//                memcpy((uint8_t *)(&gree_data.data4), &mm[21], 2);

                gree_data.data1 = (mm[12] << 24) | (mm[13] << 16) | (mm[14] << 8) | mm[15];
                gree_data.data2 = (mm[16] << 24) | (mm[17] << 16) | (mm[18] << 8) | mm[19];
                gree_data.data3  = mm[20];
                gree_data.data4 = (mm[21] << 8)   | mm[22];

                ir_send_gree();


            } else if(mm[27] == 0 && mm[28] == 0) {

                rf_cmd = mm[12];
                rf_data = mm[13];

                if(           (rf_cmd & 0xf0) == CMD_GET_TEMP_SET) {
                        g_dev.set_temp = rf_data;
                    } else if((rf_cmd & 0xf0) == CMD_GET_TEMP_ROOM) {
                        float num = ((float)(rf_cmd & 0x0f)) / 10;
                        g_dev.room_temp = (float)rf_data + num;
                    } else if((rf_cmd & 0xf0) == CMD_GET_PWR) {
                        g_dev.mod = rf_data;
                    } else if((rf_cmd & 0xf0) == CMD_GET_MODE) {
                        g_dev.mod = rf_data;
                    } else if((rf_cmd & 0xf0) == CMD_GET_FAN) {
                        g_dev.mod = rf_data;
                    }

                modbus_tx();
            }

            rf433_flag = 0;
//            uint8_t str[] = "RF433 received...";
//            uart_send_buf(str, sizeof(str));
//            uart_send_buf(mm, 31);
//            for(i=0; i<31; i++) {
//                mm[i] = 0;
//            }
            modbus_status++;
        } else {
            LED_RF_OFF;
        }

        modbus_rx_check();

        rf433_receive_check();

        if(temp_systick_counter != systick_counter) {
            temp_systick_counter = systick_counter;
            if(systick_counter % 2 == 0) {   // 10 ms

//                Refresh_WWDG_Window();
            }

            if(systick_counter % 100 == 0) { // 500 ms
                LED_RED_TOGGLE;
            }

            if(systick_counter % 200 == 0) { // 1 s

//                uart_printf("%s-%s\n", __DATE__, __TIME__);
            }
        }
    }


    while(1) {
        gree_control();
        delay_ms(300);

        LED_RED_TOGGLE;
    }


    while(1) {
        ir_bit_start();
        ir_send_byte(0x31);
        ir_send_byte(0x0C);
        ir_send_byte(0x20);
        ir_send_byte(0x50);
        ir_send_bit(0x02, 3);
        ir_send_pulse(1, 20000);       // 20ms high pulse
        ir_send_bit(0, 28);
        ir_send_bit(0x07, 4);

        ir_send_pulse(1, 40000);       // 40ms high pulse

        ir_bit_start();
        ir_send_byte(0x31);
        ir_send_byte(0x0C);
        ir_send_byte(0x20);
        ir_send_byte(0x70);
        ir_send_bit(0x02, 3);
        ir_send_pulse(1, 20000);       // 20ms high pulse
        ir_send_bit(0, 20);
        ir_send_bit(0x05, 4);

        ir_send_byte(0xC0);

        delay_ms(555);
        LED_RED_TOGGLE;

        ir_bit_start();
        ir_send_byte(0x39);
        ir_send_byte(0x0C);
        ir_send_byte(0x20);
        ir_send_byte(0x50);
        ir_send_bit(0x02, 3);
        ir_send_pulse(1, 20000);       // 20ms high pulse
        ir_send_bit(0, 28);
        ir_send_bit(0x0F, 4);

        ir_send_pulse(1, 40000);       // 40ms high pulse

        ir_bit_start();
        ir_send_byte(0x39);
        ir_send_byte(0x0C);
        ir_send_byte(0x20);
        ir_send_byte(0x70);
        ir_send_bit(0x02, 3);
        ir_send_pulse(1, 20000);       // 20ms high pulse
        ir_send_bit(0, 20);
        ir_send_bit(0x05, 4);

        ir_send_byte(0x40);

        delay_ms(555);
        LED_RED_TOGGLE;
    }


    while(1) {
		CheckFlg = 0;
        LED_RF_ON;
        delay_ms(50);
        LED_RF_OFF;
        delay_ms(2000);

        for(uint8_t i=0; i<g_rx_index; i++) {
            Uart_SendData(UARTCH1, g_rx_buf[i]);
        }

        g_rx_index = 0;
	}
}
