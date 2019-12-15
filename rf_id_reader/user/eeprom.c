#include "eeprom.h"
#include "global_variables.h"

eep_t g_eep;


void eeprom_init(void)
{
    flash_read(EEP_RF_ADDR, (uint8_t *)(&g_eep), sizeof(g_eep));
    
    if(g_eep.header == EEP_RF_MAGIC) {
        g_rf_channel = g_eep.rf_channel;
    } else {
        g_rf_channel = 0;
    }
}

