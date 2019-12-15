#include "bsp_pwm.h"
#include "bsp_timer.h"
#include "global_variable.h"


/*****************************************************************************************************
*程序名：PWM_Init
***功能：PWM初始化
***参数：无参数。
*返回值：无返回值。
***作者：王利均
***时间：2018年7月11号。
*****************************************************************************************************/
void PWM_Init(void)
{
  TIM1->PSCRH =0;
  TIM1->PSCRL =1;                               //16分频，定时器时钟等于系统时钟的16m分之一=1M   
  
  /*自动重装寄存器*/
  TIM1->ARRH=(unsigned char)((uint16_t)8000>>8);
  TIM1->ARRL=(unsigned char)((uint16_t)8000&0xFF);
  TIM1->CR1=0x00;
  
  /*捕获/比较使能寄存器*/
  TIM1->CCER1|=0x22;                            // 使能通道1和2为输出                                                             
  TIM1->CCER2|=0x22;                            //使能通道3和4为输出。
  
  /*PWM工作模式1*/
  TIM1->CCMR1|=0x60;                            //通道1
  TIM1->CCMR2|=0x60;                            //通道2
  TIM1->CCMR3|=0x60;                            //通道3
  TIM1->CCMR4|=0x60;                            //通道4
  
  SetTIM1_PWM_DutyCycle(8888,1);
  SetTIM1_PWM_DutyCycle(8888,2);
  SetTIM1_PWM_DutyCycle(8888,3);
  SetTIM1_PWM_DutyCycle(8888,4);
  
  PWM_channel( 1,1);
  PWM_channel( 2,1);
  PWM_channel( 3,1);
  PWM_channel( 4,1);
  TIM1->OISR|=0x10;                             //空闲时输出高电平
  TIM1->CR1|=0x01;                              //TIM1 计数器使能                   
  TIM1->BKR|=0x80;                              //使能OC 主输出
}
/*****************************************************************************************************
*程序名：SetTIM1_PWM_Frequency
***功能：设置定时器1PWM的频率。
***参数：参数1：传入频率数值。
*返回值：无返回值。
***作者：王利均
***时间：2018年7月11号。
*****************************************************************************************************/
void SetTIM1_PWM_Frequency(uint16_t TIM1_Period)
{
  TIM1->ARRH = (uint8_t)(TIM1_Period >> 8);
  TIM1->ARRL = (uint8_t)(TIM1_Period);
}
/*****************************************************************************************************
*程序名：SetTIM1_PWM_DutyCycle
***功能：定时器1PWM的占空比控制。
***参数：参数1：占空比高电平数，参数2：通道号。
*返回值：无返回值。
***作者：王利均
***时间：2018年7月12号。
*****************************************************************************************************/
void SetTIM1_PWM_DutyCycle( uint16_t TIM1_Pulse,uint8_t ChannelNumber)
{  
  switch(ChannelNumber)
  {
  case 1:
    TIM1->CCR1H = (uint8_t)(TIM1_Pulse >> 8);
    TIM1->CCR1L = (uint8_t)(TIM1_Pulse);
    break;
  case 2:
    TIM1->CCR2H = (uint8_t)(TIM1_Pulse >> 8);
    TIM1->CCR2L = (uint8_t)(TIM1_Pulse);
    break;
  case 4:
    TIM1->CCR3H = (uint8_t)(TIM1_Pulse >> 8);
    TIM1->CCR3L = (uint8_t)(TIM1_Pulse);
    break;
  case 3:
    TIM1->CCR4H = (uint8_t)(TIM1_Pulse >> 8);
    TIM1->CCR4L = (uint8_t)(TIM1_Pulse);
    break;
  default:
    break;
  }   
}
/*****************************************************************************************************
*程序名：PWM_Contorl_LED
***功能：通过PWM控制LED的变亮与变暗。
***参数：参数1：通道号。参数2：开关状态。
*返回值：无返回值。
***作者：王利均
***时间：2018年7月12号。
*****************************************************************************************************/
# if(0)
void PWM_Contorl_LED(uint8_t ch,uint8_t ch_state)
{
  u16 Duty_Val=0;
  if(ch_state==1)
  { 
    for(Duty_Val=2000;Duty_Val>0;Duty_Val--)          //控制着由暗到亮的总共时间。
    {
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*4),1);     //修改通道1占空比。既然设置捕获比较的寄存器的值。
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*4),2);     //修改通道1占空比。既然设置捕获比较的寄存器的值。
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*4),3);     //修改通道1占空比。既然设置捕获比较的寄存器的值。
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*4),4);     //修改通道1占空比。既然设置捕获比较的寄存器的值。
      time2_xms(1);                                   //延时函数控制的是呼吸的速度。也就是快慢。
    }
  }
  if(ch_state==0)                                     //控制由亮到暗。
  {
    for(Duty_Val=0;Duty_Val<=1000;Duty_Val++)
    {
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*8),1);     //修改通道1占空比。既然设置捕获比较的寄存器的值。
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*8),2);     //修改通道1占空比。既然设置捕获比较的寄存器的值。
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*8),3);     //修改通道1占空比。既然设置捕获比较的寄存器的值。
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*8),4);     //修改通道1占空比。既然设置捕获比较的寄存器的值。
      time2_xms(1);  
    }
  }
}
#endif

