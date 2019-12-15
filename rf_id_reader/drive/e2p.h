#ifndef __E2P_H
#define __E2P_H

#include "flash.h"
#include "global_variables.h"


typedef struct _eep_t_ {
    u8 SOH;
    u8 addr;
    device_baudrate baud_rate;
} eep;

#define     COM_EEPROM_ADDR     0x40A5
extern void eep_init(void);
extern void eep_write(eep *pe2p);
extern eep g_eep;

#endif

