#ifndef _MODBUS_RTU_H_
#define _MODBUS_RTU_H_


#define ADDR_READ_KEY_STA       0x00
#define ADDR_READ_SET_TEMP      0x01
#define ADDR_READ_MODE          0x02
#define ADDR_READ_ROOM_TEMP_H   0x03
#define ADDR_READ_ROOM_TEMP_L   0x04


#define CTRL_FUN_RAED		    0x03
#define CTRL_FUN_WRITE		    0x10

void modbus_ctrl_send(uint8_t fun,uint8_t s_addr,uint8_t *pdat,uint8_t len);
uint8_t uart_read_packet(uint8_t *buff);
void modbus_recv_hander(void);
void modbus_get_mac(void);


#endif
