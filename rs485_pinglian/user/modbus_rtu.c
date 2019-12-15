#include <string.h>
#include "bsp_uart.h"
#include "modbus_rtu.h"
#include "global_variables.h"
#include "eeprom.h"
#include "key_handler.h"


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
    
    return 0;
}

void modbus_recv_hander(uint8_t *buf, uint8_t *len)
{
    uint8_t data_len = 0;

    data_len = uart_read_packet();

    if(data_len == 0) {                         // not received

        g_panel_ctrl = NOT_RECV;
        return ;

    } else if(data_len == 6) {                  // rcu query panel

        if(g_uart_buf[4] == PROTOCOL_BROADCAST) {
            if(g_uart_buf[3] == RCU_ADDR) {
                g_panel_ctrl = QUERY_POLL;
                master_flag = 1;
                master_counter = 0;
                return ;
            }
        }

        g_panel_ctrl = NOT_RECV;

    } else if(data_len > 6) {                    // rcu control panel

        if(g_uart_buf[5] == RCU_CTRL_ONE_PANEL) {
            if(g_uart_buf[4] == g_dev.addr) {   // control one panel
                g_panel_ctrl = CTRL_ONE;
                master_flag = 1;
                master_counter = 0;
            }

        } else if(g_uart_buf[5] == RCU_CTRL_MULTI_PANEL) {
            if(g_uart_buf[4] == 0) {            // control multi panel
                g_panel_ctrl = CTRL_MULTI;
                master_flag = 1;
                master_counter = 0;
            }
        } else if(g_uart_buf[5] == RCU_SET_ONE_PANEL) {
            if(g_uart_buf[4] == g_dev.addr) {   // set one panel

                g_dev.addr = g_uart_buf[6];
                g_dev.key_num = g_uart_buf[7];
//                g_dev.addr = g_uart_buf[8];

                dev_param_set();

                g_dev.addr = 0;

                dev_param_get();

                if(g_dev.addr == g_uart_buf[6]) {
                    uart_send_buf(g_uart_buf, 10);
                }
            }
        } else if(g_uart_buf[5] == RCU_SET_MULTI_PANEL) {
            if(g_uart_buf[4] == 0) {            // set multi panel

                g_dev.addr = g_uart_buf[6];
                g_dev.key_num = g_uart_buf[7];
//                g_dev.addr = g_uart_buf[8];

                dev_param_set();

                g_dev.addr = 0;

                dev_param_get();

                if(g_dev.addr == g_uart_buf[6]) {
                    uart_send_buf(g_uart_buf, 10);
                }

            }
        }

        *len = g_uart_buf[2] - 4;
        memcpy(buf, &g_uart_buf[6], *len);
    }

}

void master_check(void)
{
    if(!master_flag) {          // lost master heartbeat

        key_bit.key_res = 0;
        key_down.key_res = 0;
    }
}

