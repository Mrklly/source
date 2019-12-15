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
#include "hc32_ddl.h"
#include "hc32_it.h"

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
    AdcSglMode  = 0u,        /*!< ������ͨ�����β���ģʽ */
    AdcSCanMode     = 1u,        /*!< ������ͨ��˳��ɨ�����ģʽ,������ͨ�����ɨ�����ģʽ*/   
} en_adc_op_mode_t;

/**
 ******************************************************************************
 ** \brief ADCʱ��ѡ��
 *****************************************************************************/
typedef enum en_adc_clk_sel
{
    AdcClkSysTDiv1  = 0u,        /*!< PCLK */
    AdcClkSysTDiv2  = 1u,        /*!< 1/2 PCLK */
    AdcClkSysTDiv4  = 2u,        /*!< 1/4 PCLK */
    AdcClkSysTDiv8  = 3u,        /*!< 1/8 PCLK */

} en_adc_clk_div_t;

/**
 ******************************************************************************
 ** \brief ADC�ο���ѹ
 *****************************************************************************/
typedef enum en_adc_ref_vol_sel
{
    RefVolSelInBgr1p5 = 0u,        /*!<�ڲ��ο���ѹ1.5V(SPS<=200kHz)*/
    RefVolSelInBgr2p5 = 1u,        /*!<�ڲ��ο���ѹ2.5V(avdd>3V,SPS<=200kHz)*/
    RefVolSelExtern1  = 2u,        /*!<�ⲿ����(max avdd)   PB01*/
    RefVolSelAVDD     = 3u,        /*!<AVDD*/

}en_adc_ref_vol_sel_t;

/**
 ******************************************************************************
 ** \brief ADC����ͨ��ѡ��
 *****************************************************************************/
typedef enum en_adc_samp_ch_sel
{
    AdcExInputCH0    =  0u,        /*!<ʹ��ͨ��0����PA00*/
    AdcExInputCH1    =  1u,        /*!<ʹ��ͨ��1����PA01*/
    AdcExInputCH2    =  2u,        /*!<ʹ��ͨ��2����PA02*/
    AdcExInputCH3    =  3u,        /*!<ʹ��ͨ��3����PA03*/
    AdcExInputCH4    =  4u,        /*!<ʹ��ͨ��4����PA04*/
    AdcExInputCH5    =  5u,        /*!<ʹ��ͨ��5����PA05*/
    AdcExInputCH6    =  6u,        /*!<ʹ��ͨ��6����PA06*/
    AdcExInputCH7    =  7u,        /*!<ʹ��ͨ��7����PA07*/
    AdcExInputCH8    =  8u,        /*!<ʹ��ͨ��8����PB00*/
    AdcExInputCH9    =  9u,        /*!<ʹ��ͨ��8����PB01*/
    AdcExInputCH10   =  10u,       /*!<ʹ��ͨ��8����PC00*/
    AdcExInputCH11   =  11u,       /*!<ʹ��ͨ��1����PC01*/
    AdcExInputCH12   =  12u,       /*!<ʹ��ͨ��2����PC02*/
    AdcExInputCH13   =  13u,       /*!<ʹ��ͨ��3����PC03*/
    AdcExInputCH14   =  14u,       /*!<ʹ��ͨ��4����PC04*/
    AdcExInputCH15   =  15u,       /*!<ʹ��ͨ��5����PC05*/
    AdcExInputCH16   =  16u,       /*!<ʹ��ͨ��6����PB02*/
    AdcExInputCH17   =  17u,       /*!<ʹ��ͨ��7����PB10*/
    AdcExInputCH18   =  18u,       /*!<ʹ��ͨ��8����PB11*/
    AdcExInputCH19   =  19u,       /*!<ʹ��ͨ��8����PB12*/
    AdcExInputCH20   =  20u,       /*!<ʹ��ͨ��7����PB13*/
    AdcExInputCH21   =  21u,       /*!<ʹ��ͨ��8����PB14*/
    AdcExInputCH22   =  22u,       /*!<ʹ��ͨ��8����PB15*/
    AdcExInputCH23   =  23u,       /*!<ʹ��ͨ��8����PC06*/  
    AdcOPA0Input     =  24u,       /*!<ʹ��ͨ��8����OPA0*/
    AdcOPA1Input     =  25u,       /*!<ʹ��ͨ��8����OPA1*/
    AdcOPA2Input     =  26u,       /*!<ʹ��ͨ��8����OPA2*/                     
    AdcAVccDiV3Input =  27u,       /*!<ʹ��1/3 AVCC(����ʹ����������)*/
    AdcAiTsInput     =  28u,       /*!<ʹ��BGR_TS(����ʹ����������)*/
    AdcVref1P2Input  =  29u,       /*!<ʹ��Vref1P2(����ʹ����������)*/

}en_adc_samp_ch_sel_t;

