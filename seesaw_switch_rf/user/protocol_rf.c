#include <string.h>

#include "bsp_gpio.h"
#include "bsp_pwm.h"
#include "drv_spi_soft.h"
#include "protocol_rf.h"
#include "global_variables.h"
#include "eeprom.h"


void protocol_rf_run()
{
    for(uint8_t i=0; i<4; i++) {

    }
}

void protocol_rf_send(uint8_t key, uint8_t cmd, uint8_t data)
{
    uint8_t i, temp[31];
    uint16_t crc;

    if(g_rf_send_flag == 1) {
        g_rf_send_flag = 0;
    }

    memcpy(&temp[0], dev_param.mac_ctrl, 6);    // 0 - 5
    memcpy(&temp[6], dev_param.mac_this, 6);    // 6 - 11

    temp[12] = cmd;                             // 12
    temp[13] = data;                            // 13
    temp[14] = 0xac;                            // 14

    memcpy(&temp[15], &dev_param.mul_ctrl_id[key][1], 6);      // 15 - 20

    temp[21] = group_state;                     // 21

    memset(&temp[22], 0, 4);                    // 22 - 25

    if(reg_dev_sum > 0) {                       // 26
        temp[26] = is_bridge_flag - 1;
    } else {
        temp[26] = is_bridge_flag;
    }

    temp[27] = (uint8_t )(self_dev_num & 0xff); // 27 - 28
    temp[28] = (uint8_t )(self_dev_num >> 8);

    crc = getCRCByMsg(temp,29);                 // 29 - 30
    temp[29] = (uint8_t )(crc >> 8);
    temp[30] = (uint8_t )(crc & 0xff);

    rf433_tx(temp, 31);
}

void multi_pair_prepare(uint8_t key)
{
    pwm_to_io();

    for(uint8_t i=0; i<4; i++) {
        GPIO_LOW(gpio_pwm[i].port,        gpio_pwm[i].pin);
        GPIO_LOW(gpio_relay[i].port,      gpio_relay[i].pin);
        GPIO_HIGH(gpio_backlight[i].port, gpio_backlight[i].pin);
    }

    rf433_set_channel(0);

    protocol_rf_send(0, 0xfa, is_bridge_flag);
    protocol_rf_send(0, 0x00, dev_param.key_num);
}

void multi_pair_end(void)
{
    io_to_pwm();

    for(uint8_t i=0; i<4; i++) {
        GPIO_LOW(gpio_relay[i].port,      gpio_relay[i].pin);
        GPIO_HIGH(gpio_backlight[i].port, gpio_backlight[i].pin);

        SetTIM1_PWM_DutyCycle(PWM_MAX, i);
        key_led_state[i] = 0;
    }

    rf433_set_channel(dev_param.rf_ch);
}


void multi_pair_indicator(uint8_t key)
{
    static uint8_t temp = 0;

    if(multi_pair_flag == PAIR_NONE) {
        temp = 0;
        return ;
    }

    temp++;

    if(temp % 10 != 0) {        // 50ms * 10 = 500ms interval
        return ;
    }

    GPIO_TOG(gpio_pwm[key].port,   gpio_pwm[key].pin);
    GPIO_TOG(gpio_relay[key].port, gpio_relay[key].pin);

    temp = 0;
}

void multi_pair_clear(uint8_t key)
{
    memset(dev_param.mul_ctrl_id[key], 0, sizeof(dev_param.mul_ctrl_id[key]));
    eeprom_set_param(EEP_ADDR_THIS, (uint8_t *)(&dev_param), sizeof(dev_param));
}

void multi_pair_run(uint8_t key)
{
    if(multi_pair_flag == PAIR_MASTER) {

        memcpy(dev_param.mul_ctrl_id[key], dev_param.mac_this, 6);
        dev_param.mul_ctrl_id[key][0] = 0xac;
        dev_param.mul_ctrl_id[key][6] = key+1;

        protocol_rf_send(key, 0xac, dev_param.key_num);

    } else if(multi_pair_flag == PAIR_SLAVE) {

        if(g_rf_buf[12] == 0xAC) {

            memcpy(dev_param.mul_ctrl_id[key], &g_rf_buf[14], 7);

        } else {

            return ;

        }

    } else {

        return ;

    }

    eeprom_set_param(EEP_ADDR_THIS, (uint8_t *)(&dev_param), sizeof(dev_param));

    multi_pair_flag = PAIR_END;
}

void multi_control(uint8_t *data, uint8_t len)
{
    uint8_t i=0, j=0;

    for(; i<dev_param.key_num; i++) {    // each key handle separately
        if(dev_param.mul_ctrl_id[i][0] == 0) {
            continue ;
        }

        for(; j<len-1; j++) {        // len = 7
            if(dev_param.mul_ctrl_id[i][j] != data[j]) {
                break ;
            }
        }

        if(j != len-1) {                        // receive id not match stored id
            continue ;
        }

        if(data[len-1] != i+1) {                // check which key in multi control
            continue ;
        }

        if(data[len]) {            // on
            GPIO_LOW(gpio_backlight[i].port, gpio_backlight[i].pin);
            GPIO_HIGH(gpio_relay[i].port,    gpio_relay[i].pin);
        } else {                    // off
            GPIO_HIGH(gpio_backlight[i].port, gpio_backlight[i].pin);
            GPIO_LOW(gpio_relay[i].port,      gpio_relay[i].pin);
        }
    }
}



