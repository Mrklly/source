/*******************************************************************************
* Copyright (C) 2018, Huada Semiconductor Co.,Ltd All rights reserved.
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
/** \file bt.h
 **
 ** ������ʱ�����ݽṹ��API����
 ** @link BT Timer3 Group Some description @endlink
 **
 ** History:
 **   - 2018-04-29   Husj     First Version
 **
 *****************************************************************************/

#ifndef __TIMER3_H__
#define __TIMER3_H__

/*****************************************************************************
 * Include files
 *****************************************************************************/
#include "hc32_ddl.h"
#include "hc32_it.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup Tim3Group Base Timer (BT)
  **
 ******************************************************************************/
//@{
    
/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief Timer3 ͨ������
 *****************************************************************************/
typedef enum en_tim3_channel
{
    Tim3CH0  = 0u,        ///< Timer3ͨ��0
    Tim3CH1  = 1u,        ///< Timer3ͨ��1
    Tim3CH2  = 2u,        ///< Timer3ͨ��2
}en_tim3_channel_t;

/**
 ******************************************************************************
 ** \brief ����ģʽѡ�����������ض��� (MODE)(ģʽ0/1/23)
 *****************************************************************************/
typedef enum en_tim3_work_mode
{
    Tim3WorkMode0  = 0u,         ///< ��ʱ��ģʽ
    Tim3WorkMode1  = 1u,         ///< PWCģʽ
    Tim3WorkMode2  = 2u,         ///< ��ݲ�ģʽ
    Tim3WorkMode3  = 3u,         ///< ���ǲ�ģʽ
}en_tim3_work_mode_t;

/**
 ******************************************************************************
 ** \brief ���Կ������������ض��� (GATE_P)(ģʽ0)
 *****************************************************************************/
typedef enum en_tim3_m0cr_gatep
{
    Tim3GatePositive = 0u,            ///< �ߵ�ƽ��Ч
    Tim3GateOpposite = 1u,            ///< �͵�ƽ��Ч
}en_tim3_m0cr_gatep_t;

/**
 ******************************************************************************
 ** \brief TIM3 Ԥ��Ƶѡ�� (PRS)(ģʽ0/1/23)
 *****************************************************************************/
typedef enum en_tim3_cr_timclkdiv
{
    Tim3PCLKDiv1   = 0u,           ///< Div 1
    Tim3PCLKDiv2   = 1u,           ///< Div 2
    Tim3PCLKDiv4   = 2u,           ///< Div 4
    Tim3PCLKDiv8   = 3u,           ///< Div 8
    Tim3PCLKDiv16  = 4u,           ///< Div 16
    Tim3PCLKDiv32  = 5u,           ///< Div 32
    Tim3PCLKDiv64  = 6u,           ///< Div 64
    Tim3PCLKDiv256 = 7u,           ///< Div 256
}en_tim3_cr_timclkdiv_t;

/**
 ******************************************************************************
 ** \brief ����/��ʱ������ѡ�����������ض��� (CT)(ģʽ0/1/23)
 *****************************************************************************/
typedef enum en_tim3_cr_ct
{
    Tim3Timer   = 0u,            ///< ��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK
    Tim3Counter = 1u,            ///< ���������ܣ�����ʱ��Ϊ�ⲿETR
}en_tim3_cr_ct_t;


/**
 ******************************************************************************
 ** \brief ��ʱ������ģʽ���������ض��� (MD)(ģʽ0)
 *****************************************************************************/
typedef enum en_tim3_m0cr_md
{
    Tim332bitFreeMode  = 0u,         ///< 32λ������/��ʱ��
    Tim316bitArrMode  = 1u,         ///< �Զ���װ��16λ������/��ʱ��
}en_tim3_m0cr_md_t;

/**
 ******************************************************************************
** \brief TIM3�ж��������������ض���(ģʽ0/1/23)
 *****************************************************************************/
typedef enum en_tim3_irq_type
{
    Tim3UevIrq   = 0u,         ///< ���/�¼������ж�
    Tim3CA0Irq   = 2u,         ///< CH0A����/�Ƚ��ж�(��ģʽ1/23����)
    Tim3CA1Irq   = 3u,         ///< CH1A����/�Ƚ��ж�(��ģʽ23����)
    Tim3CA2Irq   = 4u,         ///< CH2A����/�Ƚ��ж�(��ģʽ23����)
    Tim3CB0Irq   = 5u,         ///< CH0B����/�Ƚ��ж�(��ģʽ23����)
    Tim3CB1Irq   = 6u,         ///< CH1B����/�Ƚ��ж�(��ģʽ23����)
    Tim3CB2Irq   = 7u,         ///< CH2B����/�Ƚ��ж�(��ģʽ23����)
    Tim3CA0E     = 8u,         ///< CH0A�������ݶ�ʧ��־(��ģʽ23����)(�����ж�)
    Tim3CA1E     = 9u,         ///< CH1A�������ݶ�ʧ��־(��ģʽ23����)(�����ж�)
    Tim3CA2E     = 10u,        ///< CH2A�������ݶ�ʧ��־(��ģʽ23����)(�����ж�)
    Tim3CB0E     = 11u,        ///< CH0B�������ݶ�ʧ��־(��ģʽ23����)(�����ж�)
    Tim3CB1E     = 12u,        ///< CH1B�������ݶ�ʧ��־(��ģʽ23����)(�����ж�)
    Tim3CB2E     = 13u,        ///< CH2B�������ݶ�ʧ��־(��ģʽ23����)(�����ж�)
    Tim3BkIrq    = 14u,        ///< ɲ���ж�(��ģʽ23���ڣ�
    Tim3TrigIrq  = 15u,        ///< �����ж�(��ģʽ23���ڣ�   
}en_tim3_irq_type_t;

