#include "ir_study.h"
#include "global_variables.h"
#include "hc32_crc.h"
#include "drv_w25qxx.h"
#include "bsp_gpio.h"

void ir_study_run(void)
{
    if(!g_ir_flag) {
        return ;
    }

    g_ir_study_buf[0] = (uint8_t)(g_ir_idx >> 8);
    g_ir_study_buf[1] = (uint8_t)(g_ir_idx);

    for(uint16_t i=2,temp=0; i<g_ir_idx; i+=2) {
        temp = g_ir_study_buf[i] << 8 | g_ir_study_buf[i+1];
        temp /= 3;
        temp <<= 2;
        g_ir_study_buf[i] = (uint8_t)(temp >> 8);
        g_ir_study_buf[i+1] = (uint8_t)(temp);
    }

    uint16_t crc16 = CRC16_Get8(g_ir_study_buf, g_ir_idx);

    g_ir_study_buf[g_ir_idx++] = (uint8_t)(crc16 >> 8);
    g_ir_study_buf[g_ir_idx++] = (uint8_t)(crc16);

    W25QXX_Write_NoCheck(g_ir_study_buf, ADDR_IR_DATA, g_ir_idx);

    printf("g_dev.set_temp: %d\n", g_dev.set_temp);
    printf("ADDR_IR_DATA: %lx\n", ADDR_IR_DATA);

    if(g_dev.set_temp < 30) {
        g_dev.set_temp++;
    } else if(g_dev.set_temp == 30) {
        g_dev.set_temp = 16;
        if(g_dev.sta.wind < FAN_MAX) {
            g_dev.sta.wind++;
        } else if(g_dev.sta.wind == FAN_MAX) {
            g_dev.sta.wind = FAN_MIN;
            if(g_dev.sta.mode < TEMP_MODE_FAN) {
                g_dev.sta.mode++;
            } else if(g_dev.sta.mode == TEMP_MODE_FAN) {
                g_dev.sta.mode = TEMP_MODE_COOL;
                if(g_dev.sta.power == 0) {
                    g_dev.sta.power++;
                } else if(g_dev.sta.power) {
                    printf("exit ir study...\n");
                }
            }
        }
    }

    g_ir_study_buf[2] = 0;
    g_ir_flag = 0;
    g_ir_idx = 0;
}

void ir_study_clear_data(void)
{
    for(uint32_t i=0; i<=NUM_IR_ARRAY>>2; i++) {       // NUM_IR_ARRAY << 10 >> 12
        W25QXX_Erase_Sector(i<<12);
    }
}

void ir_study_read_one(void)
{
    uint8_t buf[1024];
    W25QXX_Read(buf, ADDR_IR_DATA, 1024);
    uart_send_buf(buf, 1024);
}

void ir_study_read_part(void)
{
    uint8_t buf[1024];
    for(uint16_t i=0; i<NUM_IR_ARRAY; i++) {
        W25QXX_Read(buf, i<<10, 1024);
        uart_send_buf(buf, 1024);
        LED_GREEN_TOG;
    }
}

void ir_study_read_whole(void)
{
    uint8_t buf[1024];
    for(uint16_t i=0; i<8<<10; i++) {
        W25QXX_Read(buf, i<<10, 1024);
        uart_send_buf(buf, 1024);
    }
}

void ir_study_write_part(void)
{
    static uint32_t addr = 0;
    uint8_t buff[4];

    if(g_uart_idx == 0) {
        return ;
    } else if (g_uart_idx == 1024) {
        W25QXX_Write_NoCheck(g_uart_buff, addr, 1024);
        addr += 1024;
        g_uart_idx = 0;

        buff[0] = 0x55;
        buff[1] = 0xaa;
        buff[2] = g_dev.addr;
        buff[3] = 0x5a;

        uart_send_buf(buff, 4);
    }

    if(addr >= ((NUM_IR_ARRAY << 10) - 1)) {
        addr = 0;
        g_ir_transfer = 0;
    }
}

void ir_repeater_send(void)
{
    uint8_t buf_flash_read[1024];
    uint16_t crc16;

    W25QXX_Read(buf_flash_read, ADDR_IR_DATA, 1024);

    uint16_t ir_len = buf_flash_read[0] << 8 | buf_flash_read[1];
    if(ir_len == 0xffff) {
        ir_len = 0;
    }
    crc16 = CRC16_Get8(buf_flash_read, ir_len);
    if(buf_flash_read[ir_len] == (uint8_t)(crc16>>8) && buf_flash_read[ir_len+1] == (uint8_t)(crc16)) {
        ir_send_raw(&buf_flash_read[2], ir_len);
    }
}

void ir_repeater_run(void)
{
    static dev_sta_t last_sta = 0;
    static uint8_t last_set_temp = 0;
    volatile uint8_t flag_send = 0;

    if(g_dev.set_temp != last_set_temp) {
        flag_send++;
        last_set_temp = g_dev.set_temp;
    }

    if(g_dev.sta.status != last_sta.status) {
        flag_send++;
        last_sta.status = g_dev.sta.status;
    }

    if(flag_send) {
        ir_repeater_send();
        flag_send = 0;
    }
}

void crc_test(void)
{
    uint8_t buf[255];
    uint16_t crc16 = 0;

    for(uint8_t i=0; i<255; i++) {
        buf[i] = i;
        printf("crc buf: 0x%X\n", buf[i]);
    }

    crc16 = CRC16_Get8(buf, 255);
}

