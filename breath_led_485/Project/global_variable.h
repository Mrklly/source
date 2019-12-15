#ifndef _GLOBAL_VARIABLE_H_
#define _GLOBAL_VARIABLE_H_

#include "stm8s.h"

#define UART_TX_LEN 8
#define UART_RX_LEN 8

#define UART_DELAY_MS   40


extern uint8_t key_Result[4];

extern uint8_t g_tx_buf[UART_TX_LEN];
extern uint8_t g_rx_buf[UART_RX_LEN];
extern volatile uint8_t g_rx_idx;

extern volatile uint8_t pwm_flag[4];
extern uint8_t key_led_state[4];
extern volatile uint16_t count[4];

extern volatile uint8_t g_led_flag;
extern volatile uint8_t g_uart_delay;



#endif

