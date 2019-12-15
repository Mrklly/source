#include "bsp_pwm.h"
#include "hc32_adt.h"
#include "hc32_bt.h"


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

    Gpio_Init(GpioPortA, GpioPin10, &stcTIM3Port);
    Gpio_SetAfMode(GpioPortA, GpioPin10, GpioAf2);                // PA10 TIM3_CH2A

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
    stcTim3BaseCfg.pfnTim3Cb     = NULL;                       //中断函数入口
    Tim3_Mode23_Init(&stcTim3BaseCfg);                            //TIM3 的模式3功能初始化

    EnableNvic(TIM3_IRQn, IrqLevel0, FALSE);                       //TIM3中断使能

    //  Tim3_ClearAllIntFlag();                                       //清中断标志
    //  Tim3_Mode23_EnableIrq(Tim3UevIrq);                            //使能TIM3 UEV更新中断

    u16ArrValue = 1600;
    Tim3_M23_ARRSet(u16ArrValue, TRUE);                           //设置重载值,并使能缓存

    u16CompareBValue = 1800;

    //  Tim3_M23_CCR_Set(Tim3CCR0A, u16CompareAValue);              //设置比较值A,(PWM互补模式下只需要设置比较值A)
    Tim3_M23_CCR_Set(Tim3CCR0B, u16CompareBValue);           // PA07

    //  Tim3_M23_CCR_Set(Tim3CCR1A, u16CompareAValue);
    Tim3_M23_CCR_Set(Tim3CCR1B, u16CompareBValue);           // PB00

    Tim3_M23_CCR_Set(Tim3CCR2A, u16CompareBValue);

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
    stcTim1BaseCfg.pfnTim0Cb     = NULL;
    stcTim1BaseCfg.pfnTim1Cb     = NULL;
    stcTim1BaseCfg.pfnTim2Cb     = NULL;

    Bt_Mode23_Init(TIM1,&stcTim1BaseCfg);                         //TIM3 的模式3功能初始化

    EnableNvic(TIM1_IRQn, IrqLevel0, FALSE);                       //TIM3中断使能

    //  Bt_ClearAllIntFlag(TIM1);                                     //清中断标志
    //  Bt_Mode23_EnableIrq(TIM1,BtUevIrq);                           //使能TIM3 UEV更新中断

    u16ArrValue = 1600;
    Bt_M23_ARRSet(TIM1,u16ArrValue, TRUE);                        //设置重载值,并使能缓存

    u16CompareBValue = 1800;                                     //越大灯越暗，占空比越小。

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



// BL6 -> PA00 -> TIM0_CHA -> FUNC(7)
// BL5 -> PA04 -> TIM5_CHA -> FUNC(5)
// BL1 -> PA05 -> TIM5_CHB -> FUNC(5)
// BL2 -> PB02 -> TIM4_CHA -> FUNC(4)
// BL3 -> PB10 -> TIM3_CH1A -> FUNC(5)
// BL4 -> PB11 -> TIM6_CHA -> FUNC(5)
void backlight_tim5_init(void)
{
    uint16_t                  u16Period;
    uint16_t                  u16CompareValue;

    stc_adt_basecnt_cfg_t     stcAdtBaseCntCfg;
    stc_adt_CHxX_port_cfg_t   stcAdtTIM5Cfg;
    stc_gpio_config_t         stcTIM5Port;

    DDL_ZERO_STRUCT(stcAdtBaseCntCfg);
    DDL_ZERO_STRUCT(stcAdtTIM5Cfg);
    DDL_ZERO_STRUCT(stcTIM5Port);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, TRUE);

    stcTIM5Port.enDir = GpioDirOut;
    Gpio_Init(GpioPortA, GpioPin4, &stcTIM5Port);
    Gpio_Init(GpioPortA, GpioPin5, &stcTIM5Port);

    Gpio_SetAfMode(GpioPortA, GpioPin4,GpioAf5);
    Gpio_SetAfMode(GpioPortA, GpioPin5,GpioAf5);

    stcAdtBaseCntCfg.enCntMode = AdtSawtoothMode;
    stcAdtBaseCntCfg.enCntDir =  AdtCntUp;
    stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0Div256;
    Adt_Init(AdtTIM5, &stcAdtBaseCntCfg);                      //ADT载波、计数模式、时钟配置

    u16Period = 1600;
    Adt_SetPeriod(AdtTIM5, u16Period);                         //周期设置

    u16CompareValue = 0;
    Adt_SetCompareValue(AdtTIM5, AdtCompareA, u16CompareValue);  //通用比较基准值寄存器A设置
    Adt_SetCompareValue(AdtTIM5, AdtCompareB, u16CompareValue);  //通用比较基准值寄存器B设置

    stcAdtTIM5Cfg.enCap = AdtCHxCompareOutput;            //比较输出
    stcAdtTIM5Cfg.bOutEn = TRUE;                          //CHA输出使能
    stcAdtTIM5Cfg.enPerc = AdtCHxPeriodLow;               //计数值与周期匹配时CHA电平保持不变
    stcAdtTIM5Cfg.enCmpc = AdtCHxCompareInv;             //计数值与比较值A匹配时，CHA电平翻转
    stcAdtTIM5Cfg.enStaStp = AdtCHxStateSelKeep;            //CHA起始结束电平由STACA与STPCA控制
    stcAdtTIM5Cfg.enStaOut = AdtCHxPortOutLow;            //CHA起始电平为低
    stcAdtTIM5Cfg.enStpOut = AdtCHxPortOutLow;            //CHA结束电平为低
    Adt_CHxXPortConfig(AdtTIM5, AdtCHxA, &stcAdtTIM5Cfg);   //端口CHA配置
    Adt_CHxXPortConfig(AdtTIM5, AdtCHxB, &stcAdtTIM5Cfg);    //端口CHB配置

    Adt_SetCount(AdtTIM5, 0);
    Adt_StartCount(AdtTIM5);
}

