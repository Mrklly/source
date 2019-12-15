#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIABLES_H_


#include "hc32l13x.h"
#include "hc32_gpio.h"
#include "bsp_pwm.h"


#define DEV_TYPE_THIS   0x01

#define UART_BUF_LEN     90      // to support getting setting mac from PC
#define UART_DELAY_MS    40


#define RF433_MAX_SIZE           32
#define RF433_DEFAULT_TX_SIZE    31
#define RF433_DEFAULT_RX_SIZE    31


typedef struct _dev_param_t_{
    uint8_t SOH;                                        //协议头。
    uint8_t def_sta;                                    //默认状态
    uint8_t led_state[4];                               //4路LED灯上电状态。
    uint8_t key_function[8];                            //4路按键功能配置。2个字节代表一个按键，共8个字节。
    uint8_t key_partition[4];                          //4路按键区域设置，数据为0时无分区。
    uint8_t Transboundary_zoning[4];                   //4路按键跨区域属性，最多跨8个区域，没有设置区域属性时不起作用。00位没有跨区属性。
    uint8_t panel_add;                                  //面板地址。
    uint8_t key_num;
    uint8_t rf_ch;
    uint8_t mac_this[6];
    uint8_t mac_ctrl[6];
    uint8_t mul_ctrl_id[4][7];          // 4 keys, each key has 1 byte header and 6 bytes id
} dev_param_t;

extern dev_param_t dev_param;

typedef struct _dev_settings_t_{
    uint8_t SOH;          // 1 0xA5
    uint8_t def_sta;      // 2 default state at start
    uint8_t res;          // 3 reserved
    uint8_t channel;      // 4
    uint8_t ctrl_type[4]; // 8 four control type
    uint8_t name[16];     // 24 device name, 10~15 is ID
    uint8_t len;          // 25, control len
    uint8_t dat[64];      // control info
} dev_settings_t;


typedef struct _btn_ctrl_t_ {
    uint16_t shirt;
    uint8_t up;
    uint8_t down;
} btn_ctrl_t;


typedef enum _pair_flag_t_ {
    PAIR_NONE    = 0x00,
    PAIR_PREPARE = 0x01,
    PAIR_CLEAR   = 0x02,
    PAIR_SLAVE   = 0x03,
    PAIR_MASTER  = 0x04,
    PAIR_END     = 0x05
} pair_flag_t;

typedef struct _gpio_port_pin_t_ {
    en_gpio_port_t port;
    en_gpio_pin_t  pin;
} gpio_port_pin_t;


extern const gpio_port_pin_t gpio_pwm[4];
extern const gpio_port_pin_t gpio_backlight[4];
extern const gpio_port_pin_t gpio_relay[4];


extern uint8_t g_rx_buf[UART_BUF_LEN];
extern volatile uint8_t g_rx_idx;

extern uint8_t Point_flag[4];
extern uint8_t Exclude_flag[4];

extern uint8_t key_Result[4];
extern uint8_t pwm_flag[4];
extern uint8_t key_led_state[4];
extern uint8_t g_led_flag;
extern uint8_t g_uart_delay;


extern volatile uint16_t systick_counter;

extern uint8_t g_rf_buf[RF433_DEFAULT_RX_SIZE];
extern uint8_t g_rf_flag;
extern uint8_t g_rf_send_flag;

extern uint8_t group_state;
extern uint8_t reg_dev_sum;
extern uint8_t is_bridge_flag;
extern uint16_t self_dev_num;

extern btn_ctrl_t g_btn[4];

extern uint8_t reg_mode_flag;

extern pair_flag_t multi_pair_flag;


#endif

