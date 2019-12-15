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

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);       //GPIO ����ʱ��ʹ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE);       //Timer3����ʱ��ʹ��

    DDL_ZERO_STRUCT(stcTim3BaseCfg);                              //�ṹ���ʼ������
    DDL_ZERO_STRUCT(stcTIM3Port);

    stcTIM3Port.enDir  = GpioDirOut;

    Gpio_Init(GpioPortA, GpioPin7, &stcTIM3Port);
    Gpio_SetAfMode(GpioPortA,GpioPin7,GpioAf4);                   //PA07����Ϊ2��TIM3_CH0B

    Gpio_Init(GpioPortB, GpioPin0, &stcTIM3Port);
    Gpio_SetAfMode(GpioPortB,GpioPin0,GpioAf2);                   //PB00����Ϊ3,TIM3_CH1B

    Gpio_Init(GpioPortA, GpioPin10, &stcTIM3Port);
    Gpio_SetAfMode(GpioPortA, GpioPin10, GpioAf2);                // PA10 TIM3_CH2A

    Gpio_Init(GpioPortB, GpioPin1, &stcTIM3Port);
    Gpio_SetAfMode(GpioPortB,GpioPin1,GpioAf3);                   //PB01����Ϊ4,TIM3_CH2B

    stcTim3BaseCfg.enWorkMode    = Tim3WorkMode2;                 //���ǲ�ģʽ
    stcTim3BaseCfg.enCT          = Tim3Timer;                     //��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK
    stcTim3BaseCfg.enPRS         = Tim3PCLKDiv256;                //PCLK
    stcTim3BaseCfg.enCntDir      = Tim3CntUp;                     //���ϼ����������ǲ�ģʽʱֻ��
    stcTim3BaseCfg.enPWMTypeSel  = Tim3IndependentPWM;            //�������PWM
    stcTim3BaseCfg.enPWM2sSel    = Tim3SinglePointCmp;            //����ȽϹ���
    stcTim3BaseCfg.bOneShot      = FALSE;                         //ѭ������
    stcTim3BaseCfg.bURSSel       = FALSE;                         //���������
    stcTim3BaseCfg.pfnTim3Cb     = NULL;                       //�жϺ������
    Tim3_Mode23_Init(&stcTim3BaseCfg);                            //TIM3 ��ģʽ3���ܳ�ʼ��

    EnableNvic(TIM3_IRQn, IrqLevel0, FALSE);                       //TIM3�ж�ʹ��

    //  Tim3_ClearAllIntFlag();                                       //���жϱ�־
    //  Tim3_Mode23_EnableIrq(Tim3UevIrq);                            //ʹ��TIM3 UEV�����ж�

    u16ArrValue = 1600;
    Tim3_M23_ARRSet(u16ArrValue, TRUE);                           //��������ֵ,��ʹ�ܻ���

    u16CompareBValue = 1800;

    //  Tim3_M23_CCR_Set(Tim3CCR0A, u16CompareAValue);              //���ñȽ�ֵA,(PWM����ģʽ��ֻ��Ҫ���ñȽ�ֵA)
    Tim3_M23_CCR_Set(Tim3CCR0B, u16CompareBValue);           // PA07

    //  Tim3_M23_CCR_Set(Tim3CCR1A, u16CompareAValue);
    Tim3_M23_CCR_Set(Tim3CCR1B, u16CompareBValue);           // PB00

    Tim3_M23_CCR_Set(Tim3CCR2A, u16CompareBValue);

    Tim3_M23_CCR_Set(Tim3CCR2B, u16CompareBValue);                //4��

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
    stcTim1BaseCfg.pfnTim0Cb     = NULL;
    stcTim1BaseCfg.pfnTim1Cb     = NULL;
    stcTim1BaseCfg.pfnTim2Cb     = NULL;

    Bt_Mode23_Init(TIM1,&stcTim1BaseCfg);                         //TIM3 ��ģʽ3���ܳ�ʼ��

    EnableNvic(TIM1_IRQn, IrqLevel0, FALSE);                       //TIM3�ж�ʹ��

    //  Bt_ClearAllIntFlag(TIM1);                                     //���жϱ�־
    //  Bt_Mode23_EnableIrq(TIM1,BtUevIrq);                           //ʹ��TIM3 UEV�����ж�

    u16ArrValue = 1600;
    Bt_M23_ARRSet(TIM1,u16ArrValue, TRUE);                        //��������ֵ,��ʹ�ܻ���

    u16CompareBValue = 1800;                                     //Խ���Խ����ռ�ձ�ԽС��

    Bt_M23_CCR_Set(TIM1,BtCCR0A, u16CompareBValue);               //���ñȽ�ֵA,(PWM����ģʽ��ֻ��Ҫ���ñȽ�ֵA)

    stcTim1PortCmpCfg.enCH0ACmpCtrl   = BtPWMMode2;               //OCREFA�������OCMA:PWMģʽ2
    stcTim1PortCmpCfg.enCH0APolarity  = BtPortPositive;           //�������
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
    Adt_Init(AdtTIM5, &stcAdtBaseCntCfg);                      //ADT�ز�������ģʽ��ʱ������

    u16Period = 1600;
    Adt_SetPeriod(AdtTIM5, u16Period);                         //��������

    u16CompareValue = 0;
    Adt_SetCompareValue(AdtTIM5, AdtCompareA, u16CompareValue);  //ͨ�ñȽϻ�׼ֵ�Ĵ���A����
    Adt_SetCompareValue(AdtTIM5, AdtCompareB, u16CompareValue);  //ͨ�ñȽϻ�׼ֵ�Ĵ���B����

    stcAdtTIM5Cfg.enCap = AdtCHxCompareOutput;            //�Ƚ����
    stcAdtTIM5Cfg.bOutEn = TRUE;                          //CHA���ʹ��
    stcAdtTIM5Cfg.enPerc = AdtCHxPeriodLow;               //����ֵ������ƥ��ʱCHA��ƽ���ֲ���
    stcAdtTIM5Cfg.enCmpc = AdtCHxCompareInv;             //����ֵ��Ƚ�ֵAƥ��ʱ��CHA��ƽ��ת
    stcAdtTIM5Cfg.enStaStp = AdtCHxStateSelKeep;            //CHA��ʼ������ƽ��STACA��STPCA����
    stcAdtTIM5Cfg.enStaOut = AdtCHxPortOutLow;            //CHA��ʼ��ƽΪ��
    stcAdtTIM5Cfg.enStpOut = AdtCHxPortOutLow;            //CHA������ƽΪ��
    Adt_CHxXPortConfig(AdtTIM5, AdtCHxA, &stcAdtTIM5Cfg);   //�˿�CHA����
    Adt_CHxXPortConfig(AdtTIM5, AdtCHxB, &stcAdtTIM5Cfg);    //�˿�CHB����

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

    DDL_ZERO_STRUCT(stcTim0BaseCfg);                              //�ṹ���ʼ������
    DDL_ZERO_STRUCT(stcTIM0Port);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE);       //Base Timer����ʱ��ʹ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);       //GPIO ����ʱ��ʹ��

    stcTIM0Port.enDir  = GpioDirOut;
    Gpio_Init(GpioPortA, GpioPin0, &stcTIM0Port);
    Gpio_SetAfMode(GpioPortA, GpioPin0, GpioAf7);

    stcTim0BaseCfg.enWorkMode    = BtWorkMode2;                   //���ǲ�ģʽ
    stcTim0BaseCfg.enCT          = BtTimer;                       //��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK
    stcTim0BaseCfg.enPRS         = BtPCLKDiv256;                  //PCLK
    stcTim0BaseCfg.enCntDir      = BtCntUp;                       //���ϼ����������ǲ�ģʽʱֻ��
    stcTim0BaseCfg.enPWMTypeSel  = BtIndependentPWM ;             //�������PWM
    stcTim0BaseCfg.enPWM2sSel    = BtSinglePointCmp;              //����ȽϹ���
    stcTim0BaseCfg.bOneShot      = FALSE;                         //ѭ������
    stcTim0BaseCfg.bURSSel       = FALSE;                         //���������
    Bt_Mode23_Init(TIM0,&stcTim0BaseCfg);                         //TIM3 ��ģʽ3���ܳ�ʼ��

    Bt_ClearAllIntFlag(TIM0);                                     //���жϱ�־
    EnableNvic(TIM0_IRQn, IrqLevel0, FALSE);                       //TIM3�ж�ʹ��
