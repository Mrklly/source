#include "bsp_gpio.h"
#include "global_variable.h"

const gpio_t gpio_led_key[4] = {
    {LED1_PORT, LED1_PIN},
    {LED2_PORT, LED2_PIN},
    {LED3_PORT, LED3_PIN},
    {LED4_PORT, LED4_PIN}
};

const gpio_t gpio_led_touch[4] = {
    {PORT_LED1_TOUCH, PIN_LED1_TOUCH},
    {PORT_LED2_TOUCH, PIN_LED2_TOUCH},
    {PORT_LED3_TOUCH, PIN_LED3_TOUCH},
    {PORT_LED4_TOUCH, PIN_LED4_TOUCH}
};

const gpio_t *p_gpio_led = (gpio_t *)0;


void LED_Init(void)
{
    uint8_t i;

    if(g_dev.name == PANEL_KEY) {
        p_gpio_led = gpio_led_key;
    } else if(g_dev.name == PANEL_TOUCH) {
        p_gpio_led = gpio_led_touch;
    }

    const gpio_t *p = p_gpio_led;

    for(i=0; i<4; i++) {
        GPIO_Init(p->key_port, p->key_pin, GPIO_MODE_OUT_PP_HIGH_FAST);
        GPIO_LOW(p->key_port, p->key_pin);
        p++;
    }
}

void LED_open_Close_Config(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins,uint8_t EffectiVelevel)
{
  if(EffectiVelevel==0)
  {
    GPIO_WriteLow(GPIOx,PortPins);              //库函数：输出低电平
  }
  else if(EffectiVelevel==1)
  {
    GPIO_WriteHigh(GPIOx,PortPins);            //库函数：输出高电平
  }
  else
  {
    return;
  }
}

void gpio_set_value(GPIO_TypeDef* port, GPIO_Pin_TypeDef pin,uint8_t value)
{
    if(value) {
        port->ODR |=  pin;
    } else {
        port->ODR &= ~pin;
    }
}

void LED_Toggle(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins)
{
  GPIO_WriteReverse(GPIOx,PortPins);            //库函数实现电平翻转。
}

void Relay_Init(void)
{
    GPIO_Init(PORT_RELAY_CH1,PIN_RELAY_CH1, GPIO_MODE_OUT_PP_HIGH_FAST);      //继电器通道1。
    GPIO_Init(PORT_RELAY_CH2,PIN_RELAY_CH2, GPIO_MODE_OUT_PP_HIGH_FAST);      //继电器通道2。
    GPIO_Init(PORT_RELAY_CH3,PIN_RELAY_CH3, GPIO_MODE_OUT_PP_HIGH_FAST);      //继电器通道3。
    GPIO_Init(PORT_RELAY_CH4,PIN_RELAY_CH4, GPIO_MODE_OUT_PP_HIGH_FAST);      //继电器通道4。
    GPIO_LOW(PORT_RELAY_CH1, PIN_RELAY_CH1);                  //继电器通道1设置低电平。
    GPIO_LOW(PORT_RELAY_CH2, PIN_RELAY_CH2);                  //继电器通道2设置低电平。
    GPIO_LOW(PORT_RELAY_CH3, PIN_RELAY_CH3);                  //继电器通道3设置低电平。
    GPIO_LOW(PORT_RELAY_CH4, PIN_RELAY_CH4);                  //继电器通道4设置低电平。
}

void Relay_open_Close_Config(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins,uint8_t EffectiVelevel)
{
  if(EffectiVelevel==0)
  {
    GPIO_WriteLow(GPIOx,PortPins);              //库函数：输出低电平
  }
  else if(EffectiVelevel==1)
  {
    GPIO_WriteHigh(GPIOx,PortPins);    
    //库函数：输出高电平
  }
  else
  {
    return;
  }
}
