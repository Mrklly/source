#include "bsp_system.h"
#include "bsp_flash.h"
#include "bsp_it.h"

#include "eeprom.h"
#include "third_agreement.h"
#include "key_handler.h"


int main()
{
    system_init();
    flash_init();
    TIMER2_Init();
    RS485_Init();

    protocol_param_get();

    LED_Init();
    Relay_Init();
    KEY_Init();
    PWM_timer1();
    PWM_timer3();

    uint8_t buf_key_value[KEY_COUNT], buf_key_status[KEY_COUNT];
    memset(buf_key_value,  0, KEY_COUNT);
    memset(buf_key_status, 0, KEY_COUNT);

    while(1) {
        key_value_pack(buf_key_value, buf_key_status);
        protocol_send_key_status(buf_key_value, buf_key_status);
        protocol_run();
    }
}

