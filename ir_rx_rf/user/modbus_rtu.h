#ifndef _MODBUS_RTU_H_
#define _MODBUS_RTU_H_

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

#define ADDR_READ_KEY_STA       0x00
#define ADDR_READ_SET_TEMP      0x01
#define ADDR_READ_MODE          0x02
#define ADDR_READ_ROOM_TEMP_H   0x03
#define ADDR_READ_ROOM_TEMP_L   0x04


#define CTRL_FUN_RAED		    0x03
#define CTRL_FUN_WRITE		    0x10

typedef struct _link_dat_t_ {
    uint8_t		soh_h;
    uint8_t		soh_l;
    uint8_t		sl_addr;
    uint8_t		fun;
    uint8_t		st_addr;
    uint8_t		reg_cnt;
    uint8_t		buff[4];
} link_dat;

void modbus_ctrl_send(uint8_t fun,uint8_t s_addr,uint8_t *pdat,uint8_t len);
uint8_t uart_read_packet(uint8_t *buff);
void modbus_recv_hander(void);
void modbus_get_mac(void);
void modbus_tx(void);
void modbus_rx_check(void);

#endif
