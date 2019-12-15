#include "led_dimmer.h"
#include "eeprom.h"


void pwm_set_all(uint16_t *buf)
{
    uint8_t i=0;
    for(; i<g_dev.key_num; i++) {
        if(pwm_flag[i] == 0x01) {
            break ;
        }
    }

    if(i == g_dev.key_num) {
        return ;
    }

    switch (g_dev.key_num) {

    case 6:
        PWM_LED_0 = buf[0];
        PWM_LED_1 = buf[1];
        PWM_LED_2 = buf[2];
        PWM_LED_3 = buf[3];
        PWM_LED_4 = buf[4];
        PWM_LED_5 = buf[5];

        if(g_dev.name != QB502) {
            PWM_BACKLIGHT_0 = 1800 - buf[0];
            PWM_BACKLIGHT_1 = 1800 - buf[1];
            PWM_BACKLIGHT_2 = 1800 - buf[2];
            PWM_BACKLIGHT_3 = 1800 - buf[3];
            PWM_BACKLIGHT_4 = 1800 - buf[4];
            PWM_BACKLIGHT_5 = 1800 - buf[5];
        }

        break;

    case 5:
        PWM_LED_0 = buf[0];
        PWM_LED_3 = buf[1];
        PWM_LED_4 = buf[2];
        PWM_LED_1 = buf[3];
        PWM_LED_5 = buf[4];
        PWM_LED_2 = buf[4];

        if(g_dev.name != QB502) {
            PWM_BACKLIGHT_0 = 1800 - buf[0];
            PWM_BACKLIGHT_3 = 1800 - buf[1];
            PWM_BACKLIGHT_4 = 1800 - buf[2];
            PWM_BACKLIGHT_1 = 1800 - buf[3];
            PWM_BACKLIGHT_5 = 1800 - buf[4];
            PWM_BACKLIGHT_2 = 1800 - buf[4];
        }

        break;

    case 4:
        PWM_LED_0 = buf[0];
        PWM_LED_1 = buf[1];
        PWM_LED_2 = buf[2];
        PWM_LED_3 = buf[3];

        if(g_dev.name != QB502) {
            PWM_BACKLIGHT_0 = 1800 - buf[0];
            PWM_BACKLIGHT_1 = 1800 - buf[1];
            PWM_BACKLIGHT_2 = 1800 - buf[2];
            PWM_BACKLIGHT_3 = 1800 - buf[3];

            PWM_BACKLIGHT_4 = 1800 - buf[0];
            PWM_BACKLIGHT_5 = 1800 - buf[3];
        }

        break;

    case 3:
        if(g_dev.name == QB811) {
            PWM_LED_0 = buf[0];
            PWM_LED_1 = buf[1];
            PWM_LED_2 = buf[2];
            PWM_LED_3 = buf[2];
            PWM_LED_4 = buf[0];
            PWM_LED_5 = buf[2];

            PWM_BACKLIGHT_0 = 1800 - buf[0];
            PWM_BACKLIGHT_1 = 1800 - buf[1];
            PWM_BACKLIGHT_2 = 1800 - buf[2];
            PWM_BACKLIGHT_3 = 1800 - buf[2];
            PWM_BACKLIGHT_4 = 1800 - buf[0];
            PWM_BACKLIGHT_5 = 1800 - buf[2];
        } else {
            PWM_LED_0 = buf[0];
            PWM_LED_1 = buf[1];
            PWM_LED_2 = buf[2];
            PWM_LED_3 = buf[0];
            PWM_LED_4 = buf[1];
            PWM_LED_5 = buf[2];

            if(g_dev.name != QB502) {
                PWM_BACKLIGHT_0 = 1800 - buf[0];
                PWM_BACKLIGHT_1 = 1800 - buf[1];
                PWM_BACKLIGHT_2 = 1800 - buf[2];
                PWM_BACKLIGHT_3 = 1800 - buf[0];
                PWM_BACKLIGHT_4 = 1800 - buf[1];
                PWM_BACKLIGHT_5 = 1800 - buf[2];
            }
        }

        break;

    case 2:
        if(g_dev.name == QB811) {
            PWM_LED_0 = buf[0];
            PWM_LED_1 = buf[0];
            PWM_LED_2 = buf[1];
            PWM_LED_3 = buf[1];

            PWM_BACKLIGHT_0 = 1800 - buf[0];
            PWM_BACKLIGHT_1 = 1800 - buf[0];
            PWM_BACKLIGHT_2 = 1800 - buf[1];
            PWM_BACKLIGHT_3 = 1800 - buf[1];
            PWM_BACKLIGHT_4 = 1800 - buf[0];
            PWM_BACKLIGHT_5 = 1800 - buf[1];
        } else {
            if(dev_key_2_type == KEY_2_V) {
                PWM_LED_0 = buf[0];
                PWM_LED_1 = buf[0];
                PWM_LED_2 = buf[1];
                PWM_LED_3 = buf[1];

                if(g_dev.name != QB502) {
                    PWM_BACKLIGHT_0 = 1800 - buf[0];
                    PWM_BACKLIGHT_1 = 1800 - buf[0];
                    PWM_BACKLIGHT_2 = 1800 - buf[1];
                    PWM_BACKLIGHT_3 = 1800 - buf[1];
                }
            } else if(dev_key_2_type == KEY_2_H) {
                PWM_LED_0 = buf[1];
                PWM_LED_1 = buf[0];
                PWM_LED_2 = buf[0];
                PWM_LED_3 = buf[1];

                if(g_dev.name != QB502) {
                    PWM_BACKLIGHT_0 = 1800 - buf[1];
                    PWM_BACKLIGHT_1 = 1800 - buf[0];
                    PWM_BACKLIGHT_2 = 1800 - buf[0];
                    PWM_BACKLIGHT_3 = 1800 - buf[1];
                }
            } else if(dev_key_2_type == KEY_2_6) {
                PWM_LED_1 = buf[1];
                PWM_LED_4 = buf[0];

                if(g_dev.name != QB502) {
                    PWM_BACKLIGHT_1 = 1800 - buf[1];
                    PWM_BACKLIGHT_4 = 1800 - buf[0];
                }
            }
        }

        break;

    case 1:
        if(g_dev.name == QB811) {
            PWM_LED_0 = buf[0];
            PWM_LED_1 = buf[0];

            PWM_BACKLIGHT_0 = 1800 - buf[0];
            PWM_BACKLIGHT_1 = 1800 - buf[0];
            PWM_BACKLIGHT_4 = 1800 - buf[0];

        } else {
            PWM_LED_1 = buf[0];
            PWM_LED_4 = buf[0];

            if(g_dev.name != QB502) {
                PWM_BACKLIGHT_1 = 1800 - buf[0];
                PWM_BACKLIGHT_4 = 1800 - buf[0];
            }
        }

        break;

    default :
        break;
    }
}

