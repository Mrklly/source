#ifndef _GLOBAL_VARIABLE_H_
#define _GLOBAL_VARIABLE_H_


#include "stm8s.h"


typedef struct _gpio_port_pin_t_ {
    GPIO_TypeDef*    port;
    GPIO_Pin_TypeDef pin;
} gpio_port_pin_t;


extern const gpio_port_pin_t gpio_led[4];
extern const gpio_port_pin_t gpio_relay[4];

extern volatile uint8_t key_Result;
extern volatile uint8_t key_down_value;
extern volatile uint8_t key_send_flag;
extern volatile uint8_t key_change_flag;
extern volatile uint8_t master_flag;


typedef struct _dev_param_t_{
    uint8_t header;             // start magic number
    uint8_t addr;               // panel address
    uint8_t ch;                 // keys number
    union {
        uint8_t status;
        struct {
            uint8_t reservered :4;
            uint8_t name       :3;
            uint8_t near_sen   :1;
        };
    };
    uint8_t addr_range;
} dev_param_t;

typedef enum _panel_name_t_ {
    NONE  = 0x00,
    PANEL_KEY = 0x01,
    PANEL_TOUCH = 0x02
} panel_name_t;


extern dev_param_t g_dev;

#define EEPROM_ADDR_THIS 0x004050
#define EEPROM_MAGIC     0x5A

#define UART_BUF_LEN    32

extern uint8_t g_uart_buf[UART_BUF_LEN];
extern uint8_t g_uart_idx;

extern volatile uint8_t pwm_flag[4];
extern uint8_t key_led_state[4];
extern volatile uint16_t count[4];


void dev_param_get(void);
void dev_param_set(void);
void dev_status_set(uint8_t data);


#endif

