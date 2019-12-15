#include "adc.h"

static stc_adc_irq_t stcAdcIrqFlag = {0, 0, 0, 0, 0, 0};
volatile uint8_t adc_flag = 0;
uint16_t u16AdcRestult = 0;
uint32_t u32AdcRestultAcc = 0;


void AdcContIrqCallback(void)
{
    Adc_GetSqrResult(&u16AdcRestult, 0);

    Adc_GetAccResult(&u32AdcRestultAcc);

    adc_flag = 1;
    stcAdcIrqFlag.bAdcSQRIrq = TRUE;

    Adc_ClrAccResult();
    Adc_SQR_Start();
}

void adc_init(void)
{
    uint8_t                    u8AdcScanCnt;

    stc_adc_cfg_t              stcAdcCfg;
    stc_adc_irq_t              stcAdcIrq;
    stc_adc_irq_calbakfn_pt_t  stcAdcIrqCalbaks;
    stc_gpio_config_t          stcAdcAN2Port;

    DDL_ZERO_STRUCT(stcAdcCfg);
    DDL_ZERO_STRUCT(stcAdcIrq);
    DDL_ZERO_STRUCT(stcAdcIrqCalbaks);
    DDL_ZERO_STRUCT(stcAdcIrqFlag);
    DDL_ZERO_STRUCT(stcAdcAN2Port);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE);

    Gpio_SetAnalogMode(GpioPortA, GpioPin2);        //PA02 (AIN2)
    stcAdcAN2Port.enDir = GpioDirIn;
    Gpio_Init(GpioPortA, GpioPin2, &stcAdcAN2Port);

    Adc_Enable();
    M0P_BGR->CR_f.BGR_EN = 0x1u;                 //BGR必须使能
    M0P_BGR->CR_f.TS_EN = 0x0u;
    delay_us(500);

    stcAdcCfg.enAdcOpMode = AdcSCanMode;         //扫描模式
    stcAdcCfg.enAdcClkDiv = AdcClkSysTDiv8;
    stcAdcCfg.enAdcSampTimeSel = AdcSampTime12Clk;
    stcAdcCfg.enAdcRefVolSel   = RefVolSelAVDD;
    stcAdcCfg.bAdcInBufEn      = FALSE;

    Adc_Init(&stcAdcCfg);

    Adc_ConfigSqrChannel(CH0MUX, AdcExInputCH2);

    EnableNvic(ADC_IRQn, IrqLevel3, TRUE);

    Adc_EnableIrq();

    stcAdcIrq.bAdcSQRIrq = TRUE;
    stcAdcIrqCalbaks.pfnAdcSQRIrq = AdcContIrqCallback;

    Adc_ConfigIrq(&stcAdcIrq, &stcAdcIrqCalbaks);

    u8AdcScanCnt = 1;

    Adc_ConfigSqrMode(&stcAdcCfg, u8AdcScanCnt, TRUE);//使能ADC转换结果自动累加

    Adc_SQR_Start();
}
