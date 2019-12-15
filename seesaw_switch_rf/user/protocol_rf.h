#ifndef _PROTOCOL_RF_H_
#define _PROTOCOL_RF_H_


#include "drv_cmt2300a.h"


void protocol_rf_send(uint8_t key, uint8_t cmd, uint8_t data);
void multi_pair_prepare(uint8_t key);
void multi_pair_end(void);
void multi_pair_indicator(uint8_t key);
void multi_control(uint8_t *data, uint8_t len);


#endif

