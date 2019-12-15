#ifndef _EEPROM_H_
#define _EEPROM_H_


#include "drv_flash.h"


#define EEP_RF_ADDR     0x4000
#define EEP_RF_MAGIC    0xA5

typedef struct _eep_t_ {
    uint8_t header;
    uint8_t rf_channel;
} eep_t;


extern eep_t g_eep;


void eeprom_init(void);


#endif