/**
 ******************************************************************************
 ** \brief ������ʼ�������������ض��� (Edg1stEdg2nd)(ģʽ1)
 *****************************************************************************/
typedef enum en_tim3_m1cr_Edge
{
    Tim3PwcRiseToRise  = 0u,            ///< �����ص�������(����)
    Tim3PwcFallToRise  = 1u,            ///< �½��ص�������(�͵�ƽ)
    Tim3PwcRiseToFall  = 2u,            ///< �����ص��½���(�ߵ�ƽ)
    Tim3PwcFallToFall  = 3u,            ///< �½��ص��½���(����)
}en_tim3_m1cr_Edge_t;

/**
 ******************************************************************************
 ** \brief PWC��������ģʽѡ�����������ض��� (Oneshot)(ģʽ1)
 *****************************************************************************/
typedef enum en_tim3_m1cr_oneshot
{
    Tim3PwcCycleDetect    = 0u,         ///< PWCѭ������
    Tim3PwcOneShotDetect  = 1u,         ///< PWC���β���
}en_tim3_m1cr_oneshot_t;

/**
 ******************************************************************************
 ** \brief PWC IA0ѡ�����������ض��� (IA0S)(ģʽ1)
 *****************************************************************************/
typedef enum en_tim3_m1_mscr_ia0s
{
    Tim3IA0Input    = 0u,         ///< IAO����
    Tim3XORInput    = 1u,         ///< IA0 ETR GATE XOR(TIM0/1/2)/IA0 IA1 IA2 XOR(TIM3)
}en_tim3_m1_mscr_ia0s_t;

/**
 ******************************************************************************
 ** \brief PWC IB0ѡ�����������ض��� (IA0S)(ģʽ1)
 *****************************************************************************/
typedef enum en_tim3_m1_mscr_ib0s
{
    Tim3IB0Input    = 0u,         ///< IBO����
    Tim3TsInput     = 1u,         ///< �ڲ�����TSѡ���ź�
}en_tim3_m1_mscr_ib0s_t;

/**
 ******************************************************************************
 ** \brief ������ԡ�������λ ���������ض��� (CCPA0/CCPB0/ETP/BKP)(ģʽ1/23)
 *****************************************************************************/
typedef enum en_tim3_port_polarity
{
    Tim3PortPositive      = 0u,         ///< �����������
    Tim3PortOpposite      = 1u,         ///< �����������
}en_tim3_port_polarity_t;

/**
 ******************************************************************************
 ** \brief �˲�ѡ�����������ض��� (FLTET/FLTA0/FLAB0)(ģʽ1/23)
 *****************************************************************************/
typedef enum en_tim3_flt
{
    Tim3FltNone          = 0u,         ///< ���˲�
    Tim3FltPCLKCnt3      = 4u,         ///< PCLK 3��������Ч
    Tim3FltPCLKDiv4Cnt3  = 5u,         ///< PCLK/4 3��������Ч
    Tim3FltPCLKDiv16Cnt3 = 6u,         ///< PCLK/16 3��������Ч
    Tim3FltPCLKDiv64Cnt3 = 7u,         ///< PCLK/64 3��������Ч
}en_tim3_flt_t;

/**
 ******************************************************************************
 ** \brief ͨ���ȽϿ��� ���������ض��� (OCMA/OCMB)(ģʽ23)
 *****************************************************************************/
typedef enum en_tim3_m23_fltr_ocm
{
    Tim3ForceLow      = 0u,         ///< ǿ��Ϊ0
    Tim3ForceHigh     = 1u,         ///< ǿ��Ϊ1
    Tim3CMPForceLow   = 2u,         ///< �Ƚ�ƥ��ʱǿ��Ϊ0
    Tim3CMPForceHigh  = 3u,         ///< �Ƚ�ƥ��ʱǿ��Ϊ1
    Tim3CMPInverse    = 4u,         ///< �Ƚ�ƥ��ʱ��ת��ƽ
    Tim3CMPOnePrdHigh = 5u,         ///< �Ƚ�ƥ��ʱ���һ���������ڵĸߵ�ƽ
    Tim3PWMMode1      = 6u,         ///< ͨ������ΪPWM mode 1
    Tim3PWMMode2      = 7u,         ///< ͨ������ΪPWM mode 2
}en_tim3_m23_fltr_ocm_t;

