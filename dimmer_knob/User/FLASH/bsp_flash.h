#ifndef _BSP_FLASH_H
#define _BSP_FLASH_H
#include "flash.h"
/*****************************************************************************************************/


/**********************************************º¯ÊıÉùÃ÷***********************************************/
void flash_interrupt(void);
en_result_t eeprom_write_byte(uint32_t addr, uint8_t data);
en_result_t eeprom_write_half_word(uint32_t addr, uint16_t data);
en_result_t eeprom_write_word(uint32_t addr, uint32_t data);
void flash_read_buf(uint32_t addr, uint8_t *pbuff, uint8_t len);
uint8_t flash_write_buf(uint32_t addr, uint8_t *pdat, uint8_t len);

#endif