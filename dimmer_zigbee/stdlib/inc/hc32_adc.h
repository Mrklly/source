/******************************************************************************
* Copyright (C) 2017, Huada Semiconductor Co.,Ltd All rights reserved.
*
* This software is owned and published by:
* Huada Semiconductor Co.,Ltd ("HDSC").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with HDSC
* components. This software is licensed by HDSC to be adapted only
* for use in systems utilizing HDSC components. HDSC shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. HDSC is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* Disclaimer:
* HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
* WARRANTY OF NONINFRINGEMENT.
* HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
* SAVINGS OR PROFITS,
* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
* FROM, THE SOFTWARE.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/
/******************************************************************************/
/** \file adc.h
 **
 ** Header file for AD Converter functions
 ** @link ADC Group Some description @endlink
 **
 **   - 2017-06-28 Alex    First Version
 **
 ******************************************************************************/

#ifndef __ADC_H__
#define __ADC_H__


/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "ddl.h"
#include "interrupts_hc32f005.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup AdcGroup AD Converter (ADC)
  **
 ******************************************************************************/
 
//@{

/******************************************************************************
 * Global definitions
 ******************************************************************************/
#define ADC_SCAN_CH0_EN     (0x1u)           /*!< SCANģʽʹ��ADC CH0 */
#define ADC_SCAN_CH1_EN     (0x1u<<1)        /*!< SCANģʽʹ��ADC CH1 */
#define ADC_SCAN_CH2_EN     (0x1u<<2)        /*!< SCANģʽʹ��ADC CH2 */
#define ADC_SCAN_CH3_EN     (0x1u<<3)        /*!< SCANģʽʹ��ADC CH3 */
#define ADC_SCAN_CH4_EN     (0x1u<<4)        /*!< SCANģʽʹ��ADC CH4 */
#define ADC_SCAN_CH5_EN     (0x1u<<5)        /*!< SCANģʽʹ��ADC CH5 */
#define ADC_SCAN_CH6_EN     (0x1u<<6)        /*!< SCANģʽʹ��ADC CH6 */
#define ADC_SCAN_CH7_EN     (0x1u<<7)        /*!< SCANģʽʹ��ADC CH7 */


/******************************************************************************
 ** Global type definitions
 *****************************************************************************/

 /**
 ******************************************************************************
 ** \brief ADC����ģʽ
 *****************************************************************************/
typedef enum en_adc_op_mode
{
    AdcNormalMode  = 0,        /*!< ������ͨ�����β���ģʽ */
    AdcContMode    = 1,        /*!< ������ͨ����������ģʽ */
    AdcScanMode    = 2,        /*!< ������ͨ��ɨ�����ģʽ*/

} en_adc_op_mode_t;

/**
 ******************************************************************************
 ** \brief ADCʱ��ѡ��
 *****************************************************************************/
typedef enum en_adc_clk_sel
{
    AdcClkSysTDiv1  = 0,        /*!< PCLK */
    AdcClkSysTDiv2  = 1,        /*!< 1/2 PCLK */
    AdcClkSysTDiv4  = 2,        /*!< 1/4 PCLK */
    AdcClkSysTDiv8  = 3,        /*!< 1/8 PCLK */

} en_adc_clk_sel_t;

/**
 ******************************************************************************
 ** \brief ADC�ο���ѹ
 *****************************************************************************/
typedef enum en_adc_ref_vol_sel
{
    RefVolSelInBgr1p5 = 0,        /*!<�ڲ��ο���ѹ1.5V(SPS<=200kHz)*/
    RefVolSelInBgr2p5 = 1,        /*!<�ڲ��ο���ѹ2.5V(avdd>3V,SPS<=200kHz)*/
    RefVolSelExtern1  = 2,        /*!<�ⲿ����(max avdd)   P3.6*/
    RefVolSelAVDD     = 3,        /*!<AVDD*/

}en_adc_ref_vol_sel_t;

