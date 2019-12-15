#include <string.h>
#include "drv_uart.h"
#include "modbus_rtu.h"
#include "global_variables.h"
#include "cmt2300.h"

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
    uart_send_buf(buff, index);
}

uint8_t uart_read_packet(uint8_t *buff)
{
    uint16_t crc16 = 0;
    uint8_t crc_len = 0;
    volatile uint8_t register_number = 0;

    if(g_rx_index == 0) {
        return 0;
    }

    if(g_rx_index >= 6) {
        if(g_rx_buf[3] == CTRL_FUN_RAED) {
            register_number = 0;
            crc_len = 4;
        } else if(g_rx_buf[3] == CTRL_FUN_WRITE) {
            register_number = g_rx_buf[5];
            crc_len = g_rx_buf[5] + 4;
        } else {
            g_rx_index = 0;
            return 0;
        }
    }

    if(g_rx_index >= 4) {
        if(g_rx_buf[3] != CTRL_FUN_RAED && g_rx_buf[3] != CTRL_FUN_WRITE) {
            g_rx_index = 0;
            return 0;
        }
    }

    if(g_rx_index >= 3) {
        if(g_rx_buf[2] != g_dev.addr && g_rx_buf[2] != 0xFF) {
            g_rx_index = 0;
            return 0;
        }
    }

    if(g_rx_index >= 2) {
        if(g_rx_buf[1] != 0x99 ) {
            g_rx_index = 0;
            return 0;
        }
    }

    if(g_rx_index >= 1) {
        if(g_rx_buf[0] != 0x77 ) {
            g_rx_index = 0;
            return 0;
        }
    }

    if((g_rx_index >= (2 + 4 + register_number + 2)) && (g_rx_index <= 16)) {
        __disable_irq();     // disable_interrupt(), disable all interrupt
        memcpy(buff, &g_rx_buf[0], (2 + 4 + register_number + 2));// backup the whole frame
        __enable_irq();      // enable_interrupt(), enable all interrupt
    } else {
        return 0;
    }

    crc16 = crc16_modbus((&g_rx_buf[2]), crc_len);

    if((g_rx_buf[2 + crc_len]      != (uint8_t)(crc16 & 0xff)) ||
        (g_rx_buf[2 + crc_len + 1] != (uint8_t)(crc16 >> 8))) {

        g_rx_index = 0;
        return 0;
    }

    g_rx_index = 0;
    return 1;
}

void modbus_recv_hander(void)
{
    uint8_t dat[16] = {0};
    link_dat	*pdat = (link_dat*)&dat;

    if(!uart_read_packet(dat)) {
        return ;
    }

    if(pdat->fun == CTRL_FUN_RAED) {
        modbus_flag = 1;

    } else if (pdat->fun == CTRL_FUN_WRITE) {
        g_dev.set_temp = pdat->buff[1];
        g_dev.mod      = pdat->buff[2];

        rf433_dev_tx(CMD_SET_TEMP, g_dev.set_temp);
        delay_ms(20);
        rf433_dev_tx(CMD_SET_MODE, g_dev.mod);
    }
}

void modbus_rx_check(void)
{
    uint8_t rf433_tx_cmd = 0, rf433_tx_data = 0;

    if(modbus_flag == 0) {
        return ;
    } else if(modbus_flag == 1) {
        if(modbus_status == 0) {
            rf433_tx_cmd = CMD_GET_TEMP_SET;
        } else if(modbus_status == 1) {
            rf433_tx_cmd = CMD_GET_TEMP_ROOM;
        } else if(modbus_status == 2) {
            rf433_tx_cmd = CMD_GET_PWR;
        } else if(modbus_status == 3) {
            rf433_tx_cmd = CMD_GET_MODE;
        } else if(modbus_status == 4) {
            rf433_tx_cmd = CMD_GET_FAN;
        } else {
            modbus_status = 0;
            rf433_tx_cmd = CMD_GET_TEMP_SET;
        }
    } else if(modbus_flag == 2) {
        if(modbus_status == 0) {
            rf433_tx_cmd = CMD_SET_TEMP;
            rf433_tx_data = g_dev.set_temp;
        } else if(modbus_status == 1) {
            rf433_tx_cmd = CMD_SET_POWER;
            rf433_tx_data = g_dev.mod;
        } else if(modbus_status == 2) {
            rf433_tx_cmd = CMD_SET_MODE;
            rf433_tx_data = g_dev.mod;
        } else if(modbus_status == 3) {
            rf433_tx_cmd = CMD_SET_FAN;
            rf433_tx_data = g_dev.mod;
        } else {
            modbus_status = 0;
            rf433_tx_cmd = CMD_SET_TEMP;
            rf433_tx_data = g_dev.set_temp;
        }
    }

    rf433_dev_tx(rf433_tx_cmd, rf433_tx_data);
    modbus_flag = 0;
}

void modbus_tx(void)
{
    uint8_t buff[16] = {0};

    buff[ADDR_READ_KEY_STA]     = 0x00; // reserved for button check sent
    buff[ADDR_READ_SET_TEMP]    = (uint8_t)(g_dev.set_temp);  //  16 >= set_temp >= 32
    buff[ADDR_READ_MODE]        = (uint8_t)(g_dev.mod);
    buff[ADDR_READ_ROOM_TEMP_H] = (uint8_t)((uint16_t)(g_dev.room_temp * 10) >> 8) ;
    buff[ADDR_READ_ROOM_TEMP_L] = (uint8_t)((uint16_t)(g_dev.room_temp * 10) & 0xFF);

    modbus_ctrl_send(CTRL_FUN_RAED, g_dev.addr, buff, 5);
}
