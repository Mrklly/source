#include "bsp_wwdg.h"
/*****************************************************************************************************
*��������WWDG_Configuration
***���ܣ����Ź����á�
***�������޲�����
*����ֵ���޷���ֵ��
***���ߣ�������
***ʱ�䣺2018��5��9�š�
*****************************************************************************************************/
void WWDG_Configuration(void) 
{
  /* WWDG Configuration */
  /* Watchdog Window= 0x7F step to 0x3F step
  = (0x7F-0x3F) * 1 step
  = 64 * 1 step
  = 64 * (12288/2Mhz)
  = 393,216ms
  */
  /* Allowed Window = (0x7F-window) * 1 step
  = (0x7F-0x77) * 1 step
  = 7 * 1 step 
  = 7 * (12288/2Mhz) 
  = 43.008ms 
  */
  /* So the non allowed window starts from 0.0ms to 43.008ms
  and the alowed window starts from 43.008ms to 393,216ms */
  WWDG_Init(CounterInit,window);
}
/*****************************************************************************************************
*��������WWDG_Configuration
***���ܣ����Ź���λ��
***�������޲�����
*����ֵ���޷���ֵ��
***���ߣ�������
***ʱ�䣺2018��5��9�š�
*****************************************************************************************************/
void WWDGReset(void)
{
  FlagStatus WwdgFlag; 
  /*Get WWDG Reset Status */ 
  WwdgFlag = RST_GetFlagStatus(RST_FLAG_WWDGF);
  /* Test if a WWDG Reset has occured */
  if (WwdgFlag)
  {
    /* Clear IWDGF Flag */
    RST_ClearFlag(RST_FLAG_WWDGF); 
  }
}
/*****************************************************************************************************
*��������Refresh_WWDG_Window
***���ܣ����Ź�ι������
***�������޲�����
*����ֵ���޷���ֵ��
***���ߣ�������
***ʱ�䣺2018��5��9�š�
*****************************************************************************************************/
void Refresh_WWDG_Window(void)//ι��
{
  u8 CounterValue;
  CounterValue = (u8)(WWDG_GetCounter() & 0x7F);
  
  if(CounterValue < window)
  {
    WWDG_SetCounter(CounterInit);
  }
}
/*******************************************END-WWDG.C***********************************************/