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
    uint8_t status;             // power on status
    uint8_t addr_range;
} dev_param_t;

extern dev_param_t g_dev;

#define EEPROM_ADDR_THIS 0x004050
#define EEPROM_MAGIC     0x5A

#define UART_BUF_LEN    16

extern uint8_t g_uart_buf[UART_BUF_LEN];
extern volatile uint8_t g_uart_idx;

extern uint8_t g_ctrl_index;

void dev_param_get(void);
void dev_param_set(void);
void dev_status_set(uint8_t data);
void delay_ms(uint16_t ms);


#endif

