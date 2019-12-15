#include "bsp_gpio.h"


void LED_Init(void)
{
    stc_gpio_config_t     stcBacklightPortCfg;

    DDL_ZERO_STRUCT(stcBacklightPortCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    stcBacklightPortCfg.enDir   = GpioDirOut;
    stcBacklightPortCfg.enDrv   = GpioDrvL;
    stcBacklightPortCfg.enPuPd  = GpioPu;
    stcBacklightPortCfg.enOD    = GpioOdDisable;

    Gpio_Init(BACK_LED1_PORT, BACK_LED1_PIN,  &stcBacklightPortCfg);
    Gpio_Init(BACK_LED2_PORT, BACK_LED2_PIN,  &stcBacklightPortCfg);
    Gpio_Init(BACK_LED3_PORT, BACK_LED3_PIN,  &stcBacklightPortCfg);
    Gpio_Init(BACK_LED4_PORT, BACK_LED4_PIN,  &stcBacklightPortCfg);
    Gpio_Init(BACK_LED5_PORT, BACK_LED5_PIN,  &stcBacklightPortCfg);
    Gpio_Init(BACK_LED6_PORT, BACK_LED6_PIN,  &stcBacklightPortCfg);

    back_LED_open_Close_Config(1,1);
    back_LED_open_Close_Config(2,1);
    back_LED_open_Close_Config(3,1);
    back_LED_open_Close_Config(4,1);
    back_LED_open_Close_Config(5,1);
    back_LED_open_Close_Config(6,1);
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
    stcRelayPortCfg.enDrv   = GpioDrvL;
    stcRelayPortCfg.enPuPd  = GpioPu;
    stcRelayPortCfg.enOD    = GpioOdDisable;

    Gpio_Init(GpioPortB, GpioPin6,  &stcRelayPortCfg);
    Gpio_Init(GpioPortB, GpioPin7,  &stcRelayPortCfg);
    Gpio_Init(GpioPortB, GpioPin8,  &stcRelayPortCfg);
    Gpio_Init(GpioPortB, GpioPin9,  &stcRelayPortCfg);

    Relay_open_Close_Config(CH1_PORT,CH1_PIN,0);
    Relay_open_Close_Config(CH2_PORT,CH2_PIN,0);
    Relay_open_Close_Config(CH3_PORT,CH3_PIN,0);
    Relay_open_Close_Config(CH4_PORT,CH4_PIN,0);
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
    case 1:
      Gpio_ClrIO(BACK_LED1_PORT,BACK_LED1_PIN);
      break;
    case 2:
      Gpio_ClrIO(BACK_LED2_PORT,BACK_LED2_PIN);
      break;
    case 3:
      Gpio_ClrIO(BACK_LED3_PORT,BACK_LED3_PIN);
      break;
    case 4:
      Gpio_ClrIO(BACK_LED4_PORT,BACK_LED4_PIN);
      break;
    case 5:
      Gpio_ClrIO(BACK_LED5_PORT,BACK_LED5_PIN);
      break;
    case 6:
      Gpio_ClrIO(BACK_LED6_PORT,BACK_LED6_PIN);
      break;
    default:
      break;
    }
  }
  else if(EffectiVelevel==1)
  {
    switch(ch)
    {
    case 1:
      Gpio_SetIO(BACK_LED1_PORT,BACK_LED1_PIN);
      break;
    case 2:
      Gpio_SetIO(BACK_LED2_PORT,BACK_LED2_PIN);
      break;
    case 3:
      Gpio_SetIO(BACK_LED3_PORT,BACK_LED3_PIN);
      break;
    case 4:
      Gpio_SetIO(BACK_LED4_PORT,BACK_LED4_PIN);
      break;
    case 5:
      Gpio_SetIO(BACK_LED5_PORT,BACK_LED5_PIN);
      break;
    case 6:
      Gpio_SetIO(BACK_LED6_PORT,BACK_LED6_PIN);
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

