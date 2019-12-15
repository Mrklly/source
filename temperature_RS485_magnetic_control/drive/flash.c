#include "flash.h"

void flash_init(void)
{
//    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
    FLASH->CR1 &= (uint8_t)(~FLASH_CR1_FIX);
    FLASH->CR1 |= (uint8_t)FLASH_PROGRAMTIME_STANDARD;

//    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    FLASH->DUKR = FLASH_RASS_KEY2; /* Warning: keys are reversed on data memory !!! */
    FLASH->DUKR = FLASH_RASS_KEY1;
}

void flash_read(u32 addr,u8 *pbuff,u8 len)
{
    u8 i;
    for(i = 0; i < len; i++) {
        *pbuff = (*(PointerAttr uint8_t *) (uint16_t)addr);
        pbuff++;
        addr++;
    }
}
void flash_write(u32 addr,u8 *pdat,u8 len)
{
    u8 i;
    for(i = 0; i < len; i++) {
        *(PointerAttr uint8_t*) (uint16_t)addr = *pdat;
        addr++;
        pdat++;
    }
}
void flash_erase(u32 addr,u8 len)
{
    u8 i;
    for(i = 0; i < len; i++) {
        *(PointerAttr uint8_t*) (uint16_t)addr = ((uint8_t)0x00);
        addr++;
    }
}
void flash_lock()
{
//    FLASH_Lock(FLASH_MEMTYPE_DATA);
    FLASH->IAPSR &= (uint8_t)FLASH_MEMTYPE_DATA;
}





