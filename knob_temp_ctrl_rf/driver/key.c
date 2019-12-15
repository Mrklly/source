#include "key.h"
#include "global_variables.h"


#if TEMP_CTRL_KNOB
const com_io g_io[4] = {
    {KEY_PORT_MODE,    KEY_PIN_MODE,   btn_hander_mode},   // MODE
    {KEY_PORT_FAN,     KEY_PIN_FAN,    btn_hander_fan},    // FAN
    {KEY_PORT_FUNC,    KEY_PIN_FUNC,   btn_hander_func},     // UP
    {KEY_PORT_PWR,     KEY_PIN_PWR,    btn_hander_pwr}    // DOWN
};
#elif TEMP_CTRL_TOUCH
const com_io g_io[5] = {
    {KEY_PORT_MODE,     KEY_PIN_MODE,   btn_hander_mode},   // MODE
    {KEY_PORT_FAN,      KEY_PIN_FAN,    btn_hander_fan},    // FAN
    {KEY_PORT_UP,       KEY_PIN_UP,     btn_hander_up},     // UP
    {KEY_PORT_DOWN,     KEY_PIN_DOWN,   btn_hander_down},   // DOWN
    {KEY_PORT_PWR,      KEY_PIN_PWR,    btn_hander_pwr},    // POWER
};
#endif


void key_init(void)
{
    stc_gpio_config_t           stcLEDPortCfg;
    DDL_ZERO_STRUCT(stcLEDPortCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    Gpio_SetAfMode(GpioPortA, GpioPin9, GpioAf0);

    stcLEDPortCfg.enDir  = GpioDirIn;
    stcLEDPortCfg.enDrv  = GpioDrvH;
    stcLEDPortCfg.enOD   = GpioOdDisable;
    stcLEDPortCfg.enPuPd = GpioPu;
    stcLEDPortCfg.enCtrlMode = GpioFastIO;
    
    for(uint8_t i = 0; i < (size_of(g_io)); i++) {
        Gpio_SetAfMode(g_io[i].key_port, g_io[i].key_pin, GpioAf0);
        Gpio_Init(g_io[i].key_port, g_io[i].key_pin, &stcLEDPortCfg);
    }

    Gpio_SetAfMode(PORT_DEV_TYPE, PIN_DEV_TYPE, GpioAf0);
    Gpio_Init(PORT_DEV_TYPE, PIN_DEV_TYPE, &stcLEDPortCfg);

}

uint8_t key_scan(en_gpio_port_t enPort, en_gpio_pin_t enPin, uint8_t *press_counter)
{
    if( 0 == GPIO_PIN_STA(enPort, enPin)) {
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

#if TEMP_CTRL_KNOB
    if((g_btn[2].shirt == 0) && (g_btn[3].shirt > 0)) {
        g_btn[3].down = 1;
    } else {
        g_btn[3].down = 0;
    }
#elif TEMP_CTRL_TOUCH
    if((g_btn[3].shirt == 0) && (g_btn[4].shirt > 0)) {
        g_btn[4].down = 1;
    } else {
        g_btn[4].down = 0;
    }
#endif
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
            if(g_dev.sta.power || (g_lcd_on)) {
                g_lcd_on = 1;
                g_lcd_delay = g_dev.led_timeout * 10;
                g_led_delay = g_dev.led_timeout * 10;
            } else {
                g_lcd_on = 0;
                g_lcd_delay = 0;
                g_led_delay = 20;       // 20 * 100ms = 2s
            }
        }
    }
    i = size_of(g_io) - 1;
    if(g_btn[i].down == 1) {    // TODO:we have to handler ONOFF button specially
        g_lcd_delay = g_dev.led_timeout * 10;   // 100 * 100ms = 10s
        g_led_delay = g_dev.led_timeout * 10;
        g_io[i].hander(g_btn[i].shirt);
    }
}

void get_dev_type(void)
{
    if(GPIO_PIN_STA(PORT_DEV_TYPE, PIN_DEV_TYPE)) {
        g_dev_type = DEV_TYPE_RS485;
    } else {
        g_dev_type = DEV_TYPE_RF433;
    }
}

