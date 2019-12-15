#include "bsp_pwm.h"
#include "bsp_timer.h"
#include "global_variable.h"


/*****************************************************************************************************
*��������PWM_Init
***���ܣ�PWM��ʼ��
***�������޲�����
*����ֵ���޷���ֵ��
***���ߣ�������
***ʱ�䣺2018��7��11�š�
*****************************************************************************************************/
void PWM_Init(void)
{
  TIM1->PSCRH =0;
  TIM1->PSCRL =1;                               //16��Ƶ����ʱ��ʱ�ӵ���ϵͳʱ�ӵ�16m��֮һ=1M   
  
  /*�Զ���װ�Ĵ���*/
  TIM1->ARRH=(unsigned char)((uint16_t)8000>>8);
  TIM1->ARRL=(unsigned char)((uint16_t)8000&0xFF);
  TIM1->CR1=0x00;
  
  /*����/�Ƚ�ʹ�ܼĴ���*/
  TIM1->CCER1|=0x22;                            // ʹ��ͨ��1��2Ϊ���                                                             
  TIM1->CCER2|=0x22;                            //ʹ��ͨ��3��4Ϊ�����
  
  /*PWM����ģʽ1*/
  TIM1->CCMR1|=0x60;                            //ͨ��1
  TIM1->CCMR2|=0x60;                            //ͨ��2
  TIM1->CCMR3|=0x60;                            //ͨ��3
  TIM1->CCMR4|=0x60;                            //ͨ��4
  
  SetTIM1_PWM_DutyCycle(8888,1);
  SetTIM1_PWM_DutyCycle(8888,2);
  SetTIM1_PWM_DutyCycle(8888,3);
  SetTIM1_PWM_DutyCycle(8888,4);
  
  PWM_channel( 1,1);
  PWM_channel( 2,1);
  PWM_channel( 3,1);
  PWM_channel( 4,1);
  TIM1->OISR|=0x10;                             //����ʱ����ߵ�ƽ
  TIM1->CR1|=0x01;                              //TIM1 ������ʹ��                   
  TIM1->BKR|=0x80;                              //ʹ��OC �����
}
/*****************************************************************************************************
*��������SetTIM1_PWM_Frequency
***���ܣ����ö�ʱ��1PWM��Ƶ�ʡ�
***����������1������Ƶ����ֵ��
*����ֵ���޷���ֵ��
***���ߣ�������
***ʱ�䣺2018��7��11�š�
*****************************************************************************************************/
void SetTIM1_PWM_Frequency(uint16_t TIM1_Period)
{
  TIM1->ARRH = (uint8_t)(TIM1_Period >> 8);
  TIM1->ARRL = (uint8_t)(TIM1_Period);
}
/*****************************************************************************************************
*��������SetTIM1_PWM_DutyCycle
***���ܣ���ʱ��1PWM��ռ�ձȿ��ơ�
***����������1��ռ�ձȸߵ�ƽ��������2��ͨ���š�
*����ֵ���޷���ֵ��
***���ߣ�������
***ʱ�䣺2018��7��12�š�
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
*��������PWM_Contorl_LED
***���ܣ�ͨ��PWM����LED�ı�����䰵��
***����������1��ͨ���š�����2������״̬��
*����ֵ���޷���ֵ��
***���ߣ�������
***ʱ�䣺2018��7��12�š�
*****************************************************************************************************/
# if(0)
void PWM_Contorl_LED(uint8_t ch,uint8_t ch_state)
{
  u16 Duty_Val=0;
  if(ch_state==1)
  { 
    for(Duty_Val=2000;Duty_Val>0;Duty_Val--)          //�������ɰ��������ܹ�ʱ�䡣
    {
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*4),1);     //�޸�ͨ��1ռ�ձȡ���Ȼ���ò���ȽϵļĴ�����ֵ��
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*4),2);     //�޸�ͨ��1ռ�ձȡ���Ȼ���ò���ȽϵļĴ�����ֵ��
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*4),3);     //�޸�ͨ��1ռ�ձȡ���Ȼ���ò���ȽϵļĴ�����ֵ��
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*4),4);     //�޸�ͨ��1ռ�ձȡ���Ȼ���ò���ȽϵļĴ�����ֵ��
      time2_xms(1);                                   //��ʱ�������Ƶ��Ǻ������ٶȡ�Ҳ���ǿ�����
    }
  }
  if(ch_state==0)                                     //��������������
  {
    for(Duty_Val=0;Duty_Val<=1000;Duty_Val++)
    {
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*8),1);     //�޸�ͨ��1ռ�ձȡ���Ȼ���ò���ȽϵļĴ�����ֵ��
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*8),2);     //�޸�ͨ��1ռ�ձȡ���Ȼ���ò���ȽϵļĴ�����ֵ��
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*8),3);     //�޸�ͨ��1ռ�ձȡ���Ȼ���ò���ȽϵļĴ�����ֵ��
      SetTIM1_PWM_DutyCycle((u16)(Duty_Val*8),4);     //�޸�ͨ��1ռ�ձȡ���Ȼ���ò���ȽϵļĴ�����ֵ��
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
    } else {                                      //���Ƶ���
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
*��������PWM_channel
***���ܣ�����ͨ���Ŀ���״̬��
***����������1��ͨ���š�����2������״̬��
*����ֵ���޷���ֵ��
***���ߣ�������
***ʱ�䣺2018��7��12�š�
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