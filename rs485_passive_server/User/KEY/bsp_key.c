#include "bsp_key.h"
#include "bsp_timer.h"
#include "bsp_wwdg.h"

/*�����װ�����˿������źţ����㰴��ɨ�躯������*/
port_io g_io[4] = {  
	{KEY1_PORT,KEY1_PIN,KEY1_VALUE},
	{KEY2_PORT,KEY2_PIN,KEY2_VALUE},	
	{KEY3_PORT,KEY3_PIN,KEY3_VALUE}, //ʵ����3
	{KEY4_PORT,KEY4_PIN,KEY4_VALUE}, //�������û���õ�
};
/*****************************************************************************************************
*��������KEY_Init
***���ܣ�����4��������������������������ģʽ��
***�������޲�����
*����ֵ���޷���ֵ��
***���ߣ�������
***ʱ�䣺2018��4��21�š�
*****************************************************************************************************/
void KEY_Init(void)
{
  GPIO_Init(KEY1_PORT,KEY1_PIN, GPIO_MODE_IN_PU_NO_IT);//�⺯��������Ϊ���ж��������롣
  GPIO_Init(KEY2_PORT,KEY2_PIN, GPIO_MODE_IN_PU_NO_IT);//�⺯��������Ϊ���ж��������롣
  GPIO_Init(KEY3_PORT,KEY3_PIN, GPIO_MODE_IN_PU_NO_IT);//�⺯��������Ϊ���ж��������롣
  GPIO_Init(KEY4_PORT,KEY4_PIN, GPIO_MODE_IN_PU_NO_IT);//�⺯��������Ϊ���ж��������롣
}
/*****************************************************************************************************
*��������scan_key
***���ܣ�����ɨ�躯����ɨ��4������IO��״̬������һ����ֵ��
***�������޲�����
*����ֵ�����ؼ�ֵ��
***���ߣ�������
***ʱ�䣺2018��4��21�š�
*****************************************************************************************************/
uint8_t Key_Scan(void)
{
  uint8_t KeyTemp=0x00;                               //��������ʱ����ֵ��
  if(GPIO_ReadInputPin(KEY1_PORT,KEY1_PIN)==0)        //Key����1
  { 
    time1_xms(20);
    if(GPIO_ReadInputPin(KEY1_PORT,KEY1_PIN)==0)  
    { 
      KeyTemp=KEY1_VALUE;
      KEY_value_Loading_Data(KeyTemp);
      Uploading_data();                                  //������λ���а������¡�
      while(GPIO_ReadInputPin(KEY1_PORT,KEY1_PIN)==0);  //�ȴ������ͷš�
      return KeyTemp;                                   //���ذ�����ֵ��
    }
  }
  else if(GPIO_ReadInputPin(KEY2_PORT,KEY2_PIN)==0)   //KEY����2
  {
    time1_xms(20);
    if(GPIO_ReadInputPin(KEY2_PORT,KEY2_PIN)==0)
    {
      KeyTemp=KEY2_VALUE;
      KEY_value_Loading_Data(KeyTemp);
      Uploading_data();                                 
      while(GPIO_ReadInputPin(KEY2_PORT,KEY2_PIN)==0);
      return KeyTemp;
    }
  }
  else if(GPIO_ReadInputPin(KEY3_PORT,KEY3_PIN)==0)  //KEY����3
  {
    time1_xms(20);
    if(GPIO_ReadInputPin(KEY3_PORT,KEY3_PIN)==0)
    {
      KeyTemp=KEY3_VALUE;
      KEY_value_Loading_Data(KeyTemp);
      Uploading_data();                                 
      while(GPIO_ReadInputPin(KEY3_PORT,KEY3_PIN)==0); 
      return KeyTemp;
    }
  }
  else if(GPIO_ReadInputPin(KEY4_PORT,KEY4_PIN)==0)  //KEY����4
  {
    time1_xms(20);
    if(GPIO_ReadInputPin(KEY4_PORT,KEY4_PIN)==0)
    {
      KeyTemp=KEY4_VALUE;
      KEY_value_Loading_Data(KeyTemp);
      Uploading_data();                                
      while(GPIO_ReadInputPin(KEY4_PORT,KEY4_PIN)==0); 
      return KeyTemp;
    }
  }
  else
  {
    KeyTemp=NO_KEY;
  }
  return KeyTemp;
}

