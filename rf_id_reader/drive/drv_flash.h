#ifndef _DRV_FLASH_H_
#define _DRV_FLASH_H_


#include "stm8s.h"


void flash_init(void);
void flash_read(uint16_t addr, uint8_t *pbuff, uint8_t len);
void flash_write(uint16_t addr, uint8_t *pdat, uint8_t len);
void flash_erase(uint16_t addr, uint8_t len);
void flash_lock(void);


#endif
