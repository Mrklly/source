#include "key.h"
#include "stm8s_gpio.h"
#include "led.h"
#include "pwm.h"
#include "e2p.h"
#include "global_variables.h"


void KEY_Init(void)
{
    u8	i;
    for(i = 0; i < (size_of(g_io)); i++) {
        GPIO_Init(g_io[i].key_port,g_io[i].key_pin,GPIO_MODE_IN_PU_NO_IT);
    }
}

u8 KEY_Down(GPIO_TypeDef* GPIOx,GPIO_Pin_TypeDef  GPIO_Pin,u8 *press_counter)
{
    u8 i ;
//    *shirt = *shirt << 1;
    i = GPIO_ReadInputPin(GPIOx,GPIO_Pin);
    if( i == 0) {
        (*press_counter)++;
    } else {
        *press_counter = 0;
    }
    return *press_counter;
}

void button_check(void)
{
    u8 i;

    for(i = 0; i < size_of(g_io); i++) {
        KEY_Down(g_io[i].key_port,g_io[i].key_pin,&g_btn[i].shirt);
    }

    if((g_btn[0].shirt > 0) && (g_btn[1].shirt == 0)) { // && to avoid near button being pressed
        g_btn[0].down = 1;
    } else {
        g_btn[0].down = 0;
    }

    for(i = 1; i < size_of(g_io) - 1; i++) {
        if((g_btn[i-1].shirt == 0) && (g_btn[i].shirt > 0) && (g_btn[i+1].shirt == 0)) {
            g_btn[i].down = 1;
        } else {
            g_btn[i].down = 0;
        }
    }

    if((g_btn[3].shirt == 0) && (g_btn[4].shirt > 0)) {
        g_btn[4].down = 1;
    } else {
        g_btn[4].down = 0;
    }
}

void button_handler(void)
{
    u8 i;

    if(g_set_addr_baud) {   // setting function handler key by themselves
        return;
    }

    for(i = 0; i < size_of(g_io) - 1; i++) {
        if(g_btn[i].down == 1) {

            if(g_lcd_on) {
                g_io[i].hander(g_btn[i].shirt);
            }

            g_led_on = 1;
            if(g_dev.mode.power || (g_lcd_on)) {
                g_lcd_on = 1;
                g_lcd_delay = 100;   // 50 * 100ms
                g_led_delay = 100;
            } else {
                g_lcd_on = 0;
                g_lcd_delay = 0;
                g_led_delay = 20;
            }
        }
    }
    i = size_of(g_io) - 1;
    if(g_btn[i].down == 1) {    // TODO:we have to handler ONOFF button specially
        g_lcd_delay = 100;   // 50 * 100ms
        g_led_delay = 100;
        g_io[i].hander(g_btn[i].shirt);
    }

    if((g_btn[0].shirt) || (g_btn[1].shirt) || (g_btn[2].shirt) || (g_btn[3].shirt)) {
        g_led_setting_flag = 1;
    } else if (!g_set_addr_baud){
        g_led_setting_flag = 0;
    }
}

#pragma  optimize=none

void AddrMod_Set(void)  // run at every 50 ms
{
    static volatile u16 dev_setting_counter = 0;
    volatile u8 save_status = 0 , led_flash_counter = 3;

//    if(BUTTON_MODE.down==1 &&  BUTTON_FAN.down==0 &&  BUTTON_UP.down==1 &&  BUTTON_DOWN.down==0 &&  BUTTON_ONOFF.down==1 ) {
    if(BUTTON_MODE.down==1 &&  BUTTON_UP.down==1 && BUTTON_ONOFF.down==1 ) {
        dev_setting_counter = 0;
        if(g_set_addr_baud) {
            return ;
        }
        lcd_clear();
        g_dev_set.set_type  = set_addr;
        g_dev_set.dev_addr  = g_eep.addr;
        g_dev_set.baud = g_eep.baud_rate;
        g_set_addr_baud = 1;
        g_led_setting_flag = 1;
        g_led_on = 1;
        g_lcd_on = 1;
        g_saving_flag = 0;
    }
        
    if(g_set_addr_baud == 1) {
        dev_setting_counter++;
        if(dev_setting_counter < 20) {
            return ;
        }
        if(dev_setting_counter % 4 == 0) {
            if(BUTTON_MODE.down==0 && BUTTON_FAN.down==0 &&  BUTTON_UP.down==1 && BUTTON_DOWN.down==0 && BUTTON_ONOFF.down==0) {
                if((g_dev_set.set_type == set_addr) && (g_dev_set.dev_addr < 99)) {
                    g_dev_set.dev_addr++;
                } else if(g_dev_set.set_type == set_baud) {
                    if(BUTTON_UP.shirt > 0x04) {
                        return ;
                    }
                    if(g_dev_set.baud < BAUDRATE38400) {
                        g_dev_set.baud++;
                    } else {
                        g_dev_set.baud = BAUDRATE9600;
                    }
                }
            } if(BUTTON_MODE.down==0 && BUTTON_FAN.down==0 &&  BUTTON_UP.down==0 && BUTTON_DOWN.down==1 && BUTTON_ONOFF.down==0) {
                if((g_dev_set.set_type == set_addr) && (g_dev_set.dev_addr > 0)) {
                    g_dev_set.dev_addr--;
                }else if(g_dev_set.set_type == set_baud) {
                    if(BUTTON_DOWN.shirt > 0x04) {
                        return ;
                    }
                    if(g_dev_set.baud > BAUDRATE9600) {
                        g_dev_set.baud--;
                    } else {
                        g_dev_set.baud = BAUDRATE38400;
                    }
                }
            } else if(BUTTON_MODE.down==0 && BUTTON_FAN.down==1 &&  BUTTON_UP.down==0 && BUTTON_DOWN.down==0 && BUTTON_ONOFF.down==0) {
                if(BUTTON_FAN.shirt > 0x04) {
                    return ;
                }
                if(g_dev_set.set_type == set_addr) {    // BUTTON FAN changes setting type
                    g_dev_set.set_type = set_baud;
                } else if (g_dev_set.set_type == set_baud) {
                    g_dev_set.set_type = set_addr;
                }
            } else if(BUTTON_MODE.down==1 &&  BUTTON_FAN.down==0 &&  BUTTON_UP.down==0 &&  BUTTON_DOWN.down==0 &&  BUTTON_ONOFF.down==0) {
                if(BUTTON_MODE.shirt > 0x04 || g_saving_flag) {
                    return ;
                }
                g_saving_flag = 1;
                g_eep.SOH       = 0xA5;
                g_eep.addr      = g_dev_set.dev_addr;
                g_eep.baud_rate = g_dev_set.baud;
                eep_write(&g_eep);
            } else if(BUTTON_MODE.down==0 && BUTTON_FAN.down==0 &&  BUTTON_UP.down==0 && BUTTON_DOWN.down==0 && BUTTON_ONOFF.down==1) {
                if(BUTTON_ONOFF.shirt > 0x04) {
                    return ;
                }
                g_set_addr_baud = 0;

                g_dev.addr= g_eep.addr;
                if(g_eep.baud_rate != g_dev.baud) {
                    g_dev.baud = g_eep.baud_rate;
                    g_uart_baud_changed = 1;
                } else {
                    g_uart_baud_changed = 0;
                }

                g_lcd_on = 1;   // TODO: called after key_handler(), maybe ugly
                g_led_on = 1;
            }
        }
    }
}

