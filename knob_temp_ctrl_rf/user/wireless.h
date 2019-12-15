#ifndef _WIRELESS_H_
#define _WIRELESS_H_

#include "cmt2300a.h"

#define CMD_GET_TEMP_SET    0xA0
#define CMD_GET_TEMP_ROOM   0xB0
#define CMD_GET_PWR         0xC0
#define CMD_GET_MODE        0xD0
#define CMD_GET_FAN         0xE0

#define DATA_GET            0x00

#define CMD_SET_TEMP        0x30
#define CMD_SET_POWER       0x40
#define CMD_SET_MODE        0x50
#define CMD_SET_FAN         0x60

#define DATA_POWER_OFF      0x00
#define DATA_POWER_ON       0x01
#define DATA_MODE_COLD      0x01
#define DATA_MODE_WARM      0x02
#define DATA_MODE_FAN       0x03
#define DATA_MODE_DRY       0x04
#define DATA_MODE_AUTO      0x05
#define DATA_FAN_CLZ        0x00
#define DATA_FAN_LOW        0x01
#define DATA_FAN_MID        0x02
#define DATA_FAN_HIGH       0x03
#define DATA_FAN_AUTO       0xFF



void rf433_dev_tx(uint8_t datax, uint8_t datay);
void reg_to_controller(void);    // register to master controller
void rf_recv_check(void);
void wireless_handler(void);



#endif
