#include "key.h"
#include "global_variables.h"


const com_io g_io[5] = {
    {KEY_PORT_MODE,     KEY_PIN_MODE,   btn_hander_mode},   // MODE
    {KEY_PORT_FAN,      KEY_PIN_FAN,    btn_hander_fan},    // FAN
    {KEY_PORT_UP,       KEY_PIN_UP,     btn_hander_up},     // UP
    {KEY_PORT_DOWN,     KEY_PIN_DOWN,   btn_hander_down},   // DOWN
    {KEY_PORT_ON_OFF,   KEY_PIN_ON_OFF, btn_hander_pwn},    // ON/OFF
};

void key_init(void)
{
    stc_gpio_config_t           stcLEDPortCfg;
    DDL_ZERO_STRUCT(stcLEDPortCfg);

    stcLEDPortCfg.enDir  = GpioDirIn;
    stcLEDPortCfg.enPuPd = GpioPu;
    
    for(uint8_t i = 0; i < (size_of(g_io)); i++) {
        Gpio_Init(g_io[i].key_port, g_io[i].key_pin, &stcLEDPortCfg);
    }
}

uint8_t key_scan(en_gpio_port_t enPort, en_gpio_pin_t enPin, uint8_t *press_counter)
{
    if( 0 == Gpio_GetInputIO(enPort, enPin)) {
        (*press_counter)++;
    } else {
        *press_counter = 0;
    }
    return *press_counter;
}

void key_check_all(void)
{
    uint8_t i = 0;

    for(; i < size_of(g_io); i++) {
        key_scan(g_io[i].key_port,g_io[i].key_pin,&g_btn[i].shirt);
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

void key_handler(void)
{
    uint8_t i = 0;

    if(g_set_type) {   // setting function handler key by themselves
        return;
    }

    for(; i < size_of(g_io) - 1; i++) {
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
}

