#include "protocol_passive.h"
#include "bsp_pwm.h"

#include "global_variables.h"
#include "eeprom.h"


void protocol_run(void)
{
    uint8_t crc, i, led_status;
    uint8_t buf_tmp[3] = {0, 0, 0};
    uint8_t buf_recv[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    if(g_uart_buf[0] == g_dev.addr_range) { // sync all panel status

        if(g_uart_idx >= 8) {

            __disable_irq();

            memcpy(buf_recv, g_uart_buf, 8);
            memset(g_uart_buf, 0, 8);
            g_uart_idx = 0;

            crc = get_crc8(buf_recv, 7);

            if(crc == buf_recv[7]) {

                led_status = buf_recv[g_ctrl_index];

                protocol_control_out(led_status);

                master_counter = 0;
                master_flag = 1;
            }

            protocol_clear_buf();

            __enable_irq();


        } else {
            return ;                         // waiting for 8 bytes
        }

    } else if(g_uart_buf[0] == 0x74) {       // query a panel

        if(g_uart_idx >= 3) {

            protocol_clear_buf();

            if(g_uart_buf[2] != 0xA9) {
                return ;
            }

            master_counter = 0;
            master_flag = 1;

            if(key_change_flag) {

                buf_tmp[0] = g_dev.addr;

                if(key_send_flag == 0) {
                    if(key_bit.key_res == 0 && key_down.key_res != 0) {
                        buf_tmp[1] = key_down.key_res;
                    } else {
                        buf_tmp[1] = key_bit.key_res;
                        key_change_flag = 0;
                    }

                    key_down.key_res = 0;
                } else {

                    buf_tmp[1] = key_bit.key_res;
                    key_change_flag = 0;
                }

                buf_tmp[2] = get_crc8(buf_tmp, 2);
                uart_send_buf(buf_tmp, 3);
                key_send_flag = 1;
                key_bit.key_res = 0;

                return ;
            }

        }

        if(g_uart_idx >= 2) {
            if(g_uart_buf[1] != 0xAA) {
                protocol_clear_buf();
                return ;
            }
        }

    } else if(g_uart_buf[0] == 0x75) {       // call a special panel

        if(g_uart_idx >= 3) {

            volatile uint8_t crc;

            crc = get_crc8(g_uart_buf, 2);

            protocol_clear_buf();

            if(g_uart_buf[2] != crc) {
                return ;
            }

            master_counter = 0;
            master_flag = 1;

            if(key_change_flag) {

                buf_tmp[0] = g_dev.addr;

                if(key_send_flag == 0) {
                    if(key_bit.key_res == 0 && key_down.key_res != 0) {
                        buf_tmp[1] = key_down.key_res;
                    } else {
                        buf_tmp[1] = key_bit.key_res;
                        key_change_flag = 0;
                    }

                    key_down.key_res = 0;
                } else {

                    buf_tmp[1] = key_bit.key_res;
                    key_change_flag = 0;
                }

                buf_tmp[2] = get_crc8(buf_tmp, 2);
                uart_send_buf(buf_tmp, 3);
                key_send_flag = 1;
                key_bit.key_res = 0;

                return ;
            }

        }

        if(g_uart_idx >= 2) {
            if(g_uart_buf[1] != g_dev.addr) {
                protocol_clear_buf();
                return ;
            }
        }

    } else if(g_uart_buf[0] == 0x5A) {                  // 0x5A, setting addr
        if(g_uart_idx >= 5) {
            crc = get_crc8(g_uart_buf, 4);

            if(crc == g_uart_buf[4]) {
                g_dev.header = EEP_MAGIC;
                g_dev.addr = g_uart_buf[1];
                g_dev.key_num = g_uart_buf[2];
                g_dev.status = g_uart_buf[3];

                master_counter = 0;
                master_flag = 1;

                if((g_dev.key_num & 0x07) > 6) {    // key_num <= 6
                    return ;
                }

                if(g_dev.addr < 0x10 || g_dev.addr > 0xEA) {
                    return ;                        // 0x10 <= g_dev.addr <= 0xEA
                }

                dev_param_set();

                g_dev.addr = 0;

                dev_param_get();

                if(g_dev.addr == g_uart_buf[1]) {
                    uart_send_buf(g_uart_buf, 5);

                    if(g_dev_with_near) {
                        near_sensor_init();
                    }
                } else {
                    g_dev.header = 0xff;
                }

                for(i = 0; i < g_dev.key_num; i++) {
                    if(g_dev.status & (1 << i)) {
                        GPIO_HIGH(gpio_backlight[i].port,   gpio_backlight[i].pin);

                        if(g_dev.key_num < 5) {
                            GPIO_HIGH(gpio_relay[i].port, gpio_relay[i].pin);
                        }
                    } else {
                        GPIO_LOW(gpio_backlight[i].port,   gpio_backlight[i].pin);

                        if(g_dev.key_num < 5) {
                            GPIO_LOW(gpio_relay[i].port, gpio_relay[i].pin);
                        }
                    }
                }

            }

            protocol_clear_buf();
        } else {
            return ;
        }
    } else {                                            // throw other data
        protocol_clear_buf();
    }
}

void protocol_control_out(uint8_t data)
{
    uint8_t i;
    static uint8_t data_last = 0;

    data_last ^= data;          // compare data_last and data,if not changed, no dimming

    for(i = 0; i < g_dev.key_num; i++) {

        if((data_last & (1 << i)) == 0) {
            continue ;
        }

        if(data & (1 << i)) {
            key_led_state[i] = 1;

            if(g_dev.key_num < 5) {     // only have 4 relays
                GPIO_HIGH(gpio_relay[i].port, gpio_relay[i].pin);
            }
        } else {
            key_led_state[i] = 0;

            if(g_dev.key_num < 5) {
                GPIO_LOW(gpio_relay[i].port, gpio_relay[i].pin);
            }
        }

        pwm_flag[i] = 0x01;
    }

    data_last = data;
}

void protocol_clear_buf(void)
{
    g_uart_idx = 0;
    g_uart_buf[0] = 0;
}

void protocol_master_check(void)
{
    if(!master_flag) {          // lost master heartbeat

        key_bit.key_res = 0x00;
    }
}

uint8_t get_crc_xor(uint8_t *buf, uint8_t len)
{
    uint8_t temp, i;
    temp = buf[0];

    for(i = 1; i < len; i++) {
        temp ^= buf[i];
    }

    return temp;
}

uint8_t get_crc_sum(uint8_t *buf, uint8_t len)
{
    uint8_t sum = 0, i;

    for(i = 0; i < len; i++) {
        sum += buf[i];
    }

    return sum;
}

uint8_t get_crc8(const void *buf, int len)
{
    const uint8_t *data = buf;
    unsigned crc = 0;
    int i, j;

    for (j = len; j; j--, data++) {
        crc ^= (*data << 8);

        for(i = 8; i; i--) {
            if (crc & 0x8000) {
                crc ^= (0x1070 << 3);
            }

            crc <<= 1;
        }
    }

    return (uint8_t)(crc >> 8);
}

uint16_t get_crc16(uint8_t *buf, uint8_t len)
{
    uint16_t crc = 0xFFFF;

    for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos];          // XOR byte into least sig. byte of crc

        for (int i = 8; i != 0; i--) {    // Loop over each bit
            if ((crc & 0x0001) != 0) {      // If the LSB is set
                crc >>= 1;                    // Shift right and XOR 0xA001
                crc ^= 0xA001;
            } else {                        // Else LSB is not set
                crc >>= 1;    // Just shift right
            }
        }
    }

    // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
    return crc;
}

