#include "global_variables.h"


uint8_t led_sta[6] = {0, 0, 0, 0, 0, 0};

volatile uint8_t pwm_share_cont = 0;
volatile uint8_t adjust1_flag = 1;
volatile uint8_t adjust2_flag = 1;
volatile uint8_t adjust1_cont = 0;
volatile uint8_t adjust2_cont = 0;
volatile uint8_t Chopping1_channel_cont = 13;
volatile uint8_t Chopping2_channel_cont = 13;

volatile uint8_t key_Result = 0;

volatile zig_net_sta_t g_zig_net_sta = ZIG_NET_STA_NONE;


//void delay_us(uint16_t cnt)  // 1 = 2.37, 2 = 3.56, 10= 13.55
//{
//    while (cnt--) {
//        __NOP();
//        __NOP();
//        __NOP();
//        __NOP();
//        __NOP();
//        __NOP();
//        __NOP();
//        __NOP();
//        __NOP();
//        __NOP();
//        __NOP();
//    }
//}
//
//void delay_ms(uint16_t cnt)  // 100 = 102.1, 10 = 10.22, 2 = 2.048, 1 = 1.002
//{
//    uint16_t  i;
//    while(cnt--) {
//        for(i = 0; i < 430; i++) {
//            __NOP();
//            __NOP();
//            __NOP();
//            __NOP();
//            __NOP();
//            __NOP();
//            __NOP();
//            __NOP();
//            __NOP();
//            __NOP();
//        }
//    }
//}

unsigned int crc16_modbus(uint8_t *buf, uint8_t len)
{
    uint16_t crc = 0xFFFF;

    for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos];          // XOR byte into least sig. byte of crc

        for (int i = 8; i != 0; i--) {    // Loop over each bit
            if ((crc & 0x0001) != 0) {      // If the LSB is set
                crc >>= 1;                    // Shift right and XOR 0xA001
                crc ^= 0xA001;
            } else {                        // Else LSB is not set
                crc >>= 1;    // Just shift right
            }
        }
    }

    // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
    return crc;
}
