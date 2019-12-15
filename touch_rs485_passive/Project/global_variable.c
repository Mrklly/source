#include "global_variable.h"
#include "bsp_gpio.h"

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
    {CH1_PORT, CH1_PIN},
    {CH2_PORT, CH2_PIN},
    {CH3_PORT, CH3_PIN},
    {CH4_PORT, CH4_PIN}
};


volatile uint8_t key_Result = 0;
volatile uint8_t key_down_value = 0;
volatile uint8_t key_send_flag = 0;
volatile uint8_t key_change_flag = 0;
volatile uint8_t master_flag = 1;

dev_param_t g_dev;

uint8_t g_uart_buf[UART_BUF_LEN];
volatile uint8_t g_uart_idx = 0;

uint8_t g_ctrl_index = 0;


void dev_param_get(void)
{
    FLASH_Unlock(FLASH_MEMTYPE_DATA);
    flash_read(EEPROM_ADDR_THIS, (uint8_t *)(&g_dev), sizeof(g_dev));
    FLASH_Lock(FLASH_MEMTYPE_DATA);

    if(g_dev.header != EEPROM_MAGIC) {
        g_dev.addr_range = 0xD8;
        g_dev.addr = 0x10;
        g_dev.ch = 4;
    }

    g_dev.addr_range = (uint8_t)((g_dev.addr - 0x10) / 6) + 0xD8;
    g_ctrl_index = g_dev.addr - 0x10 + 0x01 - ((g_dev.addr_range - 0xD8) * 6);

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
    
    master_flag = 1;
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
    for(i=0; i<4; i++) {
        if(data & ((uint8_t)0x01 << i)) {
            GPIO_HIGH(gpio_led[i].port,   gpio_led[i].pin);
            GPIO_HIGH(gpio_relay[i].port, gpio_relay[i].pin);
        } else {
            GPIO_LOW(gpio_led[i].port,   gpio_led[i].pin);
            GPIO_LOW(gpio_relay[i].port, gpio_relay[i].pin);
        }
    }
}

void delay_ms(uint16_t ms)
{
    uint16_t i;

    while(ms--) {
        for(i=0;i<1420;i++);
    }
}