void backlight_tim0_init(void)
{
    uint16_t                        u16ArrValue;
    uint16_t                        u16CompareValue;
    uint16_t                        u16CntValue;

    stc_bt_mode23_config_t          stcTim0BaseCfg;
    stc_gpio_config_t               stcTIM0Port;
    stc_bt_m23_compare_config_t     stcTim0PortCmpCfg;

    DDL_ZERO_STRUCT(stcTim0BaseCfg);                              //结构体初始化清零
    DDL_ZERO_STRUCT(stcTIM0Port);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE);       //Base Timer外设时钟使能
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);       //GPIO 外设时钟使能

    stcTIM0Port.enDir  = GpioDirOut;
    Gpio_Init(GpioPortA, GpioPin0, &stcTIM0Port);
    Gpio_SetAfMode(GpioPortA, GpioPin0, GpioAf7);

    stcTim0BaseCfg.enWorkMode    = BtWorkMode2;                   //三角波模式
    stcTim0BaseCfg.enCT          = BtTimer;                       //定时器功能，计数时钟为内部PCLK
    stcTim0BaseCfg.enPRS         = BtPCLKDiv256;                  //PCLK
    stcTim0BaseCfg.enCntDir      = BtCntUp;                       //向上计数，在三角波模式时只读
    stcTim0BaseCfg.enPWMTypeSel  = BtIndependentPWM ;             //独立输出PWM
    stcTim0BaseCfg.enPWM2sSel    = BtSinglePointCmp;              //单点比较功能
    stcTim0BaseCfg.bOneShot      = FALSE;                         //循环计数
    stcTim0BaseCfg.bURSSel       = FALSE;                         //上下溢更新
    Bt_Mode23_Init(TIM0,&stcTim0BaseCfg);                         //TIM3 的模式3功能初始化

    Bt_ClearAllIntFlag(TIM0);                                     //清中断标志
    EnableNvic(TIM0_IRQn, IrqLevel0, FALSE);                       //TIM3中断使能
//    Bt_Mode23_EnableIrq(TIM0,BtUevIrq);                           //使能TIM3 UEV更新中断

    u16ArrValue = 1600;
    Bt_M23_ARRSet(TIM0,u16ArrValue, TRUE);                        //设置重载值,并使能缓存

    u16CompareValue = 0;                                     //越大灯越暗，占空比越小。
    Bt_M23_CCR_Set(TIM0,BtCCR0A, u16CompareValue);               //设置比较值A,(PWM互补模式下只需要设置比较值A)

    stcTim0PortCmpCfg.enCH0ACmpCtrl   = BtPWMMode2;               //OCREFA输出控制OCMA:PWM模式2
    stcTim0PortCmpCfg.enCH0APolarity  = BtPortPositive;           //正常输出
    stcTim0PortCmpCfg.bCh0ACmpBufEn   = TRUE;                     //A通道缓存控制
    stcTim0PortCmpCfg.enCh0ACmpIntSel = BtCmpIntNone;             //A通道比较控制:无
    Bt_M23_PortOutput_Config(TIM0, &stcTim0PortCmpCfg);           //比较输出端口配置

