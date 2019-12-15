#include "global_variables.h"


dev_param_t g_eep;
dev_this_t  g_dev;
dev_mac g_dev_mac;
volatile uint8_t g_mod_backup;
volatile uint8_t g_mod_backup_flag;
volatile uint8_t g_fan_ultralow;

btn_ctrl_t g_btn[KET_COUNT] = {0};

uint8_t g_uart_buff[UART_BUF_LEN];
volatile uint16_t g_uart_idx = 0;

volatile uint16_t systick_counter = 0;

uint8_t g_rf_buf[RF433_DEFAULT_RX_SIZE];
uint8_t g_rf_flag = 0;

uint8_t g_ir_study_buf[1024];
uint16_t g_ir_idx = 0;
uint8_t g_ir_flag = 0;
uint8_t g_ir_transfer = 0;






