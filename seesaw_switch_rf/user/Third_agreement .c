#include "Third_agreement.h"
#include "bsp_pwm.h"

#include "global_variables.h"
#include "eeprom.h"


void protocol_run(void)
{
    if(!ReceiveFlag) {
        return ;
    }

    ReceiveFlag = 0;
    g_rx_idx = 0;

    if((protocol_get_crc(g_rx_buf, 7) != g_rx_buf[7])) {
        return ;
    }

    if(g_rx_buf[1] == MODIFY_ADDRESS) {
        Modify_Address();
    }

    if(g_rx_buf[2] != dev_param.panel_add) {
        return ;
    }

    switch(g_rx_buf[1]){
        case CONTROL_SWITCH:
            KEY_Contr();
            break;
        case SWITCH_STATE:
            SWITCH_State();
            break;
        case RESET_CMD :
            eeprom_erase_all();
            rest_config();
    }
}

void Modify_Address(void)
{
    uint8_t data[8];

    memcpy(data, g_rx_buf, 8);

    data[7] = protocol_get_crc(data, 7);
    
    uart_send_buf(data, 8);

    dev_param.SOH = EEP_ADDR_THIS_SOH;
    dev_param.panel_add = g_rx_buf[2];

    eeprom_erase_all();
    eeprom_set_param(EEP_ADDR_THIS, (uint8_t *)(&dev_param), sizeof(dev_param));
}

void KEY_Contr(void)
{
    switch(g_rx_buf[3]) {                                     //LED1
    case 0x00:                                                     //¹ØµÆÃüÁî¡£
        pwm_flag[0]=0x01;
        key_led_state[0] = 0;
        Relay_open_Close_Config(RELAY_PORT_CH1,RELAY_PIN_CH1,0);
        break;

    case 0x01:                                                     //¿ªµÆÃüÁî¡£
        pwm_flag[0]=0x01;
        key_led_state[0] = 1;
        Relay_open_Close_Config(RELAY_PORT_CH1,RELAY_PIN_CH1,1);
        break;

    case 0x02:                                                     //µÆ×´Ì¬²»±ä¡£
        break;

    default:
        break;
    }

    switch(g_rx_buf[4]) {                                    //LED2
    case 0x00:                                                     //¹ØµÆÃüÁî¡£
        pwm_flag[1]=0x01;
        key_led_state[1] = 0;
        Relay_open_Close_Config(RELAY_PORT_CH2,RELAY_PIN_CH2,0);
        break;

    case 0x01:                                                     //¿ªµÆÃüÁî¡£
        pwm_flag[1]=0x01;
        key_led_state[1] = 1;
        Relay_open_Close_Config(RELAY_PORT_CH2,RELAY_PIN_CH2,1);
        break;

    case 0x02:                                                    
        break;

    default:
        break;
    }

    switch(g_rx_buf[5]) {                                   //LED3
    case 0x00:                                                    //¹ØµÆÃüÁî¡£
        pwm_flag[2]=0x01;
        key_led_state[2] = 0;
        Relay_open_Close_Config(RELAY_PORT_CH3,RELAY_PIN_CH3,0);
        break;

    case 0x01:                                                   //¿ªµÆÃüÁî¡£
        pwm_flag[2]=0x01;
        key_led_state[2] = 1;
        Relay_open_Close_Config(RELAY_PORT_CH3,RELAY_PIN_CH3,1);
        break;

    case 0x02:                                          
        break;

    default:
        break;
    }

    switch(g_rx_buf[6]) {                                  //LED4
    case 0x00:                                                   //¹ØµÆÃüÁî¡£
        pwm_flag[3]=0x01;
        key_led_state[3] = 0;
        Relay_open_Close_Config(RELAY_PORT_CH4,RELAY_PIN_CH4,0);
        break;

    case 0x01:                                                   //¿ªµÆÃüÁî¡£
        pwm_flag[3]=0x01;
        key_led_state[3] = 1;
        Relay_open_Close_Config(RELAY_PORT_CH4,RELAY_PIN_CH4,1);
        break;

    case 0x02:                                                   //¹ØµÆÃüÁî¡£
        break;

    default:
        break;
    }
}

void SWITCH_State(void)
{
    uint8_t data[8];

    data[0] = 0x55;
    data[1] = 0x03;
    data[2] = dev_param.panel_add;

    memcpy(&data[3], key_led_state, 4);

    data[7] = protocol_get_crc(data, 7);

    uart_send_buf(data, 8);
}

void protocol_send_key_status(uint8_t *buf_key, uint8_t *buf_sta)
{
    uint8_t data[8];

    if(!g_led_flag) {
        return ;
    }

    if(g_uart_delay) {
        return ;
    }

    memset(data, 0, 8);

    data[0] = 0x55;
    data[1] = 0x05;
    data[2] = dev_param.panel_add;

    for(uint8_t i=0; i<4; i++) {
        if(buf_key[i] == KEY_VALUE(i)) {
            data[i+3]  = buf_sta[i];
        } else {
            data[i+3]  = 0;
        }
    }

    memset(buf_key, 0, 4);
    memset(buf_sta, 0, 4);

    data[7] = protocol_get_crc(data, 7);

    uart_send_buf(data, 8);

    g_led_flag = 0;
}


void rest_config(void)
{
    uint8_t data[8];

    data[0] = 0x55;
    data[1] = 0x06;
    data[2] = dev_param.panel_add;

    memset(&data[3], 0, 4);

    data[7] = protocol_get_crc(data, 7);

    uart_send_buf(data, 8);
}

uint8_t protocol_get_crc(uint8_t *buf, uint8_t len)
{
    uint8_t temp,i;
    temp = buf[0];

    for(i=1; i<len; i++) {
        temp ^= buf[i];
    }

    return temp;
}

void protocol_param_get(void)
{
    eeprom_get_param(EEP_ADDR_THIS, (uint8_t *)(&dev_param), sizeof(dev_param));

    if(dev_param.SOH != EEP_ADDR_THIS_SOH) {

        dev_param.panel_add = 0x03;

    }

    key_led_state[0]=dev_param.led_state[0];
    key_led_state[1]=dev_param.led_state[1];
    key_led_state[2]=dev_param.led_state[2];
    key_led_state[3]=dev_param.led_state[3];
}

void protocol_param_save(void)
{
    eeprom_set_param(EEP_ADDR_THIS, (uint8_t *)(&dev_param), sizeof(dev_param));
}

