#include "led_dimmer.h"
#include "global_variable.h"

void led_dimmer_run(void)
{
    uint8_t i=0;
    volatile uint8_t *p = &(TIM1->CCR1H);

    for(i=0; i<4; i++) {

        if(pwm_flag[i] == 0x01) {
            PWM_Contorl_LED(i, key_led_state[i]);

            if(i == 3) {
                *(p+4) = (uint8_t )(pwm[i] >> 8);
                *(p+5) = (uint8_t )(pwm[i]);
            } else if(i == 2) {
                *(p+6) = (uint8_t )(pwm[i] >> 8);
                *(p+7) = (uint8_t )(pwm[i]);
            } else {
                *(p+i*2)   = (uint8_t )(pwm[i] >> 8);
                *(p+i*2+1) = (uint8_t )(pwm[i]);
            }
        }
    }
}

