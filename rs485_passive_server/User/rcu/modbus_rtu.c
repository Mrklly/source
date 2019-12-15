#include <string.h>
#include "bsp_uart.h"
#include "modbus_rtu.h"
#include "global_variables.h"


void modbus_ctrl_send(uint8_t *buf, uint8_t len)
{
    uart_send_buf(buf, len);
}

uint8_t uart_read_packet(void)
{
    uint8_t crc_res = 0, crc_len = 0, receive_len = 0;
    volatile uint8_t register_number = 0;

    if(g_uart_idx == 0) {
        return 0;
    }

    if(g_protocol_type == protocol_pinglian) {

        if(g_uart_idx >= 3 + g_uart_buf[2]) {
            crc_res = get_crc_sum(&g_uart_buf[3], g_uart_buf[2]-1);

            if(crc_res != g_uart_buf[g_uart_idx-1]) {
                g_uart_idx = 0;
                return 0;
            }
            
            receive_len = g_uart_idx;
            g_uart_idx = 0;
            return receive_len;
        }

        if(g_uart_idx >= 3) {

        }

        if(g_uart_idx >= 2) {
            if(g_uart_buf[1] != PROTOCOL_HEAD_2) {
                g_uart_idx = 0;
                return 0;
            }
        }

        if(g_uart_idx >= 1) {
            if(g_uart_buf[0] != PROTOCOL_HEAD_1) {
                g_uart_idx = 0;
                return 0;
            }
        }

    } else {        // g_protocol_type == protocol_poll g_protocol_type == protocol_call

        if(g_uart_idx >= 3) {

            uint8_t i=0;

            crc_res = get_crc(g_uart_buf, 2);

            if(crc_res == g_uart_buf[2]) {

                buf_send[0] = (uint8_t)((g_uart_buf[0] - 0x10) / 6) + 0xD8;
                
                for(i=0; i<6; i++) {
                    if(g_uart_buf[1] & (1 << i)) {
                        if(g_dev_status[g_uart_buf[0]] & (1 << i)) {
                            g_dev_status[g_uart_buf[0]] &= ~(1 << i);
                        } else {
                            g_dev_status[g_uart_buf[0]] |=  (1 << i);
                        }
                    }
                }

                for(i=1; i<7; i++) {
                    buf_send[i] = g_dev_status[((g_uart_buf[0] - 0x10) / 6) * 6 + 0x10 + i - 1];
                }

                buf_send[7] = get_crc(buf_send, 7);

                g_uart_send = 1;
                uart_send_buf(buf_send, 8);
                g_uart_send = 0;
            }

            g_uart_idx = 0;

        } else {
        
        }
    }

    return 0;
}

void modbus_recv_hander(void)
{
    uint8_t data_len = 0;

    data_len = uart_read_packet();

    if(data_len == 0) {                         // not received

        return ;

    } else if(data_len == 6) {                  // rcu query panel

        if(g_uart_buf[4] == RCU_ADDR) {
            if(g_uart_buf[3] == g_current_panel-1) {
                return ;                        // panel key not changed
            }
        }

    } else if(data_len > 6){                   // rcu control panel

        if(g_uart_buf[5] == PANEL_REPLY_RCU) {
            if(g_uart_buf[4] == RCU_ADDR) {    // control one panel

                g_current_panel = g_uart_buf[3];
                g_panel_key = g_uart_buf[6];
                g_panel_backlight1 = g_uart_buf[7];
                g_panel_backlight2 = g_uart_buf[8];

                uint8_t i=0;
                for(; i<4; i++) {
                    if(g_panel_key & (1 << i)) {
                        g_panel_backlight1 ^= (1 << i);
                    }
                }

                for(; i<6; i++) {
                    if(g_panel_key & (1 << i)) {
                        g_panel_backlight2 ^= (1 << (i-4));
                    }
                }

                uint8_t buf_send[10];

                buf_send[0] = 0xFF;
                buf_send[1] = 0xAA;
                buf_send[2] = 0x07;
                buf_send[3] = 0xFE;
                buf_send[4] = g_current_panel;
                buf_send[5] = RCU_CTRL_ONE_PANEL;
                buf_send[6] = g_panel_key;
                buf_send[7] = g_panel_backlight1;
                buf_send[8] = g_panel_backlight2;
                buf_send[9] = get_crc_sum(&buf_send[3], buf_send[2] - 1);

                g_uart_send = 1;
                uart_send_buf(buf_send, 10);
                g_uart_send = 0;
            }
        }
    }

}


