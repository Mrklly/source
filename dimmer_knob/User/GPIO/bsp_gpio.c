#include "bsp_gpio.h"  
extern uint8_t key_led_state[6];                                //LED??????
extern uint8_t pwm_flag[6];
/****************************************************************************************************
程序名：LED_Init
功能：完成LED1-LED4初始化，默认状态输出0，LED不打开。
参数：无参数。
返回值：无返回值。
*****************************************************************************************************/
void LED_Init(void)
{
  stc_gpio_config_t     stcBacklightPortCfg;
  
  DDL_ZERO_STRUCT(stcBacklightPortCfg);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);           
  stcBacklightPortCfg.enDir   = GpioDirOut;                           //输出。
  stcBacklightPortCfg.enDrv   = GpioDrvH;                             //高驱动能力。
  stcBacklightPortCfg.enPuPd  = GpioPu;                               //上拉。
  stcBacklightPortCfg.enOD    = GpioOdDisable;                        //开漏关闭。
  
  Gpio_Init(BACK_LED1_PORT, BACK_LED1_PIN,  &stcBacklightPortCfg);              //1         
  Gpio_Init(BACK_LED2_PORT, BACK_LED2_PIN,  &stcBacklightPortCfg);              //2
  Gpio_Init(BACK_LED3_PORT, BACK_LED3_PIN,  &stcBacklightPortCfg);              //3
  Gpio_Init(BACK_LED4_PORT, BACK_LED4_PIN,  &stcBacklightPortCfg);              //4
  Gpio_Init(BACK_LED5_PORT, BACK_LED5_PIN,  &stcBacklightPortCfg);              //4
  Gpio_Init(BACK_LED6_PORT, BACK_LED6_PIN,  &stcBacklightPortCfg);              //4
  
  back_LED_open_Close_Config(1,1);
  back_LED_open_Close_Config(2,1);
  back_LED_open_Close_Config(3,1);
  back_LED_open_Close_Config(4,1);
  back_LED_open_Close_Config(5,1);
  back_LED_open_Close_Config(6,1);
}
/*****************************************************************************************************
程序名：LED_open_Close_Config
功能：根据参数实现打开或者关闭LED灯。
参数：参数1：端口号，参数2：引脚号，参数3：有效电平，是低有效还是高有效（0/1参数）
返回值：无返回值。
*****************************************************************************************************/
#if(0)
void LED_open_Close_Config(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins,uint8_t EffectiVelevel)
{
  if(EffectiVelevel==0)
  {
    GPIO_WriteLow(GPIOx,PortPins);              //库函数：输出低电平
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
void LED_open_Close_Config(uint8_t channel,uint8_t EffectiVelevel)
{
  if(key_led_state[channel]!=EffectiVelevel)
  {
    key_led_state[channel]=EffectiVelevel;
    pwm_flag[channel]=0x01;
  }
}
/****************************************************************************************************
程序名：LED_Toggle
功能：LED灯电平翻转
参数：参数1：端口号，参数2：端口引脚。
返回值：无返回值。
*****************************************************************************************************/
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
/*****************************************************************************************************
程序名：Relay_Init
功能：实现继电器CH1-CH4初始化，默认输出状态0,继电器不打开。
参数：无参数。
返回值：无返回值。
*****************************************************************************************************/
void Relay_Init(void)
{
  stc_gpio_config_t     stcRelayPortCfg;
  
  DDL_ZERO_STRUCT(stcRelayPortCfg);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);           
  stcRelayPortCfg.enDir   = GpioDirOut;                           //输入。
  stcRelayPortCfg.enDrv   = GpioDrvL;                             //高驱动能力。
  stcRelayPortCfg.enPuPd  = GpioPu;                               //上拉。
  stcRelayPortCfg.enOD    = GpioOdDisable;                        //开漏关闭。
  
  Gpio_Init(GpioPortB, GpioPin6,  &stcRelayPortCfg);              //1         
//  Gpio_Init(GpioPortB, GpioPin7,  &stcRelayPortCfg);              //2
  Gpio_Init(GpioPortB, GpioPin8,  &stcRelayPortCfg);              //3
  Gpio_Init(GpioPortB, GpioPin9,  &stcRelayPortCfg);              //4
  
  Relay_open_Close_Config(CH1_PORT,CH1_PIN,0);                    //继电器通道1设置低电平。
//  Relay_open_Close_Config(CH2_PORT,CH2_PIN,0);                    //继电器通道2设置低电平。
  Relay_open_Close_Config(CH3_PORT,CH3_PIN,0);                    //继电器通道3设置低电平。
  Relay_open_Close_Config(CH4_PORT,CH4_PIN,0);                    //继电器通道4设置低电平。
}
/*****************************************************************************************************
程序名：Relay_open_Close_Config
功能：根据参数实现继电器打开关闭。
参数：参数1：端口号，参数2：引脚号，参数3：有效电平，是低有效还是高有效（0/1参数）。
返回值：无返回值。
*****************************************************************************************************/
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
/*******************************************END-GPIO.C***********************************************/