/**
 ******************************************************************************
 ** \brief ����ģʽTS���������ض��� (TS)(ģʽ1/23)
 *****************************************************************************/
typedef enum en_tim3_mscr_ts
{
    Tim3Ts0ETR        = 0u,         ///< ETR�ⲿ�����˲������λѡ���ź�
    Tim3Ts1TIM0TRGO   = 1u,         ///< Timer0��TRGO����ź�
    Tim3Ts2TIM1TRGO   = 2u,         ///< Timer1��TRGO����ź�
    Tim3Ts3TIM2TRGO   = 3u,         ///< Timer2��TRGO����ź�
    Tim3Ts4TIM3TRGO   = 4u,         ///< Timer3��TRGO����ź�
    //Tim3Ts5IA0ED    = 5u,         ///< ��Ч
    Tim3Ts6IAFP       = 6u,         ///< CH0A �ⲿ�������˲������λѡ���ź�
    Tim3Ts7IBFP       = 7u,         ///< CH0B �ⲿ�������˲������λѡ����
}en_tim3_mscr_ts_t;

/**
 ******************************************************************************
 ** \brief PWM���ģʽѡ�����������ض��� (COMP)(ģʽ23)
 *****************************************************************************/
typedef enum en_tim3_m23cr_comp
{
    Tim3IndependentPWM      = 0u,         ///< ����PWM���
    Tim3ComplementaryPWM    = 1u,         ///< ����PWM���
}en_tim3_m23cr_comp_t;

/**
 ******************************************************************************
 ** \brief ��������ѡ�����������ض��� (DIR)(ģʽ23)
 *****************************************************************************/
typedef enum en_tim3_m23cr_dir
{
    Tim3CntUp      = 0u,         ///< ���ϼ���
    Tim3CntDown    = 1u,         ///< ���¼���
}en_tim3_m23cr_dir_t;

/**
 ******************************************************************************
 ** \brief ��������ѡ�����������ض��� (PWM2S)(ģʽ23)
 *****************************************************************************/
typedef enum en_tim3_m23cr_pwm2s
{
    Tim3DoublePointCmp      = 0u,         ///< ˫��Ƚ�ʹ�ܣ�ʹ��CCRA,CCRB�ȽϿ���OCREFA���
    Tim3SinglePointCmp      = 1u,         ///< ����Ƚ�ʹ�ܣ�ʹ��CCRA�ȽϿ���OCREFA���
}en_tim3_m23cr_pwm2s_t;

/**
 ******************************************************************************
 ** \brief GATE��PWM����ģʽ�²����ȽϹ��� ѡ�����������ض��� (CSG)(ģʽ23)
 *****************************************************************************/
typedef enum en_tim3_m23cr_csg
{
     Tim3PWMCompGateCmpOut    = 0u,         ///< ��PWM����ģʽ�£�Gate��Ϊ�Ƚ����
     Tim3PWMCompGateCapIn     = 1u,         ///< ��PWM����ģʽ�£�Gate��Ϊ��������
}en_tim3_m23cr_csg_t;


/**
 ******************************************************************************
 ** \brief �Ƚϲ���Ĵ��� ���������ض��� (CCR0A,CCR0B)(ģʽ23)
 *****************************************************************************/
typedef enum en_tim3_m23_ccrx
{
     Tim3CCR0A    = 0u,         ///< CCR0A�Ƚϲ���Ĵ���
     Tim3CCR0B    = 1u,         ///< CCR0B�Ƚϲ���Ĵ���
     Tim3CCR1A    = 2u,         ///< CCR1A�Ƚϲ���Ĵ���
     Tim3CCR1B    = 3u,         ///< CCR1B�Ƚϲ���Ĵ���
     Tim3CCR2A    = 4u,         ///< CCR2A�Ƚϲ���Ĵ���
     Tim3CCR2B    = 5u,         ///< CCR2B�Ƚϲ���Ĵ���
}en_tim3_m23_ccrx_t;

/**
 ******************************************************************************
 ** \brief  OCREF���Դ ѡ�����������ض��� (OCCS)(ģʽ23)
 *****************************************************************************/
typedef enum en_tim3_m23ce_occs
{
     Tim3OC_Ref_Clr  = 0u,         ///< ����VC��OC_Ref_Clr
     Tim3ETRf        = 1u,         ///< �ⲿETRf
}en_tim3_m23ce_occs_t;

/**
 ******************************************************************************
 ** \brief  �Ƚ�ƥ���ж�ģʽ ѡ�����������ض��� (CIS/CISB)(ģʽ23)
 *****************************************************************************/
