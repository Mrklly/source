#include "bsp_pwm.h"
#include "hc32_adt.h"
#include "hc32_bt.h"


// PWM2 -> PA07 -> TIM3_CH0B -> FUNC(4)
// PWM3 -> PB00 -> TIM3_CH1B -> FUNC(2)
// PWM4 -> PB01 -> TIM3_CH2B -> FUNC(3)
// PWM5 -> PA10 -> TIM3_CH2A -> FUNC(2)
void PWM_timer3(void)
{
    uint16_t                        u16ArrValue;
    uint16_t                        u16CompareBValue;
    uint16_t                        u16CntValue;
    //  uint8_t                         u8ValidPeriod;

    stc_tim3_mode23_config_t        stcTim3BaseCfg;
    stc_gpio_config_t               stcTIM3Port;
    stc_tim3_m23_compare_config_t   stcTim3PortCmpCfg;

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE);

    DDL_ZERO_STRUCT(stcTim3BaseCfg);
    DDL_ZERO_STRUCT(stcTIM3Port);

    stcTIM3Port.enDir  = GpioDirOut;

    Gpio_Init(GpioPortA, GpioPin7, &stcTIM3Port);
    Gpio_SetAfMode(GpioPortA,GpioPin7,GpioAf4);

    Gpio_Init(GpioPortB, GpioPin0, &stcTIM3Port);
    Gpio_SetAfMode(GpioPortB,GpioPin0,GpioAf2);

    Gpio_Init(GpioPortA, GpioPin10, &stcTIM3Port);
    Gpio_SetAfMode(GpioPortA, GpioPin10, GpioAf2);

    Gpio_Init(GpioPortB, GpioPin1, &stcTIM3Port);
    Gpio_SetAfMode(GpioPortB,GpioPin1,GpioAf3);

    stcTim3BaseCfg.enWorkMode    = Tim3WorkMode2;
    stcTim3BaseCfg.enCT          = Tim3Timer;
    stcTim3BaseCfg.enPRS         = Tim3PCLKDiv256;
    stcTim3BaseCfg.enCntDir      = Tim3CntUp;
    stcTim3BaseCfg.enPWMTypeSel  = Tim3IndependentPWM;
    stcTim3BaseCfg.enPWM2sSel    = Tim3SinglePointCmp;
    stcTim3BaseCfg.bOneShot      = FALSE;
    stcTim3BaseCfg.bURSSel       = FALSE;
    stcTim3BaseCfg.pfnTim3Cb     = NULL;
    Tim3_Mode23_Init(&stcTim3BaseCfg);

    EnableNvic(TIM3_IRQn, IrqLevel0, FALSE);

    //  Tim3_ClearAllIntFlag();
    //  Tim3_Mode23_EnableIrq(Tim3UevIrq);

    u16ArrValue = 1600;
    Tim3_M23_ARRSet(u16ArrValue, TRUE);

    u16CompareBValue = 1800;

    //  Tim3_M23_CCR_Set(Tim3CCR0A, u16CompareAValue);
    Tim3_M23_CCR_Set(Tim3CCR0B, u16CompareBValue);           // PA07

    //  Tim3_M23_CCR_Set(Tim3CCR1A, u16CompareAValue);
    Tim3_M23_CCR_Set(Tim3CCR1B, u16CompareBValue);           // PB00

    Tim3_M23_CCR_Set(Tim3CCR2A, u16CompareBValue);

    Tim3_M23_CCR_Set(Tim3CCR2B, u16CompareBValue);

    stcTim3PortCmpCfg.enCHxACmpCtrl   = Tim3PWMMode2;
    stcTim3PortCmpCfg.enCHxAPolarity  = Tim3PortPositive;
    stcTim3PortCmpCfg.bCHxACmpBufEn   = TRUE;
    stcTim3PortCmpCfg.enCHxACmpIntSel = Tim3CmpIntNone;

    stcTim3PortCmpCfg.enCHxBCmpCtrl   = Tim3PWMMode2;
    stcTim3PortCmpCfg.enCHxBPolarity  = Tim3PortPositive;
    stcTim3PortCmpCfg.bCHxBCmpBufEn   = TRUE;
    stcTim3PortCmpCfg.enCHxBCmpIntSel = Tim3CmpIntNone;

    Tim3_M23_PortOutput_Config(Tim3CH0, &stcTim3PortCmpCfg);
    Tim3_M23_PortOutput_Config(Tim3CH1, &stcTim3PortCmpCfg);
    Tim3_M23_PortOutput_Config(Tim3CH2, &stcTim3PortCmpCfg);

    //  u8ValidPeriod = 1;
    //  Tim3_M23_SetValidPeriod(u8ValidPeriod);

    u16CntValue = 0u;
    Tim3_M23_Cnt16Set(u16CntValue);
    Tim3_M23_EnPWM_Output(TRUE, FALSE);
    Tim3_M23_Run();
}