void near_check(void)
{
    uint8_t i;
    uint16_t temp_pwm;

    for(i = 0; i < g_dev.key_num; i++) {

        if(pwm_flag[i] != 0x02) {
            continue ;
        }

        if(key_led_state[i]) {
            continue ;
        }

        if(!NEAR_SENSOR_STA) {
            temp_pwm = 0;
        } else {
            temp_pwm = 1800;
        }

        if(g_dev.name == QB811) {
            switch(g_dev.key_num) {
            case 4:
                backlight_set_pwm(i, temp_pwm);

                if(i == 0) {
                    backlight_set_pwm(4, temp_pwm);
                } else if(i == 3) {
                    backlight_set_pwm(5, temp_pwm);
                }

                break;

            case 3:
                backlight_set_pwm(i, temp_pwm);

                if(i == 0) {
                    backlight_set_pwm(4, temp_pwm);
                } else if(i == 2) {
                    backlight_set_pwm(3, temp_pwm);
                    backlight_set_pwm(5, temp_pwm);
                }

                break;

            case 2:
                if(i == 0) {
                    backlight_set_pwm(0, temp_pwm);
                    backlight_set_pwm(1, temp_pwm);
                    backlight_set_pwm(4, temp_pwm);
                } else if(i == 1) {
                    backlight_set_pwm(2, temp_pwm);
                    backlight_set_pwm(3, temp_pwm);
                    backlight_set_pwm(5, temp_pwm);
                }

                break;

            case 1:
                backlight_set_pwm(0, temp_pwm);
                backlight_set_pwm(1, temp_pwm);
                backlight_set_pwm(4, temp_pwm);
                break;

            default:
                break;
            }
        } else {
            backlight_set_pwm(i, temp_pwm);
        }
    }
}

