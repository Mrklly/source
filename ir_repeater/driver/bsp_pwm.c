#include "bsp_pwm.h"
#include "global_variables.h"


uint16_t count[4]={1800,1800,1800,1800};


void PWM_timer3(void)
{
  uint16_t                        u16ArrValue;
  uint16_t                        u16CompareBValue;
  uint16_t                        u16CntValue;
  //  uint8_t                         u8ValidPeriod;
  
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
//  EnableNvic(TIM3_IRQn, IrqLevel0, TRUE);                       //TIM3中断使能
//  Tim3_Mode23_EnableIrq(Tim3UevIrq);                            //使能TIM3 UEV更新中断
  
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
//  EnableNvic(TIM1_IRQn, IrqLevel0, TRUE);                       //TIM3中断使能
//  Bt_Mode23_EnableIrq(TIM1,BtUevIrq);                           //使能TIM3 UEV更新中断
  
  u16ArrValue = 1600u;
  Bt_M23_ARRSet(TIM1,u16ArrValue, TRUE);                        //设置重载值,并使能缓存
  
  u16CompareBValue = 1800u;                                     //越大灯越暗，占空比越小。
  
  Bt_M23_CCR_Set(TIM1,BtCCR0A, u16CompareBValue);               //设置比较值A,(PWM互补模式下只需要设置比较值A)
  
  stcTim1PortCmpCfg.enCH0ACmpCtrl   = BtPWMMode2;               //OCREFA输出控制OCMA:PWM模式2
  stcTim1PortCmpCfg.enCH0APolarity  = BtPortPositive;           //正常输出
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

void SetTIM1_PWM_Frequency(uint16_t TIM1_Period)
{
  Tim3_M23_ARRSet(TIM1_Period, TRUE);  
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

void pwm_to_io(void)
{
    M0P_GPIO->PA06_SEL_f.SEL = 0;
    M0P_GPIO->PA07_SEL_f.SEL = 0;
    M0P_GPIO->PB00_SEL_f.SEL = 0;
    M0P_GPIO->PB01_SEL_f.SEL = 0;
}

void io_to_pwm(void)
{
    M0P_GPIO->PA06_SEL_f.SEL = 4;
    M0P_GPIO->PA07_SEL_f.SEL = 4;
    M0P_GPIO->PB00_SEL_f.SEL = 2;
    M0P_GPIO->PB01_SEL_f.SEL = 3;

    memset(count, PWM_MAX, 4);
}