/**
 ******************************************************************************
 ** \brief ADC����������ѡ��
 *****************************************************************************/
typedef enum en_adc_samp_time_sel
{
    AdcSampTime4Clk  =  0u,        /*!<4������ʱ��*/
    AdcSampTime6Clk  =  1u,        /*!<6������ʱ��*/
    AdcSampTime8Clk  =  2u,        /*!<8������ʱ��*/
    AdcSampTime12Clk =  3u,        /*!<12������ʱ��*/

}en_adc_samp_time_sel_t;

/**
 ******************************************************************************
 ** \brief ADC�ܱ�ģ�鷴��Դѡ��
 *****************************************************************************/
typedef enum en_adc_trig_sel
{
    AdcTrigTimer0     =  0u,         /*!<ѡ��timer0�ж�Դ���Զ�����ADC����*/
    AdcTrigTimer1     =  1u,         /*!<ѡ��timer1�ж�Դ���Զ�����ADC����*/
    AdcTrigTimer2     =  2u,         /*!<ѡ��timer2�ж�Դ���Զ�����ADC����*/
    AdcTrigTimer3     =  3u,         /*!<ѡ��timer3�ж�Դ���Զ�����ADC����*/
    AdcTrigTimer4     =  4u,         /*!<ѡ��timer4�ж�Դ���Զ�����ADC����*/
    AdcTrigTimer5     =  5u,         /*!<ѡ��timer5�ж�Դ���Զ�����ADC����*/
    AdcTrigTimer6     =  6u,         /*!<ѡ��timer6�ж�Դ���Զ�����ADC����*/
    AdcTrigUart0      =  7u,         /*!<ѡ��uart0�ж�Դ���Զ�����ADC����*/
    AdcTrigUart1      =  8u,         /*!<ѡ��uart1�ж�Դ���Զ�����ADC����*/
    AdcTrigLpuart0    =  9u,         /*!<ѡ��lpuart0�ж�Դ���Զ�����ADC����*/
    AdcTrigLpuart1    =  10u,        /*!<ѡ��lpuart1�ж�Դ���Զ�����ADC����*/
    AdcTrigVC0        =  11u,        /*!<ѡ��VC0�ж�Դ���Զ�����ADC����*/
    AdcTrigVC1        =  12u,        /*!<ѡ��VC1�ж�Դ���Զ�����ADC����*/
    AdcTrigRTC        =  13u,        /*!<ѡ��RTC�ж�Դ���Զ�����ADC����*/
    AdcTrigPCA        =  14u,        /*!<ѡ��PCA�ж�Դ���Զ�����ADC����*/
    AdcTrigSPI0       =  15u,        /*!<ѡ��SPI0�ж�Դ���Զ�����ADC����*/
    AdcTrigSPI1       =  16u,        /*!<ѡ��SPI1�ж�Դ���Զ�����ADC����*/
    AdcTrigDMA        =  17u,        /*!<ѡ��DMA�ж�Դ���Զ�����ADC����*/
    AdcTrigPA03       =  18u,        /*!<ѡ��PA03�ж�Դ���Զ�����ADC����*/
    AdcTrigPB03       =  19u,        /*!<ѡ��PB03�ж�Դ���Զ�����ADC����*/
    AdcTrigPC03       =  20u,        /*!<ѡ��PC03�ж�Դ���Զ�����ADC����*/
    AdcTrigPD03       =  21u,        /*!<ѡ��PD03�ж�Դ���Զ�����ADC����*/
    AdcTrigPA07       =  22u,        /*!<ѡ��PA07�ж�Դ���Զ�����ADC����*/
    AdcTrigPB07       =  23u,        /*!<ѡ��PB07�ж�Դ���Զ�����ADC����*/
    AdcTrigPC07       =  24u,        /*!<ѡ��PC07�ж�Դ���Զ�����ADC����*/
    AdcTrigPD07       =  25u,        /*!<ѡ��PD07�ж�Դ���Զ�����ADC����*/
    AdcTrigPA11       =  26u,        /*!<ѡ��PA11�ж�Դ���Զ�����ADC����*/
    AdcTrigPB11       =  27u,        /*!<ѡ��PB11�ж�Դ���Զ�����ADC����*/
    AdcTrigPC11       =  28u,        /*!<ѡ��PC11�ж�Դ���Զ�����ADC����*/
    AdcTrigPA15       =  29u,        /*!<ѡ��PA15�ж�Դ���Զ�����ADC����*/
    AdcTrigPB15       =  30u,        /*!<ѡ��PB15�ж�Դ���Զ�����ADC����*/
    AdcTrigPC15       =  31u,        /*!<ѡ��PC15�ж�Դ���Զ�����ADC����*/
}en_adc_trig_sel_t;

