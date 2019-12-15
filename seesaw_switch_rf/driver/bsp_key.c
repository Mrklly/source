#include "bsp_key.h"
#include "global_variables.h"


const uint8_t key_define[4][2] = {
    {KEY1_PORT, KEY1_PIN},
    {KEY2_PORT, KEY2_PIN},
    {KEY3_PORT, KEY3_PIN},
    {KEY4_PORT, KEY4_PIN}
};


const com_io_t key_io[4] = {
    {KEY1_PORT, KEY1_PIN, key_handler_callback},
    {KEY2_PORT, KEY2_PIN, key_handler_callback},
    {KEY3_PORT, KEY3_PIN, key_handler_callback},
    {KEY4_PORT, KEY4_PIN, key_handler_callback}
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

    Gpio_Init(GpioPortA, GpioPin15, &stcKEYPortCfg);
    Gpio_Init(GpioPortB, GpioPin3,  &stcKEYPortCfg);
    Gpio_Init(GpioPortB, GpioPin4,  &stcKEYPortCfg);
    Gpio_Init(GpioPortB, GpioPin5,  &stcKEYPortCfg);
}

uint8_t Key_Scan(void)
{
    volatile uint8_t KeyTemp = NO_KEY;

    if(KEY_CHECK(KEY1_PORT,KEY1_PIN) == 0) {

        KeyTemp |= KEY1_VALUE;

    }

    if(KEY_CHECK(KEY2_PORT,KEY2_PIN) == 0) {

        KeyTemp |= KEY2_VALUE;

    }

    if(KEY_CHECK(KEY3_PORT,KEY3_PIN) == 0) {

        KeyTemp |= KEY3_VALUE;

    }

    if(KEY_CHECK(KEY4_PORT,KEY4_PIN) == 0) {

        KeyTemp |= KEY4_VALUE;

    }

    return KeyTemp;
}


uint8_t Key_value_detection (void)
{// key_Result[4]
    static uint8_t state[4] = {0, 0, 0, 0};                       //按键初始化按键状态机标志。                           
    static uint8_t key_last[4] = {0, 0, 0, 0};                    //上一次按键记录。                        
    static uint8_t key_now[4] = {0, 0, 0, 0};                     //现在按键记录。 
    static uint8_t time_cont[4] = {0, 0, 0, 0};                      //长按计时。
    uint8_t key_value[4] = {0, 0, 0, 0};                          //键值。

//    key_now = Key_Scan();                           //读取按键值。

    uint8_t i = 0;

    for(; i<4; i++) {

        if(KEY_CHECK(key_define[i][0], key_define[i][1]) == 0) {
            key_now[i] = 1 << i;
        } else {
            key_now[i] = 0;
        }

        switch(state[i])  {

        case STATE_KEY_UP:                            //初始化按键状态。
            if(key_now[i] != key_last[i]) {                     //判断本次按键是否与上一次按键相等。
                state[i] = 1;                                //转换到按键去抖状态。
            }
            key_value[i] = 0;
            time_cont[i] = 0;
            break;
        case STATE_KEY_DOWM:                          //按键去抖动
            if(key_now[i] == key_last[i]) {
            
                state[i] = 2;                                //转换到按键短按状态。
                key_value[i] = key_last[i]|KEY_SHORT;

            } else {
                state[i] = 0;                                //是抖动。
                key_value[i] = 0;
            }
            time_cont[i] = 0;
            break;
        case STATE_KEY_SHORT:                         //按键短按状态。
            if(key_now[i] == key_last[i]) {                     //有按键按下。
                state[i] = 3;                                //转换到按键长按状态。
            } else {                                      //短按按键。
                state[i] = 0;
    //           key_value[i] = key_last[i]|KEY_SHORT;         //键值短按。
            }
            key_value[i] = 0;
            time_cont[i] = 0;
            break;
        case STATE_KEY_LONG:                         //长按状态。
            if(key_now[i] == key_last[i]) {                    //开始计时。
                if(time_cont[i]++ > 75) {                   //1.5秒以上为长按。
                    time_cont[i] = 0;
                    state[i] = 4;                             //按键转换为等待长按释放状态。
                    key_value[i] = key_last[i] | KEY_LONG;       //键值是长按。
                }
            } else {
                state[i] = 0;
    //           key_value[i] = key_last[i]|KEY_SHORT;        //键值修改为短按。
            }
            break;
        case STATE_KEY_LONG_UP:                      //长按松手检测
            if(key_now[i] != key_last[i]) {                 //按键释放
                key_value[i] = key_last[i] | KEY_LONG_UP;      //键值修改为长按抬起。
                if(key_now[i] == NO_KEY) {                   //按键正常抬起。
                    state[i] = 0;
                } else {                                  //别的按键按下造成抬起。
                    state[i] = 1;
                }
            }
            time_cont[i] = 0;
            break;
            default:
            key_value[i] = 0;
            key_last[i] = 0;
            key_now[i] = 0;
            break;
        }

        key_last[i] = key_now[i];                          //更新记录上一次按键。

    }

//    return key_value;                           //返回键值。

    memcpy(key_Result, key_value, 4);
    
    return 0;
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

            if(key_shirt_last) {
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



