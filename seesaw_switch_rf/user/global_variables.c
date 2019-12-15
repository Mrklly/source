#include "global_variables.h"


dev_param_t dev_param;


const gpio_port_pin_t gpio_pwm[4] = {
    {PWM_PORT_CH1, PWM_PIN_CH1},
    {PWM_PORT_CH2, PWM_PIN_CH2},
    {PWM_PORT_CH3, PWM_PIN_CH3},
    {PWM_PORT_CH4, PWM_PIN_CH4}
};


const gpio_port_pin_t gpio_backlight[4] = {
    {BACK_LED1_PORT, BACK_LED1_PIN},
    {BACK_LED2_PORT, BACK_LED2_PIN},
    {BACK_LED3_PORT, BACK_LED3_PIN},
    {BACK_LED4_PORT, BACK_LED4_PIN}
};

const gpio_port_pin_t gpio_relay[4] = {
    {RELAY_PORT_CH1, RELAY_PIN_CH1},
    {RELAY_PORT_CH2, RELAY_PIN_CH2},
    {RELAY_PORT_CH3, RELAY_PIN_CH3},
    {RELAY_PORT_CH4, RELAY_PIN_CH4}
};



uint8_t g_rx_buf[UART_BUF_LEN];
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

volatile uint16_t systick_counter = 0;

uint8_t g_rf_buf[RF433_DEFAULT_RX_SIZE];
uint8_t g_rf_flag = 0;
uint8_t g_rf_send_flag = 0;

uint8_t group_state;
uint8_t reg_dev_sum;
uint8_t is_bridge_flag;
uint16_t self_dev_num;

btn_ctrl_t g_btn[4];

uint8_t reg_mode_flag = 0;

pair_flag_t multi_pair_flag = PAIR_NONE;




