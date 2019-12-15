#include <string.h>
#include "uart.h"
#include "modbus_rtu.h"
#include "global_variables.h"
#include "e2p.h"
#include "led.h"
#include "key_handler.h"

void ctrl_send(u8 fun,u8 s_addr,u8 *pdat,u8 len)
{
    u8    	buff[sizeof(link_dat) + 5];
    u8		index = 0;
    uint16_t crc16 = 0;

    if(len > 5) {
        return;
    }
    buff[index++] = 0x55;
    buff[index++] = 0xAA;
    buff[index++] = g_dev.addr;
    buff[index++] = fun;
    buff[index++] = s_addr;
    buff[index++] = len;
    memcpy(&buff[index],pdat,len);
    index += len;
    crc16 = crc16_modbus(&buff[2], index - 2);
    buff[index++] = (uint8_t)(crc16 & 0xff);
    buff[index++] = (uint8_t)(crc16 >> 8);
    UART_Send(buff,index);
}

#pragma  optimize=none
uint8_t uart_read_packet(uint8_t *buff)
{
    uint16_t crc16 = 0;
    uint8_t crc_len = 0;
    volatile uint8_t register_number = 0;

    if(g_idx == 0) {
        return 0;
    }

    if(g_idx >= 6) {
        if(g_rx_buff[3] == CTRL_FUN_RAED) {
            register_number = 0;
            crc_len = 4;
        } else if(g_rx_buff[3] == CTRL_FUN_WRITE) {
            register_number = g_rx_buff[5];
            crc_len = g_rx_buff[5] + 4;
        } else {
            g_idx = 0;
            return 0;
        }
    }

    if(g_idx >= 4) {
        if(g_rx_buff[3] != CTRL_FUN_RAED && g_rx_buff[3] != CTRL_FUN_WRITE) {
            g_idx = 0;
            return 0;
        }
    }

    if(g_idx >= 3) {
        if(g_rx_buff[2] != g_dev.addr && g_rx_buff[2] != 0xFF) {
            g_idx = 0;
            return 0;
        }
    }

    if(g_idx >= 2) {
        if(g_rx_buff[1] != 0x99 ) {
            g_idx = 0;
            return 0;
        }
    }

    if(g_idx >= 1) {
        if(g_rx_buff[0] != 0x77 ) {
            g_idx = 0;
            return 0;
        }
    }

    if(g_idx >= (2 + 4 + register_number + 2)) {
        asm("sim");     // disable_interrupt(), disable all interrupt
        memcpy(buff, (void const *)g_rx_buff, (2 + 4 + register_number + 2));// backup the whole frame
        asm("rim");     // enable_interrupt(), enable all interrupt
    } else {
        return 0;
    }

    crc16 = crc16_modbus((&g_rx_buff[2]), crc_len);
    
    if((g_rx_buff[2 + crc_len]      != (uint8_t)(crc16 & 0xff)) || 
        (g_rx_buff[2 + crc_len + 1] != (uint8_t)(crc16 >> 8))) {
        
        g_idx = 0;
        return 0;
    }

    g_idx = 0;
    return 1;
}

void recv_hander(void)
{
    uint8_t dat[16] = {0}, buff[16] = {0};
    link_dat	*pdat = (link_dat*)&dat;

    if(!uart_read_packet(dat)) {
        return ;
    }

    if(pdat->fun == CTRL_FUN_RAED) {
        buff[0] = 0x00; // reserved for button check sent
        buff[1] = (u8)(g_dev.set_temp);  //  16 >= set_temp >= 32
        buff[2] = (u8)(g_dev.mod);
        buff[3] = (u8)((u16)(g_dev.room_temp * 10) >> 8) ;
        buff[4] = (u8)((u16)(g_dev.room_temp * 10) & 0xFF);
        
        ctrl_send(pdat->fun, pdat->st_addr, buff, pdat->reg_cnt);
        
    } else if (pdat->fun == CTRL_FUN_WRITE) {

        ctrl_send(pdat->fun, pdat->st_addr, pdat->buff, pdat->reg_cnt);
        // XX = = pdat->buff[0];    // reserved for button

        if((g_dev.mode.power) && (((pdat->buff[2]) & 0x80) != 0x80)) {
            g_mod_backup = g_dev.mod;
            g_mod_backup_flag = 1;
            g_dev.mode.power = 0;
            g_dev.mode.valve = 0;
            g_dev.mode.wind = 0;
            g_lcd_on = 0;
            g_led_on = 0;
        } else {

            if((!g_dev.mode.power) && ((pdat->buff[2] & 0x80) == 0x80) && g_mod_backup_flag) {
                    g_dev.mod = g_mod_backup;
                    g_mod_backup_flag = 0;
            } else {
                g_dev.mod = pdat->buff[2];
            }
            
            g_dev.set_temp = pdat->buff[1];
            g_lcd_on = 1;
            g_led_on = 1;
            g_lcd_delay = 100;   // turn on LCD for 50 * 100ms
            g_led_delay = 100;
        }
    }
}

