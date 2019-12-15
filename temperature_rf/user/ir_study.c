#include "string.h"
#include "ir_study.h"
#include "global_variables.h"
#include "key.h"


void ir_study_run(void)
{
    if(!g_ir_study_flag) {
        return ;
    }

    if(!ir_parse()) {
        return ;
    }

    __disable_irq();
    flash_write_buf(EEP_ADDR_IR + EEP_IR_OFS_ALL, (uint8_t *)(&gree_data), sizeof(gree_data));
    __enable_irq();

    printf("EEP_IR_OFS: %lx\n", EEP_ADDR_IR + EEP_IR_OFS_ALL);

    if(g_dev.set_temp < 30) {
        g_dev.set_temp++;
    } else if(g_dev.set_temp == 30) {
        g_dev.set_temp = 16;
        if(g_dev.mode.wind < FAN_MAX) {
            g_dev.mode.wind++;
        } else if(g_dev.mode.wind == FAN_MAX) {
            g_dev.mode.wind = FAN_MIN;
            if(g_dev.mode.mod < TEMP_MODE_FAN) {
                g_dev.mode.mod++;
            } else if(g_dev.mode.mod == TEMP_MODE_FAN) {
                g_dev.mode.mod = TEMP_MODE_COOL;
                if(g_dev.mode.power < 1) {
                    g_dev.mode.power++;
                } else if(g_dev.mode.power == 1) {
                    g_ir_study_flag = 0;
                    printf("exit ir study...\n");
                }
            }
        }
    }
}

void array_test(void)
{
    uint8_t array4[2][3][3][14];

    uint8_t *p = &array4[0][0][0][0];

     printf("EEP_IR_SIZE: %d\n", EEP_IR_SIZE);

    for(uint8_t i=0; i<sizeof(array4); i++){
        *p = i;
        p++;
    }

    uint8_t array3[2][3][4];

    uint8_t *q = &array3[0][0][0];

    for(uint8_t i=0; i<sizeof(array3); i++){
        *q = i;
        q++;
    }
}

const uint8_t ir_rx_mac[6] = {0x07, 0x00, 0x00, 0x00, 0x00, 0x99};

void btn_callback(uint8_t key)
{
    uint8_t rf_data[31], i;
    gree_data_t data_temp;
    uint16_t addr = EEP_ADDR_IR + EEP_IR_OFS_ALL;

    memset(rf_data, 0, 31);

    printf("read IR at: %lx\n", addr);

    flash_read_buf(addr, (uint8_t *)(&data_temp), sizeof(data_temp));

    for(i=0; i<6; i++) {
        rf_data[i] = ir_rx_mac[i];
    }
    for(i=0; i<6; i++) {
        rf_data[i+6] = g_dev_mac.name[10+i];
    }

    rf_data[12] = (uint8_t)(data_temp.data1 >> 24);
    rf_data[13] = (uint8_t)(data_temp.data1 >> 16);
    rf_data[14] = (uint8_t)(data_temp.data1 >> 8);
    rf_data[15] = (uint8_t)(data_temp.data1);

    rf_data[16] = (uint8_t)(data_temp.data2 >> 24);
    rf_data[17] = (uint8_t)(data_temp.data2 >> 16);
    rf_data[18] = (uint8_t)(data_temp.data2 >> 8);
    rf_data[19] = (uint8_t)(data_temp.data2);

    rf_data[20] = (uint8_t)(data_temp.data3);

    rf_data[21] = (uint8_t)(data_temp.data4 >> 8);
    rf_data[22] = (uint8_t)(data_temp.data4);

    for(i=23; i<26; i++) {
        rf_data[i] = 0;
    }

    rf_data[27] = 'I';
    rf_data[28] = 'R';

    uint16_t crc = getCRCByMsg(rf_data, 29);
    rf_data[29] = (uint8_t)(crc >> 8);
    rf_data[30] = (uint8_t)(crc & 0xff);

    rf433_tx(rf_data, sizeof(rf_data));
}


