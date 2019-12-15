#include <string.h>
#include "uart.h"
#include "modbus_rtu.h"
#include "global_variables.h"
#include "eeprom.h"
#include "led.h"
#include "key_handler.h"

void modbus_ctrl_send(uint8_t fun,uint8_t s_addr,uint8_t *pdat,uint8_t len)
{
    uint8_t buff[sizeof(link_dat) + 5];
    uint8_t index = 0;
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
        __disable_irq();     // disable_interrupt(), disable all interrupt
        memcpy(buff, (void const *)g_rx_buff, (2 + 4 + register_number + 2));// backup the whole frame
        __enable_irq();     // enable_interrupt(), enable all interrupt
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

#pragma  optimize=none
void modbus_recv_hander(void)
{
    uint8_t dat[16] = {0}, buff[16] = {0};
    link_dat	*pdat = (link_dat*)&dat;

    if(!uart_read_packet(dat)) {
        return ;
    }

    if(pdat->fun == CTRL_FUN_RAED) {
        buff[ADDR_READ_KEY_STA]     = 0x00; // reserved for button check sent
        buff[ADDR_READ_SET_TEMP]    = (uint8_t)(g_dev.set_temp);  //  16 >= set_temp >= 32
        buff[ADDR_READ_MODE]        = (uint8_t)(g_dev.mod);
        buff[ADDR_READ_ROOM_TEMP_H] = (uint8_t)((uint16_t)(g_dev.room_temp * 10) >> 8) ;
        buff[ADDR_READ_ROOM_TEMP_L] = (uint8_t)((uint16_t)(g_dev.room_temp * 10) & 0xFF);

        modbus_ctrl_send(pdat->fun, pdat->st_addr, buff, pdat->reg_cnt);

    } else if (pdat->fun == CTRL_FUN_WRITE) {

        modbus_ctrl_send(pdat->fun, pdat->st_addr, pdat->buff, pdat->reg_cnt);
        // XX = = pdat->buff[0];    // reserved for button

        if((g_dev.mode.power) && (!((pdat->buff[2]) & 0x80))) {
            g_mod_backup = g_dev.mod;
            g_mod_backup_flag = 1;
            g_dev.mode.power = 0;
            g_dev.mode.valve = 0;
            g_dev.mode.wind = 0;
            g_lcd_on = 0;
            g_led_on = 0;
        } else {

            if((!g_dev.mode.power) && ((pdat->buff[2]) & 0x80) && g_mod_backup_flag) {
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

void modbus_get_mac(void)
{
    uint8_t i = 0;
    g_idx = 0;

    for(; i<4; i++) {
        RS485_SEND;
        LPUart_SendData(LPUART1, 0xAA);
        RS485_RECV;
        delay_ms(100);

        if((g_idx) && (g_rx_buff[0] == 0x55)) {
            break;
        }
    }

    for(i=0; i<10; i++) {
        if(g_idx == RX_BUFF_LEN) {
            break;
        }
        delay_ms(50);
    }

    if(i == 10) {
        printf("\n");
        return ;
    }

    if((g_idx >= 25) && (g_rx_buff[1] == 0xA5)) {

        memcpy(&g_dev_mac.SOH, &g_rx_buff[1], g_idx - 1);

        if(g_dev_mac.name[10] != DEV_TYPE_THIS) {
            memset(&g_dev_mac.name[10], 0xFF, 6);
        }

        UART_Send(&g_dev_mac.name[10], 6);
    }

    if(g_dev_mac.name[10] == DEV_TYPE_THIS) {
        eeprom_save_mac(&g_dev_mac.name[10]);
    }

    printf("\n");
}