/**
 ******************************************************************************
 ** \brief ADC�ⲿ����Դ�Ĵ���ѡ��
 *****************************************************************************/
typedef enum en_adc_ext_trig_sel
{
    AdcExtTrig0     =  0u,         /*!<���μ�˳��ɨ��ת�� �ⲿ����Դѡ��Ĵ���*/
    AdcExtTrig1     =  1u,         /*!<���ɨ��ת�� �ⲿ����Դѡ��Ĵ���*/
}en_adc_ext_trig_sel_t;

/**
 ******************************************************************************
 ** \brief ADC˳��ת��ͨ��
 *****************************************************************************/
typedef enum en_adc_sqr_chmux
{
    CH0MUX     =  0u,         /*!<ת��ͨ��0*/
    CH1MUX     =  1u,         /*!<ת��ͨ��1*/
    CH2MUX     =  2u,         /*!<ת��ͨ��2*/
    CH3MUX     =  3u,         /*!<ת��ͨ��3*/
    CH4MUX     =  4u,         /*!<ת��ͨ��4*/
    CH5MUX     =  5u,         /*!<ת��ͨ��5*/
    CH6MUX     =  6u,         /*!<ת��ͨ��6*/
    CH7MUX     =  7u,         /*!<ת��ͨ��7*/
    CH8MUX     =  8u,         /*!<ת��ͨ��8*/
    CH9MUX     =  9u,         /*!<ת��ͨ��9*/
    CH10MUX    =  10u,        /*!<ת��ͨ��10*/
    CH11MUX    =  11u,        /*!<ת��ͨ��11*/
    CH12MUX    =  12u,        /*!<ת��ͨ��12*/
    CH13MUX    =  13u,        /*!<ת��ͨ��13*/
    CH14MUX    =  14u,        /*!<ת��ͨ��14*/
    CH15MUX    =  15u,        /*!<ת��ͨ��15*/
}en_adc_sqr_chmux_t;
/**
 ******************************************************************************
 ** \brief ADC���ת��ͨ��
 *****************************************************************************/
typedef enum en_adc_jqr_chmux
{
    JQRCH0MUX     =  0u,         /*!<ת��ͨ��0*/
    JQRCH1MUX     =  1u,         /*!<ת��ͨ��1*/
    JQRCH2MUX     =  2u,         /*!<ת��ͨ��2*/
    JQRCH3MUX     =  3u,         /*!<ת��ͨ��3*/
}en_adc_jqr_chmux_t;
/**
 ******************************************************************************
 ** \brief ADC������뷽ʽ
 *****************************************************************************/
