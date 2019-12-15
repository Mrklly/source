#include "wireless.h"
#include "eeprom.h"
#include "global_variables.h"
#include "spi_soft.h"


void rf433_dev_tx(uint8_t datax, uint8_t datay)
{
    uint8_t i;
    uint16_t crc;
    uint8_t temp[31];

    memset(temp, 0, 31);

    for(i=0; i<6; i++) {
        temp[i] = g_dev_mac.controller_name[i];
    }
    for(i=0; i<6; i++) {
        temp[i+6] = g_dev_mac.name[10+i];
    }

    temp[12] = datax;
    temp[13] = datay;   //flag_a;  //当前四个开关的状态
    temp[14] = 0xac;

    for(i=15; i<29; i++) {
        temp[i] = 0;
    }

    crc = getCRCByMsg(temp,29); //得到CRC校验
    temp[29] = crc >> 8;
    temp[30] = crc & 0xff;

    rf433_tx(temp, 31);
}

void reg_to_controller(void)    // register to master controller
{
    rf433_set_channel(0);
    reg_mode_flag = 1;
    rf433_dev_tx(0x00, 1);
}

void rf_recv_check(void)
{
    uint8_t i, temp;
    if(SPI_SOFT_INT_IN_DATA) {
        vEnableRdFifo();
        temp = FIFO_ReadByte();
        for(i=0; i<31; i++) {
            mm[i] = 0;
        }
        for(i=0; i<31; i++) {
            mm[i] = FIFO_ReadByte();
        }
        bIntSrcFlagClr();
        vClearFIFO();
        rf433_flag = 1;
    }
}