#define PWM_DEAD_ZONE   888
uint16_t pwm[4] = {(8888-PWM_DEAD_ZONE), (8888-PWM_DEAD_ZONE), (8888-PWM_DEAD_ZONE), (8888-PWM_DEAD_ZONE)};
uint16_t pwm_set[4];

void PWM_Contorl_LED(uint8_t ch,uint8_t ch_state)
{
    if(ch_state) {

        if(!pwm_set[ch-1]) {

            pwm_set[ch-1] = 1;
            pwm[ch-1] = count[ch-1] << 3;
            if(pwm[ch-1] >= 8000) {
                pwm[ch-1] = 8000;
            }
        }

        if(pwm[ch-1] >= 6500) {

            pwm[ch-1] -= 2;
            count[ch-1] = pwm[ch-1] >> 3;

        } else if(pwm[ch-1] >= 4000) {

            pwm[ch-1] -= 5;
            count[ch-1] = pwm[ch-1] >> 3;


        } else if(pwm[ch-1] >= 10) {

            pwm[ch-1] -= 10;
            count[ch-1] = pwm[ch-1] >> 3;

        } else if(pwm[ch-1] >= 1) {

            pwm[ch-1] --;
            count[ch-1] = pwm[ch-1] >> 3;

        }else {
            pwm[ch-1] = 0;
            pwm_set[ch-1] = 0;
            pwm_flag[ch-1] = 0x02;
            count[ch-1] = 0;
            return ;
        }
    } else {                                      //控制灯灭。
        pwm_set[ch-1] = 0;
        if(count[ch-1] < 1111) {

            count[ch-1]++;
            pwm[ch-1] = count[ch-1] << 3;

        } else {

            pwm_flag[ch-1] = 0x02;
            return ;
        }
    }
    SetTIM1_PWM_DutyCycle(pwm[ch-1], ch);
}
/*****************************************************************************************************
*程序名：PWM_channel
***功能：设置通道的开关状态。
***参数：参数1：通道号。参数2：开关状态。
*返回值：无返回值。
***作者：王利均
***时间：2018年7月12号。
*****************************************************************************************************/
void PWM_channel(uint8_t ch,uint8_t ch_state)
{
  if(ch_state)
  {
    switch(ch)
    {
    case 1:
      TIM1->CCER1|=0x03;
      break;
    case 2:
      TIM1->CCER1|=0x30;
      break;
    case 4:
      TIM1->CCER2|=0x03;
      break;
    case 3:
      TIM1->CCER2|=0x30;
      break;
    default:
      break;
    }
  }
  else
  {
    switch(ch)
    {
    case 1:
      TIM1->CCER1&=0xfd;
      break;
    case 2:
      TIM1->CCER1&=0xdf;
      break;
    case 3:
      TIM1->CCER2&=0xfd;
      break;
    case 4:
      TIM1->CCER2&=0xdf;
      break;
    default:
      break;
    }
  }
}
/********************************************PWM.C*************************************************/