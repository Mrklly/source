#include "global_variables.h"


volatile dev_type_t g_dev_type = DEV_TYPE_RS485;
volatile uint16_t systick_counter = 0;
volatile uint8_t g_set_type = SET_NONE;

uint8_t g_lcd_on = 1;
uint16_t g_lcd_delay = 0;   // g_dev.led_timeout = 1 = 1s
uint8_t g_led_on = 1;
uint16_t g_led_delay = 0;   // g_dev.led_timeout = 1 = 1s
uint8_t g_rx_buff[RX_BUFF_LEN];
uint8_t g_uart_baud_changed;
uint8_t g_saving_flag = 0;
volatile clr_type g_erasing_index = CLR_NONE;
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
lcd_func g_lcd_func = SHOW_TEMP_ROOM;
volatile uint16_t g_lcd_func_delay = LCD_FUNC_DELAY_TIME;
volatile knob_direction g_knob_direction = DIRECTION_NONE;
volatile uint8_t g_knob_counter = 0;
volatile uint8_t g_knob_changed = 0;
volatile uint8_t g_timing_task  = 0;
volatile uint16_t g_counter_sec = 0;
volatile uint16_t g_counter_min = 0;
volatile uint8_t g_fan_ultralow = 0;
volatile zigbee_ctrl_t g_zig_ctrl = ZIG_REG;
volatile zig_net_sta_t g_zig_net_sta = ZIG_NET_STA_NONE;


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

uint8_t check_device_type(void)
{
#if (TEMP_CTRL_KNOB && TEMP_CTRL_TOUCH)
#error Can not define TEMP_CTRL_KNOB and TEMP_CTRL_TOUCH at the same time
#elif(!TEMP_CTRL_KNOB && !TEMP_CTRL_TOUCH)
#error Must define TEMP_CTRL_KNOB or TEMP_CTRL_TOUCH
#endif
}

