#include "led.h"
#include "system.h"
#include "timer.h"
#include "uart.h"
#include "key.h"
#include "key_handler.h"
#include "adc.h"
#include "temperature.h"
#include "spi_soft.h"
#include "spi.h"
#include "cmt2300a.h"
#include "lcd.h"
#include "eeprom.h"
#include "modbus_rtu.h"
#include "control.h"
#include "wireless.h"
#include "ir_study.h"


int32_t main(void)
{
    uint16_t temp_systick_counter;

    clock_init();
    eeprom_init();
    uart_init();
    modbus_get_mac();
    eeprom_get_all();
    led_init();
    key_init();
    seglcd_init();
    systick_init(); // 5ms
    timer3_init();  // 100ms
    Tim3_M0_Run();
    adc_init();
    control_init();         // vavle and fan gpio init

#if SPI_SOFT
    spi_soft_io_init();
#else
    spi_init();
#endif

    cmt2300_init();

    ir_init();

    static uint8_t i, temp = 0;

    while(1) {

        if(SPI_SOFT_INT_IN_DATA) {
            vEnableRdFifo();
			temp = FIFO_ReadByte();
			for(i=0; i<31; i++) {
                mm[i] = 0;
            }
			for(i=0; i<31; i++) {
                mm[i] = FIFO_ReadByte();
            }
			bIntSrcFlagClr();
			vClearFIFO();
            rf433_flag = 1;
        }

        modbus_recv_hander();

        wireless_handler();

        if(rf433_flag) {
            UART_Send(mm, 10);
            rf433_flag = 0;
        } else {

        }

        if(temp_systick_counter != systick_counter) {

            temp_systick_counter = systick_counter;

            if(systick_counter % 2 == 0) {   // 10 ms

            }

            if(systick_counter % 4 == 0) { // 20 ms
                get_room_temp();
            }

            if(systick_counter % 10 == 0) { // 50 ms
                key_check_all();
                key_handler();
                device_setting();
            }

            if(systick_counter % 20 == 0) { // 100 ms
                temperature_control();
                set_led_indicator();
                set_lcd_backlight();
                lcd_update();
            }

            if(systick_counter % 100 == 0) { // 500 ms
                control_fan();
                control_valve();

//                static uint16_t counter=0;
//                counter++;
//
//                uint8_t temp[31] = {0};
//                for(uint8_t i=0; i<31; i++) {
//                    temp[i] = i;
//                }
//                temp[0] = (uint8_t)(counter >> 8);
//                temp[1] = (uint8_t)(counter);
//                rf433_tx(temp, 31);
            }

            if(systick_counter % 200 == 0) { // 1 s
//                ir_debug();
                ir_study_run();
                ir_eep_send();
                ir_eep_recv();

//                uint8_t buf[] = "rf433 test\n";
//                rf433_tx(buf, sizeof(buf));
            }
        }
    }
}



