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

        if(       key_Result[i] & KEY_SHORT) {
            buf_key_status[i] = 0x01;
        } else if(key_Result[i] & KEY_LONG) {
            buf_key_status[i] = 0x03;
        } else if(key_Result[i] & KEY_LONG_UP) {
            buf_key_status[i] = 0x04;
        }

        key_Result[i] = NO_KEY;
    }
}

void key_handler_callback(uint8_t key, uint16_t shirt)
{
    static uint8_t key_temp = 0, pair_temp = 0, led_flag = 0;

    if(       multi_pair_flag == PAIR_NONE) {   // pairing start ?

        if((g_btn[key].down == 0) || (g_btn[key].down > 300)) {
            return ;
        }

    } else if(multi_pair_flag == PAIR_END) {    // pairing end ?

        multi_pair_end();
        multi_pair_flag = PAIR_NONE;
        return ;

    } else {                                    // in pairing

        if(g_btn[key].down) {                   // any key pressed again, end pairing
            if(g_btn[key].shirt == 1) {
                multi_pair_end();
                multi_pair_flag = PAIR_NONE;
                return ;
            }
        }

        if(key != pair_temp) {                  // other key pressed, ignored it
            return ;
        }
    }

// to make a pair, enter pairing mode
    if(       g_btn[key].shirt > 300) {     // 50ms * 300 = 15s

        if(multi_pair_flag == PAIR_CLEAR) {
            multi_pair_flag = PAIR_MASTER;
            led_flag = 1;
        } else {
            if((led_flag == 0) && (multi_pair_flag != PAIR_MASTER)) {
                return ;
            }
        }

// clear pair info
    } else if(g_btn[key].shirt > 200) {     // 50ms * 200 = 10s

        if(multi_pair_flag == PAIR_PREPARE) {

            multi_pair_flag = PAIR_CLEAR;
            multi_pair_clear(key);
            led_flag = 1;

        }

// prepare for pairing, set rf channel to 0
    } else if(g_btn[key].shirt > 100) {     // 50ms * 100 = 5s

        if(multi_pair_flag == PAIR_NONE) {
            led_flag = 1;
            multi_pair_prepare(key);
            multi_pair_flag = PAIR_PREPARE;
        }

        pair_temp = key;

    } else if(g_btn[key].shirt > 1) {
        return ;
    }

    if(led_flag) {

        if(led_flag % 8 == 0) {             // 50ms * 4 = 200ms interval
            GPIO_LOW(gpio_pwm[key].port,   gpio_pwm[key].pin);
            GPIO_LOW(gpio_relay[key].port, gpio_relay[key].pin);
        } else if(led_flag % 8 == 4) {
            GPIO_HIGH(gpio_pwm[key].port,   gpio_pwm[key].pin);
            GPIO_HIGH(gpio_relay[key].port, gpio_relay[key].pin);
        }

        if(led_flag >= 20) {
            led_flag = 0;
        } else {
            led_flag++;
        }

        return ;
    }

    if(multi_pair_flag != PAIR_NONE) {

//        multi_pair_indicator(key);  // led indicator task
        multi_pair_run(key);        // pairing master or slave task

        return ;

    } else {
        
    }

    if(       key_led_state[key] == 0x01) {
        key_led_state[key] = 0;
        key_temp &= ~(1 << key);
        group_state = 0;
    } else if(key_led_state[key] == 0) {
        key_led_state[key] = 0x01;
        key_temp |= (1 << key);
        group_state = 1;
    }

    protocol_rf_send(key, 0x40, key_temp);

    gpio_set(gpio_relay[key].port, gpio_relay[key].pin, key_led_state[key]);

    pwm_flag[key]=0x01;

    if(reg_mode_flag) {
        reg_mode_flag = 0;
        rf433_set_channel(dev_param.rf_ch);
    }

}