typedef enum en_tim3_m23_cisa_cisb
{
     Tim3CmpIntNone        = 0u,         ///< �ޱȽ�ƥ���ж�
     Tim3CmpIntRise        = 1u,         ///< �Ƚ�ƥ���������ж�
     Tim3CmpIntFall        = 2u,         ///< �Ƚ�ƥ���½����ж�
     Tim3CmpIntRiseFall    = 3u,         ///< �Ƚ�ƥ���������½����ж�
}en_tim3_m23_cisa_cisb_t;

/**
 ******************************************************************************
 ** \brief TIM3�˿ڿ��� - ɲ��ʱCHx���״̬����(BKSA/BKSB)(ģʽ23)
 **
 ** \note
 ******************************************************************************/
typedef enum en_tim3_m23_crchx_bks
{
    Tim3CHxBksHiZ         = 0u,         ///< ɲ��ʹ��ʱ��CHx�˿��������̬
    Tim3CHxBksNorm        = 1u,         ///< ɲ��ʹ��ʱ��CHx�˿��������
    Tim3CHxBksLow         = 2u,         ///< ɲ��ʹ��ʱ��CHx�˿�����͵�ƽ
    Tim3CHxBksHigh        = 3u,         ///< ɲ��ʹ��ʱ��CHx�˿�����ߵ�ƽ
}en_tim3_m23_crchx_bks_t;

/**
 ******************************************************************************
** \brief TIM3�˿ڿ��� - CHx�������½��ز���(CRx/CFx)(ģʽ23)
 **
 ** \note
 ******************************************************************************/
typedef enum en_tim3_m23_crch0_cfx_crx
{
    Tim3CHxCapNone        = 0u,         ///< CHxͨ�������ֹ
    Tim3CHxCapRise        = 1u,         ///< CHxͨ�������ز���ʹ��
    Tim3CHxCapFall        = 2u,         ///< CHxͨ���½��ز���ʹ��
    Tim3CHxCapFallRise    = 3u,         ///< CHxͨ���������½��ز���ʹ��
}en_tim3_m23_crch0_cfx_crx_t;

/**
 ******************************************************************************
** \brief TIM3�˿ڿ��� - CHx�Ƚϲ���ģʽ(CSA/CSB)(ģʽ23)
 **
 ** \note
 ******************************************************************************/
typedef enum en_tim3_m23_crch0_csa_csb
{
    Tim3CHxCmpMode        = 0u,         ///< CHxͨ������Ϊ�Ƚ�ģʽ
    Tim3CHxCapMode        = 1u,         ///< CHxͨ������Ϊ����ģʽ
}en_tim3_m23_crch0_csa_csb_t;

/**
 ******************************************************************************
 ** \brief  �Ƚ�ģʽ�� DMA�Ƚϴ���ѡ�� ���������ض��� (CCDS)(ģʽ23)
 *****************************************************************************/
typedef enum en_tim3_m23_mscr_ccds
{
     Tim3CmpTrigDMA  = 0u,         ///< �Ƚ�ƥ�䴥��DMA
     Tim3UEVTrigDMA  = 1u,         ///< �¼����´���Ƚ�ƥ�䴥��DMA
}en_tim3_m23_mscr_ccds_t;

/**
 ******************************************************************************
 ** \brief  ����ģʽѡ�� ���������ض��� (MSM)(ģʽ23)
 *****************************************************************************/
typedef enum en_tim3_m23_mscr_msm
{
     Tim3SlaveMode    = 0u,         ///< ��ģʽ
     Tim3MasterMode   = 1u,         ///< ��ģʽ
}en_tim3_m23_mscr_msm_t;

/**
 ******************************************************************************
 ** \brief  ������ģʽ���Դ ���������ض��� (MMS)(ģʽ23)
 *****************************************************************************/
typedef enum en_tim3_m23_mscr_mms
{
     Tim3MasterUG         = 0u,         ///< UG(�������)Դ
     Tim3MasterCTEN       = 1u,         ///< CTENԴ
     Tim3MasterUEV        = 2u,         ///< UEV����Դ
     Tim3MasterCMPSO      = 3u,         ///< �Ƚ�ƥ��ѡ�����Դ
     Tim3MasterOCA0Ref    = 4u,         ///< OCA0_RefԴ
     Tim3MasterOCB0Ref    = 5u,         ///< OCB0_RefԴ
     //Tim3MasterOCB0Ref  = 6u,
     //Tim3MasterOCB0Ref  = 7u,
}en_tim3_m23_mscr_mms_t;

/**
 ******************************************************************************
 ** \brief  ������ģʽѡ�� ���������ض��� (SMS)(ģʽ23)
 *****************************************************************************/
