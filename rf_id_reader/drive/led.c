#include "led.h"

void led_init(void)
{
    GPIO_Init(LED_L_PORT, LED_L_PIN, GPIO_MODE_OUT_PP_HIGH_FAST );
    GPIO_Init(LED_R_PORT, LED_R_PIN, GPIO_MODE_OUT_PP_HIGH_FAST );
    GPIO_Init(LED_RF_PORT, LED_RF_PIN, GPIO_MODE_OUT_PP_HIGH_FAST );
    
    LED_L_OFF;
    LED_R_OFF;
    LED_RF_OFF;
}

