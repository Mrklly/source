#ifndef _CHECKSUM_H_
#define _CHECKSUM_H_


#include "stm8s.h"


uint8_t get_crc(uint8_t *buf, uint8_t len);
uint8_t get_crc_sum(uint8_t *buf, uint8_t len);


#endif

