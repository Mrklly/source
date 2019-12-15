#include "bsp_gpio.h"

void LED_Init(void)
{
  GPIO_Init(LED1_PORT, LED1_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(LED2_PORT, LED2_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(LED3_PORT, LED3_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(LED4_PORT, LED4_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_LOW(LED1_PORT, LED1_PIN);  //LED1输出0
  GPIO_LOW(LED2_PORT, LED2_PIN);  //LED2输出0
  GPIO_LOW(LED3_PORT, LED3_PIN);  //LED3输出0
  GPIO_LOW(LED4_PORT, LED4_PIN);  //LED4输出0
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
  GPIO_Init(CH1_PORT,CH1_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);      //继电器通道1。
  GPIO_Init(CH2_PORT,CH2_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);      //继电器通道2。
  GPIO_Init(CH3_PORT,CH3_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);      //继电器通道3。
  GPIO_Init(CH4_PORT,CH4_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);      //继电器通道4。
  GPIO_LOW(CH1_PORT, CH1_PIN);                  //继电器通道1设置低电平。
  GPIO_LOW(CH2_PORT, CH2_PIN);                  //继电器通道2设置低电平。
  GPIO_LOW(CH3_PORT, CH3_PIN);                  //继电器通道3设置低电平。
  GPIO_LOW(CH4_PORT, CH4_PIN);                  //继电器通道4设置低电平。
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