//    u8ValidPeriod = 1;                                        //事件更新周期设置，0表示三角波每半个周期更新一次，每+1代表延迟半个周期
//    Tim3_M23_SetValidPeriod(u8ValidPeriod);                   //间隔周期设置

    u16CntValue = 0u;
    Bt_M23_Cnt16Set(TIM0,u16CntValue);                            //设置计数初值
    Bt_M23_EnPWM_Output(TIM0,TRUE, FALSE);                        //端口输出使能
    Bt_M23_Run(TIM0);                                             //运行。
}

void backlight_tim4_init(void)
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
    Gpio_Init(GpioPortB, GpioPin2, &stcTIM4Port);       // PB02 TIM4 CHA
    Gpio_SetAfMode(GpioPortB, GpioPin2, GpioAf4);

    Gpio_Init(GpioPortA, GpioPin11, &stcTIM4Port);
    Gpio_SetAfMode(GpioPortA, GpioPin11, GpioAf7);      // PA11 TIM4 CHB

    stcAdtBaseCntCfg.enCntMode = AdtSawtoothMode;
    stcAdtBaseCntCfg.enCntDir =  AdtCntUp;
    stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0Div256;
    Adt_Init(AdtTIM4, &stcAdtBaseCntCfg);                      //ADT载波、计数模式、时钟配置

    u16Period = 1600;
    Adt_SetPeriod(AdtTIM4, u16Period);                         //周期设置

    u16CompareValue = 0;
    Adt_SetCompareValue(AdtTIM4, AdtCompareA, u16CompareValue);  //通用比较基准值寄存器A设置

    Adt_SetCompareValue(AdtTIM4, AdtCompareB, 1800);        // PWM LED 6

    stcAdtTIM4Cfg.enCap = AdtCHxCompareOutput;            //比较输出
    stcAdtTIM4Cfg.bOutEn = TRUE;                          //CHA输出使能
    stcAdtTIM4Cfg.enPerc = AdtCHxPeriodLow;               //计数值与周期匹配时CHA电平保持不变
    stcAdtTIM4Cfg.enCmpc = AdtCHxCompareHigh;             //计数值与比较值A匹配时，CHA电平翻转
    stcAdtTIM4Cfg.enStaStp = AdtCHxStateSelSS;            //CHA起始结束电平由STACA与STPCA控制
    stcAdtTIM4Cfg.enStaOut = AdtCHxPortOutLow;            //CHA起始电平为低
    stcAdtTIM4Cfg.enStpOut = AdtCHxPortOutLow;            //CHA结束电平为低
    Adt_CHxXPortConfig(AdtTIM4, AdtCHxA, &stcAdtTIM4Cfg);   //端口CHA配置
    Adt_CHxXPortConfig(AdtTIM4, AdtCHxB, &stcAdtTIM4Cfg);   // PWM LED 6

    Adt_StartCount(AdtTIM4);
}

void backlight_tim3_init(void)
{
    uint16_t                        u16ArrValue;
    uint16_t                        u16CompareValue;
    uint16_t                        u16CntValue;

    stc_tim3_mode23_config_t        stcTim3BaseCfg;
    stc_gpio_config_t               stcTIM3Port;
    stc_tim3_m23_compare_config_t   stcTim3PortCmpCfg;

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);       //GPIO 外设时钟使能
    Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE);       //Timer3外设时钟使能

    DDL_ZERO_STRUCT(stcTim3BaseCfg);                              //结构体初始化清零
    DDL_ZERO_STRUCT(stcTIM3Port);

    stcTIM3Port.enDir  = GpioDirOut;

    Gpio_Init(GpioPortB, GpioPin10, &stcTIM3Port);
    Gpio_SetAfMode(GpioPortB, GpioPin10, GpioAf5);