void pwm_duty_update(uint16_t *buf_duty)
{
    static volatile uint16_t pwm_set[6] = {0, 0, 0, 0, 0, 0};

    volatile uint8_t i=0;
        
    for(; i<6; i++) {

        if(pwm_flag[i] != 0x01) {
            continue ;
        }

        if(key_led_state[i]) {

            if(1500 <= buf_duty[i] && buf_duty[i] < 1600) {
                pwm_set[i]++;
                if(pwm_set[i] == 2) {
                    pwm_set[i]=0;
                    buf_duty[i]--;
                }
            } else if(1200 <= buf_duty[i] && buf_duty[i] < 1500) {
                pwm_set[i]++;
                if(pwm_set[i] == 2) {
                    pwm_set[i]=0;
                    buf_duty[i]--;
                }
            } else if(1000 <= buf_duty[i] && buf_duty[i] < 1200) {
                pwm_set[i]++;
                if(pwm_set[i] == 1) {
                    pwm_set[i] = 0;
                    buf_duty[i]--;
                }
            } else {
                if(buf_duty[i]) {
                    buf_duty[i]--;
                } else if(buf_duty[i] == 0) {
                    pwm_flag[i] = 0x02;
                }
            }

        } else {

            pwm_set[i] = 0;

            buf_duty[i]++;

            if(buf_duty[i] >= 1800) {
                pwm_flag[i] = 0x02;
                buf_duty[i] = 1800;
            }
        }
    }
}

void led_dimmer_run(void)
{
    static uint16_t pwm_duty[6] = {1800, 1800, 1800, 1800, 1800, 1800};

    pwm_duty_update(pwm_duty);

    pwm_set_all(pwm_duty);

}

