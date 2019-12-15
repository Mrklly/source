#include "drv_key.h"


void hal_key_init(void)
{
    Gpio_InitIOExt(PORT_KEY_1, PIN_KEY_1, GpioDirIn, FALSE, TRUE, FALSE, FALSE);
    Gpio_InitIOExt(PORT_KEY_2, PIN_KEY_2, GpioDirIn, FALSE, TRUE, FALSE, FALSE);
    Gpio_InitIOExt(PORT_KEY_3, PIN_KEY_3, GpioDirIn, FALSE, TRUE, FALSE, FALSE);
    Gpio_InitIOExt(PORT_KEY_4, PIN_KEY_4, GpioDirIn, FALSE, TRUE, FALSE, FALSE);
}

uint8_t Key_Scan(void)
{
    uint8_t KeyTemp = 0x00;

    if(KEY_STA_0) {
        KeyTemp = KEY1_VALUE;
        return KeyTemp;
    } else if(KEY_STA_1) {
        KeyTemp = KEY2_VALUE;
        return KeyTemp;
    } else if(KEY_STA_2) {
        KeyTemp = KEY3_VALUE;
        return KeyTemp;
    } else if(KEY_STA_3) {
        KeyTemp = KEY4_VALUE;
        return KeyTemp;
    } else {
        KeyTemp = NO_KEY;
    }

    return KeyTemp;
}

uint8_t Key_value_detection (void)
{
    static uint8_t state = STATE_KEY_UP;                             //按键初始化按键状态机标志。
    static uint8_t key_last = 0;                          //上一次按键记录。
    static uint8_t key_now = 0;                           //现在按键记录。
    static uint8_t time_keep_cont = 0;                    //短按维持计时。
    static uint8_t time_cont = 0;                         //长按计时。
    static uint8_t temp = 0;                              //维持按键计时。
    uint8_t key_value = 0;                                //键值。

    key_now = Key_Scan();                                 //读取按键值。

    switch(state) {
    case STATE_KEY_UP: {                                  //初始化按键状态。
        if(key_now != key_last) {                         //判断本次按键是否与上一次按键相等。
            state = STATE_KEY_DOWM;                       //转换到按键去抖状态。
        }
    }
    break;

    case STATE_KEY_DOWM: {                                //按键去抖动
        if(key_now == key_last) {
            state = STATE_KEY_SHORT;                      //转换到按键短按状态。
        } else {
            state = STATE_KEY_UP;                         //是抖动。
        }
    }
    break;

    case STATE_KEY_SHORT: {                               //按键短按状态。

        if(key_now == key_last) {                         //有按键按下。
            state = STATE_KEY_KEEP;                                      //转换到按键短按维持状态。
            key_value = key_last | KEY_SHORT;               //键值短按。
        } else {                                          //短按按键。
            state = STATE_KEY_UP;
            //key_value = key_last|KEY_SHORT;                 //键值短按。
        }
    }
    break;

    case STATE_KEY_KEEP:                                 //短按按键维持。
        if(key_now == key_last) {
            if(time_keep_cont++ <= 200) {                    //维持大约4s以上,在4ms范围内输出键值为短按。
                temp++;

                if(temp >= 8) {                                //维持按键计时。每160ms推出以下短按维持键值。
                    temp = 0;
                    key_value = key_last | KEY_SHORT_KEEP;       //键值短按维持输出25次，切换到长按状态。
                }
            } else {                                        //切换为长按状态。
                time_keep_cont = 0;
                temp = 0;
                state = STATE_KEY_LONG;                                    //转换为长按状态。
            }
        } else {                                          //维持状态下按键释放。
            if(key_now == NO_KEY) {                         //按键正常抬起。
                state = STATE_KEY_UP;
                temp = 0;
                time_keep_cont = 0;
                //key_value = key_last|KEY_SHORT;               //键值短按。
                key_value = key_last | KEY_SHORT_UP;             //键值短按抬起
            } else {                                        //别的按键按下造成抬起。
                state = 1;
                temp = 0;
                time_keep_cont = 0;
                //key_value = key_last|KEY_SHORT;               //键值短按。
                key_value = key_last | KEY_SHORT_UP;      //键值短按抬起
            }
        }

        break;

    case STATE_KEY_LONG:                               //长按状态。
        if(key_now == key_last) {                      //长按开始计时。
            if(time_cont++ > 254) {                      //6秒以上为长按。
                time_cont = 0;
                state = STATE_KEY_LONG_UP;                                 //按键转换为等待长按释放状态。
                key_value = key_last | KEY_LONG;           //键值是长按。
            }
        } else {
            if(key_now == NO_KEY) {                      //按键正常抬起。
                state = 0;
                time_cont = 0;
                //key_value = key_last|KEY_SHORT;            //键值修改为短按。
                key_value = key_last | KEY_SHORT_UP;          //键值修改为短按。
            } else {                                     //别的按键按下造成抬起。
                state = 1;
                time_cont = 0;
                //key_value = key_last|KEY_SHORT;            //键值修改为短按。
                key_value = key_last | KEY_SHORT_UP;          //键值修改为短按。
            }
        }
        break;

    case STATE_KEY_LONG_UP:                            //长按松手检测
        if(key_now != key_last) {                      //按键释放
            key_value = key_last | KEY_LONG_UP;          //键值修改为长按抬起。

            if(key_now == NO_KEY) {                      //按键正常抬起。
                state = 0;
            } else {                                     //别的按键按下造成抬起。
                state = 1;
            }
        } else {
            //key_value = key_last|KEY_LONG;
        }
        break;

    default:
        key_value = 0;
        key_last = 0;
        key_now = 0;
        break;
    }

    key_last = key_now;                                //更新记录上一次按键。
    return key_value;                                  //返回键值。
}


