#include "global_variables.h"



uint8_t g_tx_buf[UART_TX_LEN];
uint8_t g_rx_buf[UART_RX_LEN];
volatile uint8_t g_rx_idx = 0;

uint8_t Point_flag[4]   = {0,0,0,0};
uint8_t Exclude_flag[4] = {0,0,0,0};

uint8_t key_Result[4] = {0,0,0,0};

/*pwm_flag��־��3��״̬��
0x00���ݲ����޸ģ��������û��ִ����ɡ�
0x01�������б仯��
0x02��������ɣ�Ҳ�������ޱ仯���������ݿ����޸ġ�
*/
uint8_t pwm_flag[4]={0x02, 0x02, 0x02, 0x02};

uint8_t key_led_state[4]={0,0,0,0};

uint8_t g_led_flag = 0;
uint8_t g_uart_delay = 0;


