#include <string.h>
#include "uart.h"
#include "modbus_rtu.h"
#include "global_variables.h"
#include "eeprom.h"
#include "drv_led.h"
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
    uart_send_buf(buff,index);
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
        } else if(CTRL_FUN_W_PWR <= g_rx_buff[3] && g_rx_buff[3] <= CTRL_FUN_W_TEMP) {
            register_number = 0;
            crc_len = 4;
        } else {
            g_idx = 0;
            return 0;
        }
    }

    if(g_idx >= 4) {
        if(g_rx_buff[3] < CTRL_FUN_RAED || (g_rx_buff[3] > CTRL_FUN_W_TEMP && g_rx_buff[3] != CTRL_FUN_WRITE)) {
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

    switch(pdat->fun) {

    case CTRL_FUN_RAED:
        buff[ADDR_READ_KEY_STA]  = 0x00; // reserved for button check sent
        buff[ADDR_READ_SET_TEMP] = g_dev.set_temp;  //  16 >= set_temp >= 32

        buff[ADDR_READ_SET_TEMP] &=~0x80;
        if(g_dev.flag_fan_ultralow && g_fan_ultralow) {
            buff[ADDR_READ_SET_TEMP] |= 0x80;
        }

        buff[ADDR_READ_MODE]        = g_dev.sta.status;
        buff[ADDR_READ_ROOM_TEMP_H] = (uint16_t)(g_dev.room_temp * 10) >> 8;
        buff[ADDR_READ_ROOM_TEMP_L] = (uint16_t)(g_dev.room_temp * 10);

        modbus_ctrl_send(pdat->fun, pdat->st_addr, buff, pdat->reg_cnt);
        break;

    case CTRL_FUN_WRITE:
        modbus_ctrl_send(pdat->fun, pdat->st_addr, pdat->buff, pdat->reg_cnt);
        // XX = = pdat->buff[0];    // reserved for button

        if((g_dev.sta.power) && (!((pdat->buff[2]) & 0x80))) {
            g_mod_backup = g_dev.sta.status;
            g_mod_backup_flag = 1;
            g_dev.sta.status = 0;
            g_lcd_on = 0;
            g_led_on = 0;
        } else {

            if((!g_dev.sta.power) && ((pdat->buff[2]) & 0x80) && g_mod_backup_flag) {
                    g_dev.sta.status = g_mod_backup;
                    g_mod_backup_flag = 0;
            } else {
                g_dev.sta.status = pdat->buff[2];
            }

            g_fan_ultralow = 0;
            if(pdat->buff[1] & 0x80) {
                pdat->buff[1] &= ~0x80;
                if(g_dev.flag_fan_ultralow) {
                    g_fan_ultralow = 1;
                    g_dev.sta.wind = FAN_MIN;
                    g_dev.sta.wind_auto = 0;
                }
            }

            if((g_dev.set_temp & (~0x80)) != pdat->buff[1]) {   // last bit used for ultralow fan speed
                g_lcd_func = SHOW_TEMP_SET;
                g_lcd_func_delay = LCD_FUNC_DELAY_TIME;
                if(15 < pdat->buff[1] && pdat->buff[1] < 33) {
                    g_dev.set_temp = pdat->buff[1];
                }
            }

            g_lcd_on = 1;
            g_led_on = 1;
            g_lcd_delay = g_dev.led_timeout * 10;
            g_led_delay = g_dev.led_timeout * 10;
        }
        return ;
        break;

    case CTRL_FUN_W_PWR:
        g_dev.sta.power = pdat->reg_cnt;
        break;

    case CTRL_FUN_W_MODE:
        g_dev.sta.mode =  pdat->reg_cnt;
        break;

    case CTRL_FUN_W_FAN:
        g_fan_ultralow = 0;
        switch(pdat->reg_cnt) {
        case 0x01:
            g_dev.sta.wind_auto = 0;
            g_dev.sta.wind = FAN_MIN;
            if(g_dev.flag_fan_ultralow) {
                g_fan_ultralow = 1;
            }
            break;

        case 0x02:
        case 0x03:
        case 0x04:
            g_dev.sta.wind = pdat->reg_cnt - 1;
            break;

        case 0x05:
            g_dev.sta.wind_auto = 1;
            break;

        case 0x06:
            g_dev.sta.wind_auto = 0;
            if(g_fan_ultralow) {
               g_fan_ultralow = 0;
               g_dev.sta.wind = FAN_MIN;
            } else if(g_dev.sta.wind < FAN_MAX) {
                g_dev.sta.wind++;
            }
            break;

        case 0x07:
            g_dev.sta.wind_auto = 0;
            if(g_dev.sta.wind > FAN_MIN) {
                g_dev.sta.wind--;
            } else if(g_dev.sta.wind == FAN_MIN){
                if(g_dev.flag_fan_ultralow) {
                    g_fan_ultralow = 1;
                }
            }
            break;
        default :
            break;

        }
        break;

    case CTRL_FUN_W_TEMP:
        g_lcd_func = SHOW_TEMP_SET;
        g_lcd_func_delay = LCD_FUNC_DELAY_TIME;
        if(pdat->reg_cnt == 0x01) {
            if(g_dev.set_temp < 32) {
                g_dev.set_temp++;
            }
        } else if(pdat->reg_cnt == 0x02) {
            if(g_dev.set_temp > 16) {
                    g_dev.set_temp--;
            }
        } else {
            if(15 < pdat->reg_cnt && pdat->reg_cnt < 33) {
                g_dev.set_temp = pdat->reg_cnt;
            }
        }

        break;

    default:
        break;

    }

    if(pdat->fun != CTRL_FUN_RAED) {

        if(g_dev.sta.power) {

            g_lcd_on = 1;
            g_lcd_delay = g_dev.led_timeout * 10;

            g_led_on = 1;
            g_led_delay = g_dev.led_timeout * 10;

        } else {

            if(pdat->fun != CTRL_FUN_W_PWR) {

                g_lcd_on = 1;
                g_lcd_delay = 3 * 10;   // 3s

                g_led_on = 1;
                g_led_delay = 3 * 10;

            } else {

                g_lcd_on = 0;
                g_lcd_delay = 0;
                g_led_on = 0;
                g_lcd_delay = 0;

            }
        }
    }

#if 0
    if(pdat->fun == CTRL_FUN_RAED) {
        buff[ADDR_READ_KEY_STA]  = 0x00; // reserved for button check sent
        buff[ADDR_READ_SET_TEMP] = g_dev.set_temp;  //  16 >= set_temp >= 32

        buff[ADDR_READ_SET_TEMP] &=~0x80;
        if(g_dev.flag_fan_ultralow && g_fan_ultralow) {
            buff[ADDR_READ_SET_TEMP] |= 0x80;
        }

        buff[ADDR_READ_MODE]        = g_dev.sta.status;
        buff[ADDR_READ_ROOM_TEMP_H] = (uint16_t)(g_dev.room_temp * 10) >> 8;
        buff[ADDR_READ_ROOM_TEMP_L] = (uint16_t)(g_dev.room_temp * 10);

        modbus_ctrl_send(pdat->fun, pdat->st_addr, buff, pdat->reg_cnt);

    } else if (pdat->fun == CTRL_FUN_WRITE) {

        modbus_ctrl_send(pdat->fun, pdat->st_addr, pdat->buff, pdat->reg_cnt);
        // XX = = pdat->buff[0];    // reserved for button

        if((g_dev.sta.power) && (!((pdat->buff[2]) & 0x80))) {
            g_mod_backup = g_dev.sta.status;
            g_mod_backup_flag = 1;
            g_dev.sta.power = 0;
            g_dev.sta.valve = 0;
            g_dev.sta.wind = 0;
            g_lcd_on = 0;
            g_led_on = 0;
        } else {

            if((!g_dev.sta.power) && ((pdat->buff[2]) & 0x80) && g_mod_backup_flag) {
                    g_dev.sta.status = g_mod_backup;
                    g_mod_backup_flag = 0;
            } else {
                g_dev.sta.status = pdat->buff[2];
            }

            g_fan_ultralow = 0;
            if(pdat->buff[1] & 0x80) {
                pdat->buff[1] &= ~0x80;
                if(g_dev.flag_fan_ultralow) {
                    g_fan_ultralow = 1;
                    g_dev.sta.wind = FAN_MIN;
                    g_dev.sta.wind_auto = 0;
                }
            }

            if((g_dev.set_temp & (~0x80)) != pdat->buff[1]) {   // last bit used for ultralow fan speed
                g_lcd_func = SHOW_TEMP_SET;
                g_lcd_func_delay = LCD_FUNC_DELAY_TIME;
                if(15 < pdat->buff[1] && pdat->buff[1] < 33) {
                    g_dev.set_temp = pdat->buff[1];
                }
            }

            g_lcd_on = 1;
            g_led_on = 1;
            g_lcd_delay = g_dev.led_timeout * 10;
            g_led_delay = g_dev.led_timeout * 10;
        }
    }
#endif
}

void modbus_get_mac(void)
{
    uint8_t i = 0;
    g_idx = 0;

    for(; i<4; i++) {
        RS485_SEND;
        LPUart_SendData(LPUART0, 0xAA);
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

        uart_send_buf(&g_dev_mac.name[10], 6);
    }

    if(g_dev_mac.name[10] == DEV_TYPE_THIS) {
        eeprom_save_mac(&g_dev_mac.name[10]);
    }

    printf("\n");
}
