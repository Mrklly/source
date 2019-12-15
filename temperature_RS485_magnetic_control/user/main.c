#include "global_variables.h"
#include "uart.h"
#include "wwdg.h"
#include "adc.h"
#include <string.h>
#include "ht1621.h"
#include "led.h"
#include "tim1.h"
#include "temperature.h"
#include "e2p.h"
#include "key.h"
#include "key_handler.h"
#include "modbus_rtu.h"
#include "system_timer.h"

void pritf_v(void)
{
    if(0) {
        s_printf("\r\nThis version was compiled in ");
        delay(1);
        s_printf(__TIME__);
        delay(1);
        s_printf(" on ");
        delay(1);
        s_printf(__DATE__);
        s_printf("\r\n");
        delay(1);
    }
}

void device_init(void)
{
    g_dev.addr = g_eep.addr;
    g_dev.baud = g_eep.baud_rate;
    g_dev.mode.mod = TEMP_MODE_COOL;
    g_dev.mode.wind = FAN_MIN;
    g_dev.mode.wind_auto = 0;
    g_dev.mode.power = 0;   // Air conditioner's main power
    g_dev.mode.valve = 0;
    g_dev.set_temp = 25;
    g_dev.room_temp = 24.0;
}


int main(void)
{
    unsigned int temp_systick_counter = 0;

    CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);
    CLK->CKDIVR |= (uint8_t)CLK_PRESCALER_HSIDIV1;

    eep_init();
    device_init();  // g_dev initialization
    
    USART_Configuration(g_dev.baud);
    pritf_v();
    KEY_Init();
    ADC_Init();
    temp_init();
    enableInterrupts();
    Ht1621_Init();
    delay_ms(100);
    led_init();
    lcd_show(&g_dev);
    WWDG_Configuration();
    WWDGReset();

    g_lcd_on = 1;
    g_led_on = 1;
    g_mod_backup = g_dev.mod;
    
    timer1_init();
    CTRL_PWR_OFF();

    while(1) {
        recv_hander();
        if(temp_systick_counter != systick_counter) {
            temp_systick_counter = systick_counter;
            if(systick_counter % 2 == 0) {   // 10 ms
                Refresh_WWDG_Window();
//                power_control();
            }
            
            if(systick_counter % 4 == 0) { // 20 ms
                get_room_temp();
            }
            
            if(systick_counter % 10 == 0) { // 50 ms
                button_check();
                button_handler();
                AddrMod_Set();
            }
            
            if(systick_counter % 20 == 0) { // 100 ms
                temperature_control();
                set_led_indicator();
                set_lcd_backlight();
                if(!g_set_addr_baud) {
                    lcd_show(&g_dev);
                } else {
                    lcd_show_settings(&g_dev_set);
                }
                
            }
            
            if(systick_counter % 100 == 0) { // 500 ms
                set_fan();
                valve_control();
            }
            
            if(systick_counter % 200 == 0) { // 1 s
                if(g_uart_baud_changed) {
                    g_uart_baud_changed = 0;
                    USART_Configuration(g_dev.baud);
                }
            }
        }
    }
}



