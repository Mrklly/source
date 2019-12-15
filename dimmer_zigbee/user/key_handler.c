#include "key_handler.h"
#include "drv_key.h"
#include "flexible_button.h"
#include "global_variables.h"
#include "dimmer.h"
#include "mcu_api.h"
#include "zg_register.h"


//#define debug_key printf
#define debug_key


typedef enum {
    USER_BUTTON_0 = 0,
    USER_BUTTON_1,
    USER_BUTTON_2,
    USER_BUTTON_3,
    USER_BUTTON_MAX
} user_button_t;

static flex_button_t user_button[USER_BUTTON_MAX];

static void btn_0_cb(flex_button_t *btn)
{
    switch (btn->event) {
    case FLEX_BTN_PRESS_DOWN:
        debug_key("btn_0 press down\n");

        if(led_sta[1] == 0) {
            led_sta[1] = 1;
        } else {
            if(Chopping1_channel_cont) {
                Chopping1_channel_cont--;
            }
        }

        break;

    case FLEX_BTN_PRESS_CLICK:
        debug_key("btn_0 press click\n");
        break;

    case FLEX_BTN_PRESS_DOUBLE_CLICK:
        debug_key("btn_0 press double click\n");
        break;

    case FLEX_BTN_PRESS_SHORT_START:
        debug_key("btn_0 press short start\n");
        break;

    case FLEX_BTN_PRESS_SHORT_UP:
        debug_key("btn_0 press short up\n");
        break;

    case FLEX_BTN_PRESS_LONG_START:
        debug_key("btn_0 press long start\n");
        break;

    case FLEX_BTN_PRESS_LONG_UP:
        debug_key("btn_0 press long up\n");
        break;

    case FLEX_BTN_PRESS_LONG_HOLD:
        debug_key("btn_0 press long hold\n");
        break;

    case FLEX_BTN_PRESS_LONG_HOLD_UP:
        debug_key("btn_0 press long hold up\n");
        break;
    }
}

static void btn_1_cb(flex_button_t *btn)
{
    switch (btn->event) {
    case FLEX_BTN_PRESS_DOWN:
        debug_key("btn_1 press down\n");

        led_sta[1] ^= 0x01;

        break;

    case FLEX_BTN_PRESS_CLICK:
        debug_key("btn_1 press click\n");
        break;

    case FLEX_BTN_PRESS_DOUBLE_CLICK:
        debug_key("btn_1 press double click\n");
        break;

    case FLEX_BTN_PRESS_SHORT_START:
        debug_key("btn_1 press short start\n");
        break;

    case FLEX_BTN_PRESS_SHORT_UP:
        debug_key("btn_1 press short up\n");
        break;

    case FLEX_BTN_PRESS_LONG_START:
        debug_key("btn_1 press long start\n");
        break;

    case FLEX_BTN_PRESS_LONG_UP:
        debug_key("btn_1 press long up\n");
        break;

    case FLEX_BTN_PRESS_LONG_HOLD:
        debug_key("btn_1 press long hold\n");
        break;

    case FLEX_BTN_PRESS_LONG_HOLD_UP:
        debug_key("btn_1 press long hold up\n");
        break;
    }
}


static void btn_2_cb(flex_button_t *btn)
{
    switch (btn->event) {
    case FLEX_BTN_PRESS_DOWN:
        debug_key("btn_2 press down\n");

        if(led_sta[1]) {

            Chopping1_channel_cont++;

            if(Chopping1_channel_cont >= STEP) {
                Chopping1_channel_cont = STEP;
                led_sta[1] = 0;
            }
        }

        break;

    case FLEX_BTN_PRESS_CLICK:
        debug_key("btn_2 press click\n");
        break;

    case FLEX_BTN_PRESS_DOUBLE_CLICK:
        debug_key("btn_2 press double click\n");
        break;

    case FLEX_BTN_PRESS_SHORT_START:
        debug_key("btn_2 press short start\n");
        break;

    case FLEX_BTN_PRESS_SHORT_UP:
        debug_key("btn_2 press short up\n");
        break;

    case FLEX_BTN_PRESS_LONG_START:
        debug_key("btn_2 press long start\n");
        break;

    case FLEX_BTN_PRESS_LONG_UP:
        debug_key("btn_2 press long up\n");
        break;

    case FLEX_BTN_PRESS_LONG_HOLD:
        debug_key("btn_2 press long hold\n");
        break;

    case FLEX_BTN_PRESS_LONG_HOLD_UP:
        debug_key("btn_2 press long hold up\n");
        break;
    }
}

static void btn_3_cb(flex_button_t *btn)
{
    switch (btn->event) {
    case FLEX_BTN_PRESS_DOWN:
        debug_key("btn_3 press down\n");
        break;

    case FLEX_BTN_PRESS_CLICK:
        debug_key("btn_3 press click\n");
        break;

    case FLEX_BTN_PRESS_DOUBLE_CLICK:
        debug_key("btn_3 press double click\n");
        break;

    case FLEX_BTN_PRESS_SHORT_START:
        debug_key("btn_3 press short start\n");
        break;

    case FLEX_BTN_PRESS_SHORT_UP:
        debug_key("btn_3 press short up\n");
        break;

    case FLEX_BTN_PRESS_LONG_START:
        debug_key("btn_3 press long start\n");
        break;

    case FLEX_BTN_PRESS_LONG_UP:
        debug_key("btn_3 press long up\n");
        break;

    case FLEX_BTN_PRESS_LONG_HOLD:
        debug_key("btn_3 press long hold\n");
        break;

    case FLEX_BTN_PRESS_LONG_HOLD_UP:
        debug_key("btn_3 press long hold up\n");
        break;
    }
}