typedef enum en_tim3_m23_mscr_sms
{
     Tim3SlaveIClk        = 0u,         ///< ʹ���ڲ�ʱ��
     Tim3SlaveResetTIM    = 1u,         ///< ��λ����
     Tim3SlaveTrigMode    = 2u,         ///< ����ģʽ
     Tim3SlaveEClk        = 3u,         ///< �ⲿʱ��ģʽ
     Tim3SlaveCodeCnt1    = 4u,         ///< �����������ģʽ1
     Tim3SlaveCodeCnt2    = 5u,         ///< �����������ģʽ2
     Tim3SlaveCodeCnt3    = 6u,         ///< �����������ģʽ3
     Tim3SlaveGateCtrl    = 7u,         ///< �ſع���
}en_tim3_m23_mscr_sms_t;

/**
 ******************************************************************************
 ** \brief ��ʱ�����п������������ض��� (CTEN)
 *****************************************************************************/
typedef enum en_tim3_start
{
    Tim3CTENDisable = 0u,            ///< ֹͣ
    Tim3CTENEnable  = 1u,            ///< ����
}en_tim3_start_t;

/**
 ******************************************************************************
 ** \brief TIM3 mode0 ���ýṹ�嶨��(ģʽ0)
 *****************************************************************************/
typedef struct stc_tim3_mode0_config
{
    en_tim3_work_mode_t     enWorkMode;       ///< ����ģʽ����
    en_tim3_m0cr_gatep_t    enGateP;          ///< �ſؼ��Կ���
    boolean_t               bEnGate;          ///< �ſ�ʹ��
    en_tim3_cr_timclkdiv_t  enPRS;            ///< Ԥ��Ƶ����
    boolean_t               bEnTog;           ///< ��ת���ʹ��
    en_tim3_cr_ct_t         enCT;             ///< ��ʱ/��������ѡ��
    en_tim3_m0cr_md_t       enCntMode;        ///< ����ģʽ����
    
    func_ptr_t              pfnTim3Cb;        ///< Timer3�жϷ���ص�����[void function(void)]
}stc_tim3_mode0_config_t;

/**
 ******************************************************************************
 ** \brief TIM3 mode1 ���ýṹ�嶨��(ģʽ1)
 *****************************************************************************/
typedef struct stc_tim3_mode1_config
{
    en_tim3_work_mode_t     enWorkMode;       ///< ����ģʽ����
    en_tim3_cr_timclkdiv_t  enPRS;            ///< Ԥ��Ƶ����
    en_tim3_cr_ct_t         enCT;             ///< ��ʱ/��������ѡ��
    en_tim3_m1cr_oneshot_t  enOneShot;        ///< ���β���/ѭ������ѡ��
    
    func_ptr_t              pfnTim3Cb;        ///< Timer3�жϷ���ص�����[void function(void)]

}stc_tim3_mode1_config_t;

/**
 ******************************************************************************
 ** \brief PWC�������ýṹ�嶨��(ģʽ1)
 *****************************************************************************/
typedef struct stc_tim3_pwc_input_config
{
    en_tim3_mscr_ts_t          enTsSel;       ///< ��������Դѡ��
    en_tim3_m1_mscr_ia0s_t     enIA0Sel;      ///< CHA0����ѡ��
    en_tim3_m1_mscr_ib0s_t     enIB0Sel;      ///< CHB0����ѡ��
    en_tim3_port_polarity_t    enETRPhase;    ///< ETR��λѡ��
    en_tim3_flt_t              enFltETR;      ///< ETR�˲�����
    en_tim3_flt_t              enFltIA0;      ///< CHA0�˲�����
    en_tim3_flt_t              enFltIB0;      ///< CHB0�˲�����
}stc_tim3_pwc_input_config_t;

