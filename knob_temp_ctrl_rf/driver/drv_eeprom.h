#ifndef _DRV_EEPROM_H_
#define _DRV_EEPROM_H_


#include "hc32_flash.h"

extern volatile uint32_t flash_irq_flag;


void flash_interrupt(void);
void eeprom_init(void);
en_result_t eeprom_write_byte(uint32_t addr, uint8_t data);
en_result_t eeprom_write_half_word(uint32_t addr, uint16_t data);
en_result_t eeprom_write_word(uint32_t addr, uint32_t data);
void flash_read_buf(uint32_t addr, uint8_t *pbuff, uint8_t len);
uint8_t flash_write_buf(uint32_t addr, uint8_t *pdat, uint8_t len);

#endif
