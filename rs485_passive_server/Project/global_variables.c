#include "global_variables.h"

volatile uint16_t systick_counter = 0;

uint8_t buf_send[8];
uint8_t g_dev_status[0xFF];
volatile uint8_t g_uart_send = 0;
volatile uint8_t g_uart_idx = 0;
uint8_t g_uart_buf[UART_BUF_LEN];

volatile uint8_t led_all_status = 0;


volatile protocol_type_t g_protocol_type = protocol_poll;
volatile uint8_t g_current_panel = PANEL_NUMBER_BASE;
volatile uint8_t g_panel_key = 0;
volatile uint8_t g_panel_backlight1 = 0;
volatile uint8_t g_panel_backlight2 = 0x04;



