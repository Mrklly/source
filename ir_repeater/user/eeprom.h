#ifndef _EEPROM_H_
#define _EEPROM_H_


#include "hc32_flash.h"


#include "global_variables.h"


#define EEP_ADDR_THIS            0xFE00
#define EEP_ADDR_THIS_SOH        0xA5
#define EEP_ADDR_SOH_CLR         0xFF


void eeprom_get_all(void);
void eeprom_erase_all(void);

void eeprom_get_param(uint16_t addr, uint8_t *buf, uint8_t len);
void eeprom_set_param(uint16_t addr, uint8_t *buf, uint8_t len);

void modbus_set_mac(void);


#endif

