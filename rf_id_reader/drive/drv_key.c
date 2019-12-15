#include "drv_key.h"


void key_init(void)
{
    GPIO_Init(KEY_PORT_1, KEY_PIN_1, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(KEY_PORT_2, KEY_PIN_2, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(KEY_PORT_3, KEY_PIN_3, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(KEY_PORT_4, KEY_PIN_4, GPIO_MODE_IN_FL_NO_IT);
}

void key_is_down(GPIO_TypeDef* GPIOx,GPIO_Pin_TypeDef  GPIO_Pin,u8 *press_counter)
{
    uint8_t i;

    i = KEY_DOWN(GPIOx, GPIO_Pin);
    if(i) {
        (*press_counter)++;
    } else {
        *press_counter = 0;
    }
}