// PWM6 -> PA11 -> TIM4_CHB -> FUNC(7)
void pwm_init_timer4(void)
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
    Gpio_Init(GpioPortA, GpioPin11, &stcTIM4Port);
    Gpio_SetAfMode(GpioPortA, GpioPin11, GpioAf7);      // PA11 TIM4 CHB

    stcAdtBaseCntCfg.enCntMode = AdtSawtoothMode;
    stcAdtBaseCntCfg.enCntDir =  AdtCntUp;
    stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0Div256;
    Adt_Init(AdtTIM4, &stcAdtBaseCntCfg);

    u16Period = 1600;
    Adt_SetPeriod(AdtTIM4, u16Period);

    u16CompareValue = 1800;
    Adt_SetCompareValue(AdtTIM4, AdtCompareB, u16CompareValue);        // PWM LED 6

    stcAdtTIM4Cfg.enCap = AdtCHxCompareOutput;
    stcAdtTIM4Cfg.bOutEn = TRUE;
    stcAdtTIM4Cfg.enPerc = AdtCHxPeriodLow;
    stcAdtTIM4Cfg.enCmpc = AdtCHxCompareHigh;
    stcAdtTIM4Cfg.enStaStp = AdtCHxStateSelSS;
    stcAdtTIM4Cfg.enStaOut = AdtCHxPortOutLow;
    stcAdtTIM4Cfg.enStpOut = AdtCHxPortOutLow;
    Adt_CHxXPortConfig(AdtTIM4, AdtCHxB, &stcAdtTIM4Cfg);

    Adt_StartCount(AdtTIM4);
}


