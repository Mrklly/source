#include "global_variables.h"

volatile u16 systick_counter = 0;
volatile u8 g_idx = 0;
uint8_t g_rx_buff[RX_BUFF_LEN];
volatile uint8_t rf433_flag;
uint8_t mm[32];
uint8_t g_rf_channel = 0;


void delay_us(u16 cnt)  // 1 = 2.37, 2 = 3.56, 10= 13.55
{
    while (cnt--) {
        nop();
        nop();
        nop();
        nop();
        nop();
        nop();
        nop();
        nop();
    }
}

void delay_ms(u16 cnt)  // 100 = 102.1, 10 = 10.22, 2 = 2.048, 1 = 1.002
{
    u16	i;
    while(cnt--) {
        for(i = 0; i < 430; i++) {
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
