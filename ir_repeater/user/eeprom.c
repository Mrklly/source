#include "eeprom.h"
#include "bsp_flash.h"
#include "bsp_uart.h"
#include "global_variables.h"


void eeprom_get_all(void)
{
    eeprom_get_param(EEP_ADDR_THIS, (uint8_t *)(&g_eep),      sizeof(g_eep));

    if(g_eep.SOH != EEP_ADDR_THIS_SOH) {
        g_eep.SOH = EEP_ADDR_THIS_SOH;
        g_eep.addr = 0x03;
        g_eep.baud_rate = BAUD19200;
        g_eep.rf_ch = 101;
        memset(g_eep.mac_this, 0, sizeof(g_eep.mac_this));
        memset(g_eep.mac_ctrl, 0, sizeof(g_eep.mac_ctrl));
        memset(g_eep.mac_panel, 0, sizeof(g_eep.mac_panel));

        eeprom_set_param(EEP_ADDR_THIS, (uint8_t *)(&g_eep), sizeof(g_eep));

        printf("eeprom device part not initialized...\n");
    }

    g_dev.rf_ch = g_eep.rf_ch;

    if(g_eep.mac_this[0] == DEV_TYPE_THIS) {
        memcpy(g_dev.mac_this, g_eep.mac_this, 6);
    } else {
        memset(g_dev.mac_this, 0, 6);
    }

    if(g_eep.mac_ctrl[0] == DEV_TYPE_CTRL) {
        memcpy(g_dev.mac_ctrl, g_eep.mac_ctrl, 6);
    } else {
        memset(g_dev.mac_ctrl, 0, 6);
    }

    if(g_eep.mac_panel[0] == DEV_TYPE_CTRL) {
        memcpy(g_dev.mac_panel, g_eep.mac_panel, 6);
    } else {
        memset(g_dev.mac_panel, 0, 6);
    }

//    printf("g_dev.rf_ch:     %d\n", g_dev.rf_ch);
//
//    printf("g_dev.mac_this:      ");
//    for(uint8_t i=0; i<6; i++) {
//        printf("%02x", g_dev.mac_this[i]);
//    }
//    printf("\n");
//
//    printf("g_dev.mac_ctrl:      ");
//    for(uint8_t i=0; i<6; i++) {
//        printf("%02x", g_dev.mac_ctrl[i]);
//    }
//    printf("\n");
//
//    printf("g_dev.mac_panel:     ");
//    for(uint8_t i=0; i<6; i++) {
//        printf("%02x", g_dev.mac_panel[i]);
//    }
//    printf("\n");

    g_dev.addr      = g_eep.addr;
    g_dev.baud      = g_eep.baud_rate;
    g_dev.temp_offset = g_eep.temp_offset;
    g_dev.sta.mode = TEMP_MODE_COOL;
    g_dev.sta.wind = FAN_MIN;
    g_dev.sta.wind_auto = 0;
    g_dev.sta.power = 0;   // Air conditioner's main power
    g_dev.sta.valve = 0;
    g_dev.set_temp = 25;
    g_dev.room_temp = 24.0;
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

void modbus_set_mac(void)
{
    uint8_t i = 0;
    g_uart_idx = 0;

    dev_settings_t dev_settings;

    for(; i<4; i++) {
        uart_send_byte(0xAA);
        delay_ms(100);

        if((g_uart_idx) && (g_uart_buff[0] == 0x55)) {   // 0x55 + SOH(0xA5)
            break;
        }
    }

    for(i=0; i<100; i++) {
        if(g_uart_idx == UART_BUF_LEN) {
            break;
        }
        delay_ms(5);
    }

    if(i == 100) {
        printf("\n");
        return ;
    }

    if((g_uart_idx >= 25) && (g_uart_buff[1] == EEP_ADDR_THIS_SOH)) {

        memcpy(&dev_settings.SOH, &g_uart_buff[1], sizeof(dev_settings));

        if(dev_settings.name[10] != DEV_TYPE_THIS) {
            memset(&dev_settings.name[10], 0xFF, 6);
        }

        uart_send_buf(&dev_settings.name[10], 6);
    }

    if(dev_settings.name[10] == DEV_TYPE_THIS) {

        g_eep.SOH = EEP_ADDR_THIS_SOH;
        memcpy(g_eep.mac_this, &dev_settings.name[10], 6);
        eeprom_set_param(EEP_ADDR_THIS, (uint8_t *)(&g_eep), sizeof(g_eep));
    }

    printf("\n");
}


