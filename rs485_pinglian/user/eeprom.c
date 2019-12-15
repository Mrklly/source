#include "eeprom.h"
#include "bsp_flash.h"
#include "global_variables.h"


dev_param_t g_dev;


void eeprom_get_all(void)
{
    eeprom_get_param(EEP_ADDR_THIS, (uint8_t *)(&g_dev), sizeof(g_dev));

    if(g_dev.header != EEP_MAGIC) {

        printf("eeprom device part not initialized...\n");

    }
}

void eeprom_erase_all(void)
{
    __disable_irq();
    Flash_SectorErase(EEP_ADDR_THIS);
    __enable_irq();
}

void eeprom_get_param(uint16_t addr, uint8_t *buf, uint8_t len)
{
    flash_read_buf(addr, buf, len);
}

void eeprom_set_param(uint16_t addr, uint8_t *buf, uint8_t len)
{
    __disable_irq();
    Flash_SectorErase(addr);
    flash_write_buf(addr, buf, len);
    __enable_irq();
}

void dev_param_get(void)
{
    eeprom_get_param(EEP_ADDR_THIS, (uint8_t *)(&g_dev), sizeof(g_dev));

    if(g_dev.header != EEP_MAGIC) {

        g_dev.addr_range = 0xD8;
        g_dev.addr = 0x00;
        g_dev.key_num = 6;
        g_dev.status = 0;
    }

    if((g_dev.key_num & 0x07) == 2) {       // last 3 bits used for key_num, key_num = 1, 2, 3, 4, 5, 6
        if((g_dev.key_num & 0xC0) == 0) {   // bits 8&7 for key 2 type define
            dev_key_2_type = KEY_2_N;       // must specify two keys type
        } else if((g_dev.key_num & 0xC0) == 0x40) {
            dev_key_2_type = KEY_2_V;       // || left and right
        } else if((g_dev.key_num & 0xC0) == 0x80) {
            dev_key_2_type = KEY_2_H;       // =  up and down
        } else if((g_dev.key_num & 0xC0) == 0xC0) {
            dev_key_2_type = KEY_2_6;       // board has 6 keys, up and down
        }
    } else {
        dev_key_2_type = KEY_2_N;
    }

    g_dev.key_num &= ~0xC0;             // clear bits 8&7

    if(g_dev.key_num > 6) {
        g_dev.key_num = 6;
    }

    if(g_dev.near_sen) {
        g_dev_with_near = 1;
    }

    if(g_dev.name == NONE) {
        
    }

    g_dev.addr_range = (uint8_t)((g_dev.addr - 0x10) / 6) + 0xD8;
    g_ctrl_index = g_dev.addr - 0x10 + 0x01 - ((g_dev.addr_range - 0xD8) * 6);

    master_flag = 1;

    key_bit.key_res = 0;
}

void dev_param_set(void)
{
    g_dev.header = EEP_MAGIC;
    eeprom_set_param(EEP_ADDR_THIS, (uint8_t *)(&g_dev), sizeof(g_dev));
}

