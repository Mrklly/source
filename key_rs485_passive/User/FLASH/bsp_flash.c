#include "bsp_flash.h"

void flash_Init(void)
{
    FLASH_DeInit();                                       //复位EEPROM寄存器
    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD); //标准编程时间。
//    FLASH_Unlock(FLASH_MEMTYPE_PROG);
}

void flash_read(uint32_t addr,u8 *pbuff,u8 len)
{
  for(uint8_t i = 0;i < len;i++){
    pbuff[i] = FLASH_ReadByte(addr + i);
  }
}

void flash_write(uint32_t addr,u8 *pdat,u8 len)
{
    for(uint8_t i = 0;i < len;i++){
        FLASH_ProgramByte(addr + i, pdat[i]);
    }
}

void flash_erase(uint32_t addr,u8 len)
{
    for(uint8_t i = 0;i < len;i++){
        FLASH_EraseByte(addr + i);
    }
}

void flash_lock(void)
{
    FLASH_Lock(FLASH_MEMTYPE_DATA);	
}

