#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIABLES_H_


#include "hc32f005.h"


#ifndef NULL
#define NULL	((void*)0)
#endif


typedef enum _ZIG_NET_STA_T_ {
    ZIG_NET_STA_NONE = 0,
    ZIG_NET_STA_OK   = 1,
    ZIG_NET_STA_ERR  = 2,
    ZIG_NET_REG_PRE  = 3,
    ZIG_NET_REG_S_1  = 4,
    ZIG_NET_REG_S_2  = 5,
    ZIG_NET_REG_S_3  = 6,
    ZIG_NET_REG_S_4  = 7,
    ZIG_NET_REG_ING  = 8,
    ZIG_NET_REG_TOUT = 9,
    ZIG_NET_REG_OVER = 10
} zig_net_sta_t;



extern uint8_t led_sta[6];

extern volatile uint8_t pwm_share_cont;
extern volatile uint8_t adjust1_flag;
extern volatile uint8_t adjust2_flag;
extern volatile uint8_t adjust1_cont;
extern volatile uint8_t adjust2_cont;
extern volatile uint8_t Chopping1_channel_cont;
extern volatile uint8_t Chopping2_channel_cont;

extern volatile uint8_t key_Result;

extern volatile zig_net_sta_t g_zig_net_sta;

//void delay_us(uint16_t cnt);
//void delay_ms(uint16_t cnt);
unsigned int crc16_modbus(uint8_t *buf, uint8_t len);


#endif

