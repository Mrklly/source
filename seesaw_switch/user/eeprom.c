#include "eeprom.h"
#include "bsp_flash.h"


dev_param_t dev_param;


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