// PWM1 -> PA06 -> TIM1_CHA -> FUNC(4)
void PWM_timer1(void)
{
    uint16_t                        u16ArrValue;
    uint16_t                        u16CompareBValue;
    uint16_t                        u16CntValue;

    stc_bt_mode23_config_t          stcTim1BaseCfg;
    stc_gpio_config_t               stcTIM1Port;
    stc_bt_m23_compare_config_t     stcTim1PortCmpCfg;

    DDL_ZERO_STRUCT(stcTim1BaseCfg);
    DDL_ZERO_STRUCT(stcTIM1Port);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stcTIM1Port.enDir  = GpioDirOut;
    Gpio_Init(GpioPortA, GpioPin6, &stcTIM1Port);
    Gpio_SetAfMode(GpioPortA,GpioPin6,GpioAf4);

    stcTim1BaseCfg.enWorkMode    = BtWorkMode2;
    stcTim1BaseCfg.enCT          = BtTimer;
    stcTim1BaseCfg.enPRS         = BtPCLKDiv256;
    stcTim1BaseCfg.enCntDir      = BtCntUp;
    stcTim1BaseCfg.enPWMTypeSel  = BtIndependentPWM;
    stcTim1BaseCfg.enPWM2sSel    = BtSinglePointCmp;
    stcTim1BaseCfg.bOneShot      = FALSE;
    stcTim1BaseCfg.bURSSel       = FALSE;
    stcTim1BaseCfg.pfnTim0Cb     = NULL;
    stcTim1BaseCfg.pfnTim1Cb     = NULL;
    stcTim1BaseCfg.pfnTim2Cb     = NULL;

    Bt_Mode23_Init(TIM1,&stcTim1BaseCfg);

    EnableNvic(TIM1_IRQn, IrqLevel0, FALSE);

    //  Bt_ClearAllIntFlag(TIM1);
    //  Bt_Mode23_EnableIrq(TIM1,BtUevIrq);

    u16ArrValue = 1600;
    Bt_M23_ARRSet(TIM1,u16ArrValue, TRUE);

    u16CompareBValue = 1800;

    Bt_M23_CCR_Set(TIM1,BtCCR0A, u16CompareBValue);

    stcTim1PortCmpCfg.enCH0ACmpCtrl   = BtPWMMode2;
    stcTim1PortCmpCfg.enCH0APolarity  = BtPortPositive;
    stcTim1PortCmpCfg.bCh0ACmpBufEn   = TRUE;
    stcTim1PortCmpCfg.enCh0ACmpIntSel = BtCmpIntNone;

    Bt_M23_PortOutput_Config(TIM1, &stcTim1PortCmpCfg);

    //  u8ValidPeriod = 1;
    //  Tim3_M23_SetValidPeriod(u8ValidPeriod);

    u16CntValue = 0u;
    Bt_M23_Cnt16Set(TIM1,u16CntValue);

    Bt_M23_EnPWM_Output(TIM1,TRUE, FALSE);

    Bt_M23_Run(TIM1);
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
    Adt_Init(AdtTIM5, &stcAdtBaseCntCfg);

    u16Period = 1600;
    Adt_SetPeriod(AdtTIM5, u16Period);

    u16CompareValue = 0;
    Adt_SetCompareValue(AdtTIM5, AdtCompareA, u16CompareValue);
    Adt_SetCompareValue(AdtTIM5, AdtCompareB, u16CompareValue);

    stcAdtTIM5Cfg.enCap = AdtCHxCompareOutput;
    stcAdtTIM5Cfg.bOutEn = TRUE;
    stcAdtTIM5Cfg.enPerc = AdtCHxPeriodLow;
    stcAdtTIM5Cfg.enCmpc = AdtCHxCompareInv;
    stcAdtTIM5Cfg.enStaStp = AdtCHxStateSelKeep;
    stcAdtTIM5Cfg.enStaOut = AdtCHxPortOutLow;
    stcAdtTIM5Cfg.enStpOut = AdtCHxPortOutLow;
    Adt_CHxXPortConfig(AdtTIM5, AdtCHxA, &stcAdtTIM5Cfg);
    Adt_CHxXPortConfig(AdtTIM5, AdtCHxB, &stcAdtTIM5Cfg);

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

    DDL_ZERO_STRUCT(stcTim0BaseCfg);
    DDL_ZERO_STRUCT(stcTIM0Port);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stcTIM0Port.enDir  = GpioDirOut;
    Gpio_Init(GpioPortA, GpioPin0, &stcTIM0Port);
    Gpio_SetAfMode(GpioPortA, GpioPin0, GpioAf7);

    stcTim0BaseCfg.enWorkMode    = BtWorkMode2;
    stcTim0BaseCfg.enCT          = BtTimer;
    stcTim0BaseCfg.enPRS         = BtPCLKDiv256;
    stcTim0BaseCfg.enCntDir      = BtCntUp;
    stcTim0BaseCfg.enPWMTypeSel  = BtIndependentPWM;
    stcTim0BaseCfg.enPWM2sSel    = BtSinglePointCmp;
    stcTim0BaseCfg.bOneShot      = FALSE;
    stcTim0BaseCfg.bURSSel       = FALSE;
    Bt_Mode23_Init(TIM0,&stcTim0BaseCfg);

    Bt_ClearAllIntFlag(TIM0);
    EnableNvic(TIM0_IRQn, IrqLevel0, FALSE);
//    Bt_Mode23_EnableIrq(TIM0,BtUevIrq);

    u16ArrValue = 1600;
    Bt_M23_ARRSet(TIM0,u16ArrValue, TRUE);

    u16CompareValue = 0;
    Bt_M23_CCR_Set(TIM0,BtCCR0A, u16CompareValue);

    stcTim0PortCmpCfg.enCH0ACmpCtrl   = BtPWMMode2;
    stcTim0PortCmpCfg.enCH0APolarity  = BtPortPositive;
    stcTim0PortCmpCfg.bCh0ACmpBufEn   = TRUE;
    stcTim0PortCmpCfg.enCh0ACmpIntSel = BtCmpIntNone;
    Bt_M23_PortOutput_Config(TIM0, &stcTim0PortCmpCfg);

//    u8ValidPeriod = 1;
//    Tim3_M23_SetValidPeriod(u8ValidPeriod);

    u16CntValue = 0u;
    Bt_M23_Cnt16Set(TIM0,u16CntValue);
    Bt_M23_EnPWM_Output(TIM0,TRUE, FALSE);
    Bt_M23_Run(TIM0);
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
    Adt_Init(AdtTIM4, &stcAdtBaseCntCfg);

    u16Period = 1600;
    Adt_SetPeriod(AdtTIM4, u16Period);

    u16CompareValue = 0;
    Adt_SetCompareValue(AdtTIM4, AdtCompareA, u16CompareValue);

    Adt_SetCompareValue(AdtTIM4, AdtCompareB, 1800);

    stcAdtTIM4Cfg.enCap = AdtCHxCompareOutput;
    stcAdtTIM4Cfg.bOutEn = TRUE;
    stcAdtTIM4Cfg.enPerc = AdtCHxPeriodLow;
    stcAdtTIM4Cfg.enCmpc = AdtCHxCompareHigh;
    stcAdtTIM4Cfg.enStaStp = AdtCHxStateSelSS;
    stcAdtTIM4Cfg.enStaOut = AdtCHxPortOutLow;
    stcAdtTIM4Cfg.enStpOut = AdtCHxPortOutLow;
    Adt_CHxXPortConfig(AdtTIM4, AdtCHxA, &stcAdtTIM4Cfg);
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

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE);

    DDL_ZERO_STRUCT(stcTim3BaseCfg);
    DDL_ZERO_STRUCT(stcTIM3Port);

    stcTIM3Port.enDir  = GpioDirOut;

    Gpio_Init(GpioPortB, GpioPin10, &stcTIM3Port);
    Gpio_SetAfMode(GpioPortB, GpioPin10, GpioAf5);