typedef enum en_adc_align
{
     AlignRight  = 0u,
     AlignLeft = 1u,    
}en_adc_align_t;
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
    en_adc_clk_div_t  enAdcClkDiv;

    /*! ADC����ʱ��*/
    en_adc_samp_time_sel_t  enAdcSampTimeSel;

    /*! ADC�ο���ѹ*/
    en_adc_ref_vol_sel_t    enAdcRefVolSel;

    /*! ADC��������ʹ��*/
    boolean_t    bAdcInBufEn;
}stc_adc_cfg_t;

 /**
 ******************************************************************************
 ** \brief  ADC DMA����Դ�Ĵ�������
 *****************************************************************************/
typedef enum en_adc_dmatrig
{
   /*!���ɨ�败��DMA��ȡ����*/ 
  DmaJqr = 0, 
   /*!˳��ɨ�败��DMA��ȡ����*/   
  DmaSqr = 1   
}en_adc_dmatrig_t;
 /**
 ******************************************************************************
 ** \brief  ADC�ⲿ����Դ�Ĵ�������
 *****************************************************************************/
typedef struct stc_adc_ext_trig_cfg
{
    /*! �ⲿ����Դ�Ĵ���ѡ��*/
    en_adc_ext_trig_sel_t   enAdcExtTrigRegSel;
    
    /*! ADC���μ�˳��ת������ѡ��*/
    en_adc_trig_sel_t       enAdcTrig0Sel;

    /*! ADC���ת������ѡ��*/
    en_adc_trig_sel_t       enAdcTrig1Sel;

}stc_adc_ext_trig_cfg_t;

/**
 ******************************************************************************
 ** \brief  ADC�ж�����
 *****************************************************************************/
typedef struct stc_adc_irq
{
    /*!<ADC���ɨ������ж�*/
    boolean_t  bAdcJQRIrq;   
    /*!<ADC˳��ɨ������ж�*/
    boolean_t  bAdcSQRIrq;  
    /*! ADC�����ж�*/
    boolean_t  bAdcRegCmp;
    /*! ADC�ϳ��������ж�*/
    boolean_t  bAdcHhtCmp;
    /*! ADC�³��������ж�*/
    boolean_t  bAdcLltCmp;    
    /*!<ADC����ת������ж�*/
    boolean_t  bAdcIrq;
}stc_adc_irq_t;

typedef struct stc_adc_threshold_cfg
{
   
    boolean_t bAdcRegCmp ;      /*!ADC����ʹ��*/ 

    boolean_t bAdcHhtCmp ;     /*!ADC�ϳ�������ʹ��*/ 

    boolean_t bAdcLltCmp ;     /*!ADC�³�������ʹ��*/
    
    uint32_t u32AdcRegHighThd; /*!ADC�³�������*/
    
    uint32_t u32AdcRegLowThd; /*!ADC�³�������*/ 
    
    en_adc_samp_ch_sel_t enThCh; /*!��ֵ�Ƚ�ͨ��ѡ��*/
    
}stc_adc_threshold_cfg_t;
/**
 ******************************************************************************
 ** \brief  ADC�жϻص�����
 *****************************************************************************/
typedef struct stc_adc_irq_calbakfn_pt
{
    /*! ADC���ɨ���жϻص�����ָ��*/
    func_ptr_t  pfnAdcJQRIrq;
    /*! ADC˳��ɨ���жϻص�����ָ��*/
    func_ptr_t  pfnAdcSQRIrq;    
    /*! ADC�����жϻص�����ָ��*/
    func_ptr_t  pfnAdcRegIrq;
    /*! ADC�ϳ��������жϻص�����ָ��*/
    func_ptr_t  pfnAdcHhtIrq;
    /*! ADC�³��������жϻص�����ָ��*/
    func_ptr_t  pfnAdcLltIrq;
    /*! ADC����ת���жϻص�����ָ��*/
    func_ptr_t  pfnAdcIrq; 
}stc_adc_irq_calbakfn_pt_t;

