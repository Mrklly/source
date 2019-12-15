#ifndef _DRV_UART_H_
#define _DRV_UART_H_

#include "hc32_gpio.h"
#include "hc32_uart.h"
#include "hc32_bt.h"


#define RX_BUF_LEN	 	0x40
#define TX_BUF_LEN	 	0x40

extern volatile uint8_t g_rx_index;
extern uint8_t g_rx_buf[RX_BUF_LEN];
extern uint8_t g_tx_buf[TX_BUF_LEN];

extern uint8_t u8RxFlg, CheckFlg;


void hal_uart_init(void);
void RxIntCallback(void);
void ErrIntCallback(void);
void uart_send_buf(uint8_t *buf, uint8_t length);

#endif
