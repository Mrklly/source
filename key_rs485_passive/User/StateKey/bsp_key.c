#include "bsp_key.h"
#include "bsp_timer.h"
#include "bsp_wwdg.h"
#include "global_variable.h"


const gpio_t key_define[4] = {
    {KEY1_PORT, KEY1_PIN},
    {KEY2_PORT, KEY2_PIN},
    {KEY3_PORT, KEY3_PIN},
    {KEY4_PORT, KEY4_PIN}
};

const gpio_t key_define_touch[4] = {
    {PORT_TOUCH_KEY1, PIN_TOUCH_KEY1},
    {PORT_TOUCH_KEY2, PIN_TOUCH_KEY2},
    {PORT_TOUCH_KEY3, PIN_TOUCH_KEY3},
    {PORT_TOUCH_KEY4, PIN_TOUCH_KEY4}
};

const gpio_t *p_key_io = (gpio_t *)0;


void KEY_Init(void)
{
    uint8_t i;

    if(g_dev.name == PANEL_KEY) {
        p_key_io = key_define;
    } else if(g_dev.name == PANEL_TOUCH) {
        p_key_io = key_define_touch;
    }

    const gpio_t *p = p_key_io;

    for(i=0; i<4; i++) {
        GPIO_Init(p->key_port, p->key_pin, GPIO_MODE_IN_PU_NO_IT);
        p++;
    }
}

uint8_t Key_value_detection (void)
{
    static uint8_t key_last[4] = {0, 0, 0, 0};
    uint8_t key_now[4] = {0, 0, 0, 0};
    volatile uint8_t temp_key_res = 0;
    volatile uint8_t temp_key_changed = 0;
    const gpio_t *p = p_key_io;


    for(uint8_t i=0; i<4; i++) {

        if(KEY_CHECK(p->key_port, p->key_pin) == 0) {
            temp_key_res |= 1 << i;
            key_now[i] = 1 << i;
        } else {
            temp_key_res &= ~(1 << i);
            key_now[i] = 0;
        }

        if(key_now[i] != key_last[i]) {
            temp_key_changed = 1;
        }

        key_last[i] = key_now[i];

        p++;
    }

    if(temp_key_changed == 0) { // no key pressed
        return 0;
    }

    key_Result = temp_key_res;
    key_change_flag = temp_key_changed;

    if(key_Result) {
        key_down_value= key_Result;
    }

    key_send_flag = 0;

    return 0;
}


#if 0
uint8_t Key_value_detection (void)
{
    uint8_t i = 0;

    static uint8_t key_last = 0x00;
    volatile uint8_t key_temp = 0x00;

//    if(!master_flag) {
//        return ;
//    }

    for(; i<4; i++) {

        if(KEY_CHECK(key_define[i].key_port, key_define[i].key_pin)) {
            key_temp |= 1 << i;
        } else {
            key_temp &= ~(1 << i);
        }

        if((key_last & (1 << i)) == (key_temp & (1 << i))) {
            key_Result &= ~(1 << i) ;
        } else {
            key_Result |= 1 << i;
        }

        if(key_temp & (1 << i)) {
            key_last |= (1 << i);
        } else {
            key_last &= ~(1 << i);
        }
    }
    
//    if(key_last == key_temp) {
//        key_Result = 0;
//    } else {
//        key_Result = key_temp;
//    }

//    key_last = key_temp;

    return 0;
}
#endif


