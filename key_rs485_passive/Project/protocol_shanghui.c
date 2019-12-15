#include <string.h>

#include "protocol_shanghui.h"
#include "global_variable.h"

#include "stm8s_tim2.h"

#include "bsp_gpio.h"
#include "bsp_uart.h"


uint8_t get_crc(uint8_t *buf, uint8_t len)
{
    uint8_t temp,i;
    temp = buf[0];

    for(i=1; i<len; i++) {
        temp ^= buf[i];
    }

    return temp;
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


void protocol_run(void)
{
    uint8_t crc, i;
    uint8_t addr_temp, led_status;
    uint8_t buf_tmp[3];
    uint8_t buf_recv[8];
    
    if(g_uart_buf[0] == g_dev.addr_range) { // sync all panel status

        if(g_uart_idx >= 8) {

//            UART1_ITConfig(UART1_IT_RXNE_OR, DISABLE);
            asm("sim");
            
            memcpy(buf_recv, g_uart_buf, 8);
            memset(g_uart_buf, 0, 8);
            g_uart_idx = 0;

            crc = get_crc8(buf_recv, 7);
            if(crc == buf_recv[7]) {

                addr_temp = g_dev.addr - 0x10 + 0x01 - ((g_dev.addr_range - 0xD8) * 6);

                led_status = buf_recv[addr_temp];

                dev_status_set(led_status);


                TIM2->CNTRH = 0;
                TIM2->CNTRL = 0;
                master_flag = 1;
            }
            protocol_clear_buf();

            asm("rim");
//            UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);

        } else {
            return ;                                    // waiting for 8 bytes
        }
        
    } else if(g_uart_buf[0] == 0x74) {       // query a panel

        if(g_uart_idx >= 3) {

            if(g_uart_buf[2] != 0xA9) {     // CRC_XOR = 0xDE, CRC8 = 0xA9
                protocol_clear_buf();
                return ;
            }

//            UART1_ITConfig(UART1_IT_RXNE_OR, DISABLE);
            protocol_clear_buf();
//            UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);

            master_flag = 1;
            TIM2->CNTRH = 0;
            TIM2->CNTRL = 0;

            if(key_change_flag) {

                buf_tmp[0] = g_dev.addr;

                if(key_send_flag == 0) {
                    if(key_Result == 0 && key_down_value != 0) {
                        buf_tmp[1] = key_down_value;
                    } else {
                        buf_tmp[1] = key_Result;
                        key_change_flag = 0;
                    }
                    key_down_value = 0;
                } else {
                    buf_tmp[1] = key_Result;
                    key_change_flag = 0;
                }

                buf_tmp[2] = get_crc8(buf_tmp, 2);
                uart_send_buf(buf_tmp, 3);
                key_send_flag = 1;
                key_Result = 0;

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

            master_flag = 1;

            if(key_change_flag) {

                buf_tmp[0] = g_dev.addr;

                if(key_send_flag == 0) {
                    if(key_Result == 0 && key_down_value != 0) {
                        buf_tmp[1] = key_down_value;
                    } else {
                        buf_tmp[1] = key_Result;
                        key_change_flag = 0;
                    }
                    key_down_value = 0;
                } else {
                    buf_tmp[1] = key_Result;
                    key_change_flag = 0;
                }

                buf_tmp[2] = get_crc8(buf_tmp, 2);
                uart_send_buf(buf_tmp, 3);
                key_send_flag = 1;
                key_Result = 0;

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
                g_dev.addr = g_uart_buf[1];
                g_dev.ch = g_uart_buf[2];
                g_dev.status = g_uart_buf[3];

                for(i=0; i<4; i++) {
                    if(g_dev.status & (1 << i)) {
                        GPIO_HIGH(gpio_led[i].port,   gpio_led[i].pin);
                        GPIO_HIGH(gpio_relay[i].port, gpio_relay[i].pin);
                    } else {
                        GPIO_LOW(gpio_led[i].port,   gpio_led[i].pin);
                        GPIO_LOW(gpio_relay[i].port, gpio_relay[i].pin);
                    }
                }
                master_flag = 1;
                TIM2->CNTRH = 0;
                TIM2->CNTRL = 0;

                asm("sim");
                dev_param_set();
                asm("rim");

                g_dev.addr = 0;

                flash_read(EEPROM_ADDR_THIS, (uint8_t *)(&g_dev), sizeof(g_dev));

                if(g_dev.addr == g_uart_buf[1]) {
                    uart_send_buf(g_uart_buf, 5);
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

void protocol_master_check(void)
{
    if(!master_flag) {          // lost master heartbeat

        key_Result = 0x00;

//        for(uint8_t i=0; i<4; i++) {
//            GPIO_LOW(gpio_led[i].port,   gpio_led[i].pin);
//            GPIO_LOW(gpio_relay[i].port, gpio_relay[i].pin);
//        }
    }
}

void protocol_clear_buf(void)
{
    g_uart_idx = 0;
    g_uart_buf[0] = 0;
}

