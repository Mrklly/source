#include "bsp_gpio.h"


void LED_Init(void)
{
    stc_gpio_config_t     stcBacklightPortCfg;

    DDL_ZERO_STRUCT(stcBacklightPortCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    stcBacklightPortCfg.enDir   = GpioDirOut;
    stcBacklightPortCfg.enDrv   = GpioDrvH;
    stcBacklightPortCfg.enPuPd  = GpioNoPuPd;
    stcBacklightPortCfg.enOD    = GpioOdDisable;

    Gpio_Init(BACK_LED1_PORT, BACK_LED1_PIN,  &stcBacklightPortCfg);
    Gpio_Init(BACK_LED2_PORT, BACK_LED2_PIN,  &stcBacklightPortCfg);
    Gpio_Init(BACK_LED3_PORT, BACK_LED3_PIN,  &stcBacklightPortCfg);
    Gpio_Init(BACK_LED4_PORT, BACK_LED4_PIN,  &stcBacklightPortCfg);

    Gpio_Init(TEST_PORT, TEST_PIN,  &stcBacklightPortCfg);

    back_LED_open_Close_Config(0,1);
    back_LED_open_Close_Config(1,1);
    back_LED_open_Close_Config(2,1);
    back_LED_open_Close_Config(3,1);
}

#if(0)
void LED_open_Close_Config(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins,uint8_t EffectiVelevel)
{
  if(EffectiVelevel==0)
  {
    GPIO_WriteLow(GPIOx,PortPins);
  }
  else if(EffectiVelevel==1)
  {
    GPIO_WriteHigh(GPIOx,PortPins);            
  }
  else
  {
    return;
  }
}
#endif

void LED_Toggle(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
  uint8_t state;
  state=Gpio_ReadOutputIO(enPort, enPin);
  if(state==1)
  {
    Gpio_ClrIO(enPort,enPin);
  }
  else
  {
    Gpio_SetIO(enPort,enPin);
  }
}

void Relay_Init(void)
{
    stc_gpio_config_t     stcRelayPortCfg;

    DDL_ZERO_STRUCT(stcRelayPortCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    stcRelayPortCfg.enDir   = GpioDirOut;
    stcRelayPortCfg.enDrv   = GpioDrvH;
    stcRelayPortCfg.enPuPd  = GpioNoPuPd;
    stcRelayPortCfg.enOD    = GpioOdDisable;

    Gpio_Init(GpioPortB, GpioPin6,  &stcRelayPortCfg);
    Gpio_Init(GpioPortB, GpioPin7,  &stcRelayPortCfg);
    Gpio_Init(GpioPortB, GpioPin8,  &stcRelayPortCfg);
    Gpio_Init(GpioPortB, GpioPin9,  &stcRelayPortCfg);

    Relay_open_Close_Config(RELAY_PORT_CH1,RELAY_PIN_CH1,0);
    Relay_open_Close_Config(RELAY_PORT_CH2,RELAY_PIN_CH2,0);
    Relay_open_Close_Config(RELAY_PORT_CH3,RELAY_PIN_CH3,0);
    Relay_open_Close_Config(RELAY_PORT_CH4,RELAY_PIN_CH4,0);
}

void Relay_open_Close_Config(en_gpio_port_t enPort, en_gpio_pin_t enPin,uint8_t EffectiVelevel)
{
  if(EffectiVelevel==0)
  {
    Gpio_ClrIO(enPort,enPin);             
  }
  else if(EffectiVelevel==1)
  {
    Gpio_SetIO(enPort,enPin);             
  }
  else
  {
    return;
  }
}

void back_LED_open_Close_Config( uint8_t ch,uint8_t EffectiVelevel)
{
  if(EffectiVelevel==0)
  {
    switch(ch)
    {
    case 0:
      Gpio_ClrIO(BACK_LED1_PORT,BACK_LED1_PIN);    
      break;
    case 1:
      Gpio_ClrIO(BACK_LED2_PORT,BACK_LED2_PIN);  
      break;
    case 2:
      Gpio_ClrIO(BACK_LED3_PORT,BACK_LED3_PIN);  
      break;
    case 3:
      Gpio_ClrIO(BACK_LED4_PORT,BACK_LED4_PIN);  
      break;
    default:
      break;
    }      
  }
  else if(EffectiVelevel==1)
  {
    switch(ch)
    {
    case 0:
      Gpio_SetIO(BACK_LED1_PORT,BACK_LED1_PIN);    
      break;
    case 1:
      Gpio_SetIO(BACK_LED2_PORT,BACK_LED2_PIN);  
      break;
    case 2:
      Gpio_SetIO(BACK_LED3_PORT,BACK_LED3_PIN);  
      break;
    case 3:
      Gpio_SetIO(BACK_LED4_PORT,BACK_LED4_PIN);  
      break;
    default:
      break;
    }            
  }
  else
  {
    return;
  }
}

void gpio_set(en_gpio_port_t port, en_gpio_pin_t pin, uint8_t status)
{
    if(status) {
        GPIO_HIGH(port, pin);
    } else {
        GPIO_LOW(port, pin);
    }
}