/**
 ******************************************************************************
 ** \brief TIM3 mode23 ���ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_tim3_mode23_config
{
    en_tim3_work_mode_t         enWorkMode;       ///< ����ģʽ����
    en_tim3_m23cr_dir_t         enCntDir;         ///< ��������
    en_tim3_cr_timclkdiv_t      enPRS;            ///< ʱ��Ԥ��Ƶ����
    en_tim3_cr_ct_t             enCT;             ///< ��ʱ/��������ѡ��
    en_tim3_m23cr_comp_t        enPWMTypeSel;     ///< PWMģʽѡ�񣨶���/������
    en_tim3_m23cr_pwm2s_t       enPWM2sSel;       ///< OCREFA˫��ȽϹ���ѡ��
    boolean_t                   bOneShot;         ///< ���δ���ģʽʹ��/��ֹ
    boolean_t                   bURSSel;          ///< ����Դѡ��
    
    func_ptr_t                  pfnTim3Cb;        ///< Timer3�жϷ���ص�����[void function(void)]
}stc_tim3_mode23_config_t;

/**
 ******************************************************************************
 ** \brief GATE��PWM����ģʽ�²����ȽϹ��� ���ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_tim3_m23_gate_config
{
    en_tim3_m23cr_csg_t        enGateFuncSel;      ///< Gate�Ƚϡ�������ѡ��
    boolean_t                  bGateRiseCap;       ///< GATE��Ϊ������ʱ�����ز�����Ч����
    boolean_t                  bGateFallCap;       ///< GATE��Ϊ������ʱ�����ز�����Ч����
}stc_tim3_m23_gate_config_t;

/**
 ******************************************************************************
 ** \brief CHA/CHBͨ���ȽϿ��� ���ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_tim3_m23_compare_config
{
    en_tim3_m23_crch0_csa_csb_t  enCHxACmpCap;     ///< CH0A�Ƚ�/������ѡ��
    en_tim3_m23_fltr_ocm_t       enCHxACmpCtrl;    ///< CH0Aͨ���ȽϿ���
    en_tim3_port_polarity_t      enCHxAPolarity;   ///< CH0A������Կ���
    boolean_t                    bCHxACmpBufEn;    ///< �Ƚ�A���湦�� ʹ��/��ֹ
    en_tim3_m23_cisa_cisb_t      enCHxACmpIntSel;  ///< CHA�Ƚ�ƥ���ж�ѡ��
    
    en_tim3_m23_crch0_csa_csb_t  enCHxBCmpCap;     ///< CH0B�Ƚ�/������ѡ��
    en_tim3_m23_fltr_ocm_t       enCHxBCmpCtrl;    ///< CH0Bͨ���ȽϿ���
    en_tim3_port_polarity_t      enCHxBPolarity;   ///< CH0B������Կ���
    boolean_t                    bCHxBCmpBufEn;    ///< �Ƚ�B���湦�� ʹ��/��ֹ
    en_tim3_m23_cisa_cisb_t      enCHxBCmpIntSel;  ///< CHB0�Ƚ�ƥ���ж�ѡ��
}stc_tim3_m23_compare_config_t;

/**
 ******************************************************************************
 ** \brief CHA/CHBͨ��������� ���ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_tim3_m23_input_config
{
    en_tim3_m23_crch0_csa_csb_t  enCHxACmpCap;     ///< CH0A�Ƚ�/������ѡ��
    en_tim3_m23_crch0_cfx_crx_t  enCHxACapSel;     ///< CH0A�������ѡ��
    en_tim3_flt_t                enCHxAInFlt;      ///< CH0Aͨ�������˲�����
    en_tim3_port_polarity_t      enCHxAPolarity;   ///< CH0A������λ
    
    en_tim3_m23_crch0_csa_csb_t  enCHxBCmpCap;     ///< CH0A�Ƚ�/������ѡ��
    en_tim3_m23_crch0_cfx_crx_t  enCHxBCapSel;     ///< CH0B�������ѡ��
    en_tim3_flt_t                enCHxBInFlt;      ///< CH0Bͨ�������˲�����
    en_tim3_port_polarity_t      enCHxBPolarity;   ///< CH0B������λ
    
}stc_tim3_m23_input_config_t;   

/**
 ******************************************************************************
 ** \brief ETR������λ�˲����ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_tim3_m23_etr_input_config
{
    en_tim3_port_polarity_t     enETRPolarity;  ///< ETR���뼫������
    en_tim3_flt_t               enETRFlt;       ///< ETR�˲�����
}stc_tim3_m23_etr_input_config_t;

/**
 ******************************************************************************
 ** \brief ɲ��BK������λ�˲����ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_tim3_m23_bk_input_config
{
    boolean_t                   bEnBrake;         ///< ɲ��ʹ��
    boolean_t                   bEnVC0Brake;      ///< ʹ��VC0ɲ��
    boolean_t                   bEnVC1Brake;      ///< ʹ��VC1ɲ��
    boolean_t                   bEnSafetyBk;      ///< ʹ��safetyɲ��
    boolean_t                   bEnBKSync;        ///< TIM0/TIM1/TIM2ɲ��ͬ��ʹ��
    en_tim3_m23_crchx_bks_t     enBkCH0AStat;     ///< ɲ��ʱCHA�˿�״̬����
    en_tim3_m23_crchx_bks_t     enBkCH0BStat;     ///< ɲ��ʱCHB�˿�״̬����
    en_tim3_m23_crchx_bks_t     enBkCH1AStat;     ///< ɲ��ʱCHA�˿�״̬����
    en_tim3_m23_crchx_bks_t     enBkCH1BStat;     ///< ɲ��ʱCHB�˿�״̬����
    en_tim3_m23_crchx_bks_t     enBkCH2AStat;     ///< ɲ��ʱCHA�˿�״̬����
    en_tim3_m23_crchx_bks_t     enBkCH2BStat;     ///< ɲ��ʱCHB�˿�״̬����
    en_tim3_port_polarity_t     enBrakePolarity;  ///< ɲ��BK���뼫������
    en_tim3_flt_t               enBrakeFlt;       ///< ɲ��BK�˲�����
}stc_tim3_m23_bk_input_config_t;

/**
 ******************************************************************************
** \brief �����������ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_tim3_m23_dt_config
{
    boolean_t       bEnDeadTime;      ///< ɲ��ʱCHA�˿�״̬����
    uint8_t         u8DeadTimeValue;  ///< ɲ��ʱCHA�˿�״̬����
}stc_tim3_m23_dt_config_t;

/**
 ******************************************************************************
 ** \brief ����ADC���ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_tim3_m23_adc_trig_config
{
    boolean_t     bEnTrigADC;          ///< ����ADCȫ�ֿ���
    boolean_t     bEnUevTrigADC;       ///< �¼����´���ADC
    boolean_t     bEnCH0ACmpTrigADC;   ///< CH0A�Ƚ�ƥ�䴥��ADC
    boolean_t     bEnCH0BCmpTrigADC;   ///< CH0B�Ƚ�ƥ�䴥��ADC
    boolean_t     bEnCH1ACmpTrigADC;   ///< CH0A�Ƚ�ƥ�䴥��ADC
    boolean_t     bEnCH1BCmpTrigADC;   ///< CH0B�Ƚ�ƥ�䴥��ADC
    boolean_t     bEnCH2ACmpTrigADC;   ///< CH0A�Ƚ�ƥ�䴥��ADC
    boolean_t     bEnCH2BCmpTrigADC;   ///< CH0B�Ƚ�ƥ�䴥��ADC
}stc_tim3_m23_adc_trig_config_t;

/**
 ******************************************************************************
 ** \brief  DMA���� ���ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_tim3_m23_trig_dma_config
{
    boolean_t                bUevTrigDMA;      ///< ���� ����DMAʹ��
    boolean_t                bTITrigDMA;       ///< Trig ����DMA����
    boolean_t                bCmpA0TrigDMA;    ///< CH0A����Ƚϴ���DMAʹ��
    boolean_t                bCmpB0TrigDMA;    ///< CH0B����Ƚϴ���DMAʹ��
    boolean_t                bCmpA1TrigDMA;    ///< CH1A����Ƚϴ���DMAʹ��
    boolean_t                bCmpB1TrigDMA;    ///< CH1B����Ƚϴ���DMAʹ��
    boolean_t                bCmpA2TrigDMA;    ///< CH2A����Ƚϴ���DMAʹ��
    boolean_t                bCmpB2TrigDMA;    ///< CH2B����Ƚϴ���DMAʹ��
    en_tim3_m23_mscr_ccds_t  enCmpUevTrigDMA;  ///< �Ƚ�ģʽ��DMA�Ƚϴ���ѡ��
}stc_tim3_m23_trig_dma_config_t;

/**
 ******************************************************************************
 ** \brief  ����ģʽ ���ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_tim3_m23_master_slave_config
{
    en_tim3_m23_mscr_msm_t      enMasterSlaveSel;  ///< ����ģʽѡ��
    en_tim3_m23_mscr_mms_t      enMasterSrc;       ///< ��ģʽ����Դѡ��
    en_tim3_m23_mscr_sms_t      enSlaveModeSel;    ///< ��ģʽѡ��
    en_tim3_mscr_ts_t           enTsSel;           ///< ��������Դѡ��
}stc_tim3_m23_master_slave_config_t;

/**
 ******************************************************************************
 ** \brief  OCREF������� ���ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_tim3_m23_OCREF_Clr_config
{
    en_tim3_m23ce_occs_t       enOCRefClrSrcSel;  ///< OCREF���Դѡ��
    boolean_t                  bVCClrEn;          ///< �Ƿ�ʹ������VC��OCREF_Clr
}stc_tim3_m23_OCREF_Clr_config_t;

/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 *****************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 *****************************************************************************/
//�ж���غ���

