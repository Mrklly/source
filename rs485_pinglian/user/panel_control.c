#include "panel_control.h"
#include "global_variables.h"
#include "eeprom.h"
#include "modbus_rtu.h"


void panel_check(uint8_t *buf, uint8_t *len)
{
    uint8_t buf_send[10], i = 0;

    switch (g_panel_ctrl) {

    case QUERY_NAME:
        buf_send[0] = 0xFF;
        buf_send[1] = 0xAA;
        buf_send[3] = g_dev.addr;    // panel addr
        buf_send[4] = RCU_ADDR;      // rcu addr

        if(key_down.key_res == 0) {

            buf_send[2] = 0x03;          // len
            buf_send[5] = get_crc_sum(&buf_send[3], buf_send[2] - 1);

        } else {

            buf_send[6] = key_down.key_res;
            key_down.key_res = 0;

            buf_send[2] = 0x07;          // len
            buf_send[5] = PANEL_REPLY_RCU;

            buf_send[7] = g_panel_backlight1;
            buf_send[8] = g_panel_backlight2;

            buf_send[9] = get_crc_sum(&buf_send[3], buf_send[2] - 1);
        }

        uart_send_buf(buf_send, buf_send[2] + 3);

        g_panel_ctrl = NOT_RECV;
        break ;

    case QUERY_POLL:

        buf_send[0] = 0xFF;
        buf_send[1] = 0xAA;
        buf_send[3] = g_dev.addr;    // panel addr
        buf_send[4] = RCU_ADDR;      // rcu addr

        if(key_down.key_res) {

            buf_send[6] = key_down.key_res;
            key_down.key_res = 0;
            key_bit.key_res = 0;

            buf_send[2] = 0x07;          // len
            buf_send[5] = PANEL_REPLY_RCU;

            buf_send[7] = g_panel_backlight1;
            buf_send[8] = g_panel_backlight2;

            buf_send[9] = get_crc_sum(&buf_send[3], buf_send[2] - 1);

            uart_send_buf(buf_send, 10);

        }

        break ;

    case CTRL_ONE:
//        key_bit.key_res = buf[0];
        g_panel_backlight1 = buf[1];
        g_panel_backlight2 = buf[2];
        break ;


    case CTRL_MULTI:

        for(i = 0; i < *len; i += 3) {
            if(buf[i] == g_dev.addr) {
                g_panel_backlight1 = buf[i + 1];
                g_panel_backlight2 = buf[i + 2];
                break ;
            }
        }

        break ;

    default :
        break ;

    }

}

void panel_control(void)
{
    static uint8_t last_backlight1 = 0, last_backlight2 = 0x04;
    uint8_t i = 0;

    last_backlight1 ^= g_panel_backlight1;
    last_backlight2 ^= g_panel_backlight2;

    for(i = 0; i < 4; i++) {

        if((last_backlight1 & (1 << i)) == 0) {
            continue ;
        }

        if(g_panel_backlight1 & (1 << i)) {

#if PWM_LED
            key_led_state[i] = 1;
#else
            GPIO_HIGH(gpio_led[i].port, gpio_led[i].pin);
#endif

            GPIO_HIGH(gpio_relay[i].port, gpio_relay[i].pin);
        } else {

#if PWM_LED
            key_led_state[i] = 0;
#else
            GPIO_LOW(gpio_led[i].port, gpio_led[i].pin);
#endif

            GPIO_LOW(gpio_relay[i].port, gpio_relay[i].pin);
        }
#if PWM_LED
        pwm_flag[i] = 0x01;
#endif
    }

    last_backlight1 = g_panel_backlight1;

    for(i = 0; i < 2; i++) {

        if((last_backlight2 & (1 << i)) == 0) {
            continue ;
        }

        if(g_panel_backlight2 & (1 << i)) {

#if PWM_LED
            key_led_state[i + 4] = 1;
#else
            GPIO_HIGH(gpio_led[i + 4].port, gpio_led[i + 4].pin);
#endif

        } else {

#if PWM_LED
            key_led_state[i + 4] = 0;
#else
            GPIO_LOW(gpio_led[i + 4].port, gpio_led[i + 4].pin);
#endif
        }

#if PWM_LED
        pwm_flag[i + 4] = 0x01;
#endif

    }

    if(last_backlight2 & 0x04) {
        for(i = 0; i < 6; i++) {
            if(g_panel_backlight2 & 0x04) {
                GPIO_HIGH(gpio_backlight[i].port, gpio_backlight[i].pin);
            } else {
                GPIO_LOW(gpio_backlight[i].port, gpio_backlight[i].pin);
            }
        }
    }

    last_backlight2 = g_panel_backlight2;
}

