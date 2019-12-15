#include "global_variables.h"


volatile u16 systick_counter = 0;
volatile u8 g_set_addr_baud = 0;

u8 g_lcd_on = 1;
u8 g_lcd_delay = 100;  // 50 * 100ms
u8 g_led_on = 1;
u8 g_led_delay = 100;
u8 g_rx_buff[RX_BUFF_LEN];
u8 g_uart_baud_changed;
u8 g_led_setting_flag = 0;
u8 g_saving_flag = 0;
volatile u8 g_idx = 0;
volatile uint8_t g_mod_backup = 0;
volatile uint8_t g_mod_backup_flag = 0;
temp_dev g_dev;
device_settings g_dev_set;
btn_ctrl g_btn[KET_COUNT] = {0};

float s_temp_bak = 0;



void delay (u16 cnt)
{
    while (cnt--) {
        Refresh_WWDG_Window();
    }
}

void delay_ms(u16 cnt)
{
    u16	i;
    while(cnt--) {
        for(i = 0; i < 480; i++) {
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            Refresh_WWDG_Window();
        }
    }
}

unsigned int crc16_modbus(u8 *buf, u8 len)
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





