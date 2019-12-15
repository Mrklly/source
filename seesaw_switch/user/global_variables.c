#include "global_variables.h"



uint8_t g_tx_buf[UART_TX_LEN];
uint8_t g_rx_buf[UART_RX_LEN];
volatile uint8_t g_rx_idx = 0;

uint8_t Point_flag[4]   = {0,0,0,0};
uint8_t Exclude_flag[4] = {0,0,0,0};

uint8_t key_Result[4] = {0,0,0,0};

/*pwm_flag标志有3种状态，
0x00数据不能修改，按键变光没有执行完成。
0x01代表按键有变化。
0x02代表变光完成，也代表按键无变化，代表数据可以修改。
*/
uint8_t pwm_flag[4]={0x02, 0x02, 0x02, 0x02};

uint8_t key_led_state[4]={0,0,0,0};

uint8_t g_led_flag = 0;
uint8_t g_uart_delay = 0;


