#include "bsp_pwm.h"
#include "adt.h"

uint16_t count[6]={1800,1800,1800,1800,1800,1800};
extern uint8_t pwm_flag[6];
/*****************************************************************************************************
程序名：PWM_timer3
功能：PWM初始化
参数：无参数。
返回值：无返回值。
*****************************************************************************************************/
void PWM_timer3(void)
{
  uint16_t                        u16ArrValue;
  uint16_t                        u16CompareBValue;
  uint16_t                        u16CntValue;

  
  stc_tim3_mode23_config_t        stcTim3BaseCfg;
  stc_gpio_config_t               stcTIM3Port;
  stc_tim3_m23_compare_config_t   stcTim3PortCmpCfg;
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);       //GPIO 外设时钟使能
  Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE);       //Timer3外设时钟使能
  
  
  DDL_ZERO_STRUCT(stcTim3BaseCfg);                              //结构体初始化清零
  DDL_ZERO_STRUCT(stcTIM3Port);
  
  stcTIM3Port.enDir  = GpioDirOut;
  
  Gpio_Init(GpioPortA, GpioPin7, &stcTIM3Port);
  Gpio_SetAfMode(GpioPortA,GpioPin7,GpioAf4);                   //PA07设置为2，TIM3_CH0B
  
  Gpio_Init(GpioPortB, GpioPin0, &stcTIM3Port);
  Gpio_SetAfMode(GpioPortB,GpioPin0,GpioAf2);                   //PB00设置为3,TIM3_CH1B
  
  Gpio_Init(GpioPortB, GpioPin1, &stcTIM3Port);
  Gpio_SetAfMode(GpioPortB,GpioPin1,GpioAf3);                   //PB01设置为4,TIM3_CH2B
  Gpio_Init(GpioPortA, GpioPin10, &stcTIM3Port);
  Gpio_SetAfMode(GpioPortA,GpioPin10,GpioAf2);                 //PA10设置为5,TIM3_CH2A
  
  
  stcTim3BaseCfg.enWorkMode    = Tim3WorkMode2;                 //三角波模式
  stcTim3BaseCfg.enCT          = Tim3Timer;                     //定时器功能，计数时钟为内部PCLK
  stcTim3BaseCfg.enPRS         = Tim3PCLKDiv256;                //PCLK
  stcTim3BaseCfg.enCntDir      = Tim3CntUp;                     //向上计数，在三角波模式时只读
  stcTim3BaseCfg.enPWMTypeSel  = Tim3IndependentPWM;            //独立输出PWM
  stcTim3BaseCfg.enPWM2sSel    = Tim3SinglePointCmp;            //单点比较功能
  stcTim3BaseCfg.bOneShot      = FALSE;                         //循环计数
  stcTim3BaseCfg.bURSSel       = FALSE;                         //上下溢更新
  
  stcTim3BaseCfg.pfnTim3Cb     = Tim3Int;                       //中断函数入口
  
  Tim3_Mode23_Init(&stcTim3BaseCfg);                            //TIM3 的模式3功能初始化
  
  Tim3_ClearAllIntFlag();                                       //清中断标志
  EnableNvic(TIM3_IRQn, IrqLevel0, TRUE);                       //TIM3中断使能
  Tim3_Mode23_EnableIrq(Tim3UevIrq);                            //使能TIM3 UEV更新中断
  
  u16ArrValue = 1600u;
  Tim3_M23_ARRSet(u16ArrValue, TRUE);                           //设置重载值,并使能缓存
  
  //  u16CompareAValue = 0x8000;                                    
  u16CompareBValue = 1800u;                                   //越大灯越暗，占空比越小。
  
  //  Tim3_M23_CCR_Set(Tim3CCR0A, u16CompareAValue);              //设置比较值A,(PWM互补模式下只需要设置比较值A)
  Tim3_M23_CCR_Set(Tim3CCR0B, u16CompareBValue);                //2键
  
  //  Tim3_M23_CCR_Set(Tim3CCR1A, u16CompareAValue);
  Tim3_M23_CCR_Set(Tim3CCR1B, u16CompareBValue);                //3键
  
  //  Tim3_M23_CCR_Set(Tim3CCR2A, u16CompareAValue);
  Tim3_M23_CCR_Set(Tim3CCR2B, u16CompareBValue);                //4键
  Tim3_M23_CCR_Set(Tim3CCR2A, u16CompareBValue);               // 5 键
  
  stcTim3PortCmpCfg.enCHxACmpCtrl   = Tim3PWMMode2;             //OCREFA输出控制OCMA:PWM模式2
  stcTim3PortCmpCfg.enCHxAPolarity  = Tim3PortPositive;         //正常输出
  stcTim3PortCmpCfg.bCHxACmpBufEn   = TRUE;                     //A通道缓存控制
  stcTim3PortCmpCfg.enCHxACmpIntSel = Tim3CmpIntNone;           //A通道比较控制:无
  
  stcTim3PortCmpCfg.enCHxBCmpCtrl   = Tim3PWMMode2;             //OCREFB输出控制OCMB:PWM模式2(PWM互补模式下也要设置，避免强制输出)
  stcTim3PortCmpCfg.enCHxBPolarity  = Tim3PortPositive;         //正常输出
  stcTim3PortCmpCfg.bCHxBCmpBufEn   = TRUE;                     //B通道缓存控制使能
  stcTim3PortCmpCfg.enCHxBCmpIntSel = Tim3CmpIntNone;           //B通道比较控制:无
  
  Tim3_M23_PortOutput_Config(Tim3CH0, &stcTim3PortCmpCfg);      //比较输出端口配置
  Tim3_M23_PortOutput_Config(Tim3CH1, &stcTim3PortCmpCfg);      //比较输出端口配置
  Tim3_M23_PortOutput_Config(Tim3CH2, &stcTim3PortCmpCfg);      //比较输出端口配置
  
  //  u8ValidPeriod = 1;                                            //事件更新周期设置，0表示三角波每半个周期更新一次，每+1代表延迟半个周期
  //  Tim3_M23_SetValidPeriod(u8ValidPeriod);                       //间隔周期设置
  
  u16CntValue = 0u;
  
  Tim3_M23_Cnt16Set(u16CntValue);                               //设置计数初值
  
  Tim3_M23_EnPWM_Output(TRUE, FALSE);                           //端口输出使能
  
  Tim3_M23_Run();                                               //运行。
  
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
    Adt_Init(AdtTIM4, &stcAdtBaseCntCfg);                      //ADT载波、计数模式、时钟配置
    u16Period = 1600;
    Adt_SetPeriod(AdtTIM4, u16Period);                         //周期设置
    u16CompareValue = 1800;
    Adt_SetCompareValue(AdtTIM4, AdtCompareB, u16CompareValue);  //通用比较基准值寄存器A设置
    stcAdtTIM4Cfg.enCap = AdtCHxCompareOutput;            //比较输出
    stcAdtTIM4Cfg.bOutEn = TRUE;                          		    //输出使能
    stcAdtTIM4Cfg.enPerc = AdtCHxPeriodLow;                    //计数值与周期匹配时CHA电平保持不变
    stcAdtTIM4Cfg.enCmpc = AdtCHxCompareHigh;             //计数值与比较值A匹配时，CHA电平翻转
    stcAdtTIM4Cfg.enStaStp = AdtCHxStateSelSS;              //起始结束电平由STACA与STPCA控制
    stcAdtTIM4Cfg.enStaOut = AdtCHxPortOutLow;            //起始电平为低
    stcAdtTIM4Cfg.enStpOut = AdtCHxPortOutLow;            //结束电平为低
    Adt_CHxXPortConfig(AdtTIM4, AdtCHxB, &stcAdtTIM4Cfg);   //端口CHB 配置
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
  
  DDL_ZERO_STRUCT(stcTim1BaseCfg);                              //结构体初始化清零
  DDL_ZERO_STRUCT(stcTIM1Port);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE);       //Base Timer外设时钟使能
  Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);       //GPIO 外设时钟使能
  
  stcTIM1Port.enDir  = GpioDirOut;
  Gpio_Init(GpioPortA, GpioPin6, &stcTIM1Port);
  Gpio_SetAfMode(GpioPortA,GpioPin6,GpioAf4);                   //PA06设置为 1，TIM1_CHA
  
  stcTim1BaseCfg.enWorkMode    = BtWorkMode2;                   //三角波模式
  stcTim1BaseCfg.enCT          = BtTimer;                       //定时器功能，计数时钟为内部PCLK
  stcTim1BaseCfg.enPRS         = BtPCLKDiv256;                  //PCLK
  stcTim1BaseCfg.enCntDir      = BtCntUp;                       //向上计数，在三角波模式时只读
  stcTim1BaseCfg.enPWMTypeSel  = BtIndependentPWM ;             //独立输出PWM
  stcTim1BaseCfg.enPWM2sSel    = BtSinglePointCmp;              //单点比较功能
  stcTim1BaseCfg.bOneShot      = FALSE;                         //循环计数
  stcTim1BaseCfg.bURSSel       = FALSE;                         //上下溢更新
  
  stcTim1BaseCfg.pfnTim1Cb     = Tim1Int;                       //中断函数入口
  
  Bt_Mode23_Init(TIM1,&stcTim1BaseCfg);                         //TIM3 的模式3功能初始化
  
  Bt_ClearAllIntFlag(TIM1);                                     //清中断标志
  EnableNvic(TIM1_IRQn, IrqLevel0, TRUE);                       //TIM3中断使能
  Bt_Mode23_EnableIrq(TIM1,BtUevIrq);                           //使能TIM3 UEV更新中断
  
  u16ArrValue = 1600u;
  Bt_M23_ARRSet(TIM1,u16ArrValue, TRUE);                        //设置重载值,并使能缓存
  
  u16CompareBValue = 1800u;                                     //越大灯越暗，占空比越小。
  
  Bt_M23_CCR_Set(TIM1,BtCCR0A, u16CompareBValue);               //设置比较值A,(PWM互补模式下只需要设置比较值A)
  
  stcTim1PortCmpCfg.enCH0ACmpCtrl   = BtPWMMode2;               //OCREFA输出控制OCMA:PWM模式2
  stcTim1PortCmpCfg.enCH0BPolarity  = BtPortPositive;           //正常输出
  stcTim1PortCmpCfg.bCh0ACmpBufEn   = TRUE;                     //A通道缓存控制
  stcTim1PortCmpCfg.enCh0ACmpIntSel = BtCmpIntNone;             //A通道比较控制:无
  
  Bt_M23_PortOutput_Config(TIM1, &stcTim1PortCmpCfg);           //比较输出端口配置
  
  //  u8ValidPeriod = 1;                                        //事件更新周期设置，0表示三角波每半个周期更新一次，每+1代表延迟半个周期
  //  Tim3_M23_SetValidPeriod(u8ValidPeriod);                   //间隔周期设置
  
  u16CntValue = 0u;
  Bt_M23_Cnt16Set(TIM1,u16CntValue);                            //设置计数初值
  
  Bt_M23_EnPWM_Output(TIM1,TRUE, FALSE);                        //端口输出使能
  
  Bt_M23_Run(TIM1);                                             //运行。
}
/*****************************************************************************************************
程序名：SetTIM1_PWM_Frequency
功能：设置定时器1PWM的频率。
参数：参数1：传入频率数值。
返回值：无返回值。
*****************************************************************************************************/
void SetTIM1_PWM_Frequency(uint16_t TIM1_Period)
{
  Tim3_M23_ARRSet(TIM1_Period, TRUE);  
}
/*****************************************************************************************************
程序名：SetTIM1_PWM_DutyCycle
功能：定时器1PWM的占空比控制。
参数：参数1：占空比高电平数，参数2：通道号。
返回值：无返回值。
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
    Tim3_M23_CCR_Set(Tim3CCR2A, TIM1_Pulse);       //    5 键
    break;
  case 6:
    Adt_SetCompareValue(AdtTIM4,  AdtCompareB, TIM1_Pulse);       //    6 键
    break;
  default:
    break;
  }   
}
/*****************************************************************************************************
程序名：PWM_Contorl_LED
功能：通过PWM控制LED的变亮与变暗。
参数：参数1：通道号。参数2：开关状态。
返回值：无返回值。
*****************************************************************************************************/
# if 0
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