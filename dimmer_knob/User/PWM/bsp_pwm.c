#include "bsp_pwm.h"
#include "adt.h"

uint16_t count[6]={1800,1800,1800,1800,1800,1800};
extern uint8_t pwm_flag[6];
/*****************************************************************************************************
��������PWM_timer3
���ܣ�PWM��ʼ��
�������޲�����
����ֵ���޷���ֵ��
*****************************************************************************************************/
void PWM_timer3(void)
{
  uint16_t                        u16ArrValue;
  uint16_t                        u16CompareBValue;
  uint16_t                        u16CntValue;

  
  stc_tim3_mode23_config_t        stcTim3BaseCfg;
  stc_gpio_config_t               stcTIM3Port;
  stc_tim3_m23_compare_config_t   stcTim3PortCmpCfg;
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);       //GPIO ����ʱ��ʹ��
  Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE);       //Timer3����ʱ��ʹ��
  
  
  DDL_ZERO_STRUCT(stcTim3BaseCfg);                              //�ṹ���ʼ������
  DDL_ZERO_STRUCT(stcTIM3Port);
  
  stcTIM3Port.enDir  = GpioDirOut;
  
  Gpio_Init(GpioPortA, GpioPin7, &stcTIM3Port);
  Gpio_SetAfMode(GpioPortA,GpioPin7,GpioAf4);                   //PA07����Ϊ2��TIM3_CH0B
  
  Gpio_Init(GpioPortB, GpioPin0, &stcTIM3Port);
  Gpio_SetAfMode(GpioPortB,GpioPin0,GpioAf2);                   //PB00����Ϊ3,TIM3_CH1B
  
  Gpio_Init(GpioPortB, GpioPin1, &stcTIM3Port);
  Gpio_SetAfMode(GpioPortB,GpioPin1,GpioAf3);                   //PB01����Ϊ4,TIM3_CH2B
  Gpio_Init(GpioPortA, GpioPin10, &stcTIM3Port);
  Gpio_SetAfMode(GpioPortA,GpioPin10,GpioAf2);                 //PA10����Ϊ5,TIM3_CH2A
  
  
  stcTim3BaseCfg.enWorkMode    = Tim3WorkMode2;                 //���ǲ�ģʽ
  stcTim3BaseCfg.enCT          = Tim3Timer;                     //��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK
  stcTim3BaseCfg.enPRS         = Tim3PCLKDiv256;                //PCLK
  stcTim3BaseCfg.enCntDir      = Tim3CntUp;                     //���ϼ����������ǲ�ģʽʱֻ��
  stcTim3BaseCfg.enPWMTypeSel  = Tim3IndependentPWM;            //�������PWM
  stcTim3BaseCfg.enPWM2sSel    = Tim3SinglePointCmp;            //����ȽϹ���
  stcTim3BaseCfg.bOneShot      = FALSE;                         //ѭ������
  stcTim3BaseCfg.bURSSel       = FALSE;                         //���������
  
  stcTim3BaseCfg.pfnTim3Cb     = Tim3Int;                       //�жϺ������
  
  Tim3_Mode23_Init(&stcTim3BaseCfg);                            //TIM3 ��ģʽ3���ܳ�ʼ��
  
  Tim3_ClearAllIntFlag();                                       //���жϱ�־
  EnableNvic(TIM3_IRQn, IrqLevel0, TRUE);                       //TIM3�ж�ʹ��
  Tim3_Mode23_EnableIrq(Tim3UevIrq);                            //ʹ��TIM3 UEV�����ж�
  
  u16ArrValue = 1600u;
  Tim3_M23_ARRSet(u16ArrValue, TRUE);                           //��������ֵ,��ʹ�ܻ���
  
  //  u16CompareAValue = 0x8000;                                    
  u16CompareBValue = 1800u;                                   //Խ���Խ����ռ�ձ�ԽС��
  
  //  Tim3_M23_CCR_Set(Tim3CCR0A, u16CompareAValue);              //���ñȽ�ֵA,(PWM����ģʽ��ֻ��Ҫ���ñȽ�ֵA)
  Tim3_M23_CCR_Set(Tim3CCR0B, u16CompareBValue);                //2��
  
  //  Tim3_M23_CCR_Set(Tim3CCR1A, u16CompareAValue);
  Tim3_M23_CCR_Set(Tim3CCR1B, u16CompareBValue);                //3��
  
  //  Tim3_M23_CCR_Set(Tim3CCR2A, u16CompareAValue);
  Tim3_M23_CCR_Set(Tim3CCR2B, u16CompareBValue);                //4��
  Tim3_M23_CCR_Set(Tim3CCR2A, u16CompareBValue);               // 5 ��
  
  stcTim3PortCmpCfg.enCHxACmpCtrl   = Tim3PWMMode2;             //OCREFA�������OCMA:PWMģʽ2
  stcTim3PortCmpCfg.enCHxAPolarity  = Tim3PortPositive;         //�������
  stcTim3PortCmpCfg.bCHxACmpBufEn   = TRUE;                     //Aͨ���������
  stcTim3PortCmpCfg.enCHxACmpIntSel = Tim3CmpIntNone;           //Aͨ���ȽϿ���:��
  
  stcTim3PortCmpCfg.enCHxBCmpCtrl   = Tim3PWMMode2;             //OCREFB�������OCMB:PWMģʽ2(PWM����ģʽ��ҲҪ���ã�����ǿ�����)
  stcTim3PortCmpCfg.enCHxBPolarity  = Tim3PortPositive;         //�������
  stcTim3PortCmpCfg.bCHxBCmpBufEn   = TRUE;                     //Bͨ���������ʹ��
  stcTim3PortCmpCfg.enCHxBCmpIntSel = Tim3CmpIntNone;           //Bͨ���ȽϿ���:��
  
  Tim3_M23_PortOutput_Config(Tim3CH0, &stcTim3PortCmpCfg);      //�Ƚ�����˿�����
  Tim3_M23_PortOutput_Config(Tim3CH1, &stcTim3PortCmpCfg);      //�Ƚ�����˿�����
  Tim3_M23_PortOutput_Config(Tim3CH2, &stcTim3PortCmpCfg);      //�Ƚ�����˿�����
  
  //  u8ValidPeriod = 1;                                            //�¼������������ã�0��ʾ���ǲ�ÿ������ڸ���һ�Σ�ÿ+1�����ӳٰ������
  //  Tim3_M23_SetValidPeriod(u8ValidPeriod);                       //�����������
  
  u16CntValue = 0u;
  
  Tim3_M23_Cnt16Set(u16CntValue);                               //���ü�����ֵ
  
  Tim3_M23_EnPWM_Output(TRUE, FALSE);                           //�˿����ʹ��
  
  Tim3_M23_Run();                                               //���С�
  
}
void PWM_timer4(void)
{
    uint16_t                  u16Period;
    uint16_t                  u16CompareValue;
    stc_adt_basecnt_cfg_t     stcAdtBaseCntCfg;
    stc_adt_CHxX_port_cfg_t   stcAdtTIM4Cfg;
    stc_gpio_config_t         stcTIM4Port;
    DDL_ZERO_STRUCT(stcAdtBaseCntCfg);
    DDL_ZERO_STRUCT(stcAdtTIM4Cfg);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, TRUE);
    stcTIM4Port.enDir = GpioDirOut;
    Gpio_Init(GpioPortA, GpioPin11, &stcTIM4Port);			// K6 backLight PWM
    Gpio_SetAfMode(GpioPortA, GpioPin11, GpioAf7);
    stcAdtBaseCntCfg.enCntMode = AdtSawtoothMode;
    stcAdtBaseCntCfg.enCntDir =  AdtCntUp;
    stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0Div256;
    Adt_Init(AdtTIM4, &stcAdtBaseCntCfg);                      //ADT�ز�������ģʽ��ʱ������
    u16Period = 1600;
    Adt_SetPeriod(AdtTIM4, u16Period);                         //��������
    u16CompareValue = 1800;
    Adt_SetCompareValue(AdtTIM4, AdtCompareB, u16CompareValue);  //ͨ�ñȽϻ�׼ֵ�Ĵ���A����
    stcAdtTIM4Cfg.enCap = AdtCHxCompareOutput;            //�Ƚ����
    stcAdtTIM4Cfg.bOutEn = TRUE;                          		    //���ʹ��
    stcAdtTIM4Cfg.enPerc = AdtCHxPeriodLow;                    //����ֵ������ƥ��ʱCHA��ƽ���ֲ���
    stcAdtTIM4Cfg.enCmpc = AdtCHxCompareHigh;             //����ֵ��Ƚ�ֵAƥ��ʱ��CHA��ƽ��ת
    stcAdtTIM4Cfg.enStaStp = AdtCHxStateSelSS;              //��ʼ������ƽ��STACA��STPCA����
    stcAdtTIM4Cfg.enStaOut = AdtCHxPortOutLow;            //��ʼ��ƽΪ��
    stcAdtTIM4Cfg.enStpOut = AdtCHxPortOutLow;            //������ƽΪ��
    Adt_CHxXPortConfig(AdtTIM4, AdtCHxB, &stcAdtTIM4Cfg);   //�˿�CHB ����
    Adt_StartCount(AdtTIM4);
}
void PWM_timer1(void)
{
  uint16_t                        u16ArrValue;
  uint16_t                        u16CompareBValue;
  uint16_t                        u16CntValue;
  
  stc_bt_mode23_config_t          stcTim1BaseCfg;
  stc_gpio_config_t               stcTIM1Port;
  stc_bt_m23_compare_config_t     stcTim1PortCmpCfg;
  
  DDL_ZERO_STRUCT(stcTim1BaseCfg);                              //�ṹ���ʼ������
  DDL_ZERO_STRUCT(stcTIM1Port);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE);       //Base Timer����ʱ��ʹ��
  Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);       //GPIO ����ʱ��ʹ��
  
  stcTIM1Port.enDir  = GpioDirOut;
  Gpio_Init(GpioPortA, GpioPin6, &stcTIM1Port);
  Gpio_SetAfMode(GpioPortA,GpioPin6,GpioAf4);                   //PA06����Ϊ 1��TIM1_CHA
  
  stcTim1BaseCfg.enWorkMode    = BtWorkMode2;                   //���ǲ�ģʽ
  stcTim1BaseCfg.enCT          = BtTimer;                       //��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK
  stcTim1BaseCfg.enPRS         = BtPCLKDiv256;                  //PCLK
  stcTim1BaseCfg.enCntDir      = BtCntUp;                       //���ϼ����������ǲ�ģʽʱֻ��
  stcTim1BaseCfg.enPWMTypeSel  = BtIndependentPWM ;             //�������PWM
  stcTim1BaseCfg.enPWM2sSel    = BtSinglePointCmp;              //����ȽϹ���
  stcTim1BaseCfg.bOneShot      = FALSE;                         //ѭ������
  stcTim1BaseCfg.bURSSel       = FALSE;                         //���������
  
  stcTim1BaseCfg.pfnTim1Cb     = Tim1Int;                       //�жϺ������
  
  Bt_Mode23_Init(TIM1,&stcTim1BaseCfg);                         //TIM3 ��ģʽ3���ܳ�ʼ��
  
  Bt_ClearAllIntFlag(TIM1);                                     //���жϱ�־
  EnableNvic(TIM1_IRQn, IrqLevel0, TRUE);                       //TIM3�ж�ʹ��
  Bt_Mode23_EnableIrq(TIM1,BtUevIrq);                           //ʹ��TIM3 UEV�����ж�
  
  u16ArrValue = 1600u;
  Bt_M23_ARRSet(TIM1,u16ArrValue, TRUE);                        //��������ֵ,��ʹ�ܻ���
  
  u16CompareBValue = 1800u;                                     //Խ���Խ����ռ�ձ�ԽС��
  
  Bt_M23_CCR_Set(TIM1,BtCCR0A, u16CompareBValue);               //���ñȽ�ֵA,(PWM����ģʽ��ֻ��Ҫ���ñȽ�ֵA)
  
  stcTim1PortCmpCfg.enCH0ACmpCtrl   = BtPWMMode2;               //OCREFA�������OCMA:PWMģʽ2
  stcTim1PortCmpCfg.enCH0BPolarity  = BtPortPositive;           //�������
  stcTim1PortCmpCfg.bCh0ACmpBufEn   = TRUE;                     //Aͨ���������
  stcTim1PortCmpCfg.enCh0ACmpIntSel = BtCmpIntNone;             //Aͨ���ȽϿ���:��
  
  Bt_M23_PortOutput_Config(TIM1, &stcTim1PortCmpCfg);           //�Ƚ�����˿�����
  
  //  u8ValidPeriod = 1;                                        //�¼������������ã�0��ʾ���ǲ�ÿ������ڸ���һ�Σ�ÿ+1�����ӳٰ������
  //  Tim3_M23_SetValidPeriod(u8ValidPeriod);                   //�����������
  
  u16CntValue = 0u;
  Bt_M23_Cnt16Set(TIM1,u16CntValue);                            //���ü�����ֵ
  
  Bt_M23_EnPWM_Output(TIM1,TRUE, FALSE);                        //�˿����ʹ��
  
  Bt_M23_Run(TIM1);                                             //���С�
}
/*****************************************************************************************************
��������SetTIM1_PWM_Frequency
���ܣ����ö�ʱ��1PWM��Ƶ�ʡ�
����������1������Ƶ����ֵ��
����ֵ���޷���ֵ��
*****************************************************************************************************/
void SetTIM1_PWM_Frequency(uint16_t TIM1_Period)
{
  Tim3_M23_ARRSet(TIM1_Period, TRUE);  
}
/*****************************************************************************************************
��������SetTIM1_PWM_DutyCycle
���ܣ���ʱ��1PWM��ռ�ձȿ��ơ�
����������1��ռ�ձȸߵ�ƽ��������2��ͨ���š�
����ֵ���޷���ֵ��
*****************************************************************************************************/
void SetTIM1_PWM_DutyCycle( uint16_t TIM1_Pulse,uint8_t ChannelNumber)
{  
  switch(ChannelNumber)
  {
  case 1:
    Bt_M23_CCR_Set(TIM1,BtCCR0A, TIM1_Pulse);  
    break;
  case 2:
    Tim3_M23_CCR_Set(Tim3CCR0B, TIM1_Pulse);                
    break;
  case 3:
    Tim3_M23_CCR_Set(Tim3CCR1B, TIM1_Pulse);                
    break;
  case 4:
    Tim3_M23_CCR_Set(Tim3CCR2B, TIM1_Pulse);                
    break;
  case 5:
    Tim3_M23_CCR_Set(Tim3CCR2A, TIM1_Pulse);       //    5 ��
    break;
  case 6:
    Adt_SetCompareValue(AdtTIM4,  AdtCompareB, TIM1_Pulse);       //    6 ��
    break;
  default:
    break;
  }   
}
/*****************************************************************************************************
��������PWM_Contorl_LED
���ܣ�ͨ��PWM����LED�ı�����䰵��
����������1��ͨ���š�����2������״̬��
����ֵ���޷���ֵ��
*****************************************************************************************************/
# if 0
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

