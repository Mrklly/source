#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIBALES_H_


#include "stm8s.h"


#define UART_BUF_LEN        64

#define PANEL_NUMBER_BASE   0x10
#define PANEL_NUMBER_MAX    16


typedef enum _protocol_type_t_ {
    protocol_poll = 0,
    protocol_call = 1,
    protocol_pinglian = 2
} protocol_type_t;



extern volatile uint16_t systick_counter;
extern uint8_t buf_send[8];
extern uint8_t g_dev_status[0xFF];
extern volatile uint8_t g_uart_send;
extern volatile uint8_t g_uart_idx;
extern uint8_t g_uart_buf[UART_BUF_LEN];

extern volatile uint8_t led_all_status;
extern volatile protocol_type_t g_protocol_type;
extern volatile uint8_t g_current_panel;
extern volatile uint8_t g_panel_key;
extern volatile uint8_t g_panel_backlight1;
extern volatile uint8_t g_panel_backlight2;


#endif

