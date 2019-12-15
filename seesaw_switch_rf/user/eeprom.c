#include "eeprom.h"
#include "bsp_flash.h"
#include "bsp_uart.h"


void eeprom_get_all(void)
{
    eeprom_get_param(EEP_ADDR_THIS, (uint8_t *)(&dev_param), sizeof(dev_param));

    if(dev_param.SOH != EEP_ADDR_THIS_SOH) {

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

void modbus_set_mac(void)
{
    uint8_t i = 0;
    g_rx_idx = 0;

    dev_settings_t dev_settings;

    for(; i<4; i++) {
        uart_send_byte(0xAA);
        delay_ms(100);

        if((g_rx_idx) && (g_rx_buf[0] == 0x55)) {   // 0x55 + SOH(0xA5)
            break;
        }
    }

    for(i=0; i<100; i++) {
        if(g_rx_idx == UART_BUF_LEN) {
            break;
        }
        delay_ms(5);
    }

    if(i == 100) {
        printf("\n");
        return ;
    }

    if((g_rx_idx >= 25) && (g_rx_buf[1] == EEP_ADDR_THIS_SOH)) {

        memcpy(&dev_settings.SOH, &g_rx_buf[1], sizeof(dev_settings));

        if(dev_settings.name[10] != DEV_TYPE_THIS) {
            memset(&dev_settings.name[10], 0xFF, 6);
        }

        uart_send_buf(&dev_settings.name[10], 6);
    }

    if(dev_settings.name[10] == DEV_TYPE_THIS) {

        dev_param.SOH = EEP_ADDR_THIS_SOH;
        memcpy(dev_param.mac_this, &dev_settings.name[10], 6);
        memset(dev_param.led_state, 0, 4);
        dev_param.key_num = dev_settings.channel;
        eeprom_set_param(EEP_ADDR_THIS, (uint8_t *)(&dev_param), sizeof(dev_param));
    }

    printf("\n");
}


