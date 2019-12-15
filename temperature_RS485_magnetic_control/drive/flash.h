#ifndef __FLASH_H
#define __FLASH_H

#include "stm8s.h"



extern void flash_init(void);
extern void flash_read(u32 addr,u8 *pbuff,u8 len);
extern void flash_write(u32 addr,u8 *pdat,u8 len);
extern void flash_erase(u32 addr,u8 len);
extern void flash_lock();

#endif