/**
 ******************************************************************************
 ** \brief ADC����ͨ��ѡ��
 *****************************************************************************/
typedef enum en_adc_samp_ch_sel
{
    AdcExInputCH0    =  0,        /*!<ʹ��ͨ��0����P2.4*/
    AdcExInputCH1    =  1,        /*!<ʹ��ͨ��1����P2.6*/
    AdcExInputCH2    =  2,        /*!<ʹ��ͨ��2����P3.2*/
    AdcExInputCH3    =  3,        /*!<ʹ��ͨ��3����P3.3*/
    AdcExInputCH4    =  4,        /*!<ʹ��ͨ��4����P3.4*/
    AdcExInputCH5    =  5,        /*!<ʹ��ͨ��5����P3.5*/
    AdcExInputCH6    =  6,        /*!<ʹ��ͨ��6����P3.6*/
    AdcExInputCH7    =  7,        /*!<ʹ��ͨ��7����P0.1*/
    AdcExInputCH8    =  8,        /*!<ʹ��ͨ��8����P0.2*/
    AdcAVccDiV3Input =  9,        /*!<ʹ��1/3 AVCC(����ʹ����������)*/
    AdcAiTsInput     =  10,       /*!<ʹ��AI_TS(����ʹ����������)*/
    AdcVref1P2Input  =  11,       /*!<ʹ��Vref1P2(����ʹ����������)*/

}en_adc_samp_ch_sel_t;

/**
 ******************************************************************************
 ** \brief ADC����������ѡ��
 *****************************************************************************/
typedef enum en_adc_samp_time_sel
{
    AdcSampTime4Clk  =  0,        /*!<4������ʱ��*/
    AdcSampTime6Clk  =  1,        /*!<6������ʱ��*/
    AdcSampTime8Clk  =  2,        /*!<8������ʱ��*/
    AdcSampTime12Clk =  3,        /*!<12������ʱ��*/

}en_adc_samp_time_sel_t;

/**
 ******************************************************************************
 ** \brief ADC�ܱ�ģ�鷴��Դѡ��
 *****************************************************************************/
typedef enum en_adc_trig_sel
{
    AdcTrigDisable    =  0,         /*!<�����ܱ�ģ�鷴��Դ*/
    AdcTrigTimer0     =  1,         /*!<ѡ��timer0�ж�Դ���Զ�����ADC����*/
    AdcTrigTimer1     =  2,         /*!<ѡ��timer1�ж�Դ���Զ�����ADC����*/
    AdcTrigTimer2     =  3,         /*!<ѡ��timer2�ж�Դ���Զ�����ADC����*/
    AdcTrigTimer4     =  5,         /*!<ѡ��timer4�ж�Դ���Զ�����ADC����*/
    AdcTrigTimer5     =  6,         /*!<ѡ��timer5�ж�Դ���Զ�����ADC����*/
    AdcTrigTimer6     =  7,         /*!<ѡ��timer6�ж�Դ���Զ�����ADC����*/
    AdcTrigUart0      =  8,         /*!<ѡ��uart0�ж�Դ���Զ�����ADC����*/
    AdcTrigUart1      =  9,         /*!<ѡ��uart1�ж�Դ���Զ�����ADC����*/
    AdcTrigVC1        =  11,        /*!<ѡ��VC1�ж�Դ���Զ�����ADC����*/
    AdcTrigVC2        =  12,        /*!<ѡ��VC2�ж�Դ���Զ�����ADC����*/
    AdcTrigPCA        =  14,        /*!<ѡ��PCA�ж�Դ���Զ�����ADC����*/
    AdcTrigSPI        =  15,        /*!<ѡ��SPI�ж�Դ���Զ�����ADC����*/
    AdcTrigP01        =  16,        /*!<ѡ��P01�ж�Դ���Զ�����ADC����*/
    AdcTrigP02        =  17,        /*!<ѡ��P02�ж�Դ���Զ�����ADC����*/
    AdcTrigP03        =  18,        /*!<ѡ��P03�ж�Դ���Զ�����ADC����*/
    AdcTrigP14        =  19,        /*!<ѡ��P14�ж�Դ���Զ�����ADC����*/
    AdcTrigP15        =  20,        /*!<ѡ��P15�ж�Դ���Զ�����ADC����*/
    AdcTrigP23        =  21,        /*!<ѡ��P23�ж�Դ���Զ�����ADC����*/
    AdcTrigP24        =  22,        /*!<ѡ��P24�ж�Դ���Զ�����ADC����*/
    AdcTrigP25        =  23,        /*!<ѡ��P25�ж�Դ���Զ�����ADC����*/
    AdcTrigP26        =  24,        /*!<ѡ��P26�ж�Դ���Զ�����ADC����*/
    AdcTrigP27        =  25,        /*!<ѡ��P27�ж�Դ���Զ�����ADC����*/
    AdcTrigP31        =  26,        /*!<ѡ��P31�ж�Դ���Զ�����ADC����*/
    AdcTrigP32        =  27,        /*!<ѡ��P32�ж�Դ���Զ�����ADC����*/
    AdcTrigP33        =  28,        /*!<ѡ��P33�ж�Դ���Զ�����ADC����*/
    AdcTrigP34        =  29,        /*!<ѡ��P34�ж�Դ���Զ�����ADC����*/
    AdcTrigP35        =  30,        /*!<ѡ��P35�ж�Դ���Զ�����ADC����*/
    AdcTrigP36        =  31,        /*!<ѡ��P36�ж�Դ���Զ�����ADC����*/

}en_adc_trig_sel_t;

