#include "global_variables.h"
#include "e2p.h"
#include "uart.h"
#include <string.h>
eep		g_eep;

void eep_init(void)
{
    flash_init();

    flash_read(COM_EEPROM_ADDR,(u8*)&g_eep,sizeof(g_eep));


    if(g_eep.SOH != 0xA5) {
        g_eep.SOH = 0xA5;
        g_eep.addr = 0x03;
        g_eep.baud_rate = BAUDRATE19200;
    }
    //g_eep.addr = 0x03;

}

void eep_write(eep *pe2p)
{
    flash_erase(COM_EEPROM_ADDR,sizeof(g_eep));
    flash_write(COM_EEPROM_ADDR,(u8*)pe2p,sizeof(g_eep));
}
