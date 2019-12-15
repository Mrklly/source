#ifndef _MODBUS_RTU_H_
#define _MODBUS_RTU_H_


#include <stdint.h>


#define ADDR_READ_KEY_STA       0x00
#define ADDR_READ_SET_TEMP      0x01
#define ADDR_READ_MODE          0x02
#define ADDR_READ_ROOM_TEMP_H   0x03
#define ADDR_READ_ROOM_TEMP_L   0x04


#define CTRL_FUN_RAED           0x03
#define CTRL_FUN_WRITE          0x10

#define FS_ERASE_PART           0x20        // 270 * 1024 bytes that contains ir data
#define FS_ERASE_ALL            0x21        // 8M bytes, it takes too long
#define FS_READ_ONE             0x22        // e.g. power=1, temp=16, fan=mid
#define FS_READ_PART            0x23        // 270 * 1024 bytes that contains ir data
#define FS_READ_WHOLE           0x24        // 8M bytes, it takes too long
#define FS_WRITE_ONE            0x25        // e.g. power=1, temp=16, fan=mid
#define FS_WRITE_PART           0x26        // 270 * 1024 bytes that contains ir data
#define FS_WRITE_WHOLE          0x27        // 8M bytes, it takes too long

#define CTRL_FUN_W_PWR          0x04
#define CTRL_FUN_W_MODE         0x05
#define CTRL_FUN_W_FAN          0x06
#define CTRL_FUN_W_TEMP         0x07


void modbus_ctrl_send(uint8_t fun,uint8_t s_addr,uint8_t *pdat,uint8_t len);
uint8_t uart_read_packet(uint8_t *buff);
void modbus_recv_hander(void);
void modbus_get_mac(void);

uint16_t crc16_modbus(uint8_t *buf, uint8_t len);



#endif