//    stcTim3BaseCfg.enWorkMode    = Tim3WorkMode2;                 //三角波模式
//    stcTim3BaseCfg.enCT          = Tim3Timer;                     //定时器功能，计数时钟为内部PCLK
//    stcTim3BaseCfg.enPRS         = Tim3PCLKDiv256;                //PCLK
//    stcTim3BaseCfg.enCntDir      = Tim3CntUp;                     //向上计数，在三角波模式时只读
//    stcTim3BaseCfg.enPWMTypeSel  = Tim3IndependentPWM;            //独立输出PWM
//    stcTim3BaseCfg.enPWM2sSel    = Tim3SinglePointCmp;            //单点比较功能
//    stcTim3BaseCfg.bOneShot      = FALSE;                         //循环计数
//    stcTim3BaseCfg.bURSSel       = FALSE;                         //上下溢更新
//    stcTim3BaseCfg.pfnTim3Cb     = NULL;
//    Tim3_Mode23_Init(&stcTim3BaseCfg);                            //TIM3 的模式3功能初始化

//    u16ArrValue = 1600;
//    Tim3_M23_ARRSet(u16ArrValue, TRUE);                           //设置重载值,并使能缓存
                            
    u16CompareValue = 0;                                   //越大灯越暗，占空比越小。
    Tim3_M23_CCR_Set(Tim3CCR1A, u16CompareValue);

//    stcTim3PortCmpCfg.enCHxACmpCtrl   = Tim3PWMMode2;             //OCREFA输出控制OCMA:PWM模式2
//    stcTim3PortCmpCfg.enCHxAPolarity  = Tim3PortPositive;         //正常输出
//    stcTim3PortCmpCfg.bCHxACmpBufEn   = TRUE;                     //A通道缓存控制
//    stcTim3PortCmpCfg.enCHxACmpIntSel = Tim3CmpIntNone;           //A通道比较控制:无

//    Tim3_M23_PortOutput_Config(Tim3CH1, &stcTim3PortCmpCfg);      //比较输出端口配置

    u16CntValue = 0u;
    Tim3_M23_Cnt16Set(u16CntValue);                               //设置计数初值
    Tim3_M23_EnPWM_Output(TRUE, FALSE);                           //端口输出使能
    Tim3_M23_Run();                                               //运行。
}

void backlight_tim6_init(void)
{
    uint16_t                  u16Period;
    uint16_t                  u16CompareValue;

    stc_adt_basecnt_cfg_t     stcAdtBaseCntCfg;
    stc_adt_CHxX_port_cfg_t   stcAdtTIM6Cfg;
    stc_gpio_config_t         stcTIM6Port;

    DDL_ZERO_STRUCT(stcAdtBaseCntCfg);
    DDL_ZERO_STRUCT(stcAdtTIM6Cfg);
    DDL_ZERO_STRUCT(stcTIM6Port);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, TRUE);

    stcTIM6Port.enDir = GpioDirOut;
    Gpio_Init(GpioPortB, GpioPin11, &stcTIM6Port);
    Gpio_SetAfMode(GpioPortB, GpioPin11, GpioAf5);

    stcAdtBaseCntCfg.enCntMode = AdtSawtoothMode;
    stcAdtBaseCntCfg.enCntDir =  AdtCntUp;
    stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0Div256;
    
    Adt_Init(AdtTIM6, &stcAdtBaseCntCfg);                      //ADT载波、计数模式、时钟配置

    u16Period = 1600;
    Adt_SetPeriod(AdtTIM6, u16Period);                         //周期设置

    u16CompareValue = 0;
    Adt_SetCompareValue(AdtTIM6, AdtCompareA, u16CompareValue);  //通用比较基准值寄存器A设置

    stcAdtTIM6Cfg.enCap = AdtCHxCompareOutput;            //比较输出
    stcAdtTIM6Cfg.bOutEn = TRUE;                          //CHA输出使能
    stcAdtTIM6Cfg.enPerc = AdtCHxPeriodLow;               //计数值与周期匹配时CHA电平保持不变
    stcAdtTIM6Cfg.enCmpc = AdtCHxCompareInv;             //计数值与比较值A匹配时，CHA电平翻转
    stcAdtTIM6Cfg.enStaStp = AdtCHxStateSelKeep;            //CHA起始结束电平由STACA与STPCA控制
    stcAdtTIM6Cfg.enStaOut = AdtCHxPortOutLow;            //CHA起始电平为低
    stcAdtTIM6Cfg.enStpOut = AdtCHxPortOutLow;            //CHA结束电平为低
    Adt_CHxXPortConfig(AdtTIM6, AdtCHxA, &stcAdtTIM6Cfg);   //端口CHA配置

    Adt_StartCount(AdtTIM6);
}

