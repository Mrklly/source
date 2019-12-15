#include "key_handler.h"
#include "led.h"
#include "global_variables.h"
#include "eeprom.h"


const com_io_t g_io[] = {
    {KEY_PORT_1, KEY_PIN_1, NULL},
    {KEY_PORT_2, KEY_PIN_2, NULL},
    {KEY_PORT_3, KEY_PIN_3, NULL},
    {KEY_PORT_4, KEY_PIN_4, NULL}
};

key_status_t g_key_status[KEY_COUNT] = {0};


void key_check(void)
{
    uint8_t i = 0;
    for(; i < KEY_COUNT; i++) {
        key_is_down(g_io[i].key_port, g_io[i].key_pin, &g_key_status[i].shirt);
    }
}

void key_handler(void)
{
    if(       g_key_status[3].shirt == 0) {

        return ;

    } else if(g_key_status[3].shirt == 1) {

        if(g_rf_channel == 0) {
            if(g_eep.rf_channel != 0) {
                g_rf_channel = g_eep.rf_channel;
                LED_L_ON;
            }
        } else if(g_rf_channel == g_eep.rf_channel){
            g_rf_channel = 0;
            LED_L_OFF;
        }

    } else if(g_key_status[3].shirt > 40) {

        g_rf_channel = 0;
        g_eep.header = 0;
        g_eep.rf_channel = 0;
        flash_erase(EEP_RF_ADDR, sizeof(g_eep));
        
        for(uint8_t i=0; i<5; i++) {
            LED_L_TOGGLE;
            delay_ms(200);
        }
        
        LED_L_OFF;

    } else if(g_key_status[0].shirt > 4) {

        return ;

    }

    rf_set_channel(g_rf_channel);
    
//    UART_Send("111", 3);
}