//    stcTim3BaseCfg.enWorkMode    = Tim3WorkMode2;
//    stcTim3BaseCfg.enCT          = Tim3Timer;
//    stcTim3BaseCfg.enPRS         = Tim3PCLKDiv256;
//    stcTim3BaseCfg.enCntDir      = Tim3CntUp;
//    stcTim3BaseCfg.enPWMTypeSel  = Tim3IndependentPWM;
//    stcTim3BaseCfg.enPWM2sSel    = Tim3SinglePointCmp;
//    stcTim3BaseCfg.bOneShot      = FALSE;
//    stcTim3BaseCfg.bURSSel       = FALSE;
//    stcTim3BaseCfg.pfnTim3Cb     = NULL;
//    Tim3_Mode23_Init(&stcTim3BaseCfg);

//    u16ArrValue = 1600;
//    Tim3_M23_ARRSet(u16ArrValue, TRUE);
                            
    u16CompareValue = 0;
    Tim3_M23_CCR_Set(Tim3CCR1A, u16CompareValue);

//    stcTim3PortCmpCfg.enCHxACmpCtrl   = Tim3PWMMode2;
//    stcTim3PortCmpCfg.enCHxAPolarity  = Tim3PortPositive;
//    stcTim3PortCmpCfg.bCHxACmpBufEn   = TRUE;
//    stcTim3PortCmpCfg.enCHxACmpIntSel = Tim3CmpIntNone;

//    Tim3_M23_PortOutput_Config(Tim3CH1, &stcTim3PortCmpCfg);

    u16CntValue = 0u;
    Tim3_M23_Cnt16Set(u16CntValue);
    Tim3_M23_EnPWM_Output(TRUE, FALSE);
    Tim3_M23_Run();
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
    
    Adt_Init(AdtTIM6, &stcAdtBaseCntCfg);

    u16Period = 1600;
    Adt_SetPeriod(AdtTIM6, u16Period);
    u16CompareValue = 0;
    Adt_SetCompareValue(AdtTIM6, AdtCompareA, u16CompareValue);

    stcAdtTIM6Cfg.enCap = AdtCHxCompareOutput;
    stcAdtTIM6Cfg.bOutEn = TRUE;
    stcAdtTIM6Cfg.enPerc = AdtCHxPeriodLow;
    stcAdtTIM6Cfg.enCmpc = AdtCHxCompareInv;
    stcAdtTIM6Cfg.enStaStp = AdtCHxStateSelKeep;
    stcAdtTIM6Cfg.enStaOut = AdtCHxPortOutLow;
    stcAdtTIM6Cfg.enStpOut = AdtCHxPortOutLow;
    Adt_CHxXPortConfig(AdtTIM6, AdtCHxA, &stcAdtTIM6Cfg);

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

