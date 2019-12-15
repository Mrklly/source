#include "bsp_key.h"
#include "eeprom.h"
#include "global_variables.h"


const uint8_t gpio_key[6][2] = {
    {PORT_KEY1, PIN_KEY1},
    {PORT_KEY2, PIN_KEY2},
    {PORT_KEY3, PIN_KEY3},
    {PORT_KEY4, PIN_KEY4},
    {PORT_KEY5, PIN_KEY5},
    {PORT_KEY6, PIN_KEY6},
};


void key_init_all(void)
{
    stc_gpio_config_t stcKEYPortCfg;

    DDL_ZERO_STRUCT(stcKEYPortCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stcKEYPortCfg.enDir   = GpioDirIn;
    stcKEYPortCfg.enDrv   = GpioDrvL;
    stcKEYPortCfg.enPuPd  = GpioPu;
    stcKEYPortCfg.enOD    = GpioOdDisable;

    for(uint8_t i=0; i<6; i++) {

        if(g_dev.key_num == 4) {                        // exclude key5 and key6
            if(i == 4 || i == 5) {
                stcKEYPortCfg.enPuPd = GpioPd;          // unused keys setting in pull down mode
            }
        } else if (g_dev.key_num == 2) {
            if(g_dev.name == QB811) {
                if(i == 4 || i == 5) {
                    stcKEYPortCfg.enPuPd = GpioPd;
                }
            } else {
                if(dev_key_2_type == KEY_2_V || dev_key_2_type == KEY_2_H) {
                    if(i == 4 || i == 5) {
                        stcKEYPortCfg.enPuPd = GpioPd;
                    }
                } else if(dev_key_2_type == KEY_2_6) {
                    if(i == 0 || i == 2 || i == 3 || i == 5) {  // 1 key only using key2 and key5
                        stcKEYPortCfg.enPuPd = GpioPd;
                    }
                } else {
                    stcKEYPortCfg.enPuPd = GpioPd; ;        // key 2 must specify key alignment
                }
            }
        } else if (g_dev.key_num == 1) {
            if(g_dev.name == QB811) {
                if(i == 2 || i == 3 || i == 4 || i == 5) {
                    stcKEYPortCfg.enPuPd = GpioPd;
                }
            } else {
                if(i == 0 || i == 2 || i == 3 || i == 5) {  // 1 key only using key2 and key5
                    stcKEYPortCfg.enPuPd = GpioPd;
                }
            }
        } else if (g_dev.key_num == 0) {                // this could NOT be happened
            stcKEYPortCfg.enPuPd = GpioPd;
        }

        Gpio_Init(gpio_key[i][0], gpio_key[i][1], &stcKEYPortCfg);

        stcKEYPortCfg.enPuPd = GpioPu;
    }
}

uint8_t Key_Scan(void)
{
    volatile uint8_t KeyTemp = NO_KEY;

    if(KEY_CHECK(PORT_KEY1, PIN_KEY1) == 0) {
        KeyTemp |= KEY1_VALUE;
    }

    if(KEY_CHECK(PORT_KEY2, PIN_KEY2) == 0) {
        KeyTemp |= KEY2_VALUE;
    }

    if(KEY_CHECK(PORT_KEY3, PIN_KEY3) == 0) {
        KeyTemp |= KEY3_VALUE;
    }

    if(KEY_CHECK(PORT_KEY4, PIN_KEY4) == 0) {
        KeyTemp |= KEY4_VALUE;
    }

    if(KEY_CHECK(PORT_KEY5, PIN_KEY5) == 0) {
        KeyTemp |= KEY5_VALUE;
    }

    if(KEY_CHECK(PORT_KEY6, PIN_KEY6) == 0) {
        KeyTemp |= KEY6_VALUE;
    }

    return KeyTemp;
}


uint8_t key_check_all(void)
{
    static uint8_t key_last[6]  = {0, 0, 0, 0, 0, 0};
    uint8_t key_now[6]          = {0, 0, 0, 0, 0, 0};
    volatile key_bit_t temp_key_bit;
    volatile uint8_t temp_key_change_flag = 0;

    temp_key_bit.key_res = 0;

    for(volatile uint8_t i = 0; i<6; i++) {

        if(g_dev.key_num == 4) {                        // exclude key5 and key6
            if(i == 4 || i == 5) {
                continue;
            }
        } else if (g_dev.key_num == 2) {
            if(g_dev.name == QB811) {
                if(i == 4 || i == 5) {
                    continue;
                }
            } else {
                if(dev_key_2_type == KEY_2_V || dev_key_2_type == KEY_2_H) {
                    if(i == 4 || i == 5) {
                        continue;
                    }
                } else if(dev_key_2_type == KEY_2_6) {
                    if(i == 0 || i == 2 || i == 3 || i == 5) {  // 1 key only using key2 and key5
                        continue;
                    }
                } else {
                    break ;                                // key 2 must specify key alignment
                }
            }
        } else if (g_dev.key_num == 1) {
            if(g_dev.name == QB811) {
                if(i == 2 || i == 3 || i == 4 || i == 5) {  // 1 key only using key2 and key5
                    continue;
                }
            } else {
                if(i == 0 || i == 2 || i == 3 || i == 5) {  // 1 key only using key2 and key5
                    continue;
                }
            }
        } else if (g_dev.key_num == 0) {                // this could NOT be happened
            continue;
        }

        if(KEY_CHECK(gpio_key[i][0], gpio_key[i][1]) == 0) {
            temp_key_bit.key_res |= 1 << i;
            key_now[i] = 1;
        } else {
            temp_key_bit.key_res &= ~(1 << i);
            key_now[i] = 0;
        }

        if(key_now[i] != key_last[i]) {
            temp_key_change_flag |= 1;
        }

        key_last[i] = key_now[i];
    }

    if(temp_key_change_flag == 0) { // no key pressed
        return 0;
    }

    switch (g_dev.key_num) {

    case 6:
        key_bit.key_res = temp_key_bit.key_res;
        break;

    case 5:
        key_bit.key_bit.key1 = temp_key_bit.key_bit.key1;
        key_bit.key_bit.key2 = temp_key_bit.key_bit.key4;
        key_bit.key_bit.key3 = temp_key_bit.key_bit.key5;
        key_bit.key_bit.key4 = temp_key_bit.key_bit.key2;
        key_bit.key_bit.key5 = temp_key_bit.key_bit.key3 | temp_key_bit.key_bit.key6;
        break;

    case 4:
        key_bit.key_bit.key1 = temp_key_bit.key_bit.key1;
        key_bit.key_bit.key2 = temp_key_bit.key_bit.key2;
        key_bit.key_bit.key3 = temp_key_bit.key_bit.key3;
        key_bit.key_bit.key4 = temp_key_bit.key_bit.key4;
        break;

    case 3:
        if(g_dev.name == QB811) {
            key_bit.key_bit.key1 = temp_key_bit.key_bit.key1;
            key_bit.key_bit.key2 = temp_key_bit.key_bit.key2;
            key_bit.key_bit.key3 = temp_key_bit.key_bit.key3 | temp_key_bit.key_bit.key4;    
        } else {
            key_bit.key_bit.key1 = temp_key_bit.key_bit.key1 | temp_key_bit.key_bit.key4;
            key_bit.key_bit.key2 = temp_key_bit.key_bit.key2 | temp_key_bit.key_bit.key5;
            key_bit.key_bit.key3 = temp_key_bit.key_bit.key3 | temp_key_bit.key_bit.key6;
        }
        break;

    case 2:
        if(g_dev.name == QB811) {
            key_bit.key_bit.key1 = temp_key_bit.key_bit.key1 | temp_key_bit.key_bit.key2;
            key_bit.key_bit.key2 = temp_key_bit.key_bit.key3 | temp_key_bit.key_bit.key4;
        } else {
            if(dev_key_2_type == KEY_2_V) {
                key_bit.key_bit.key1 = temp_key_bit.key_bit.key1 | temp_key_bit.key_bit.key2;
                key_bit.key_bit.key2 = temp_key_bit.key_bit.key3 | temp_key_bit.key_bit.key4;
            } else if(dev_key_2_type == KEY_2_H) {
                key_bit.key_bit.key1 = temp_key_bit.key_bit.key2 | temp_key_bit.key_bit.key3;
                key_bit.key_bit.key2 = temp_key_bit.key_bit.key1 | temp_key_bit.key_bit.key4;
            } else if(dev_key_2_type == KEY_2_6) {
                key_bit.key_bit.key1 = temp_key_bit.key_bit.key5;
                key_bit.key_bit.key2 = temp_key_bit.key_bit.key2;
            }
        }
        break;

    case 1:
        if(g_dev.name == QB811) {
            key_bit.key_bit.key1 = temp_key_bit.key_bit.key1 | temp_key_bit.key_bit.key2;
        } else {
            key_bit.key_bit.key1 = temp_key_bit.key_bit.key2 | temp_key_bit.key_bit.key5;
        }
        break;

    default :
        break;
    }

    key_change_flag = temp_key_change_flag;

    if(key_bit.key_res) {
        key_down = key_bit;
    }

    key_send_flag = 0;

    return 0;
}

