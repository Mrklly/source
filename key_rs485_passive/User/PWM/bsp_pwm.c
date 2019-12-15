#include "bsp_pwm.h"
#include "bsp_timer.h"
#include "global_variable.h"


void pwm_init(void)
{
    TIM1->PSCRH =0;
    TIM1->PSCRL =1;

    TIM1->ARRH=(uint8_t)(8000 >> 8);
    TIM1->ARRL=(uint8_t)(8000 & 0xFF);
    TIM1->CR1=0x00;

    TIM1->CCER1 |= 0x22;                            // enable channel 1 2
    TIM1->CCER2 |= 0x22;                            // enable channel 3 4

    TIM1->CCMR1 |= 0x60;
    TIM1->CCMR2 |= 0x60;
    TIM1->CCMR3 |= 0x60;
    TIM1->CCMR4 |= 0x60;

    TIM1->CCR1H = (uint8_t)(8888 >> 8);         // set pwm duty
    TIM1->CCR1L = (uint8_t)8888;
    TIM1->CCR2H = (uint8_t)(8888 >> 8);
    TIM1->CCR2L = (uint8_t)8888;
    TIM1->CCR3H = (uint8_t)(8888 >> 8);
    TIM1->CCR3L = (uint8_t)8888;
    TIM1->CCR4H = (uint8_t)(8888 >> 8);
    TIM1->CCR4L = (uint8_t)8888;

    TIM1->CCER1 |= 0x03;  // enable
    TIM1->CCER1 |= 0x30;
    TIM1->CCER2 |= 0x03;
    TIM1->CCER2 |= 0x30;
//    TIM1->CCER1&=0xfd;    // disable
//    TIM1->CCER1&=0xdf;
//    TIM1->CCER2&=0xfd;
//    TIM1->CCER2&=0xdf;

    TIM1->OISR |= 0x10;                             //空闲时输出高电平
    TIM1->CR1 |= 0x01;                              //TIM1 计数器使能                   
    TIM1->BKR |= 0x80;                              //使能OC 主输出
}

void SetTIM1_PWM_Frequency(uint16_t TIM1_Period)
{
  TIM1->ARRH = (uint8_t)(TIM1_Period >> 8);
  TIM1->ARRL = (uint8_t)(TIM1_Period);
}

void SetTIM1_PWM_DutyCycle( uint16_t TIM1_Pulse,uint8_t ChannelNumber)
{  
  switch(ChannelNumber)
  {
  case 0:
    TIM1->CCR1H = (uint8_t)(TIM1_Pulse >> 8);
    TIM1->CCR1L = (uint8_t)(TIM1_Pulse);
    break;
  case 1:
    TIM1->CCR2H = (uint8_t)(TIM1_Pulse >> 8);
    TIM1->CCR2L = (uint8_t)(TIM1_Pulse);
    break;
  case 3:
    TIM1->CCR3H = (uint8_t)(TIM1_Pulse >> 8);
    TIM1->CCR3L = (uint8_t)(TIM1_Pulse);
    break;
  case 2:
    TIM1->CCR4H = (uint8_t)(TIM1_Pulse >> 8);
    TIM1->CCR4L = (uint8_t)(TIM1_Pulse);
    break;
  default:
    break;
  }
}

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
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*8),1);
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*8),2);
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*8),3);
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*8),4);
      time2_xms(1);
    }
  }
}
#endif

#define PWM_DEAD_ZONE   888
uint16_t pwm[4] = {(8888-PWM_DEAD_ZONE), (8888-PWM_DEAD_ZONE), (8888-PWM_DEAD_ZONE), (8888-PWM_DEAD_ZONE)};

void PWM_Contorl_LED(uint8_t ch,uint8_t ch_state)
{
    static uint16_t pwm_set[4];
    if(ch_state) {

        if(!pwm_set[ch]) {

            pwm_set[ch] = 1;
            pwm[ch] = count[ch] << 3;
            if(pwm[ch] >= 8000) {
                pwm[ch] = 8000;
            }
        }

        if(pwm[ch] >= 6500) {

            pwm[ch] -= 2;
            count[ch] = pwm[ch] >> 3;

        } else if(pwm[ch] >= 4000) {

            pwm[ch] -= 5;
            count[ch] = pwm[ch] >> 3;


        } else if(pwm[ch] >= 10) {

            pwm[ch] -= 10;
            count[ch] = pwm[ch] >> 3;

        } else if(pwm[ch] >= 1) {

            pwm[ch] --;
            count[ch] = pwm[ch] >> 3;

        }else {
            pwm[ch] = 0;
            pwm_set[ch] = 0;
            pwm_flag[ch] = 0x02;
            count[ch] = 0;
            return ;
        }
    } else {                                      //控制灯灭。
        pwm_set[ch] = 0;
        if(count[ch] < 1111) {

            count[ch]++;
            pwm[ch] = count[ch] << 3;

        } else {

            pwm_flag[ch] = 0x02;
            return ;
        }
    }
//    SetTIM1_PWM_DutyCycle(pwm[ch], ch);
}

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
