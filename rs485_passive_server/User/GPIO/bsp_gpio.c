#include "bsp_gpio.h"
/*****************************************************************************************************
*��������LED_Init
***���ܣ����LED1-LED4��ʼ����Ĭ��״̬���0��LED���򿪡�
***�������޲�����
*����ֵ���޷���ֵ��
***���ߣ�������
***ʱ�䣺2018��4��21�š�
*****************************************************************************************************/
void LED_Init(void)
{
  GPIO_Init(LED1_PORT, LED1_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(LED2_PORT, LED2_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(LED3_PORT, LED3_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(LED4_PORT, LED4_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(PORT_TEST, PIN_TEST, GPIO_MODE_OUT_PP_HIGH_FAST);
  LED_open_Close_Config(LED1_PORT,LED1_PIN,0);  //LED1���0
  LED_open_Close_Config(LED2_PORT,LED2_PIN,0);  //LED2���0
  LED_open_Close_Config(LED3_PORT,LED3_PIN,0);  //LED3���0
  LED_open_Close_Config(LED4_PORT,LED4_PIN,0);  //LED4���0
}
/*****************************************************************************************************
*��������LED_open_Close_Config
***���ܣ����ݲ���ʵ�ִ򿪻��߹ر�LED�ơ�
***����������1���˿ںţ�����2�����źţ�����3����Ч��ƽ���ǵ���Ч���Ǹ���Ч��0/1������
*����ֵ���޷���ֵ��
***���ߣ�������
***ʱ�䣺2018��4��21�š�
*****************************************************************************************************/
void LED_open_Close_Config(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins,uint8_t EffectiVelevel)
{
  if(EffectiVelevel==0)
  {
    GPIO_WriteLow(GPIOx,PortPins);              //�⺯��������͵�ƽ
  }
  else if(EffectiVelevel==1)
  {
    GPIO_WriteHigh(GPIOx,PortPins);            //�⺯��������ߵ�ƽ
  }
  else
  {
    return;
  }
}
/*****************************************************************************************************
*��������LED_Toggle
***���ܣ�LED�Ƶ�ƽ��ת
***����������1���˿ںţ�����2���˿����š�
*����ֵ���޷���ֵ��
***���ߣ�������
***ʱ�䣺2018��4��21�š�
*****************************************************************************************************/
void LED_Toggle(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins)
{
  GPIO_WriteReverse(GPIOx,PortPins);            //�⺯��ʵ�ֵ�ƽ��ת��
}
/*****************************************************************************************************
*��������Relay_Init
***���ܣ�ʵ�̵ּ���CH1-CH4��ʼ����Ĭ�����״̬0,�̵������򿪡�
***�������޲�����
*����ֵ���޷���ֵ��
***���ߣ�������
***ʱ�䣺2018��4��23�š�
*****************************************************************************************************/
void Relay_Init(void)
{
  GPIO_Init(CH1_PORT,CH1_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);      //�̵���ͨ��1��
  GPIO_Init(CH2_PORT,CH2_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);      //�̵���ͨ��2��
  GPIO_Init(CH3_PORT,CH3_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);      //�̵���ͨ��3��
  GPIO_Init(CH4_PORT,CH4_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);      //�̵���ͨ��4��
  Relay_open_Close_Config(CH1_PORT,CH1_PIN,0);                  //�̵���ͨ��1���õ͵�ƽ��
  Relay_open_Close_Config(CH2_PORT,CH2_PIN,0);                  //�̵���ͨ��2���õ͵�ƽ��
  Relay_open_Close_Config(CH3_PORT,CH3_PIN,0);                  //�̵���ͨ��3���õ͵�ƽ��
  Relay_open_Close_Config(CH4_PORT,CH4_PIN,0);                  //�̵���ͨ��4���õ͵�ƽ��
}
/*****************************************************************************************************
*��������Relay_open_Close_Config
***���ܣ����ݲ���ʵ�̵ּ����򿪹رա�
***����������1���˿ںţ�����2�����źţ�����3����Ч��ƽ���ǵ���Ч���Ǹ���Ч��0/1��������
*����ֵ���޷���ֵ��
***���ߣ�������
***ʱ�䣺2018��4��23�š�
*****************************************************************************************************/
void Relay_open_Close_Config(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins,uint8_t EffectiVelevel)
{
    if(EffectiVelevel==0)
  {
    GPIO_WriteLow(GPIOx,PortPins);              //�⺯��������͵�ƽ
  }
  else if(EffectiVelevel==1)
  {
    GPIO_WriteHigh(GPIOx,PortPins);            //�⺯��������ߵ�ƽ
  }
  else
  {
    return;
  }
}



