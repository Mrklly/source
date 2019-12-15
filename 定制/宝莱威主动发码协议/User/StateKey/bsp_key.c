#include "bsp_key.h"
#include "bsp_timer.h"
#include "bsp_wwdg.h"
#include "global_variable.h"


const key_io_t key_define[4] = {
    {KEY1_PORT, KEY1_PIN},
    {KEY2_PORT, KEY2_PIN},
    {KEY3_PORT, KEY3_PIN},
    {KEY4_PORT, KEY4_PIN}
};


void KEY_Init(void)
{
    GPIO_Init(KEY1_PORT,KEY1_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(KEY2_PORT,KEY2_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(KEY3_PORT,KEY3_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(KEY4_PORT,KEY4_PIN, GPIO_MODE_IN_PU_NO_IT);
}


uint8_t Key_value_detection (void)
{
    static uint8_t key_last[4] = {0, 0, 0, 0};
    volatile uint8_t key_now[4] = {0, 0, 0, 0};

    volatile uint8_t i = 0;

    for(; i<4; i++) {

        if(KEY_CHECK(key_define[i].key_port, key_define[i].key_pin) == 0) {
            key_now[i] = 1;
            key_Result |= 1 << i;
        } else {
            key_now[i] = 0;
            key_Result &= ~(1 << i);
        }

        if(key_now[i] != key_last[i]) {
            key_changed = 1;
            g_uart_delay = UART_DELAY_MS;
        }

        key_last[i] = key_now[i];
    }

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


