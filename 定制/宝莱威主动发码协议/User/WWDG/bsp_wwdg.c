#include "bsp_wwdg.h"
/*****************************************************************************************************
*程序名：WWDG_Configuration
***功能：看门狗配置。
***参数：无参数。
*返回值：无返回值。
***作者：王利均
***时间：2018年5月9号。
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
*程序名：WWDG_Configuration
***功能：看门狗复位。
***参数：无参数。
*返回值：无返回值。
***作者：王利均
***时间：2018年5月9号。
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
*程序名：Refresh_WWDG_Window
***功能：看门狗喂狗函数
***参数：无参数。
*返回值：无返回值。
***作者：王利均
***时间：2018年5月9号。
*****************************************************************************************************/
void Refresh_WWDG_Window(void)//喂狗
{
  u8 CounterValue;
  CounterValue = (u8)(WWDG_GetCounter() & 0x7F);
  
  if(CounterValue < window)
  {
    WWDG_SetCounter(CounterInit);
  }
}
/*******************************************END-WWDG.C***********************************************/