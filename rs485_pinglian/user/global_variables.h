#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIABLES_H_


#include "hc32l13x.h"

#include "bsp_gpio.h"


#define PWM_LED         1
#define PWM_BACKLIGHT   0


typedef struct _gpio_port_pin_t_ {
    en_gpio_port_t    port;
    en_gpio_pin_t  pin;
} gpio_port_pin_t;

typedef enum _near_sensor_sta_t_ {
    NEAR_NONE = 0,
    NEAR_IDLE = 1,
    NEAR_ING  = 2
} near_sensor_sta_t;


typedef union _key_bit_t_ {
    uint8_t key_res;
    struct {
        uint8_t key1:1;
        uint8_t key2:1;
        uint8_t key3:1;
        uint8_t key4:1;
        uint8_t key5:1;
        uint8_t key6:1;
    } key_bit;
} key_bit_t;

typedef enum _dev_key_2_type_t_ {
    KEY_2_N = 0,    // key number != 2
    KEY_2_V = 1,    // key number = 2, board has 4 keys, aligned in vertial line, left and right
    KEY_2_H = 2,    // key number = 2, board has 4 keys, aligned in horizontal line, up and down
    KEY_2_6 = 3     // key number = 2, board has 6 keys, using key 2 and 5
} dev_key_2_type_t;

typedef enum _panel_name_t_ {
    NONE  = 0x00,
    QB502 = 0x01,
    QB522 = 0x02,
    QB811 = 0x03
} panel_name_t;

typedef enum _panel_control_type_t_ {
    NOT_RECV   = 0x00,
    QUERY_NAME = 0x01,
    QUERY_POLL = 0x02,
    CTRL_ONE   = 0x03,
    CTRL_MULTI = 0x04
} panel_control_type_t;


#define UART_BUF_LEN 64

extern const gpio_port_pin_t gpio_led[6];
extern const gpio_port_pin_t gpio_backlight[6];
extern const gpio_port_pin_t gpio_relay[4];

extern uint8_t g_uart_buf[UART_BUF_LEN];
extern volatile uint8_t g_uart_idx;

extern volatile key_bit_t key_bit;
extern volatile key_bit_t key_down;
extern volatile uint8_t key_send_flag;
extern volatile uint8_t key_change_flag;
extern volatile uint16_t master_counter;
extern volatile uint8_t master_flag;

extern uint8_t pwm_flag[6];

extern uint8_t key_led_state[6];

extern volatile uint8_t g_dev_with_near;
extern volatile near_sensor_sta_t g_near_sta[6];

extern uint8_t g_ctrl_index;

extern dev_key_2_type_t dev_key_2_type;
extern panel_name_t panel_name;
extern volatile panel_control_type_t g_panel_ctrl;
extern volatile uint8_t g_panel_backlight1, g_panel_backlight2;


#endif

