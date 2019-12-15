#ifndef _GLOBAL_VARIABLE_H_
#define _GLOBAL_VARIABLE_H_


#include "stm8s.h"


#define EEPROM_ADDR_THIS 0x004050
#define EEPROM_MAGIC     0x5A

#define UART_BUF_LEN    32

#define UART_DELAY_MS   40


typedef struct _gpio_port_pin_t_ {
    GPIO_TypeDef*    port;
    GPIO_Pin_TypeDef pin;
} gpio_port_pin_t;

typedef struct _dev_param_t_{
    uint8_t header;             // start magic number
    uint8_t addr;               // panel address
    uint8_t ch;                 // keys number
    uint8_t status;             // power on status
    uint8_t addr_range;
} dev_param_t;

typedef enum {
    KEY_NONE = 0,
    KEY_DOWN = 1,
    KEY_UP   = 2,
    KEY_DN_UP= 3
} key_sta_t;


extern dev_param_t g_dev;
extern uint8_t g_ctrl_index;

extern const gpio_port_pin_t gpio_led[4];
extern const gpio_port_pin_t gpio_relay[4];

extern volatile uint8_t key_Result;
extern volatile uint8_t master_flag;
extern volatile uint8_t key_changed;
extern volatile uint8_t key_sta_last;

extern volatile uint8_t g_uart_delay;

extern uint8_t g_uart_buf[UART_BUF_LEN];
extern volatile uint8_t g_uart_idx;

extern volatile uint8_t pwm_flag[4];
extern uint8_t key_led_state[4];
extern volatile uint16_t count[4];


void dev_param_get(void);
void dev_param_set(void);
void dev_status_set(uint8_t data);


#endif

