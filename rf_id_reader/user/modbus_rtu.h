#ifndef _MODBUS_RTU_H_
#define _MODBUS_RTU_H_

#include "stm8s.h"

#define CTRL_ADDR_READ_KEY_STA		0x00
#define CTRL_ADDR_READ_TEMP		0x01
#define CTRL_ADDR_CUR_TEMP		0x02
#define CTRL_ADDR_SET_TEMP		0x03
#define CTRL_ADDR_WRITE_KEY_STA		0x04
#define CTRL_ADDR_WRITE_ADDR		0x05
#define CTRL_ADDR_READ_ADDR		0x06

#define CTRL_FUN_RAED		        0x03
#define CTRL_FUN_WRITE		        0x10

uint8_t uart_read_packet(uint8_t *buff);
void recv_hander(void);


#endif
