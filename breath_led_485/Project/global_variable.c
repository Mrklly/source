#include "global_variable.h"

/* pwm_flag
0x00: Data can NOT be modified, dimming NOT done
0x01: Key state changed
0x02: dimming Done, key state not changed, data can be modified
*/

uint8_t key_Result[4] = {0,0,0,0};

uint8_t g_tx_buf[UART_TX_LEN];
uint8_t g_rx_buf[UART_RX_LEN];
volatile uint8_t g_rx_idx = 0;


volatile uint8_t pwm_flag[4]={0x02,0x02,0x02,0x02};
uint8_t key_led_state[4]={0,0,0,0};
volatile uint16_t count[4]={1111,1111,1111,1111};

volatile uint8_t g_led_flag = 0;
volatile uint8_t g_uart_delay = 0;