/******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
//ADC initialization
en_result_t Adc_Init(stc_adc_cfg_t* pstcAdcConfig);
//ADC ExtTrig Register config
en_result_t Adc_ExtTrigCfg(stc_adc_ext_trig_cfg_t* pstcExtTrigConfig);
//ADC de-init
void Adc_DeInit(void);

//ADC conversion start
void Adc_SGL_Start(void);
//ADC conversion stop
void Adc_SGL_Stop(void);

//ADC SQR conversion start
void Adc_SQR_Start(void);
//ADC SQR conversion stop
void Adc_SQR_Stop(void);

//ADC JQR conversion start
void Adc_JQR_Start(void);
//ADC JQR conversion stop
void Adc_JQR_Stop(void);

//ADC conversion enable
void Adc_Enable(void);
//ADC conversion disable
void Adc_Disable(void);

//ADC single covert mode configuration
en_result_t Adc_ConfigSglMode(stc_adc_cfg_t* pstcAdcConfig);
//ADC SQR mode configuration
en_result_t Adc_ConfigSqrMode(stc_adc_cfg_t* pstcAdcConfig, uint8_t u8AdcSampCnt,boolean_t  bAdcResultAccEn);
//ADC JQR mode configuration
en_result_t Adc_ConfigJqrMode(stc_adc_cfg_t* pstcAdcConfig, uint8_t u8AdcSampCnt,boolean_t  bAdcResultAccEn);

//ADC single covert mode channel configuraion 
en_result_t Adc_ConfigSglChannel( en_adc_samp_ch_sel_t enstcAdcSampCh);
//ADC SQR mode channel configuraion 
en_result_t Adc_ConfigSqrChannel(en_adc_sqr_chmux_t enstcAdcSqrChMux, en_adc_samp_ch_sel_t enstcAdcSampCh);
//ADC JQR mode channel configuraion 
en_result_t Adc_ConfigJqrChannel(en_adc_jqr_chmux_t enstcAdcJqrChMux, en_adc_samp_ch_sel_t enstcAdcSampCh);
//ADC DMA Trigger 
en_result_t Adc_ConfigDmaTrig(en_adc_dmatrig_t enAdcDmaTrig);
//ADC IRQ configuration
void Adc_ConfigIrq(stc_adc_irq_t* pstcAdcIrqCfg,
                   stc_adc_irq_calbakfn_pt_t* pstcAdcIrqCalbaks);
//ADC enable IRQ
void Adc_EnableIrq(void);
//ADC disable IRQ
void Adc_DisableIrq(void);
//ADC enable threshold compare
void Adc_ThresholdCfg(stc_adc_threshold_cfg_t* stcAdcThrCfg);
//ADC get IRQ state
void Adc_GetIrqState(stc_adc_irq_t* pstcAdcIrqState);
//ADC clear IRQ states
void Adc_ClrSglIrqState(void);
void Adc_ClrJqrIrqState(void);
void Adc_ClrSqrIrqState(void);
void Adc_ClrRegIrqState(void);
void Adc_ClrHtIrqState(void);
void Adc_ClrLtIrqState(void);

//��ѯADC����ת��״̬
boolean_t Adc_PollSglBusyState(void);
//��ѯADC˳��ɨ��ת��ת��״̬
boolean_t Adc_PollSqrBusyState(void);
//��ѯADC���ɨ��ת��״̬
boolean_t Adc_PollJqrBusyState(void);
//��ѯADC����Ƚ�����״̬
boolean_t Adc_PollRegBusyState(void);
//��ѯADC����Ƚ�����ֵ״̬
boolean_t Adc_PollHTBusyState(void);
//��ѯADC����Ƚ�����ֵ״̬
boolean_t Adc_PollLtBusyState(void);
//��ȡ����ת������ֵ
en_result_t Adc_GetSglResult(uint16_t* pu16AdcResult);
//��ȡ˳��ɨ�����ֵ
en_result_t Adc_GetSqrResult(uint16_t* pu16AdcResult,uint8_t SQRChannelIndex);
//��ȡ���ɨ�����ֵ
en_result_t Adc_GetJqrResult(uint16_t* pu16AdcResult,uint8_t JQRChannelIndex);
//��ȡ�ۼӲ���ֵ
en_result_t Adc_GetAccResult(uint32_t* pu32AdcAccResult);
//clear ADC accumulated result
void Adc_ClrAccResult(void);
//���òο���ѹ
en_result_t Adc_SetVref(en_adc_ref_vol_sel_t enAdcRefVolSel);
//���ý�����뷽ʽ
en_result_t Adc_SetAlign(en_adc_align_t enAlign);
//@}
#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