//    Bt_Mode23_EnableIrq(TIM0,BtUevIrq);                           //ʹ��TIM3 UEV�����ж�

    u16ArrValue = 1600;
    Bt_M23_ARRSet(TIM0,u16ArrValue, TRUE);                        //��������ֵ,��ʹ�ܻ���

    u16CompareValue = 0;                                     //Խ���Խ����ռ�ձ�ԽС��
    Bt_M23_CCR_Set(TIM0,BtCCR0A, u16CompareValue);               //���ñȽ�ֵA,(PWM����ģʽ��ֻ��Ҫ���ñȽ�ֵA)

    stcTim0PortCmpCfg.enCH0ACmpCtrl   = BtPWMMode2;               //OCREFA�������OCMA:PWMģʽ2
    stcTim0PortCmpCfg.enCH0APolarity  = BtPortPositive;           //�������
    stcTim0PortCmpCfg.bCh0ACmpBufEn   = TRUE;                     //Aͨ���������
    stcTim0PortCmpCfg.enCh0ACmpIntSel = BtCmpIntNone;             //Aͨ���ȽϿ���:��
    Bt_M23_PortOutput_Config(TIM0, &stcTim0PortCmpCfg);           //�Ƚ�����˿�����

//    u8ValidPeriod = 1;                                        //�¼������������ã�0��ʾ���ǲ�ÿ������ڸ���һ�Σ�ÿ+1�����ӳٰ������
//    Tim3_M23_SetValidPeriod(u8ValidPeriod);                   //�����������

    u16CntValue = 0u;
    Bt_M23_Cnt16Set(TIM0,u16CntValue);                            //���ü�����ֵ
    Bt_M23_EnPWM_Output(TIM0,TRUE, FALSE);                        //�˿����ʹ��
    Bt_M23_Run(TIM0);                                             //���С�
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
    Adt_Init(AdtTIM4, &stcAdtBaseCntCfg);                      //ADT�ز�������ģʽ��ʱ������

    u16Period = 1600;
    Adt_SetPeriod(AdtTIM4, u16Period);                         //��������

    u16CompareValue = 0;
    Adt_SetCompareValue(AdtTIM4, AdtCompareA, u16CompareValue);  //ͨ�ñȽϻ�׼ֵ�Ĵ���A����

    Adt_SetCompareValue(AdtTIM4, AdtCompareB, 1800);        // PWM LED 6

    stcAdtTIM4Cfg.enCap = AdtCHxCompareOutput;            //�Ƚ����
    stcAdtTIM4Cfg.bOutEn = TRUE;                          //CHA���ʹ��
    stcAdtTIM4Cfg.enPerc = AdtCHxPeriodLow;               //����ֵ������ƥ��ʱCHA��ƽ���ֲ���
    stcAdtTIM4Cfg.enCmpc = AdtCHxCompareHigh;             //����ֵ��Ƚ�ֵAƥ��ʱ��CHA��ƽ��ת
    stcAdtTIM4Cfg.enStaStp = AdtCHxStateSelSS;            //CHA��ʼ������ƽ��STACA��STPCA����
    stcAdtTIM4Cfg.enStaOut = AdtCHxPortOutLow;            //CHA��ʼ��ƽΪ��
    stcAdtTIM4Cfg.enStpOut = AdtCHxPortOutLow;            //CHA������ƽΪ��
    Adt_CHxXPortConfig(AdtTIM4, AdtCHxA, &stcAdtTIM4Cfg);   //�˿�CHA����
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

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);       //GPIO ����ʱ��ʹ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE);       //Timer3����ʱ��ʹ��

    DDL_ZERO_STRUCT(stcTim3BaseCfg);                              //�ṹ���ʼ������
    DDL_ZERO_STRUCT(stcTIM3Port);

    stcTIM3Port.enDir  = GpioDirOut;

    Gpio_Init(GpioPortB, GpioPin10, &stcTIM3Port);
    Gpio_SetAfMode(GpioPortB, GpioPin10, GpioAf5);

