#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "drv_eeprom.h"
#include "global_variables.h"

// EEP_ADDR_IR
// 0xEC00 - 0xEDFF  Sector118
// 0xEE00 - 0xEFFF  Sector119
// 0xF000 - 0xF1FF  Sector120
// 0xF200 - 0xF3FF  Sector121
// 0xF400 - 0xF5FF  Sector122
// 0xF600 - 0xF7FF  Sector123
// 0xF800 - 0xF9FF  Sector124
// 0xFA00 - 0xFBFF  Sector125

// EEP_ADDR_CTRL
// 0xFC00 - 0xFDFF  Sector126

// EEP_ADDR_THIS
// 0xFE00 - 0xFFFF  Sector127


#define EEP_ADDR_THIS            0xFE00
#define EEP_ADDR_THIS_SOH        0xA5
#define EEP_ADDR_SOH_CLR         0xFF


//#define EEP_ADDR_MAC_THIS       0xFC00
//#define EEP_ADDR_RF_CHANNEL     0xFA00

#define EEP_ADDR_CTRL            0xFC00
#define EEP_ADDR_CTRL_SOH        0x5A

// flash address 0xEC00 to 0xFBFF = 4k bytes
#define EEP_ADDR_IR    0xEC00


#define EEP_PAGE_IR    8
#define EEP_PAGE_THIS  1
#define EEP_PAGE_CTRL  1


typedef struct _eeprom_this_t_ {
    uint8_t SOH;
    uint8_t addr;
    uint8_t rf_ch;
    uint8_t mac[6];
    dev_baud_t baud_rate;
    pipe_type_t pipe_type;
    int8_t temp_offset;
    uint8_t led_timeout;
    uint8_t flag_fan_ultralow;
}eep_this;

typedef struct _eeprom_ctrl_t_ {
    uint8_t SOH;
    uint8_t rf_ch;
    uint8_t mac[6];
}eep_ctrl;


extern eep_this g_eep;
extern eep_ctrl g_eep_ctrl;


void eeprom_get_all(void);
void eeprom_erase_all(void);
void eeprom_get_param(uint16_t addr, uint8_t *buf, uint8_t len);
void eeprom_set_param(uint16_t addr, uint8_t *buf, uint8_t len);
uint8_t eeprom_get_rf_channel(void);
void eeprom_set_rf_channel(uint8_t channel);
void eeprom_get_controller_id(uint8_t *pbuf);
void eeprom_save_controller_id(uint8_t *pbuf);
void eeprom_get_mac(uint8_t *pbuf);
void eeprom_save_mac(uint8_t *pbuf);
void eeprom_clr_part(uint8_t index);


#endif