//�жϱ�־��ȡ
boolean_t Tim3_GetIntFlag(en_tim3_irq_type_t enTim3Irq);
//�жϱ�־���
en_result_t Tim3_ClearIntFlag(en_tim3_irq_type_t enTim3Irq);
//�����жϱ�־���
en_result_t Tim3_ClearAllIntFlag(void);
//ģʽ0�ж�ʹ��
en_result_t Tim3_Mode0_EnableIrq(void);
//ģʽ1�ж�ʹ��
en_result_t Tim3_Mode1_EnableIrq (en_tim3_irq_type_t enTim3Irq);
//ģʽ2�ж�ʹ��
en_result_t Tim3_Mode23_EnableIrq (en_tim3_irq_type_t enTim3Irq);
//ģʽ0�жϽ�ֹ
en_result_t Tim3_Mode0_DisableIrq(void);
//ģʽ1�жϽ�ֹ
en_result_t Tim3_Mode1_DisableIrq (en_tim3_irq_type_t enTim3Irq);
//ģʽ2�жϽ�ֹ
en_result_t Tim3_Mode23_DisableIrq (en_tim3_irq_type_t enTim3Irq);


//ģʽ0��ʼ������ع��ܲ���

//timer���ü���ʼ��
en_result_t Tim3_Mode0_Init(stc_tim3_mode0_config_t* pstcConfig);
//timer ����/ֹͣ
en_result_t Tim3_M0_Run(void);
en_result_t Tim3_M0_Stop(void);
//����ֵ����
en_result_t Tim3_M0_ARRSet(uint16_t u16Data);
//16λ����ֵ����/��ȡ
en_result_t Tim3_M0_Cnt16Set(uint16_t u16Data);
uint16_t Tim3_M0_Cnt16Get(void);
//32λ����ֵ����/��ȡ
en_result_t Tim3_M0_Cnt32Set(uint32_t u32Data);
uint32_t Tim3_M0_Cnt32Get(void);
//��ת���ʹ��/��ֹ�趨
en_result_t Tim3_M0_EnTOG_Output(boolean_t bEnOutput);


