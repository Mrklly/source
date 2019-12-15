#include "global_variables.h"
#include "uart.h"
#include "wwdg.h"
#include "led.h"
#include "tim1.h"
#include "system_timer.h"
#include "spi_soft.h"
#include "cmt2300.h"
#include "eeprom.h"
#include "key_handler.h"


int main(void)
{
    unsigned int temp_systick_counter = 0;

    CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);
    CLK->CKDIVR |= (uint8_t)CLK_PRESCALER_HSIDIV1;

    led_init();
    key_init();
    flash_init();
    USART_Configuration(19200u);
    enableInterrupts();
    WWDG_Configuration();
    WWDGReset();
    timer1_init();

    eeprom_init();
    rf433_io_init();
    cmt2300_init();
    
    if(g_eep.rf_channel != 0) {
        LED_L_ON;
    }

    while(1) {

        if(temp_systick_counter != systick_counter) {
            temp_systick_counter = systick_counter;
            if(systick_counter % 2 == 0) {   // 10 ms
                Refresh_WWDG_Window();
            }
            
            if(systick_counter % 10 == 0) {   // 50 ms
                key_check();
                key_handler();
            }

            if(systick_counter % 100 == 0) { // 500 ms

            }

            if(systick_counter % 200 == 0) { // 1 s

//                LED_L_TOGGLE;
                LED_R_OFF;
//                uart_printf("%s-%s\n", __DATE__, __TIME__);
            }
        }


        if(rf433_flag) {
            LED_R_ON;
            LED_RF_ON;

            if(mm[12] == 0x40) {
                rs485_send_buf(&mm[6], 6);
            } else if(mm[12] == 0) {
                rs485_send_buf(&mm[6], 8);
            }

            if(mm[12] == 0xf4) {
                rf_set_channel(mm[13]);
                g_eep.header = EEP_RF_MAGIC;
                g_eep.rf_channel = mm[13];
                flash_write(EEP_RF_ADDR, (uint8_t *)(&g_eep), sizeof(g_eep));
                LED_L_ON;
            }

//            rs485_send_buf(mm, 31);

            rf433_flag = 0;
        } else {
            LED_RF_OFF;
        }

        Wireless_reception_control();

    }
}



