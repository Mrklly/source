#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIABLES_H_

#include "stm8s.h"
#include "wwdg.h"

#include "drv_key.h"


#ifndef NULL
#define NULL	((void*)0)
#endif


#define RX_BUFF_LEN	 	0x10


extern volatile u16 systick_counter;
extern volatile u8 g_idx;
extern uint8_t g_rx_buff[RX_BUFF_LEN];
extern volatile uint8_t rf433_flag;
extern uint8_t mm[32];
extern uint8_t g_rf_channel;

void delay_us(u16 cnt);
void delay_ms(u16 cnt);
unsigned int crc16_modbus(u8 *buf, u8 len);


#endif
