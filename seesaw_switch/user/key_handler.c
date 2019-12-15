#include "key_handler.h"
#include "global_variables.h"
#include "Third_agreement.h"


void KEY_LED_Control(void)
{
    uint8_t buf_key[4] = {0, 0, 0, 0}, buf_sta[4] = {0, 0, 0, 0};

    for(uint8_t i=0; i<4; i++) {

        if(key_Result[i] == NO_KEY) {
            continue ;
        }

        buf_key[i] = KEY_VALUE(i);

        if(       key_Result[i] & KEY_SHORT) {  //键值0x11,按键1短按。
            buf_sta[i] = 0x01;
        } else if(key_Result[i] & KEY_LONG) {   //键值0x11,按键1短按。
            buf_sta[i] = 0x03;
        } else if(key_Result[i] & KEY_LONG_UP) {//键值0x11,按键1短按。
            buf_sta[i] = 0x04;
        }

        protocol_send_key_status(buf_key, buf_sta);

        buf_key[i] = 0;
        buf_sta[i] = 0;

        key_Result[i] = NO_KEY;
    }
}

void key_value_pack(uint8_t *buf_key_value, uint8_t *buf_key_status)
{
    for(uint8_t i=0; i<4; i++) {

        if(key_Result[i] == NO_KEY) {
            continue ;
        }

        g_led_flag = 1;
        g_uart_delay = UART_DELAY_MS;

        buf_key_value[i] = KEY_VALUE(i);

        if(       key_Result[i] & KEY_SHORT) {  //键值0x11,按键1短按。
            buf_key_status[i] = 0x01;
        } else if(key_Result[i] & KEY_LONG) {   //键值0x11,按键1短按。
            buf_key_status[i] = 0x03;
        } else if(key_Result[i] & KEY_LONG_UP) {//键值0x11,按键1短按。
            buf_key_status[i] = 0x04;
        }

        key_Result[i] = NO_KEY;
    }
}

