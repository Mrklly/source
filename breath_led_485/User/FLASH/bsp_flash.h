#ifndef _BSP_FLASH_H
#define _BSP_FLASH_H

#include "stm8s_flash.h"

/**********************************************º¯ÊýÉùÃ÷***********************************************/
void flash_Init(void);
void flash_read(u32 addr,u8 *pbuff,u8 len);
void flash_write(u32 addr,u8 *pdat,u8 len);
void flash_erase(u32 addr,u8 len);
void flash_lock();

#endif