/******************************************************************************
 * Extern type definitions ('typedef')                                        *
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief  ADC����
 *****************************************************************************/
typedef struct stc_adc_cfg
{
    /*! ADC����ģʽ*/
    en_adc_op_mode_t enAdcOpMode;

    /*! ADCʱ��ѡ��*/
    en_adc_clk_sel_t  enAdcClkSel;

    /*! ADC����ʱ��*/
    en_adc_samp_time_sel_t  enAdcSampTimeSel;

    /*! ADC�ο���ѹ*/
    en_adc_ref_vol_sel_t    enAdcRefVolSel;

    /*! ADC��������ʹ��*/
    boolean_t    bAdcInBufEn;

    /*!ADC����ģʽ����ֵ*/
    uint32_t    u32AdcRegHighThd;

    /*!ADC����ģʽ����ֵ*/
    uint32_t    u32AdcRegLowThd;

    /*! ADC����0ѡ��*/
    en_adc_trig_sel_t enAdcTrig0Sel;

    /*! ADC����1ѡ��*/
    en_adc_trig_sel_t enAdcTrig1Sel;

}stc_adc_cfg_t;

/**
 ******************************************************************************
 ** \brief  ADC���β���ģʽ����
 *****************************************************************************/
typedef struct stc_adc_norm_cfg
{
    /*! ADCͨ��ѡ��*/
    en_adc_samp_ch_sel_t    enAdcNormModeCh;
    /*! ADC����ۼ�ʹ��*/
    boolean_t  bAdcResultAccEn;

}stc_adc_norm_cfg_t;

/**
 ******************************************************************************
 ** \brief  ADC��ͨ����������ģʽ����
 *****************************************************************************/
typedef struct stc_adc_cont_cfg
{
    /*! ADCͨ��ѡ��*/
    en_adc_samp_ch_sel_t    enAdcContModeCh;
    /*! ADC��������*/
    uint8_t    u8AdcSampCnt;
    /*! ADC����ۼ�ʹ��*/
    boolean_t  bAdcResultAccEn;

}stc_adc_cont_cfg_t;

/**
 ******************************************************************************
 ** \brief  ADCɨ�����ģʽ����
 *****************************************************************************/