static uint8_t button_key0_read(void)
{
    return KEY_STA_0;
}

static uint8_t button_key1_read(void)
{
    return KEY_STA_1;
}

static uint8_t button_key2_read(void)
{
    return KEY_STA_2;
}

static uint8_t button_key3_read(void)
{
    return KEY_STA_3;
}

void user_button_init(void)
{
    uint8_t i;

    memset(&user_button[0], 0, sizeof(user_button));

    user_button[USER_BUTTON_0].usr_button_read = button_key0_read;
    user_button[USER_BUTTON_0].cb = (flex_button_response_callback)btn_0_cb;

    user_button[USER_BUTTON_1].usr_button_read = button_key1_read;
    user_button[USER_BUTTON_1].cb = (flex_button_response_callback)btn_1_cb;

    user_button[USER_BUTTON_2].usr_button_read = button_key2_read;
    user_button[USER_BUTTON_2].cb = (flex_button_response_callback)btn_2_cb;

    user_button[USER_BUTTON_3].usr_button_read = button_key3_read;
    user_button[USER_BUTTON_3].cb = (flex_button_response_callback)btn_3_cb;

    for (i = 0; i < USER_BUTTON_MAX; i ++) {
        user_button[i].pressed_logic_level = 1;
        user_button[i].click_start_tick = 20;
        user_button[i].short_press_start_tick = 100;
        user_button[i].long_press_start_tick = 200;
        user_button[i].long_hold_start_tick = 300;

        flex_button_register(&user_button[i]);
    }
}


void key_handler(void)
{
    uint8_t key;

    if(key_Result == 0) {
        return;
    }

    key = key_Result;
    key_Result = 0x0;

    switch(key) {
    //第一路开关
    case KEY2_VALUE|KEY_SHORT:

        led_sta[1] ^= 0x01;

        break;

    case KEY2_VALUE|KEY_SHORT_UP:

        zg_reg_pre();

        break;

    case KEY2_VALUE|KEY_LONG:

        zg_reg_start();

        break;

    //第二路开关
    case KEY5_VALUE|KEY_SHORT:

        led_sta[4] ^= 0x01;

        break;

    //第一路调亮
    case KEY1_VALUE|KEY_SHORT:

        led_sta[0] = 1;

        if(led_sta[1] == 0) {
            led_sta[1] = 1;
        } else {
            if(Chopping1_channel_cont) {
                Chopping1_channel_cont--;
            }
        }

        break;

    case KEY1_VALUE|KEY_SHORT_KEEP :

        if(Chopping1_channel_cont > 0) {
            Chopping1_channel_cont--;
        }

        break;

    case KEY1_VALUE|KEY_SHORT_UP:

        led_sta[0] = 0;

        zg_reg_pre();

        break;

    case KEY1_VALUE|KEY_LONG:

        zg_reg_start();

        break;

    case KEY1_VALUE|KEY_LONG_UP:

        led_sta[0] = 0;

        break;


    //第一路调暗
    case KEY3_VALUE|KEY_SHORT:

        led_sta[2] = 1;

        if(led_sta[1]) {

            Chopping1_channel_cont++;

            if(Chopping1_channel_cont >= STEP) {
                Chopping1_channel_cont = STEP;
                led_sta[1] = 0;
            }
        }

        break;

    case KEY3_VALUE|KEY_SHORT_KEEP:

        if(led_sta[1]) {

            Chopping1_channel_cont++;

            if(Chopping1_channel_cont >= STEP) {
                Chopping1_channel_cont = STEP;
                led_sta[1] = 0;
            }
        } else {

        }

        break;

    case KEY3_VALUE|KEY_LONG:

        zg_reg_start();

        break;

    case KEY3_VALUE|KEY_SHORT_UP:

        led_sta[2] = 0;

        zg_reg_pre();

        break;

    case KEY3_VALUE|KEY_LONG_UP:

        led_sta[2] = 0;

        break;

    //第二路调亮
    case KEY4_VALUE|KEY_SHORT:

        break;

    case KEY4_VALUE|KEY_SHORT_KEEP:

        break;

    case KEY4_VALUE|KEY_SHORT_UP:

        break;

    case KEY4_VALUE|KEY_LONG_UP:

        break;

    //第二路调暗
    case KEY6_VALUE|KEY_SHORT:

        break;

    case KEY6_VALUE|KEY_SHORT_KEEP:

        break;

    case KEY6_VALUE|KEY_SHORT_UP:

        break;

    case KEY6_VALUE|KEY_LONG_UP:

        break;

    default:
        key = NO_KEY;
        break;
    }
}





