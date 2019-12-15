#include "drv_eeprom.h"

volatile uint32_t flash_irq_flag = 0;

void flash_interrupt(void)
{
    if (TRUE == Flash_GetIntFlag(FlashPCInt)) {
        Flash_ClearIntFlag(FlashPCInt);
        flash_irq_flag |= 0x01;
        Flash_DisableIrq(FlashPCInt);
    }

    if (TRUE == Flash_GetIntFlag(FlashSlockInt)) {
        Flash_ClearIntFlag(FlashSlockInt);
        flash_irq_flag |= 0x02;
        Flash_DisableIrq(FlashSlockInt);
    }
}

void eeprom_init(void)
{
    Flash_Init(flash_interrupt, 12, TRUE);
}

en_result_t eeprom_write_byte(uint32_t addr, uint8_t data)
{
    en_result_t res;
    Flash_SectorErase(addr);
    res = Flash_WriteByte(addr, data);

    if(Ok == res) {
        if(*((volatile uint8_t*)addr) != data) {
            return Error;
        }
        return Ok;
    }

    return res;
}

en_result_t eeprom_write_half_word(uint32_t addr, uint16_t data)
{
    en_result_t res;
    Flash_SectorErase(addr);
    res = Flash_WriteHalfWord(addr, data);

    if(Ok == res) {
        if(*((volatile uint16_t*)addr) != data) {
            return Error;
        }
        return Ok;
    }

    return res;
}

en_result_t eeprom_write_word(uint32_t addr, uint32_t data)
{
    en_result_t res;
    Flash_SectorErase(addr);
    res = Flash_WriteHalfWord(addr, data);

    if(Ok == res) {
        if(*((volatile uint32_t*)addr) != data) {
            return Error;
        }
        return Ok;
    }

    return res;
}

void flash_read_buf(uint32_t addr, uint8_t *pbuff, uint8_t len)
{
    uint8_t i;
    for(i = 0; i < len; i++) {
        *pbuff = *((volatile uint8_t*)addr);
        pbuff++;
        addr++;
    }
}

uint8_t flash_write_buf(uint32_t addr, uint8_t *pdat, uint8_t len)
{
    uint8_t i;

    if (FLASH_END_ADDR < addr) {
        return ErrorInvalidParameter;
    }

    FLASH_BYPASS();                          // unlock flash
    M0P_FLASH->SLOCK_f.SLOCK = (uint32_t)UnlockAll;

    while (M0P_FLASH->CR_f.BUSY) {   // busy?
        ;
    }

    FLASH_BYPASS();
    M0P_FLASH->CR_f.OP = Program;

    for(i = 0; i < len; i++) {
        *((volatile uint8_t*)addr) = *pdat;
        addr++;
        pdat++;
    }

    while (M0P_FLASH->CR_f.BUSY) {    // busy?
        ;
    }

    FLASH_BYPASS();
    M0P_FLASH->SLOCK_f.SLOCK = (uint32_t)LockAll;

    return Ok;
}
