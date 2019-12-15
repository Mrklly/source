#ifndef _KEY_HANDLER_H_
#define _KEY_HANDLER_H_


#include "bsp_key.h"


void KEY_LED_Control(void);
void key_value_pack(uint8_t *buf_key_value, uint8_t *buf_key_status);
void key_handler_callback(uint8_t key, uint16_t shirt);


#endif

