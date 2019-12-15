#include "bsp_key.h"
#include "bsp_timer.h"
#include "bsp_wwdg.h"

/*数组封装按键端口与引脚号，方便按键扫描函数引用*/
port_io g_io[4] = {  
	{KEY1_PORT,KEY1_PIN,KEY1_VALUE},
	{KEY2_PORT,KEY2_PIN,KEY2_VALUE},	
	{KEY3_PORT,KEY3_PIN,KEY3_VALUE}, //实际是3
	{KEY4_PORT,KEY4_PIN,KEY4_VALUE}, //这个好像没有用到
};
/*****************************************************************************************************
*程序名：KEY_Init
***功能：配置4个独立按键，工作在上拉输入模式。
***参数：无参数。
*返回值：无返回值。
***作者：王利均
***时间：2018年4月21号。
*****************************************************************************************************/
void KEY_Init(void)
{
  GPIO_Init(KEY1_PORT,KEY1_PIN, GPIO_MODE_IN_PU_NO_IT);//库函数：配置为无中断上拉输入。
  GPIO_Init(KEY2_PORT,KEY2_PIN, GPIO_MODE_IN_PU_NO_IT);//库函数：配置为无中断上拉输入。
  GPIO_Init(KEY3_PORT,KEY3_PIN, GPIO_MODE_IN_PU_NO_IT);//库函数：配置为无中断上拉输入。
  GPIO_Init(KEY4_PORT,KEY4_PIN, GPIO_MODE_IN_PU_NO_IT);//库函数：配置为无中断上拉输入。
}
/*****************************************************************************************************
*程序名：scan_key
***功能：按键扫描函数，扫描4个按键IO口状态。返回一个键值。
***参数：无参数。
*返回值：返回键值。
***作者：王利均
***时间：2018年4月21号。
*****************************************************************************************************/
uint8_t Key_Scan(void)
{
  uint8_t KeyTemp=0x00;                               //用来做临时按键值。
  if(GPIO_ReadInputPin(KEY1_PORT,KEY1_PIN)==0)        //Key按键1
  { 
    time1_xms(20);
    if(GPIO_ReadInputPin(KEY1_PORT,KEY1_PIN)==0)  
    { 
      KeyTemp=KEY1_VALUE;
      KEY_value_Loading_Data(KeyTemp);
      Uploading_data();                                  //告诉上位机有按键按下。
      while(GPIO_ReadInputPin(KEY1_PORT,KEY1_PIN)==0);  //等待按键释放。
      return KeyTemp;                                   //返回按键键值。
    }
  }
  else if(GPIO_ReadInputPin(KEY2_PORT,KEY2_PIN)==0)   //KEY按键2
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
  else if(GPIO_ReadInputPin(KEY3_PORT,KEY3_PIN)==0)  //KEY按键3
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
  else if(GPIO_ReadInputPin(KEY4_PORT,KEY4_PIN)==0)  //KEY按键4
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