//    stcTim3BaseCfg.enWorkMode    = Tim3WorkMode2;                 //���ǲ�ģʽ
//    stcTim3BaseCfg.enCT          = Tim3Timer;                     //��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK
//    stcTim3BaseCfg.enPRS         = Tim3PCLKDiv256;                //PCLK
//    stcTim3BaseCfg.enCntDir      = Tim3CntUp;                     //���ϼ����������ǲ�ģʽʱֻ��
//    stcTim3BaseCfg.enPWMTypeSel  = Tim3IndependentPWM;            //�������PWM
//    stcTim3BaseCfg.enPWM2sSel    = Tim3SinglePointCmp;            //����ȽϹ���
//    stcTim3BaseCfg.bOneShot      = FALSE;                         //ѭ������
//    stcTim3BaseCfg.bURSSel       = FALSE;                         //���������
//    stcTim3BaseCfg.pfnTim3Cb     = NULL;
//    Tim3_Mode23_Init(&stcTim3BaseCfg);                            //TIM3 ��ģʽ3���ܳ�ʼ��

//    u16ArrValue = 1600;
//    Tim3_M23_ARRSet(u16ArrValue, TRUE);                           //��������ֵ,��ʹ�ܻ���
                            
    u16CompareValue = 0;                                   //Խ���Խ����ռ�ձ�ԽС��
    Tim3_M23_CCR_Set(Tim3CCR1A, u16CompareValue);

