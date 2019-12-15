#include "global_variables.h"
#include "e2p.h"
#include "uart.h"
#include <string.h>
eep		g_eep;

void eep_init(void)
{
    flash_init();
}

void eep_write(eep *pe2p)
{
    flash_erase(COM_EEPROM_ADDR,sizeof(g_eep));
    flash_write(COM_EEPROM_ADDR,(u8*)pe2p,sizeof(g_eep));
}
