#include "bsp_system.h"
#include "bsp_flash.h"
#include "bsp_it.h"
#include "bsp_uart.h"

#include "drv_spi_soft.h"

#include "global_variables.h"

#include "eeprom.h"
#include "third_agreement.h"
#include "key_handler.h"

#include "rf433.h"


int main()
{
    uint16_t temp_systick_counter;

    system_init();
    flash_init();
    TIMER2_Init();
    RS485_Init();

    modbus_set_mac();

    protocol_param_get();

    if(dev_param.rf_ch == 0xff) {
        dev_param.rf_ch = 0;
    }

    printf("dev_param.mac_this :      ");
    for(uint8_t i=0; i<6; i++) {
        printf("%02x", dev_param.mac_this[i]);
    }
    printf("\n");

    spi_soft_io_init();
    cmt2300_init();

    LED_Init();
    Relay_Init();
    KEY_Init();
    PWM_timer1();
    PWM_timer3();

    for(uint8_t i=0; i<4; i++) {
        PWM_Contorl_LED(i, key_led_state[i]);
    }

    systick_init();     // 5ms int

    uint8_t buf_key_value[KEY_COUNT], buf_key_status[KEY_COUNT];
    memset(buf_key_value,  0, KEY_COUNT);
    memset(buf_key_status, 0, KEY_COUNT);

    while(1) {

#if PROTOCOL_RS485
        key_value_pack(buf_key_value, buf_key_status);
        protocol_send_key_status(buf_key_value, buf_key_status);
        protocol_run();
#endif

        g_rf_flag = rf433_rx(g_rf_buf, 31);
        if(g_rf_flag) {
            uart_send_buf(g_rf_buf, 31);
            rf433_handler();
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

//                static uint16_t counter=0;
//                counter++;

//                uint8_t temp[31] = {0};
//                for(uint8_t i=0; i<31; i++) {
//                    temp[i] = i;
//                }

//                temp[0] = (uint8_t)(counter >> 8);
//                temp[1] = (uint8_t)(counter);
//                rf433_tx(temp, 31);
            }

            if(systick_counter % 200 == 0) { // 1 s
                
            }
        }
    }
}