//    stcTim3PortCmpCfg.enCHxACmpCtrl   = Tim3PWMMode2;             //OCREFA�������OCMA:PWMģʽ2
//    stcTim3PortCmpCfg.enCHxAPolarity  = Tim3PortPositive;         //�������
//    stcTim3PortCmpCfg.bCHxACmpBufEn   = TRUE;                     //Aͨ���������
//    stcTim3PortCmpCfg.enCHxACmpIntSel = Tim3CmpIntNone;           //Aͨ���ȽϿ���:��

//    Tim3_M23_PortOutput_Config(Tim3CH1, &stcTim3PortCmpCfg);      //�Ƚ�����˿�����

    u16CntValue = 0u;
    Tim3_M23_Cnt16Set(u16CntValue);                               //���ü�����ֵ
    Tim3_M23_EnPWM_Output(TRUE, FALSE);                           //�˿����ʹ��
    Tim3_M23_Run();                                               //���С�
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
    
    Adt_Init(AdtTIM6, &stcAdtBaseCntCfg);                      //ADT�ز�������ģʽ��ʱ������

    u16Period = 1600;
    Adt_SetPeriod(AdtTIM6, u16Period);                         //��������

    u16CompareValue = 0;
    Adt_SetCompareValue(AdtTIM6, AdtCompareA, u16CompareValue);  //ͨ�ñȽϻ�׼ֵ�Ĵ���A����

    stcAdtTIM6Cfg.enCap = AdtCHxCompareOutput;            //�Ƚ����
    stcAdtTIM6Cfg.bOutEn = TRUE;                          //CHA���ʹ��
    stcAdtTIM6Cfg.enPerc = AdtCHxPeriodLow;               //����ֵ������ƥ��ʱCHA��ƽ���ֲ���
    stcAdtTIM6Cfg.enCmpc = AdtCHxCompareInv;             //����ֵ��Ƚ�ֵAƥ��ʱ��CHA��ƽ��ת
    stcAdtTIM6Cfg.enStaStp = AdtCHxStateSelKeep;            //CHA��ʼ������ƽ��STACA��STPCA����
    stcAdtTIM6Cfg.enStaOut = AdtCHxPortOutLow;            //CHA��ʼ��ƽΪ��
    stcAdtTIM6Cfg.enStpOut = AdtCHxPortOutLow;            //CHA������ƽΪ��
    Adt_CHxXPortConfig(AdtTIM6, AdtCHxA, &stcAdtTIM6Cfg);   //�˿�CHA����

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