const uint8_t str_strt[] = "ir_eep_send_strt";  // master send start signal
const uint8_t str_stop[] = "ir_eep_send_stop";  // master send stop signal
const uint8_t str_rspd[] = "ir_eep_send_rspd";  // slave send responds
const uint8_t str_errf[] = "ir_eep_erro_rfrv";  // rf receive data error
const uint8_t str_erfs[] = "ir_eep_erro_fswt";  // flash writing error


void ir_eep_send(void)
{
    uint8_t ir_rf_status = 0;
    uint16_t cnt_send = 0, crc = 0;
    uint32_t crc_all = 0;

    uint8_t rf_buf[31], i;

    if(!g_ir_rf_send) {
        return ;
    }

    g_ir_rf_send = 0;

    while(1) {

        if(KEY_PRESSED_POWER) {
            break ;
        }

        if(KEY_PRESSED_MODE) {
            g_ir_rf_send = 1;
            continue ;
        }

        if(ir_rf_status == IR_RF_NONE) {

            if(!g_ir_rf_send) {
                continue ;
            }

            memset(rf_buf, 0, 31);

            for(i=0; i<6; i++) {
                rf_buf[i] = g_dev_mac.name[10+i];
            }

            memcpy(&rf_buf[6], str_strt, sizeof(str_strt));

            rf_buf[27] = 'I';
            rf_buf[28] = 'R';

            crc = getCRCByMsg(rf_buf, 29);
            rf_buf[29] = (uint8_t)(crc >> 8);
            rf_buf[30] = (uint8_t)(crc & 0xff);

            printf("Master sending start\n");
            rf433_tx(rf_buf, 31);

            ir_rf_status = IR_RF_WAIT;      // waiting for start header responds
            g_ir_rf_send = 0;

        } else if(ir_rf_status == IR_RF_WAIT) {

// waiting for start responds or stop responds

            if(!rf433_rx(rf_buf, 31)) {
                continue ;
            }

            crc = getCRCByMsg(rf_buf, 29);
            if(rf_buf[29] != (uint8_t)(crc >> 8) || rf_buf[30] != (uint8_t)crc) {
                printf("ir_rf_send responds crc error\n");
                ir_rf_status = IR_RF_NONE;      // resend the start header
                continue ;
            }

            if(!cnt_send) {     // data not sending, that's start responds

                if(strncmp(&rf_buf[6], str_rspd, sizeof(str_rspd))) {
                    printf("ir recv get start responds error\n");
                    continue ;
                }

                ir_rf_status = IR_RF_STRT;

            } else {           // data been sent, that's stop responds

                if(strncmp(&rf_buf[6], str_stop, sizeof(str_stop))) {
                    printf("ir recv get stop responds error\n");
                    continue ;
                }

                ir_rf_status = IR_RF_END;

            }

        } else if(ir_rf_status == IR_RF_STRT) {

            for(i=0; i<(EEP_IR_SIZE/31)+1; i++) {

                memset(rf_buf, 0, 31);

                flash_read_buf(EEP_ADDR_IR + i*31, rf_buf, 31);

                printf("read IR at: %lx\n", EEP_ADDR_IR + i*31);

                rf433_tx(rf_buf, 31);

                crc_all += getCRCByMsg(rf_buf, 31);

                cnt_send++;
            }

            ir_rf_status = IR_RF_STOP;

        } else if(ir_rf_status == IR_RF_STOP) {

            memset(rf_buf, 0, 31);

            for(i=0; i<6; i++) {
                rf_buf[i] = g_dev_mac.name[10+i];
            }

            memcpy(&rf_buf[6], str_stop, sizeof(str_stop));

            rf_buf[23] = (uint8_t)(crc_all >> 24);
            rf_buf[24] = (uint8_t)(crc_all >> 16);
            rf_buf[25] = (uint8_t)(crc_all >> 8);
            rf_buf[26] = (uint8_t)(crc_all);

            rf_buf[27] = 'I';
            rf_buf[28] = 'R';

            crc = getCRCByMsg(rf_buf, 29);
            rf_buf[29] = (uint8_t)(crc >> 8);
            rf_buf[30] = (uint8_t)(crc & 0xff);

            rf433_tx(rf_buf, 31);

            ir_rf_status = IR_RF_WAIT;

        } else if(ir_rf_status == IR_RF_END) {

            printf("IR data send ok\n");

        }
    }
}