#define PWM_DEAD_ZONE   0
uint16_t pwm[4] = {(PWM_MAX-PWM_DEAD_ZONE), (PWM_MAX-PWM_DEAD_ZONE), (PWM_MAX-PWM_DEAD_ZONE), (PWM_MAX-PWM_DEAD_ZONE)};
uint16_t pwm_set[6]={0,0,0,0,0,0};

void PWM_Contorl_LED(uint8_t ch,uint8_t ch_state)
{
  if(ch_state==1)
  {
    count[ch-1]--;
    //back_LED_open_Close_Config(ch,0);
    if(count[ch-1]==0)
    {
      pwm_flag[ch-1]=0x02;
      count[ch-1]=0; 
    }
    if(count[ch-1]>1500&&count[ch-1]<1600)
    {
      pwm_set[ch-1]++;
      if(pwm_set[ch-1]==10)
      {
        pwm_set[ch-1]=0;
        SetTIM1_PWM_DutyCycle((uint16_t)(count[ch-1]),ch);
      }
      else
      {
        count[ch-1]+=1;
        return;
      }
    }
    else if(count[ch-1]>1200&&count[ch-1]<1500)
    {
      pwm_set[ch-1]++;
      if(pwm_set[ch-1]==5)
      {
        pwm_set[ch-1]=0;
        SetTIM1_PWM_DutyCycle((uint16_t)(count[ch-1]),ch);
      }
      else
      {
        count[ch-1]+=1;
        return;
      }
    }
    else if(count[ch-1]>1000&&count[ch-1]<1200)
    {
      pwm_set[ch-1]++;
      if(pwm_set[ch-1]==2)
      {
        pwm_set[ch-1]=0;
        SetTIM1_PWM_DutyCycle((uint16_t)(count[ch-1]),ch);
      }
      else
      {
        count[ch-1]+=1;
        return;
      }
    }
    else
    {
      SetTIM1_PWM_DutyCycle((uint16_t)(count[ch-1]),ch);
    }
  }
  if(ch_state==0)                                      
  {
    count[ch-1]++;
    if(count[ch-1]>=1800)
    {
      pwm_flag[ch-1]=0x02;
      count[ch-1]=1800; 
      //back_LED_open_Close_Config(ch,1);
    }
    SetTIM1_PWM_DutyCycle((uint16_t)(count[ch-1]),ch);
  }
}

void Tim3Int(void)
{ 
  if(TRUE == Tim3_GetIntFlag(Tim3UevIrq))
  {
    Tim3_ClearIntFlag(Tim3UevIrq);
  }
}
void Tim1Int(void)
{ 
  if(TRUE == Bt_GetIntFlag(TIM1,BtUevIrq))
  {
    Bt_ClearIntFlag(TIM1,BtUevIrq);
  }
}
/********************************************PWM.C*************************************************/