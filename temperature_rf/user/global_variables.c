#include "global_variables.h"


volatile uint16_t systick_counter = 0;
volatile uint8_t g_set_type = SET_NONE;

uint8_t g_lcd_on = 1;
uint8_t g_lcd_delay = 100;  // 50 * 100ms
uint8_t g_led_on = 1;
uint8_t g_led_delay = 100;
uint8_t g_rx_buff[RX_BUFF_LEN];
uint8_t g_uart_baud_changed;
uint8_t g_saving_flag = 0;
uint8_t g_erasing_index = 0;
uint8_t g_erasing_flag = 0;
uint8_t g_ir_study_flag = 0;
uint16_t g_ir_study_index = 0;
uint8_t g_ir_rf_flag = 0;
uint8_t g_ir_rf_send = 0;
uint8_t g_ir_rf_recv = 0;
volatile uint8_t g_idx = 0;
volatile uint8_t g_mod_backup = 0;
volatile uint8_t g_mod_backup_flag = 0;
temp_dev g_dev;
dev_mac g_dev_mac;
uint8_t g_rf_channel;
btn_ctrl g_btn[KET_COUNT] = {0};
volatile uint8_t rf433_flag = 0;
uint8_t mm[RF433_DEFAULT_RX_SIZE];
uint8_t rf433_tx_buff[RF433_MAX_SIZE];
uint8_t rf433_rx_buff[RF433_MAX_SIZE];
uint8_t reg_mode_flag = 0;
uint8_t  ir_index = 0;
uint16_t ir_data[IR_DATA_SIZE];



float s_temp_bak = 0;


unsigned int crc16_modbus(uint8_t *buf, uint8_t len)
{
    uint16_t crc = 0xFFFF;

      for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos];          // XOR byte into least sig. byte of crc

        for (int i = 8; i != 0; i--) {    // Loop over each bit
          if ((crc & 0x0001) != 0) {      // If the LSB is set
            crc >>= 1;                    // Shift right and XOR 0xA001
            crc ^= 0xA001;
          }
          else                            // Else LSB is not set
            crc >>= 1;                    // Just shift right
        }
      }
      // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
      return crc;
}

