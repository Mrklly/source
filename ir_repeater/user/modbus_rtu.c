#include "bsp_uart.h"
#include "modbus_rtu.h"
#include "global_variables.h"


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

    if(g_uart_idx == 0) {
        return 0;
    }

    if(g_uart_idx >= 6) {
        if(g_uart_buff[3] == CTRL_FUN_RAED) {
            register_number = 0;
            crc_len = 4;
        } else if(g_uart_buff[3] == CTRL_FUN_WRITE) {
            register_number = g_uart_buff[5];
            crc_len = g_uart_buff[5] + 4;
        } else if(CTRL_FUN_W_PWR <= g_uart_buff[3] && g_uart_buff[3] <= CTRL_FUN_W_TEMP) {
            register_number = 0;
            crc_len = 4;
        } else if(g_uart_buff[3] == FS_ERASE_ALL || g_uart_buff[3] == FS_ERASE_PART || g_uart_buff[3] == FS_READ_PART || g_uart_buff[3] == FS_WRITE_PART) {
            register_number = g_uart_buff[5];
            crc_len = g_uart_buff[5] + 4;
        } else {
            g_uart_idx = 0;
            return 0;
        }
    }

    if(g_uart_idx >= 4) {
//        if(g_uart_buff[3] < CTRL_FUN_RAED || (g_uart_buff[3] > CTRL_FUN_W_TEMP && g_uart_buff[3] != CTRL_FUN_WRITE)) {
//            g_uart_idx = 0;
//            return 0;
//        }
    }

    if(g_uart_idx >= 3) {
        if(g_uart_buff[2] != g_dev.addr && g_uart_buff[2] != 0xFF) {
            g_uart_idx = 0;
            return 0;
        }
    }

    if(g_uart_idx >= 2) {
        if(g_uart_buff[1] != 0x99 ) {
            g_uart_idx = 0;
            return 0;
        }
    }

    if(g_uart_idx >= 1) {
        if(g_uart_buff[0] != 0x77 ) {
            g_uart_idx = 0;
            return 0;
        }
    }

    if(g_uart_idx >= (2 + 4 + register_number + 2)) {
        __disable_irq();     // disable_interrupt(), disable all interrupt
        memcpy(buff, (void const *)g_uart_buff, (2 + 4 + register_number + 2));// backup the whole frame
        __enable_irq();     // enable_interrupt(), enable all interrupt
    } else {
        return 0;
    }

    crc16 = crc16_modbus((&g_uart_buff[2]), crc_len);

    if((g_uart_buff[2 + crc_len]      != (uint8_t)(crc16 & 0xff)) ||
        (g_uart_buff[2 + crc_len + 1] != (uint8_t)(crc16 >> 8))) {

        g_uart_idx = 0;
        return 0;
    }

    g_uart_idx = 0;
    return 1;
}

void modbus_recv_hander(void)
{
    uint8_t dat[16] = {0}, buff[16] = {0};
    link_dat	*pdat = (link_dat*)&dat;

    if(g_ir_transfer) {

        ir_study_write_part();
        LED_RED_TOG;
        return ;

    } else if(!uart_read_packet(dat)) {
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
                if(15 < pdat->buff[1] && pdat->buff[1] < 33) {
                    g_dev.set_temp = pdat->buff[1];
                }
            }
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

// follows are for spi flash operations

    case FS_ERASE_PART:
        ir_study_clear_data();
        break;

    case FS_ERASE_ALL:
        W25QXX_Erase_Chip();
        while(W25QXX_ReadSR() & 0x01) {
            LED_RED_TOG;
            LED_GREEN_TOG;
            delay_ms(100);
        }
        break;

    case FS_READ_ONE:
        ir_study_read_one();
        break;

    case FS_READ_PART:
        ir_study_read_part();
        break;

    case FS_READ_WHOLE:
        ir_study_read_whole();
        break;

    case FS_WRITE_PART:

        ir_study_clear_data();      // erase part before writing

        g_ir_transfer = 1;
        LED_GREEN_OFF;

        buff[0] = 0x55;
        buff[1] = 0xaa;
        buff[2] = g_dev.addr;
        buff[3] = 0x5a;

        uart_send_buf(buff, 4);
        break;

    default:
        break;
    }

    if(pdat->fun != CTRL_FUN_RAED) {

        if(g_dev.sta.power) {

        } else {

            if(pdat->fun != CTRL_FUN_W_PWR) {

            } else {

            }
        }
    }
}

void modbus_get_mac(void)
{
    uint8_t i = 0;
    g_uart_idx = 0;

    for(; i<4; i++) {
        RS485_SEND;
        Uart_SendData(UARTCH1, 0xAA);
        RS485_RECV;
        delay_ms(100);

        if((g_uart_idx) && (g_uart_buff[0] == 0x55)) {
            break;
        }
    }

    for(i=0; i<10; i++) {
        if(g_uart_idx == UART_BUF_LEN) {
            break;
        }
        delay_ms(50);
    }

    if(i == 10) {
        printf("\n");
        return ;
    }

    if((g_uart_idx >= 25) && (g_uart_buff[1] == 0xA5)) {

        memcpy(&g_dev_mac.SOH, &g_uart_buff[1], g_uart_idx - 1);

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

uint16_t crc16_modbus(uint8_t *buf, uint8_t len)
{
    uint16_t crc = 0xFFFF;

      for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos];          // XOR byte into least sig. byte of crc

        for (int i = 8; i != 0; i--) {    // Loop over each bit
          if ((crc & 0x0001) != 0) {      // If the LSB is set
            crc >>= 1;                    // Shift right and XOR 0xA001
            crc ^= 0xA001;
          }
          else                            // Else LSB is not set
            crc >>= 1;                    // Just shift right
        }
      }
      // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
      return crc;
}

