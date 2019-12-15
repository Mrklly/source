#ifndef _EEPROM_H_
#define _EEPROM_H_


#include "hc32_flash.h"


#define EEP_ADDR_THIS            0xFE00
#define EEP_ADDR_SOH_CLR         0xFF
#define EEP_MAGIC                0x5A


typedef struct _dev_param_t_{
    uint8_t header;             // start magic number
    uint8_t addr;               // panel address
    uint8_t key_num;                 // keys number
    union {
        uint8_t status;
        struct {
            uint8_t reservered :4;
            uint8_t name       :3;
            uint8_t near_sen   :1;
        };
    };
    uint8_t addr_range;
} dev_param_t;


extern dev_param_t g_dev;


void eeprom_get_all(void);
void eeprom_erase_all(void);

void eeprom_get_param(uint16_t addr, uint8_t *buf, uint8_t len);
void eeprom_set_param(uint16_t addr, uint8_t *buf, uint8_t len);

void dev_param_get(void);
void dev_param_set(void);


#endif