void ir_eep_recv(void)
{
    uint8_t ir_rf_status = 0;
    uint8_t rf_buf[31], i = 0;
    uint16_t crc = 0, cnt_recv = 0;
    uint32_t crc_all = 0, crc_all_recv = 0, crc_all_flash = 0;

    if(!g_ir_rf_recv) {
        return ;
    }

    g_ir_rf_recv = 0;

    while(1) {

        if(KEY_PRESSED_POWER) {
            break;
        }

        if(KEY_PRESSED_MODE) {
            g_ir_rf_recv = 1;
            continue ;
        }

        if(!rf433_rx(rf_buf, 31)) {
            continue ;
        }

        if(       ir_rf_status == IR_RF_STRT) {

            __disable_irq();
            flash_write_buf(EEP_ADDR_IR + cnt_recv*31, rf_buf, 31);
            __enable_irq();

            crc_all  += getCRCByMsg(rf_buf, 31);
            cnt_recv++;

            if(cnt_recv >= ((EEP_IR_SIZE/31)+1)) {
                ir_rf_status = IR_RF_STOP;
            }

        } else if(ir_rf_status == IR_RF_NONE) {

            crc = getCRCByMsg(rf_buf, 29);
        
            if(rf_buf[29] != (uint8_t)(crc >> 8) || rf_buf[30] != (uint8_t)crc) {
                printf("ir recv crc error\n");
                continue ;
            }

            if(strncmp(&rf_buf[6], str_strt, sizeof(str_strt))) {
                printf("ir recv start got error\n");
                continue ;
            }

            memset(rf_buf, 0, 31);

            for(i=0; i<6; i++) {
                rf_buf[i] = g_dev_mac.name[10+i];
            }

            memcpy(&rf_buf[6], str_rspd, sizeof(str_rspd));

            rf_buf[27] = 'I';
            rf_buf[28] = 'R';

            crc = getCRCByMsg(rf_buf, 29);
            rf_buf[29] = (uint8_t)(crc >> 8);
            rf_buf[30] = (uint8_t)(crc & 0xff);

            rf433_tx(rf_buf, 31);

            ir_rf_status = IR_RF_STRT;
            
        } else if(ir_rf_status == IR_RF_STOP) {

// When all the IR data send, the master will send a stop signal,
// it contains all the IR data crc checksum in a uint32_t data

            crc = getCRCByMsg(rf_buf, 29);
        
            if(rf_buf[29] != (uint8_t)(crc >> 8) || rf_buf[30] != (uint8_t)crc) {
                printf("ir recv stop got crc error\n");
                continue ;
            }

            if(strncmp(&rf_buf[6], str_stop, sizeof(str_stop))) {
                printf("ir recv stop got error\n");
                continue ;
            }

            crc_all_recv = rf_buf[23] << 24 | rf_buf[24] << 16 | rf_buf[25] << 8 | rf_buf[26];

            if(crc_all_recv != crc_all) {
                printf("ir rf recv data crc error\n");
            }

// Check flash write error

            for(i=0; i<(EEP_IR_SIZE/31)+1; i++) {
                memset(rf_buf, 0, 31);
                flash_read_buf(EEP_ADDR_IR + 31*i, rf_buf, 31);
                crc_all_flash += getCRCByMsg(rf_buf, 31);
            }

            if(crc_all_flash != crc_all) {
                printf("ir rf recv data crc error\n");
            }

            memset(rf_buf, 0, 31);

            for(i=0; i<6; i++) {
                rf_buf[i] = g_dev_mac.name[10+i];
            }

            memcpy(&rf_buf[6], str_rspd, sizeof(str_rspd));
            
            rf_buf[27] = 'I';
            rf_buf[28] = 'R';

            crc = getCRCByMsg(rf_buf, 29);
            rf_buf[29] = (uint8_t)(crc >> 8);
            rf_buf[30] = (uint8_t)(crc & 0xff);

            rf433_tx(rf_buf, 31);

            g_ir_rf_recv = 0;
        }
    }
}