void wireless_handler(void)
{
    uint16_t crc;
    uint8_t reg_recv[31];
    volatile uint8_t flag_a, reg_dev_sum, is_bridge_flag;

    if(!rf433_flag) {
        return ;
    }

    memcpy(reg_recv, mm, 31);
    crc = getCRCByMsg(mm,29);

    if(g_dev_mac.name[10] == 0) {   // skip register when devive mac is 0
        return ;
    }

    if(((uint8_t)(crc >> 8) != reg_recv[29]) && ((uint8_t)(crc & 0xFF) != reg_recv[30])) {
        return ;
    }

    if((g_dev_mac.name[10] == reg_recv[0]) && (g_dev_mac.name[11] == reg_recv[1]) &&
       (g_dev_mac.name[12] == reg_recv[2]) && (g_dev_mac.name[13] == reg_recv[3]) &&
       (g_dev_mac.name[14] == reg_recv[4]) && (g_dev_mac.name[15] == reg_recv[5])) {

        if((g_dev_mac.controller_name[0] == reg_recv[6]) && (g_dev_mac.controller_name[1] == reg_recv[7]) &&
           (g_dev_mac.controller_name[2] == reg_recv[8]) && (g_dev_mac.controller_name[3] == reg_recv[9]) &&
           (g_dev_mac.controller_name[4] == reg_recv[10])&& (g_dev_mac.controller_name[5] == reg_recv[11])) {

            if(reg_recv[0] == DEV_TYPE_THIS) {
                uint8_t cmd, data;
                cmd  = reg_recv[12] & 0xf0;
                data = reg_recv[13];
                if(       cmd == CMD_GET_TEMP_SET) {
                    cmd |= 0x80;
                    rf433_dev_tx(cmd, g_dev.set_temp);
                } else if(cmd == CMD_GET_TEMP_ROOM) {
                    cmd |= 0x80;    // responds cmd
                    uint8_t num = (uint8_t)((g_dev.room_temp - (uint8_t)g_dev.room_temp) * 10);
                    rf433_dev_tx(cmd | num, (int8_t)g_dev.room_temp);
                } else if(cmd == CMD_GET_PWR) {
                    cmd |= 0x80;
                    rf433_dev_tx(cmd, g_dev.sta.status);
                } else if(cmd == CMD_GET_MODE) {
                    cmd |= 0x80;
                    rf433_dev_tx(cmd, g_dev.sta.status);
                } else if(cmd == CMD_GET_FAN) {
                    cmd |= 0x80;
                    rf433_dev_tx(cmd, g_dev.sta.status);
                } else if(cmd == CMD_SET_TEMP) {
                    cmd |= 0x80;
                    g_dev.set_temp = data;
//                    rf433_dev_tx(cmd, g_dev.set_temp);
                } else if(cmd == CMD_SET_POWER) {
                    cmd |= 0x80;
                    g_dev.sta.status = data;
//                    rf433_dev_tx(cmd, g_dev.sta.status);
                } else if(cmd == CMD_SET_MODE) {
                    cmd |= 0x80;
                    g_dev.sta.status = data;
                    if(g_dev.sta.power) {
                        g_lcd_delay = g_dev.led_timeout * 10;
                        g_led_delay = g_dev.led_timeout * 10;
                        g_lcd_on = 1;
                        g_led_on = 1;
                    } else {
                        g_lcd_delay = 0;
                        g_led_delay = 0;
                        g_lcd_on = 0;
                        g_led_on = 0;
                    }
//                    rf433_dev_tx(cmd, g_dev.sta.status);
                } else if(cmd == CMD_SET_FAN) {
                    cmd |= 0x80;
                    g_dev.sta.status = data;
//                    rf433_dev_tx(cmd, g_dev.sta.status);
                } else if((reg_recv[12]==0xf4) && (reg_mode_flag == 1)) {
                    eeprom_save_controller_id(&reg_recv[6]);
                    for(uint8_t k=0; k<6; k++) {
                        g_dev_mac.controller_name[k] = reg_recv[6+k];
                    }

                    eeprom_set_rf_channel(reg_recv[13]);
                    if(eeprom_get_rf_channel() == reg_recv[13]) {
                        rf433_dev_tx(0xf1,reg_recv[13]);
                        rf433_set_channel(reg_recv[13]);
                        reg_mode_flag = 0;
                    } else {
                        eeprom_set_rf_channel(0x00);
                        rf433_set_channel(g_dev_mac.name[15]);
                    }
                } else if((reg_recv[12]==0xf2) && reg_mode_flag) {
                    eeprom_save_controller_id(&reg_recv[6]);
                    for(uint8_t k=0; k<6; k++) {
                        g_dev_mac.controller_name[k] = reg_recv[6+k];
                    }

                    eeprom_set_rf_channel(reg_recv[13]);
                    if(eeprom_get_rf_channel() == reg_recv[13]) {
                        rf433_dev_tx(0xf0,reg_recv[13]);
                        rf433_set_channel(reg_recv[13]);
                        reg_mode_flag = 0;
                    } else {
                        eeprom_set_rf_channel(0x00);
                        rf433_set_channel(g_dev_mac.name[15]);
                    }
                } else if(reg_recv[12] == 0xf6) {
                    if(reg_recv[13]==0x0) {
                    } else {
                    }
                    rf433_dev_tx(0xf3, 0);
                } else if(reg_recv[12] == 0xf8) {
                    rf433_dev_tx(0xf3, 0);
                }
            }
        } else if(reg_recv[0] == DEV_TYPE_THIS) {
            if(reg_recv[12] == 0xfe) {
                if(0) {
                    rf433_dev_tx(0xff, 0);
                }
            } else if((reg_recv[12] == 0xf4) && reg_mode_flag) {
                eeprom_save_controller_id(&reg_recv[6]);
                for(uint8_t k=0; k<6; k++) {
                    g_dev_mac.controller_name[k] = reg_recv[6+k];
                }

                eeprom_set_rf_channel(reg_recv[13]);
                if(eeprom_get_rf_channel() == reg_recv[13]) {
                    rf433_dev_tx(0xf1,reg_recv[13]);
                    rf433_set_channel(reg_recv[13]);
                    printf("sign in success...\n");
                    reg_mode_flag = 0;
                } else {
                    eeprom_set_rf_channel(0x00);
                    rf433_set_channel(g_dev_mac.name[15]);
                    printf("sign in failed...\n");
                }

            } else if((reg_recv[12] == 0xf2) && reg_mode_flag) {
                eeprom_save_controller_id(&reg_recv[6]);
                for(uint8_t k=0; k<6; k++) {
                    g_dev_mac.controller_name[k] = reg_recv[6+k];
                }

                eeprom_set_rf_channel(reg_recv[13]);
                if(eeprom_get_rf_channel() == reg_recv[13]) {
                    rf433_dev_tx(0xf0,reg_recv[13]);
                    rf433_set_channel(reg_recv[13]);
                    reg_mode_flag = 0;
                } else {
                    eeprom_set_rf_channel(0x00);
                    rf433_set_channel(g_dev_mac.name[15]);
                }
            }
        } //if(reg_recv[0]==0x01)
    } else if(
    (g_dev_mac.controller_name[0] == reg_recv[0]) && (g_dev_mac.controller_name[1] == reg_recv[1]) &&
    (g_dev_mac.controller_name[2] == reg_recv[2]) && (g_dev_mac.controller_name[3] == reg_recv[3]) &&
    (g_dev_mac.controller_name[4] == reg_recv[4]) && (g_dev_mac.controller_name[5] == reg_recv[5])) {

        if((reg_recv[12] == 0xfa) && reg_mode_flag) {          // 有设备注册 写设备 成功返回层数 失败返回失败
        } else if ((reg_recv[12] == 0xfc) && reg_mode_flag) { // 注册中继成功
            if(eeprom_get_rf_channel() == 0) {
                rf433_set_channel(g_dev_mac.name[15]);
            } else {
                rf433_set_channel(eeprom_get_rf_channel());
            }
            reg_mode_flag = 0;
        } else if ((reg_recv[12] == 0xfd) && reg_mode_flag) {  // 注册中继失败
            if(eeprom_get_rf_channel() == 0) {
                rf433_set_channel(g_dev_mac.name[15]);
            } else {
                rf433_set_channel(eeprom_get_rf_channel());
            }
            reg_mode_flag = 0;
        } else if(reg_recv[12] == 0xff) {
        } else if((reg_recv[26] == is_bridge_flag) && (is_bridge_flag!=0x0)) {
            if(reg_dev_sum > 0) {
            }
        }
    } else if((reg_recv[26] == is_bridge_flag)&&(is_bridge_flag != 0)) {
        if(reg_dev_sum > 0) {
        }
    }

    if(reg_recv[12] == 0xfb) {//删除设备
    }

    rf433_flag = 0;
}

