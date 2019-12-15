#ifndef _PROTOCOL_SHANGHUI_H_
#define _PROTOCOL_SHANGHUI_H_


#include "stm8s.h"


uint8_t get_crc(uint8_t *buf, uint8_t len);
uint8_t get_crc8(const void *buf, int len);
void protocol_run(void);
void protocol_master_check(void);
void protocol_clear_buf(void);


#endif

