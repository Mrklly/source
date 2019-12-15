#include "global_variable.h"
#include "bsp_gpio.h"
#include "bsp_wwdg.h"

/* pwm_flag
0x00: Data can NOT be modified, dimming NOT done
0x01: Key state changed
0x02: dimming Done, key state not changed, data can be modified
*/


const gpio_port_pin_t gpio_led[4] = {
    {LED1_PORT, LED1_PIN},
    {LED2_PORT, LED2_PIN},
    {LED3_PORT, LED3_PIN},
    {LED4_PORT, LED4_PIN}
};

const gpio_port_pin_t gpio_relay[4] = {
    {PORT_RELAY_CH1, PIN_RELAY_CH1},
    {PORT_RELAY_CH2, PIN_RELAY_CH2},
    {PORT_RELAY_CH3, PIN_RELAY_CH3},
    {PORT_RELAY_CH4, PIN_RELAY_CH4}
};


volatile uint8_t key_Result = 0;
volatile uint8_t key_down_value = 0;
volatile uint8_t key_send_flag = 0;
volatile uint8_t key_change_flag = 0;
volatile uint8_t master_flag = 1;

dev_param_t g_dev;

uint8_t g_uart_buf[UART_BUF_LEN];
uint8_t g_uart_idx = 0;

volatile uint8_t pwm_flag[4]={0x02,0x02,0x02,0x02};
uint8_t key_led_state[4]={0,0,0,0};
volatile uint16_t count[4]={1111,1111,1111,1111};


void dev_param_get(void)
{
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    flash_read(EEPROM_ADDR_THIS, (uint8_t *)(&g_dev), sizeof(g_dev));
    FLASH_Lock(FLASH_MEMTYPE_DATA);

    if(g_dev.header != EEPROM_MAGIC) {
        g_dev.addr_range = 0xD8;
        g_dev.addr = 0x10;
        g_dev.ch = 4;
        g_dev.name = PANEL_KEY;
        return ;
    }

    g_dev.addr_range = (uint8_t)((g_dev.addr - 0x10) / 6) + 0xD8;

//    if(       0x10 <= g_dev.addr && g_dev.addr < 0x16) {
//        g_dev.addr_range = 0xD8;
//    } else if(0x16 <= g_dev.addr && g_dev.addr < 0x1C) {
//        g_dev.addr_range = 0xD9;
//    } else if(0x1C <= g_dev.addr && g_dev.addr < 0x22) {
//        g_dev.addr_range = 0xDA;
//    } else if(0x22 <= g_dev.addr && g_dev.addr < 0x28) {
//        g_dev.addr_range = 0xDB;
//    } else if(0x28 <= g_dev.addr && g_dev.addr < 0x2E) {
//        g_dev.addr_range = 0xDC;
//    } else if(0x2E <= g_dev.addr && g_dev.addr < 0x34) {
//        g_dev.addr_range = 0xDD;
//    } else if(0x34 <= g_dev.addr && g_dev.addr < 0x3A) {
//        g_dev.addr_range = 0xDE;
//    } else if(0x3A <= g_dev.addr && g_dev.addr < 0x40) {
//        g_dev.addr_range = 0xDF;
//    } else if(0x40 <= g_dev.addr && g_dev.addr < 0x46) {
//        g_dev.addr_range = 0xE0;
//    } else if(0x46 <= g_dev.addr && g_dev.addr < 0x4C) {
//        g_dev.addr_range = 0xE1;
//    } else if(0x4C <= g_dev.addr && g_dev.addr < 0x50) {
//        g_dev.addr_range = 0xE2;
//    }

//    for(uint8_t i=0; i<4; i++) {
//        if(g_dev.status & (1 << i)) {
//            GPIO_HIGH(gpio_led[i].port,   gpio_led[i].pin);
//            GPIO_HIGH(gpio_relay[i].port, gpio_relay[i].pin);
//        } else {
//            GPIO_LOW(gpio_led[i].port,   gpio_led[i].pin);
//            GPIO_LOW(gpio_relay[i].port, gpio_relay[i].pin);
//        }
//    }

    if(g_dev.name == PANEL_KEY) {

    } else if(g_dev.name == PANEL_TOUCH) {

    }
    
    master_flag = 1;

    g_dev.name = PANEL_KEY;
}

void dev_param_set(void)
{
    g_dev.header = EEPROM_MAGIC;
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    Refresh_WWDG_Window();
    flash_erase(EEPROM_ADDR_THIS, sizeof(g_dev));
    Refresh_WWDG_Window();
    flash_write(EEPROM_ADDR_THIS, (uint8_t *)(&g_dev), sizeof(g_dev));
    Refresh_WWDG_Window();
    FLASH_Lock(FLASH_MEMTYPE_DATA);
}

void dev_status_set(uint8_t data)
{
    uint8_t i;
    static uint8_t data_last;

    data_last ^= data;          // compare data_last and data,if not changed, no dimming

    for(i=0; i<4; i++) {

        if((data_last & (1 << i)) == 0) {
            continue ;
        }

        if(data & (1 << i)) {
            key_led_state[i] = 1;
            GPIO_HIGH(gpio_relay[i].port, gpio_relay[i].pin);
        } else {
            key_led_state[i] = 0;
            GPIO_LOW(gpio_relay[i].port, gpio_relay[i].pin);
        }

        pwm_flag[i] = 0x01;
    }

    data_last = data;
}