typedef struct stc_adc_scan_cfg
{
    /*! ADCͨ��ѡ��*/
    uint8_t    u8AdcScanModeCh;
    /*! ADC��������*/
    uint8_t    u8AdcSampCnt;

}stc_adc_scan_cfg_t;

/**
 ******************************************************************************
 ** \brief  ADC�ж�����
 *****************************************************************************/
typedef struct stc_adc_irq
{
    /*!<ADC���������ж�*/
    boolean_t  bAdcIrq;
    /*!ADC����*/
    boolean_t  bAdcRegCmp;
    /*!ADC�ϳ�������*/
    boolean_t  bAdcHhtCmp;
    /*!ADC�³�������*/
    boolean_t  bAdcLltCmp;

}stc_adc_irq_t;

/**
 ******************************************************************************
 ** \brief  ADC�жϻص�����
 *****************************************************************************/
typedef struct stc_adc_irq_calbakfn_pt
{
    /*! ADC���������жϻص�����ָ��*/
    func_ptr_t  pfnAdcContIrq;
    /*! ADC�����жϻص�����ָ��*/
    func_ptr_t  pfnAdcRegIrq;
    /*! ADC�ϳ��������жϻص�����ָ��*/
    func_ptr_t  pfnAdcHhtIrq;
    /*! ADC�³��������жϻص�����ָ��*/
    func_ptr_t  pfnAdcLltIrq;

}stc_adc_irq_calbakfn_pt_t;

/******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
//ADC initialization
en_result_t Adc_Init(stc_adc_cfg_t* pstcAdcConfig);
//ADC de-init
void Adc_DeInit(void);

//ADC conversion start
void Adc_Start(void);
//ADC conversion stop
void Adc_Stop(void);

//ADC conversion enable
void Adc_Enable(void);
//ADC conversion disable
void Adc_Disable(void);

//ADC normal mode configuration
en_result_t Adc_ConfigNormMode(stc_adc_cfg_t* pstcAdcConfig, stc_adc_norm_cfg_t* pstcAdcNormCfg);
//ADC continuous mode configuration
en_result_t Adc_ConfigContMode(stc_adc_cfg_t* pstcAdcConfig, stc_adc_cont_cfg_t* pstcAdcContCfg);
//ADC scan mode configuration
en_result_t Adc_ConfigScanMode(stc_adc_cfg_t* pstcAdcConfig, stc_adc_scan_cfg_t* pstcAdcScanCfg);

//ADC IRQ configuration
void Adc_ConfigIrq(stc_adc_irq_t* pstcAdcIrqCfg,
                   stc_adc_irq_calbakfn_pt_t* pstcAdcIrqCalbaks);
//ADC enable IRQ
void Adc_EnableIrq(void);
//ADC disable IRQ
void Adc_DisableIrq(void);
//ADC enable compare
void Adc_CmpCfg(stc_adc_irq_t* pstcAdcIrqCfg);
//ADC get IRQ state
void Adc_GetIrqState(stc_adc_irq_t* pstcAdcIrqState);
//ADC clear IRQ states
void Adc_ClrContIrqState(void);
void Adc_ClrRegIrqState(void);
void Adc_ClrHhtIrqState(void);
void Adc_ClrIrqLltState(void);

//��ѯADCת��״̬
boolean_t Adc_PollBusyState(void);
//��ȡ����ֵ
en_result_t Adc_GetResult(uint16_t* pu16AdcResult);
//��ȡ�ۼӲ���ֵ
en_result_t Adc_GetAccResult(uint32_t* pu32AdcAccResult);
//clear ADC accumulated result
void Adc_ClrAccResult(void);
//get ADC scan channel result
en_result_t Adc_GetScanResult(uint8_t u8Channel, uint16_t* pu16AdcResult);
//��ȡͨ��8����ֵ
en_result_t Adc_GetCH8Result(uint16_t* pu16AdcResult);

en_result_t Adc_SetVref(en_adc_ref_vol_sel_t enAdcRefVolSel);

//@}
#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
