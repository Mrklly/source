#include "bsp_key.h"
#include "global_variables.h"


const uint8_t key_define[1][2] = {
    {KEY_PORT, KEY_PIN}
};


const com_io_t key_io[1] = {
    {KEY_PORT, KEY_PIN, key_handler_callback}
};


void KEY_Init(void)
{
    stc_gpio_config_t stcKEYPortCfg;

    DDL_ZERO_STRUCT(stcKEYPortCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stcKEYPortCfg.enDir   = GpioDirIn;
    stcKEYPortCfg.enDrv   = GpioDrvL;
    stcKEYPortCfg.enPuPd  = GpioPu;
    stcKEYPortCfg.enOD    = GpioOdDisable;

    Gpio_Init(KEY_PORT, KEY_PIN, &stcKEYPortCfg);
}

uint8_t Key_Scan(void)
{
    volatile uint8_t KeyTemp = NO_KEY;

    if(KEY_CHECK(KEY_PORT,KEY_PIN) == 0) {

        KeyTemp |= KEY1_VALUE;

    }

    return KeyTemp;
}
uint8_t key_scan(en_gpio_port_t enPort, en_gpio_pin_t enPin, uint16_t *press_counter)
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
    static uint16_t key_shirt_last[4];

    for(uint8_t i = 0; i < sizeof(key_io)/sizeof(key_io[0]); i++) {

        key_scan(key_io[i].key_port, key_io[i].key_pin, &g_btn[i].shirt);

        if(g_btn[i].shirt > 0) {
            g_btn[i].down = 1;
        } else {
            g_btn[i].down = 0;

            if(key_shirt_last[i]) {
                g_btn[i].up = 1;
            } else {
                g_btn[i].up = 0;
            }
        }

        key_shirt_last[i] = g_btn[i].shirt;
    }
}

void key_handler(void)
{
    for(uint8_t i = 0; i < sizeof(key_io)/sizeof(key_io[0]); i++) {

        key_io[i].hander(i, g_btn[i].shirt);
    }
}