void SetTIM1_PWM_Frequency(uint16_t TIM1_Period)
{
  Tim3_M23_ARRSet(TIM1_Period, TRUE);  
}

void led_set_pwm(uint8_t ch, uint16_t pulse)
{
    switch(ch) {
    case 0:
//        Bt_M23_CCR_Set(TIM1,BtCCR0A, TIM1_Pulse);
        PWM_LED_0 = pulse;
        break;
    case 1:
//        Tim3_M23_CCR_Set(Tim3CCR0B, TIM1_Pulse);
        PWM_LED_1 = pulse;
        break;
    case 2:
//        Tim3_M23_CCR_Set(Tim3CCR1B, TIM1_Pulse);
        PWM_LED_2 = pulse;
        break;
    case 3:
//        Tim3_M23_CCR_Set(Tim3CCR2B, TIM1_Pulse);
        PWM_LED_3 = pulse;
        break;
    case 4:
//        Tim3_M23_CCR_Set(Tim3CCR2A, TIM1_Pulse);
        PWM_LED_4 = pulse;
        break;
    case 5:
//        Adt_SetCompareValue(AdtTIM4, AdtCompareB, TIM1_Pulse);
        PWM_LED_5 = pulse;
        break;
    default:
        break;
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

// BL6 -> PA00 -> TIM0_CHA -> FUNC(7)
// BL5 -> PA04 -> TIM5_CHA -> FUNC(5)
// BL1 -> PA05 -> TIM5_CHB -> FUNC(5)
// BL2 -> PB02 -> TIM4_CHA -> FUNC(4)
// BL3 -> PB10 -> TIM3_CH1A -> FUNC(5)
// BL4 -> PB11 -> TIM6_CHA -> FUNC(5)
void backlight_set_pwm(uint8_t ch, uint16_t pulse)
{
    switch(ch) {
    case 0:
//        Adt_SetCompareValue(AdtTIM5, AdtCompareB, pulse); // BL1
        PWM_BACKLIGHT_0 = pulse;
        break ;
    case 1:
//        Adt_SetCompareValue(AdtTIM4, AdtCompareA, pulse); // BL2
        PWM_BACKLIGHT_1 = pulse;
        break ;
    case 2:
//        Tim3_M23_CCR_Set(Tim3CCR1A, pulse);               // BL3
        PWM_BACKLIGHT_2 = pulse;
        break ;
    case 3:
//        Adt_SetCompareValue(AdtTIM6, AdtCompareA, pulse); // BL4
        PWM_BACKLIGHT_3 = pulse;
        break ;
    case 4:
//        Adt_SetCompareValue(AdtTIM5, AdtCompareA, pulse); // BL5
        PWM_BACKLIGHT_4 = pulse;
        break ;
    case 5:
//        Bt_M23_CCR_Set(TIM0, BtCCR0A, pulse);              // BL6
        PWM_BACKLIGHT_5 = pulse;
        break ;
    default :
        break;
    }
}

static inline void backlight_set_all(uint16_t pulse)
{
    Adt_SetCompareValue(AdtTIM5, AdtCompareB, pulse); // BL1
    Adt_SetCompareValue(AdtTIM4, AdtCompareA, pulse); // BL2
    Tim3_M23_CCR_Set(Tim3CCR1A, pulse);               // BL3
    Adt_SetCompareValue(AdtTIM6, AdtCompareA, pulse); // BL4
    Adt_SetCompareValue(AdtTIM5, AdtCompareA, pulse); // BL5
    Bt_M23_CCR_Set(TIM0,BtCCR0A, pulse);              // BL6
}

void backlight_set_zero(void)
{
    backlight_set_all(1800);
}


void backlight_set_mid(void)
{
    backlight_set_all(800);
}

void backlight_set_full(void)
{
    backlight_set_all(0);
}

