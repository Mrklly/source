#ifndef _MODBUS_RTU_H_
#define _MODBUS_RTU_H_


#define PROTOCOL_HEAD_1         0xFF
#define PROTOCOL_HEAD_2         0xAA
#define PROTOCOL_BROADCAST      0xFE

#define RCU_ADDR                0xFE

#define RCU_SET_ONE_PANEL      0xE2
#define RCU_SET_MULTI_PANEL    0xE8

#define RCU_CTRL_ONE_PANEL      0xF2
#define RCU_CTRL_MULTI_PANEL    0xF8
#define RCU_WARN_PANEL          0xF4
#define RCU_SET_PANEL           0xF5

#define PANEL_REPLY_RCU         0xF6



#define ADDR_READ_KEY_STA       0x00
#define ADDR_READ_SET_TEMP      0x01
#define ADDR_READ_MODE          0x02
#define ADDR_READ_ROOM_TEMP_H   0x03
#define ADDR_READ_ROOM_TEMP_L   0x04


#define CTRL_FUN_RAED           0x03
#define CTRL_FUN_WRITE          0x10

#define CTRL_FUN_W_PWR          0x04
#define CTRL_FUN_W_MODE         0x05
#define CTRL_FUN_W_FAN          0x06
#define CTRL_FUN_W_TEMP         0x07


void modbus_ctrl_send(uint8_t *buf, uint8_t len);
uint8_t uart_read_packet(void);
void modbus_recv_hander(uint8_t *buf, uint8_t *len);


#endif

