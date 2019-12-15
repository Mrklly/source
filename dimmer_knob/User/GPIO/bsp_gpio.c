#include "bsp_gpio.h"  
extern uint8_t key_led_state[6];                                //LED??????
extern uint8_t pwm_flag[6];
/****************************************************************************************************
��������LED_Init
���ܣ����LED1-LED4��ʼ����Ĭ��״̬���0��LED���򿪡�
�������޲�����
����ֵ���޷���ֵ��
*****************************************************************************************************/
void LED_Init(void)
{
  stc_gpio_config_t     stcBacklightPortCfg;
  
  DDL_ZERO_STRUCT(stcBacklightPortCfg);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);           
  stcBacklightPortCfg.enDir   = GpioDirOut;                           //�����
  stcBacklightPortCfg.enDrv   = GpioDrvH;                             //������������
  stcBacklightPortCfg.enPuPd  = GpioPu;                               //������
  stcBacklightPortCfg.enOD    = GpioOdDisable;                        //��©�رա�
  
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
��������LED_open_Close_Config
���ܣ����ݲ���ʵ�ִ򿪻��߹ر�LED�ơ�
����������1���˿ںţ�����2�����źţ�����3����Ч��ƽ���ǵ���Ч���Ǹ���Ч��0/1������
����ֵ���޷���ֵ��
*****************************************************************************************************/
#if(0)
void LED_open_Close_Config(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins,uint8_t EffectiVelevel)
{
  if(EffectiVelevel==0)
  {
    GPIO_WriteLow(GPIOx,PortPins);              //�⺯��������͵�ƽ
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
��������LED_Toggle
���ܣ�LED�Ƶ�ƽ��ת
����������1���˿ںţ�����2���˿����š�
����ֵ���޷���ֵ��
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
��������Relay_Init
���ܣ�ʵ�̵ּ���CH1-CH4��ʼ����Ĭ�����״̬0,�̵������򿪡�
�������޲�����
����ֵ���޷���ֵ��
*****************************************************************************************************/
void Relay_Init(void)
{
  stc_gpio_config_t     stcRelayPortCfg;
  
  DDL_ZERO_STRUCT(stcRelayPortCfg);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);           
  stcRelayPortCfg.enDir   = GpioDirOut;                           //���롣
  stcRelayPortCfg.enDrv   = GpioDrvL;                             //������������
  stcRelayPortCfg.enPuPd  = GpioPu;                               //������
  stcRelayPortCfg.enOD    = GpioOdDisable;                        //��©�رա�
  
  Gpio_Init(GpioPortB, GpioPin6,  &stcRelayPortCfg);              //1         
//  Gpio_Init(GpioPortB, GpioPin7,  &stcRelayPortCfg);              //2
  Gpio_Init(GpioPortB, GpioPin8,  &stcRelayPortCfg);              //3
  Gpio_Init(GpioPortB, GpioPin9,  &stcRelayPortCfg);              //4
  
  Relay_open_Close_Config(CH1_PORT,CH1_PIN,0);                    //�̵���ͨ��1���õ͵�ƽ��
//  Relay_open_Close_Config(CH2_PORT,CH2_PIN,0);                    //�̵���ͨ��2���õ͵�ƽ��
  Relay_open_Close_Config(CH3_PORT,CH3_PIN,0);                    //�̵���ͨ��3���õ͵�ƽ��
  Relay_open_Close_Config(CH4_PORT,CH4_PIN,0);                    //�̵���ͨ��4���õ͵�ƽ��
}
/*****************************************************************************************************
��������Relay_open_Close_Config
���ܣ����ݲ���ʵ�̵ּ����򿪹رա�
����������1���˿ںţ�����2�����źţ�����3����Ч��ƽ���ǵ���Ч���Ǹ���Ч��0/1��������
����ֵ���޷���ֵ��
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





