#ifndef _BSP_FLASH_H
#define _BSP_FLASH_H

#include "stm8s_flash.h"


void flash_Init(void);
void flash_read(uint32_t addr,u8 *pbuff,u8 len);
void flash_write(uint32_t addr,u8 *pdat,u8 len);
void flash_erase(uint32_t addr,u8 len);
void flash_lock(void);


#endif