//ģʽ1��ʼ������ع��ܲ���

//timer���ü���ʼ��
en_result_t Tim3_Mode1_Init(stc_tim3_mode1_config_t* pstcConfig);
//PWC ��������
en_result_t Tim3_M1_Input_Config(stc_tim3_pwc_input_config_t* pstcConfig);
//PWC����������ʼ����ѡ��
en_result_t Tim3_M1_PWC_Edge_Sel(en_tim3_m1cr_Edge_t enEdgeSel);
//timer ����/ֹͣ
en_result_t Tim3_M1_Run(void);
en_result_t Tim3_M1_Stop(void);
//16λ����ֵ����/��ȡ
en_result_t Tim3_M1_Cnt16Set(uint16_t u16Data);
uint16_t Tim3_M1_Cnt16Get(void);
//�����Ȳ��������ֵ��ȡ
uint16_t Tim3_M1_PWC_CapValueGet(void);


//ģʽ1��ʼ������ع��ܲ���

//timer���ü���ʼ��
en_result_t Tim3_Mode23_Init(stc_tim3_mode23_config_t* pstcConfig);
//timer ����/ֹͣ
en_result_t Tim3_M23_Run(void);
en_result_t Tim3_M23_Stop(void);
//PWM���ʹ��
en_result_t Tim3_M23_EnPWM_Output(boolean_t bEnOutput, boolean_t bEnAutoOutput);
//����ֵ����
en_result_t Tim3_M23_ARRSet(uint16_t u16Data, boolean_t bArrBufEn);
//16λ����ֵ����/��ȡ
en_result_t Tim3_M23_Cnt16Set(uint16_t u16Data);
uint16_t Tim3_M23_Cnt16Get(void);
//�Ƚϲ���Ĵ���CCR0A/CCR0B����/��ȡ
en_result_t Tim3_M23_CCR_Set(en_tim3_m23_ccrx_t enCCRSel, uint16_t u16Data);
uint16_t Tim3_M23_CCR_Get(en_tim3_m23_ccrx_t enCCRSel);
//PWM�������ģʽ�£�GATE����ѡ��
en_result_t Tim3_M23_GateFuncSel(stc_tim3_m23_gate_config_t* pstcConfig);
//����ģʽ����
en_result_t Tim3_M23_MasterSlave_Set(stc_tim3_m23_master_slave_config_t* pstcConfig);
//CH0A/CH0B�Ƚ�ͨ������
en_result_t Tim3_M23_PortOutput_Config(en_tim3_channel_t enTim3Chx, stc_tim3_m23_compare_config_t* pstcConfig);
//CH0A/CH0B�������
en_result_t Tim3_M23_PortInput_Config(en_tim3_channel_t enTim3Chx, stc_tim3_m23_input_config_t* pstcConfig);
//ERT�������
en_result_t Tim3_M23_ETRInput_Config(stc_tim3_m23_etr_input_config_t* pstcConfig);
//ɲ��BK�������
en_result_t Tim3_M23_BrakeInput_Config(stc_tim3_m23_bk_input_config_t* pstcConfig);
//����ADC����
en_result_t Tim3_M23_TrigADC_Config(stc_tim3_m23_adc_trig_config_t* pstcConfig);
//��������
en_result_t Tim3_M23_DT_Config(stc_tim3_m23_dt_config_t* pstcConfig);
//�ظ���������
en_result_t Tim3_M23_SetValidPeriod(uint8_t u8ValidPeriod);
//OCREF�������
en_result_t Tim3_M23_OCRefClr(stc_tim3_m23_OCREF_Clr_config_t* pstcConfig);
//ʹ��DMA����
en_result_t Tim3_M23_EnDMA(stc_tim3_m23_trig_dma_config_t* pstcConfig);
//����Ƚ�A�������
en_result_t Tim3_M23_EnSwTrigCapCmpA(en_tim3_channel_t enTim3Chx);
//����Ƚ�B�������
en_result_t Tim3_M23_EnSwTrigCapCmpB(en_tim3_channel_t enTim3Chx);
//�������ʹ��
en_result_t Tim3_M23_EnSwUev(void);
//�������ʹ��
en_result_t Tim3_M23_EnSwTrig(void);
//���ɲ��ʹ��
en_result_t Tim3_M23_EnSwBk(void);


//@} // Tim3Group

#ifdef __cplusplus
#endif


#endif /* __BT_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/


