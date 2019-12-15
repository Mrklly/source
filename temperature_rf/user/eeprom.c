#include "eeprom.h"


eep_this g_eep;
eep_ctrl g_eep_ctrl;


void eeprom_get_all(void)
{
    eeprom_get_param(EEP_ADDR_THIS, (uint8_t *)(&g_eep),      sizeof(g_eep));
    eeprom_get_param(EEP_ADDR_CTRL, (uint8_t *)(&g_eep_ctrl), sizeof(g_eep_ctrl));

    if(g_eep.SOH != EEP_ADDR_THIS_SOH) {
        g_eep.SOH = EEP_ADDR_THIS_SOH;
        g_eep.addr = 0x03;
        g_eep.baud_rate = BAUD19200;

        eeprom_set_param(EEP_ADDR_THIS, (uint8_t *)(&g_eep), sizeof(g_eep));

        printf("eeprom device part not initialized...\n");
    }

    if(g_eep_ctrl.SOH!= EEP_ADDR_CTRL_SOH) {
        g_eep_ctrl.SOH = EEP_ADDR_CTRL_SOH;
        eeprom_set_param(EEP_ADDR_CTRL, (uint8_t *)(&g_eep_ctrl), sizeof(g_eep_ctrl));
        
        printf("eeprom controller part not initialized...\n");
    }

    g_eep.rf_ch = eeprom_get_rf_channel();

    eeprom_get_mac          (&g_eep.mac[0]);
    eeprom_get_controller_id(&g_eep_ctrl.mac[0]);

    if(g_eep.rf_ch == 0xff) {
        g_eep.rf_ch = 0;
    }

    if(g_eep.mac[0] == DEV_TYPE_THIS) {
        memcpy(&g_dev_mac.name[10], g_eep.mac, 6);
    } else {
        memset(g_eep.mac, 0, 6);
    }

    if(g_eep_ctrl.mac[0] != 0xff) {
        memcpy(g_dev_mac.controller_name, g_eep_ctrl.mac, 6);
    } else {
        memset(g_eep_ctrl.mac, 0, 6);
    }

    printf("g_eep.addr:      %d\n", g_eep.addr);
    printf("g_eep.rf_ch:     %d\n", g_eep.rf_ch);
    printf("g_eep.mac :      ");
    for(uint8_t i=0; i<6; i++) {
        printf("%02x", g_eep.mac[i]);
    }
    printf("\n");

    printf("g_eep_ctrl.rf_ch: %d\n", g_eep_ctrl.rf_ch);
    printf("g_eep_ctrl.mac:   ");
    for(uint8_t i=0; i<6; i++) {
        printf("%02x", g_eep_ctrl.mac[i]);
    }
    printf("\n");

    g_dev_mac.channel = g_eep.rf_ch;

    g_dev.addr = g_eep.addr;
    g_dev.baud = g_eep.baud_rate;
    g_dev.mode.mod = TEMP_MODE_COOL;
    g_dev.mode.wind = FAN_MIN;
    g_dev.mode.wind_auto = 0;
    g_dev.mode.power = 0;   // Air conditioner's main power
    g_dev.mode.valve = 0;
    g_dev.set_temp = 25;
    g_dev.room_temp = 24.0;
}

void eeprom_erase_all(void)
{
    __disable_irq();
    for(uint16_t i=0; i<EEP_PAGE_IR; i++) {
        Flash_SectorErase(EEP_ADDR_IR + (i << 9));  // page size = 512 = 2^9
    }
    Flash_SectorErase(EEP_ADDR_THIS);
    Flash_SectorErase(EEP_ADDR_CTRL);
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

uint8_t eeprom_get_rf_channel(void)
{
    return *((volatile uint8_t*)(EEP_ADDR_THIS + 2));
}

void eeprom_set_rf_channel(uint8_t channel)
{
    g_eep.SOH = EEP_ADDR_THIS_SOH;
    g_eep.rf_ch = channel;
    eeprom_set_param(EEP_ADDR_THIS, (uint8_t *)(&g_eep), sizeof(g_eep));
}

void eeprom_get_controller_id(uint8_t *pbuf)
{
    memcpy(pbuf, (const void *)(EEP_ADDR_CTRL + 2), 6);
}

void eeprom_save_controller_id(uint8_t *pbuf)
{
    g_eep_ctrl.SOH = EEP_ADDR_CTRL_SOH;
    memcpy(g_eep_ctrl.mac, pbuf, 6);
    eeprom_set_param(EEP_ADDR_CTRL, (uint8_t *)(&g_eep_ctrl), sizeof(g_eep_ctrl));
}

void eeprom_get_mac(uint8_t *pbuf)
{
    flash_read_buf((EEP_ADDR_THIS + 3), pbuf, 6);
}

void eeprom_save_mac(uint8_t *pbuf)
{
    g_eep.SOH  = EEP_ADDR_THIS_SOH;
    g_eep.addr = DEV_ADDR_485;
    g_eep.baud_rate = DEV_UART_BAUD;
    g_eep.rf_ch = DEV_RF_CH;
    memcpy(g_eep.mac, pbuf, 6);
    eeprom_set_param(EEP_ADDR_THIS, (uint8_t *)(&g_eep), sizeof(g_eep));
}

void eeprom_clr_part(uint8_t index)
{
    switch(index) {
        case CLR_ID_THIS:
            for(uint8_t i=0; i<6; i++) {
                g_eep.mac[i] = 0;
            }
            eeprom_set_param(EEP_ADDR_THIS, (uint8_t *)(&g_eep), sizeof(g_eep));
            break;
        case CLR_ID_CTRL:
            for(uint8_t i=0; i<6; i++) {
                g_eep_ctrl.mac[i] = 0;
            }
            eeprom_set_param(EEP_ADDR_CTRL, (uint8_t *)(&g_eep_ctrl), sizeof(g_eep_ctrl));
            break;
        case CLR_RF_CH_THIS:
            g_eep.rf_ch = 10;
            eeprom_set_param(EEP_ADDR_THIS, (uint8_t *)(&g_eep), sizeof(g_eep));
            break;
        case CLR_RF_CH_CTRL:
            g_eep_ctrl.rf_ch = 0;
            eeprom_set_param(EEP_ADDR_CTRL, (uint8_t *)(&g_eep_ctrl), sizeof(g_eep_ctrl));
            break;
        case CLR_IR:
            __disable_irq();
            for(uint16_t i=0; i<EEP_PAGE_IR; i++) {
                Flash_SectorErase(EEP_ADDR_IR + (i << 9));  // page size = 512 = 2^9
            }
            __enable_irq();
            break;
        case CLR_ALL:
            g_eep.SOH      = EEP_ADDR_SOH_CLR;
            g_eep_ctrl.SOH = EEP_ADDR_SOH_CLR;
            eeprom_erase_all();
            break;
        default:
            break;
    }
}

