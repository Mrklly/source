#include "global_variables.h"


const gpio_port_pin_t gpio_led[6] = {
    {PORT_LED1, PIN_LED1},
    {PORT_LED2, PIN_LED2},
    {PORT_LED3, PIN_LED3},
    {PORT_LED4, PIN_LED4},
    {PORT_LED5, PIN_LED5},
    {PORT_LED6, PIN_LED6}
};

const gpio_port_pin_t gpio_backlight[6] = {
    {BACK_LED1_PORT, BACK_LED1_PIN},
    {BACK_LED2_PORT, BACK_LED2_PIN},
    {BACK_LED3_PORT, BACK_LED3_PIN},
    {BACK_LED4_PORT, BACK_LED4_PIN},
    {BACK_LED5_PORT, BACK_LED5_PIN},
    {BACK_LED6_PORT, BACK_LED6_PIN}
};

const gpio_port_pin_t gpio_relay[4] = {
    {RELAY_CH1_PORT, RELAY_CH1_PIN},
    {RELAY_CH2_PORT, RELAY_CH2_PIN},
    {RELAY_CH3_PORT, RELAY_CH3_PIN},
    {RELAY_CH4_PORT, RELAY_CH4_PIN}
};


uint8_t g_uart_buf[UART_BUF_LEN];
volatile uint8_t g_uart_idx = 0;

volatile key_bit_t key_bit;
volatile key_bit_t key_down;
volatile uint8_t key_send_flag = 0;
volatile uint8_t key_change_flag = 0;
volatile uint16_t master_counter = 0;
volatile uint8_t master_flag = 1;


/*pwm_flag标志有3种状态，
0x00数据不能修改，按键变光没有执行完成。
0x01代表按键有变化。
0x02代表变光完成，也代表按键无变化，代表数据可以修改。
*/
uint8_t pwm_flag[6]={0x02, 0x02, 0x02, 0x02, 0x02, 0x02};

uint8_t key_led_state[6] = {0, 0, 0, 0, 0, 0};

volatile uint8_t g_dev_with_near = 0;
volatile near_sensor_sta_t g_near_sta[6] = {NEAR_NONE, NEAR_NONE, NEAR_NONE, NEAR_NONE, NEAR_NONE, NEAR_NONE};

uint8_t g_ctrl_index = 0;

dev_key_2_type_t dev_key_2_type = KEY_2_N;
panel_name_t panel_name = NONE;

volatile panel_control_type_t g_panel_ctrl = NOT_RECV;
volatile uint8_t g_panel_backlight1 = 0, g_panel_backlight2 = 0;


