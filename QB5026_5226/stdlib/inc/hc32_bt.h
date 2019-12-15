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
 ** @link BT Group Some description @endlink
 **
 ** History:
 **   - 2018-04-18   Husj     First Version
 **
 *****************************************************************************/

#ifndef __BT_H__
#define __BT_H__

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
 ** \defgroup BtGroup Base Timer (BT)
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
 ** \brief ��ʱ��ѡ�����������ض���
 *****************************************************************************/
typedef enum en_bt_unit
{
    TIM0    = 0u,           ///< Timer 0
    TIM1    = 1u,           ///< Timer 1
    TIM2    = 2u,           ///< Timer 2
}en_bt_unit_t;

/**
 ******************************************************************************
 ** \brief ����ģʽѡ�����������ض��� (MODE)(ģʽ0/1/23)
 *****************************************************************************/
typedef enum en_bt_work_mode
{
    BtWorkMode0  = 0u,         ///< ��ʱ��ģʽ
    BtWorkMode1  = 1u,         ///< PWCģʽ
    BtWorkMode2  = 2u,         ///< ��ݲ�ģʽ
    BtWorkMode3  = 3u,         ///< ���ǲ�ģʽ
}en_bt_work_mode_t;

/**
 ******************************************************************************
 ** \brief ���Կ������������ض��� (GATE_P)(ģʽ0)
 *****************************************************************************/
typedef enum en_bt_m0_gatep
{
    BtGatePositive = 0u,            ///< �ߵ�ƽ��Ч
    BtGateOpposite = 1u,            ///< �͵�ƽ��Ч
}en_bt_m0_gatep_t;

/**
 ******************************************************************************
 ** \brief TIM Ԥ��Ƶѡ�� (PRS)(ģʽ0/1/23)
 *****************************************************************************/
typedef enum en_bt_cr_timclkdiv
{
    BtPCLKDiv1   = 0u,           ///< Div 1
    BtPCLKDiv2   = 1u,           ///< Div 2
    BtPCLKDiv4   = 2u,           ///< Div 4
    BtPCLKDiv8   = 3u,           ///< Div 8
    BtPCLKDiv16  = 4u,           ///< Div 16
    BtPCLKDiv32  = 5u,           ///< Div 32
    BtPCLKDiv64  = 6u,           ///< Div 64
    BtPCLKDiv256 = 7u,           ///< Div 256
}en_bt_cr_timclkdiv_t;

/**
 ******************************************************************************
 ** \brief ����/��ʱ������ѡ�����������ض��� (CT)(ģʽ0/1/23)
 *****************************************************************************/
typedef enum en_bt_cr_ct
{
    BtTimer   = 0u,            ///< ��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK
    BtCounter = 1u,            ///< ���������ܣ�����ʱ��Ϊ�ⲿETR
}en_bt_cr_ct_t;


/**
 ******************************************************************************
 ** \brief ��ʱ������ģʽ���������ض��� (MD)(ģʽ0)
 *****************************************************************************/
typedef enum en_bt_m0cr_md
{
    Bt32bitFreeMode  = 0u,         ///< 32λ������/��ʱ��
    Bt16bitArrMode  = 1u,         ///< �Զ�����16λ������/��ʱ��
}en_bt_m0cr_md_t;

/**
 ******************************************************************************
** \brief BT0/BT1/BT2�ж��������������ض���(ģʽ0/1/23)
 *****************************************************************************/
typedef enum en_bt_irq_type
{
    BtUevIrq   = 0u,         ///< ���/�¼������ж�
    BtCA0Irq   = 2u,         ///< ����/�Ƚ��ж�A(��ģʽ1/23����)
    BtCB0Irq   = 5u,         ///< ����/�Ƚ��ж�B(��ģʽ23����)
    BtCA0E     = 8u,         ///< CH0A�������ݶ�ʧ��־(��ģʽ23����),�����ж�
    BtCB0E     = 11u,        ///< CH0B�������ݶ�ʧ��־(��ģʽ23����),�����ж�
    BtBkIrq    = 14u,        ///< ɲ���ж�(��ģʽ23����)
    BtTrigIrq  = 15u,        ///< �����ж�(��ģʽ23����)
}en_bt_irq_type_t;

/**
 ******************************************************************************
 ** \brief ������ʼ�������������ض��� (Edg1stEdg2nd)(ģʽ1)
 *****************************************************************************/
typedef enum en_bt_m1cr_Edge
{
    BtPwcRiseToRise  = 0u,            ///< �����ص�������(����)
    BtPwcFallToRise  = 1u,            ///< �½��ص�������(�͵�ƽ)
    BtPwcRiseToFall  = 2u,            ///< �����ص��½���(�ߵ�ƽ)
    BtPwcFallToFall  = 3u,            ///< �½��ص��½���(����)
}en_bt_m1cr_Edge_t;

/**
 ******************************************************************************
 ** \brief PWC��������ģʽѡ�����������ض��� (Oneshot)(ģʽ1)
 *****************************************************************************/
typedef enum en_bt_m1cr_oneshot
{
    BtPwcCycleDetect    = 0u,         ///< PWCѭ������
    BtPwcOneShotDetect  = 1u,         ///< PWC���β���
}en_bt_m1cr_oneshot_t;

/**
 ******************************************************************************
 ** \brief PWC IA0ѡ�����������ض��� (IA0S)(ģʽ1)
 *****************************************************************************/
typedef enum en_bt_m1_mscr_ia0s
{
    BtIA0Input    = 0u,         ///< IAO����
    BtXORInput    = 1u,         ///< IA0 ETR GATE XOR(TIM0/1/2)/IA0 IA1 IA2 XOR(TIM3)
}en_bt_m1_mscr_ia0s_t;

/**
 ******************************************************************************
 ** \brief PWC IB0ѡ�����������ض��� (IA0S)(ģʽ1)
 *****************************************************************************/
typedef enum en_bt_m1_mscr_ib0s
{
    BtIB0Input    = 0u,         ///< IBO����
    BtTsInput     = 1u,         ///< �ڲ�����TSѡ���ź�
}en_bt_m1_mscr_ib0s_t;

/**
 ******************************************************************************
 ** \brief ������ԡ�������λ ���������ض��� (CCPA0/CCPB0/ETP/BKP)(ģʽ1/23)
 *****************************************************************************/
typedef enum en_bt_port_polarity
{
    BtPortPositive      = 0u,         ///< �����������
    BtPortOpposite      = 1u,         ///< �����������
}en_bt_port_polarity_t;

/**
 ******************************************************************************
 ** \brief �˲�ѡ�����������ض��� (FLTET/FLTA0/FLAB0)(ģʽ1/23)
 *****************************************************************************/
typedef enum en_bt_flt
{
    BtFltNone          = 0u,         ///< ���˲�
    BtFltPCLKCnt3      = 4u,         ///< PCLK 3��������Ч
    BtFltPCLKDiv4Cnt3  = 5u,         ///< PCLK/4 3��������Ч
    BtFltPCLKDiv16Cnt3 = 6u,         ///< PCLK/16 3��������Ч
    BtFltPCLKDiv64Cnt3 = 7u,         ///< PCLK/64 3��������Ч
}en_bt_flt_t;

/**
 ******************************************************************************
 ** \brief ͨ���ȽϿ��� ���������ض��� (OCMA/OCMB)(ģʽ23)
 *****************************************************************************/
typedef enum en_bt_m23_fltr_ocm
{
    BtForceLow      = 0u,         ///< ǿ��Ϊ0
    BtForceHigh     = 1u,         ///< ǿ��Ϊ1
    BtCMPForceLow   = 2u,         ///< �Ƚ�ƥ��ʱǿ��Ϊ0
    BtCMPForceHigh  = 3u,         ///< �Ƚ�ƥ��ʱǿ��Ϊ1
    BtCMPInverse    = 4u,         ///< �Ƚ�ƥ��ʱ��ת��ƽ
    BtCMPOnePrdHigh = 5u,         ///< �Ƚ�ƥ��ʱ���һ���������ڵĸߵ�ƽ
    BtPWMMode1      = 6u,         ///< ͨ������ΪPWM mode 1
    BtPWMMode2      = 7u,         ///< ͨ������ΪPWM mode 2
}en_bt_m23_fltr_ocm_t;

/**
 ******************************************************************************
 ** \brief ����ģʽTS���������ض��� (TS)(ģʽ1/23)
 *****************************************************************************/
typedef enum en_bt_mscr_ts
{
    BtTs0ETR        = 0u,         ///< ETR�ⲿ�����˲������λѡ���ź�
    BtTs1TIM0TRGO   = 1u,         ///< Timer0��TRGO����ź�
    BtTs2TIM1TRGO   = 2u,         ///< Timer1��TRGO����ź�
    BtTs3TIM2TRGO   = 3u,         ///< Timer2��TRGO����ź�
    BtTs4TIM3TRGO   = 4u,         ///< Timer3��TRGO����ź�
    //BtTs5IA0ED    = 5u,         ///< ��Ч
    BtTs6IAFP       = 6u,         ///< CH0A �ⲿ�������˲������λѡ���ź�
    BtTs7IBFP       = 7u,         ///< CH0B �ⲿ�������˲������λѡ����
}en_bt_mscr_ts_t;

/**
 ******************************************************************************
 ** \brief PWM���ģʽѡ�����������ض��� (COMP)(ģʽ23)
 *****************************************************************************/
typedef enum en_bt_m23cr_comp
{
    BtIndependentPWM      = 0u,         ///< ����PWM���
    BtComplementaryPWM    = 1u,         ///< ����PWM���
}en_bt_m23cr_comp_t;

/**
 ******************************************************************************
 ** \brief ��������ѡ�����������ض��� (DIR)(ģʽ23)
 *****************************************************************************/
typedef enum en_bt_m23cr_dir
{
    BtCntUp      = 0u,         ///< ���ϼ���
    BtCntDown    = 1u,         ///< ���¼���
}en_bt_m23cr_dir_t;

/**
 ******************************************************************************
 ** \brief ��������ѡ�����������ض��� (PWM2S)(ģʽ23)
 *****************************************************************************/
typedef enum en_bt_m23cr_pwm2s
{
    BtDoublePointCmp      = 0u,         ///< ˫��Ƚ�ʹ�ܣ�ʹ��CCRA,CCRB�ȽϿ���OCREFA���
    BtSinglePointCmp      = 1u,         ///< ����Ƚ�ʹ�ܣ�ʹ��CCRA�ȽϿ���OCREFA���
}en_bt_m23cr_pwm2s_t;

/**
 ******************************************************************************
 ** \brief GATE��PWM����ģʽ�²����ȽϹ��� ѡ�����������ض��� (CSG)(ģʽ23)
 *****************************************************************************/
typedef enum en_bt_m23cr_csg
{
     BtPWMCompGateCmpOut    = 0u,         ///< ��PWM����ģʽ�£�Gate��Ϊ�Ƚ����
     BtPWMCompGateCapIn     = 1u,         ///< ��PWM����ģʽ�£�Gate��Ϊ��������
}en_bt_m23cr_csg_t;


/**
 ******************************************************************************
 ** \brief �Ƚϲ���Ĵ��� ���������ض��� (CCR0A,CCR0B)(ģʽ23)
 *****************************************************************************/
typedef enum en_bt_m23_ccrx
{
     BtCCR0A    = 0u,         ///< CCR0A�Ƚϲ���Ĵ���
     BtCCR0B    = 1u,         ///< CCR0B�Ƚϲ���Ĵ���
}en_bt_m23_ccrx_t;

/**
 ******************************************************************************
 ** \brief  OCREF���Դ ѡ�����������ض��� (OCCS)(ģʽ23)
 *****************************************************************************/
typedef enum en_bt_m23ce_occs
{
     BtOC_Ref_Clr  = 0u,         ///< ����VC��OC_Ref_Clr
     BtETRf        = 1u,         ///< �ⲿETRf
}en_bt_m23ce_occs_t;

/**
 ******************************************************************************
 ** \brief  �Ƚ�ƥ���ж�ģʽ ѡ�����������ض��� (CIS/CISB)(ģʽ23)
 *****************************************************************************/
typedef enum en_bt_m23_cisa_cisb
{
     BtCmpIntNone        = 0u,         ///< �ޱȽ�ƥ���ж�
     BtCmpIntRise        = 1u,         ///< �Ƚ�ƥ���������ж�
     BtCmpIntFall        = 2u,         ///< �Ƚ�ƥ���½����ж�
     BtCmpIntRiseFall    = 3u,         ///< �Ƚ�ƥ���������½����ж�
}en_bt_m23_cisa_cisb_t;

/**
 ******************************************************************************
 ** \brief BT�˿ڿ��� - ɲ��ʱCHx���״̬����(BKSA/BKSB)(ģʽ23)
 **
 ** \note
 ******************************************************************************/
typedef enum en_bt_m23_crch0_bks
{
    BtCHxBksHiZ         = 0u,         ///< ɲ��ʹ��ʱ��CHx�˿��������̬
    BtCHxBksNorm        = 1u,         ///< ɲ��ʹ��ʱ��CHx�˿��������
    BtCHxBksLow         = 2u,         ///< ɲ��ʹ��ʱ��CHx�˿�����͵�ƽ
    BtCHxBksHigh        = 3u,         ///< ɲ��ʹ��ʱ��CHx�˿�����ߵ�ƽ
}en_bt_m23_crch0_bks_t;

/**
 ******************************************************************************
** \brief BT�˿ڿ��� - CHx�������½��ز���(CRx/CFx)(ģʽ23)
 **
 ** \note
 ******************************************************************************/
typedef enum en_bt_m23_crch0_cfx_crx
{
    BtCHxCapNone        = 0u,         ///< CHxͨ�������ֹ
    BtCHxCapRise        = 1u,         ///< CHxͨ�������ز���ʹ��
    BtCHxCapFall        = 2u,         ///< CHxͨ���½��ز���ʹ��
    BtCHxCapFallRise    = 3u,         ///< CHxͨ���������½��ز���ʹ��
}en_bt_m23_crch0_cfx_crx_t;

/**
 ******************************************************************************
** \brief BT�˿ڿ��� - CHx�Ƚϲ���ģʽ(CSA/CSB)(ģʽ23)
 **
 ** \note
 ******************************************************************************/
typedef enum en_bt_m23_crch0_csa_csb
{
    BtCHxCmpMode        = 0u,         ///< CHxͨ������Ϊ�Ƚ�ģʽ
    BtCHxCapMode        = 1u,         ///< CHxͨ������Ϊ����ģʽ
}en_bt_m23_crch0_csa_csb_t;

/**
 ******************************************************************************
 ** \brief  �Ƚ�ģʽ�� DMA�Ƚϴ���ѡ�� ���������ض��� (CCDS)(ģʽ23)
 *****************************************************************************/
typedef enum en_bt_m23_mscr_ccds
{
     BtCmpTrigDMA  = 0u,         ///< �Ƚ�ƥ�䴥��DMA
     BtUEVTrigDMA  = 1u,         ///< �¼����´���Ƚ�ƥ�䴥��DMA
}en_bt_m23_mscr_ccds_t;

/**
 ******************************************************************************
 ** \brief  ����ģʽѡ�� ���������ض��� (MSM)(ģʽ23)
 *****************************************************************************/
typedef enum en_bt_m23_mscr_msm
{
     BtSlaveMode    = 0u,         ///< ��ģʽ
     BtMasterMode   = 1u,         ///< ��ģʽ
}en_bt_m23_mscr_msm_t;

/**
 ******************************************************************************
 ** \brief  ������ģʽ���Դ ���������ض��� (MMS)(ģʽ23)
 *****************************************************************************/
typedef enum en_bt_m23_mscr_mms
{
     BtMasterUG         = 0u,         ///< UG(�������)Դ
     BtMasterCTEN       = 1u,         ///< CTENԴ
     BtMasterUEV        = 2u,         ///< UEV����Դ
     BtMasterCMPSO      = 3u,         ///< �Ƚ�ƥ��ѡ�����Դ
     BtMasterOCA0Ref    = 4u,         ///< OCA0_RefԴ
     BtMasterOCB0Ref    = 5u,         ///< OCB0_RefԴ
     //BtMasterOCB0Ref  = 6u,
     //BtMasterOCB0Ref  = 7u,
}en_bt_m23_mscr_mms_t;

/**
 ******************************************************************************
 ** \brief  ������ģʽѡ�� ���������ض��� (SMS)(ģʽ23)
 *****************************************************************************/
typedef enum en_bt_m23_mscr_sms
{
     BtSlaveIClk        = 0u,         ///< ʹ���ڲ�ʱ��
     BtSlaveResetTIM    = 1u,         ///< ��λ����
     BtSlaveTrigMode    = 2u,         ///< ����ģʽ
     BtSlaveEClk        = 3u,         ///< �ⲿʱ��ģʽ
     BtSlaveCodeCnt1    = 4u,         ///< �����������ģʽ1
     BtSlaveCodeCnt2    = 5u,         ///< �����������ģʽ2
     BtSlaveCodeCnt3    = 6u,         ///< �����������ģʽ3
     BtSlaveGateCtrl    = 7u,         ///< �ſع���
}en_bt_m23_mscr_sms_t;

/**
 ******************************************************************************
 ** \brief ��ʱ�����п������������ض��� (CTEN)
 *****************************************************************************/
typedef enum en_bt_start
{
    BtCTENDisable = 0u,            ///< ֹͣ
    BtCTENEnable  = 1u,            ///< ����
}en_bt_start_t;

/**
 ******************************************************************************
 ** \brief BaseTimer mode0 ���ýṹ�嶨��(ģʽ0)
 *****************************************************************************/
typedef struct stc_bt_mode0_config
{
    en_bt_work_mode_t     enWorkMode;       ///< ����ģʽ����
    en_bt_m0_gatep_t      enGateP;          ///< �ſؼ��Կ���
    boolean_t             bEnGate;          ///< �ſ�ʹ��
    en_bt_cr_timclkdiv_t  enPRS;            ///< Ԥ��Ƶ����
    boolean_t             bEnTog;           ///< ��ת���ʹ��
    en_bt_cr_ct_t         enCT;             ///< ��ʱ/��������ѡ��
    en_bt_m0cr_md_t       enCntMode;        ///< ����ģʽ����

    func_ptr_t            pfnTim0Cb;        ///< Timer0�жϷ���ص�����[void function(void)]
    func_ptr_t            pfnTim1Cb;        ///< Timer1�жϷ���ص�����[void function(void)]
    func_ptr_t            pfnTim2Cb;        ///< Timer2�жϷ���ص�����[void function(void)]
}stc_bt_mode0_config_t;

/**
 ******************************************************************************
 ** \brief BaseTimer mode1 ���ýṹ�嶨��(ģʽ1)
 *****************************************************************************/
typedef struct stc_bt_mode1_config
{
    en_bt_work_mode_t     enWorkMode;       ///< ����ģʽ����
    en_bt_cr_timclkdiv_t  enPRS;            ///< Ԥ��Ƶ����
    en_bt_cr_ct_t         enCT;             ///< ��ʱ/��������ѡ��
    en_bt_m1cr_oneshot_t  enOneShot;        ///< ���β���/ѭ������ѡ��


    func_ptr_t          pfnTim0Cb;        ///< Timer0�жϷ���ص�����[void function(void)]
    func_ptr_t          pfnTim1Cb;        ///< Timer1�жϷ���ص�����[void function(void)]
    func_ptr_t          pfnTim2Cb;        ///< Timer2�жϷ���ص�����[void function(void)]
}stc_bt_mode1_config_t;

/**
 ******************************************************************************
 ** \brief PWC�������ýṹ�嶨��(ģʽ1)
 *****************************************************************************/
typedef struct stc_bt_pwc_input_config
{
    en_bt_mscr_ts_t          enTsSel;       ///< ��������Դѡ��
    en_bt_m1_mscr_ia0s_t     enIA0Sel;      ///< CHA0����ѡ��
    en_bt_m1_mscr_ib0s_t     enIB0Sel;      ///< CHB0����ѡ��
    en_bt_port_polarity_t    enETRPhase;    ///< ETR��λѡ��
    en_bt_flt_t              enFltETR;      ///< ETR�˲�����
    en_bt_flt_t              enFltIA0;      ///< CHA0�˲�����
    en_bt_flt_t              enFltIB0;      ///< CHB0�˲�����
}stc_bt_pwc_input_config_t;

/**
 ******************************************************************************
 ** \brief BaseTimer mode23 ���ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_bt_mode23_config
{
    en_bt_work_mode_t         enWorkMode;       ///< ����ģʽ����
    en_bt_m23cr_dir_t         enCntDir;         ///< ��������
    en_bt_cr_timclkdiv_t      enPRS;            ///< ʱ��Ԥ��Ƶ����
    en_bt_cr_ct_t             enCT;             ///< ��ʱ/��������ѡ��
    en_bt_m23cr_comp_t        enPWMTypeSel;     ///< PWMģʽѡ�񣨶���/������
    en_bt_m23cr_pwm2s_t       enPWM2sSel;       ///< OCREFA˫��ȽϹ���ѡ��
    boolean_t                 bOneShot;         ///< ���δ���ģʽʹ��/��ֹ
    boolean_t                 bURSSel;          ///< ����Դѡ��

    func_ptr_t                pfnTim0Cb;        ///< Timer0�жϷ���ص�����[void function(void)]
    func_ptr_t                pfnTim1Cb;        ///< Timer1�жϷ���ص�����[void function(void)]
    func_ptr_t                pfnTim2Cb;        ///< Timer2�жϷ���ص�����[void function(void)]
}stc_bt_mode23_config_t;

/**
 ******************************************************************************
 ** \brief GATE��PWM����ģʽ�²����ȽϹ��� ���ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_bt_m23_gate_config
{
    en_bt_m23cr_csg_t        enGateFuncSel;      ///< Gate�Ƚϡ�������ѡ��
    boolean_t                bGateRiseCap;       ///< GATE��Ϊ������ʱ�����ز�����Ч����
    boolean_t                bGateFallCap;       ///< GATE��Ϊ������ʱ�����ز�����Ч����
}stc_bt_m23_gate_config_t;

/**
 ******************************************************************************
 ** \brief CHA/CHBͨ���ȽϿ��� ���ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_bt_m23_compare_config
{
    en_bt_m23_crch0_csa_csb_t  enCh0ACmpCap;     ///< CH0A�Ƚ�/������ѡ��
    en_bt_m23_fltr_ocm_t       enCH0ACmpCtrl;    ///< CH0Aͨ���ȽϿ���
    en_bt_port_polarity_t      enCH0APolarity;   ///< CH0A������Կ���
    boolean_t                  bCh0ACmpBufEn;    ///< �Ƚ�A���湦�� ʹ��/��ֹ
    en_bt_m23_cisa_cisb_t      enCh0ACmpIntSel;  ///< CHA�Ƚ�ƥ���ж�ѡ��

    en_bt_m23_crch0_csa_csb_t  enCh0BCmpCap;     ///< CH0B�Ƚ�/������ѡ��
    en_bt_m23_fltr_ocm_t       enCH0BCmpCtrl;    ///< CH0Bͨ���ȽϿ���
    en_bt_port_polarity_t      enCH0BPolarity;   ///< CH0B������Կ���
    boolean_t                  bCH0BCmpBufEn;    ///< �Ƚ�B���湦�� ʹ��/��ֹ
    en_bt_m23_cisa_cisb_t      enCH0BCmpIntSel;  ///< CHB0�Ƚ�ƥ���ж�ѡ��
}stc_bt_m23_compare_config_t;

/**
 ******************************************************************************
 ** \brief CHA/CHBͨ��������� ���ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_bt_m23_input_config
{
    en_bt_m23_crch0_csa_csb_t  enCh0ACmpCap;     ///< CH0A�Ƚ�/������ѡ��
    en_bt_m23_crch0_cfx_crx_t  enCH0ACapSel;     ///< CH0A�������ѡ��
    en_bt_flt_t                enCH0AInFlt;      ///< CH0Aͨ�������˲�����
    en_bt_port_polarity_t      enCH0APolarity;   ///< CH0A������λ

    en_bt_m23_crch0_csa_csb_t  enCh0BCmpCap;     ///< CH0B�Ƚ�/������ѡ��
    en_bt_m23_crch0_cfx_crx_t  enCH0BCapSel;     ///< HC0B�������ѡ��
    en_bt_flt_t                enCH0BInFlt;      ///< CH0Bͨ�������˲�����
    en_bt_port_polarity_t      enCH0BPolarity;   ///< CH0B������λ

}stc_bt_m23_input_config_t;

/**
 ******************************************************************************
 ** \brief ETR������λ�˲����ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_bt_m23_etr_input_config
{
    en_bt_port_polarity_t     enETRPolarity;  ///< ETR���뼫������
    en_bt_flt_t               enETRFlt;       ///< ETR�˲�����
}stc_bt_m23_etr_input_config_t;

/**
 ******************************************************************************
 ** \brief ɲ��BK������λ�˲����ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_bt_m23_bk_input_config
{
    boolean_t                 bEnBrake;         ///< ɲ��ʹ��
    boolean_t                 bEnVC0Brake;      ///< ʹ��VC0ɲ��
    boolean_t                 bEnVC1Brake;      ///< ʹ��VC1ɲ��
    boolean_t                 bEnSafetyBk;      ///< ʹ��safetyɲ��
    boolean_t                 bEnBKSync;        ///< TIM0/TIM1/TIM2ɲ��ͬ��ʹ��
    en_bt_m23_crch0_bks_t     enBkCH0AStat;     ///< ɲ��ʱCHA�˿�״̬����
    en_bt_m23_crch0_bks_t     enBkCH0BStat;     ///< ɲ��ʱCHB�˿�״̬����
    en_bt_port_polarity_t     enBrakePolarity;  ///< ɲ��BK���뼫������
    en_bt_flt_t               enBrakeFlt;       ///< ɲ��BK�˲�����
}stc_bt_m23_bk_input_config_t;

/**
 ******************************************************************************
** \brief �����������ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_bt_m23_dt_config
{
    boolean_t       bEnDeadTime;      ///< ɲ��ʱCHA�˿�״̬����
    uint8_t         u8DeadTimeValue;  ///< ɲ��ʱCHA�˿�״̬����
}stc_bt_m23_dt_config_t;

/**
 ******************************************************************************
 ** \brief ����ADC���ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_bt_m23_adc_trig_config
{
    boolean_t     bEnTrigADC;          ///< ����ADCȫ�ֿ���
    boolean_t     bEnUevTrigADC;       ///< �¼����´���ADC
    boolean_t     bEnCH0ACmpTrigADC;   ///< CH0A�Ƚ�ƥ�䴥��ADC
    boolean_t     bEnCH0BCmpTrigADC;   ///< CH0B�Ƚ�ƥ�䴥��ADC
}stc_bt_m23_adc_trig_config_t;

/**
 ******************************************************************************
 ** \brief  DMA���� ���ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_bt_m23_trig_dma_config
{
    boolean_t              bUevTrigDMA;      ///< ���� ����DMAʹ��
    boolean_t              bTITrigDMA;       ///< Trig ����DMA����
    boolean_t              bCmpATrigDMA;     ///< A����Ƚϴ���DMAʹ��
    boolean_t              bCmpBTrigDMA;     ///< B����Ƚϴ���DMAʹ��
    en_bt_m23_mscr_ccds_t  enCmpUevTrigDMA;  ///< �Ƚ�ģʽ��DMA�Ƚϴ���ѡ��
}stc_bt_m23_trig_dma_config_t;

/**
 ******************************************************************************
 ** \brief  ����ģʽ ���ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_bt_m23_master_slave_config
{
    en_bt_m23_mscr_msm_t      enMasterSlaveSel;  ///< ����ģʽѡ��
    en_bt_m23_mscr_mms_t      enMasterSrc;       ///< ��ģʽ����Դѡ��
    en_bt_m23_mscr_sms_t      enSlaveModeSel;    ///< ��ģʽѡ��
    en_bt_mscr_ts_t           enTsSel;           ///< ��������Դѡ��
}stc_bt_m23_master_slave_config_t;

/**
 ******************************************************************************
 ** \brief  OCREF������� ���ýṹ�嶨��(ģʽ23)
 *****************************************************************************/
typedef struct stc_bt_m23_OCREF_Clr_config
{
    en_bt_m23ce_occs_t       enOCRefClrSrcSel;  ///< OCREF���Դѡ��
    boolean_t                bVCClrEn;          ///< �Ƿ�ʹ������VC��OCREF_Clr
}stc_bt_m23_OCREF_Clr_config_t;

/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 *****************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 *****************************************************************************/
//�ж���غ���

//�жϱ�־��ȡ
boolean_t Bt_GetIntFlag(en_bt_unit_t enUnit, en_bt_irq_type_t enBtIrq);
//�жϱ�־���
en_result_t Bt_ClearIntFlag(en_bt_unit_t enUnit, en_bt_irq_type_t enBtIrq);
//�����жϱ�־���
en_result_t Bt_ClearAllIntFlag(en_bt_unit_t enUnit);
//ģʽ0�ж�ʹ��
en_result_t Bt_Mode0_EnableIrq(en_bt_unit_t enUnit);
//ģʽ1�ж�ʹ��
en_result_t Bt_Mode1_EnableIrq (en_bt_unit_t enUnit, en_bt_irq_type_t enBtIrq);
//ģʽ2�ж�ʹ��
en_result_t Bt_Mode23_EnableIrq (en_bt_unit_t enUnit, en_bt_irq_type_t enBtIrq);
//ģʽ0�жϽ�ֹ
en_result_t Bt_Mode0_DisableIrq(en_bt_unit_t enUnit);
//ģʽ1�жϽ�ֹ
en_result_t Bt_Mode1_DisableIrq (en_bt_unit_t enUnit, en_bt_irq_type_t enBtIrq);
//ģʽ2�жϽ�ֹ
en_result_t Bt_Mode23_DisableIrq (en_bt_unit_t enUnit, en_bt_irq_type_t enBtIrq);


//ģʽ0��ʼ������ع��ܲ���

//timer���ü���ʼ��
en_result_t Bt_Mode0_Init(en_bt_unit_t enUnit, stc_bt_mode0_config_t* pstcConfig);
//timer ����/ֹͣ
en_result_t Bt_M0_Run(en_bt_unit_t enUnit);
en_result_t Bt_M0_Stop(en_bt_unit_t enUnit);
//����ֵ����
en_result_t Bt_M0_ARRSet(en_bt_unit_t enUnit, uint16_t u16Data);
//16λ����ֵ����/��ȡ
en_result_t Bt_M0_Cnt16Set(en_bt_unit_t enUnit, uint16_t u16Data);
uint16_t Bt_M0_Cnt16Get(en_bt_unit_t enUnit);
//32λ����ֵ����/��ȡ
en_result_t Bt_M0_Cnt32Set(en_bt_unit_t enUnit, uint32_t u32Data);
uint32_t Bt_M0_Cnt32Get(en_bt_unit_t enUnit);
//��ת���ʹ��/��ֹ�趨
en_result_t Bt_M0_EnTOG_Output(en_bt_unit_t enUnit, boolean_t bEnOutput);


//ģʽ1��ʼ������ع��ܲ���

//timer���ü���ʼ��
en_result_t Bt_Mode1_Init(en_bt_unit_t enUnit, stc_bt_mode1_config_t* pstcConfig);
//PWC ��������
en_result_t Bt_M1_Input_Config(en_bt_unit_t enUnit, stc_bt_pwc_input_config_t* pstcConfig);
//PWC����������ʼ����ѡ��
en_result_t Bt_M1_PWC_Edge_Sel(en_bt_unit_t enUnit,en_bt_m1cr_Edge_t enEdgeSel);
//timer ����/ֹͣ
en_result_t Bt_M1_Run(en_bt_unit_t enUnit);
en_result_t Bt_M1_Stop(en_bt_unit_t enUnit);
//16λ����ֵ����/��ȡ
en_result_t Bt_M1_Cnt16Set(en_bt_unit_t enUnit, uint16_t u16Data);
uint16_t Bt_M1_Cnt16Get(en_bt_unit_t enUnit);
//�����Ȳ��������ֵ��ȡ
uint16_t Bt_M1_PWC_CapValueGet(en_bt_unit_t enUnit);


//ģʽ23��ʼ������ع��ܲ���

//timer���ü���ʼ��
en_result_t Bt_Mode23_Init(en_bt_unit_t enUnit, stc_bt_mode23_config_t* pstcConfig);
//timer ����/ֹͣ
en_result_t Bt_M23_Run(en_bt_unit_t enUnit);
en_result_t Bt_M23_Stop(en_bt_unit_t enUnit);
//PWM���ʹ��
en_result_t Bt_M23_EnPWM_Output(en_bt_unit_t enUnit, boolean_t bEnOutput, boolean_t bEnAutoOutput);
//����ֵ����
en_result_t Bt_M23_ARRSet(en_bt_unit_t enUnit, uint16_t u16Data, boolean_t bArrBufEn);
//16λ����ֵ����/��ȡ
en_result_t Bt_M23_Cnt16Set(en_bt_unit_t enUnit, uint16_t u16Data);
uint16_t Bt_M23_Cnt16Get(en_bt_unit_t enUnit);
//�Ƚϲ���Ĵ���CCR0A/CCR0B����/��ȡ
en_result_t Bt_M23_CCR_Set(en_bt_unit_t enUnit, en_bt_m23_ccrx_t enCCRSel, uint16_t u16Data);
uint16_t Bt_M23_CCR_Get(en_bt_unit_t enUnit, en_bt_m23_ccrx_t enCCRSel);
//PWM�������ģʽ�£�GATE����ѡ��
en_result_t Bt_M23_GateFuncSel(en_bt_unit_t enUnit,stc_bt_m23_gate_config_t* pstcConfig);
//����ģʽ����
en_result_t Bt_M23_MasterSlave_Set(en_bt_unit_t enUnit, stc_bt_m23_master_slave_config_t* pstcConfig);
//CH0A/CH0B�Ƚ�ͨ������
en_result_t Bt_M23_PortOutput_Config(en_bt_unit_t enUnit, stc_bt_m23_compare_config_t* pstcConfig);
//CH0A/CH0B�������
en_result_t Bt_M23_PortInput_Config(en_bt_unit_t enUnit, stc_bt_m23_input_config_t* pstcConfig);
void Bt_M23_CapEdge_Config(en_bt_unit_t enUnit, en_bt_m23_crch0_cfx_crx_t edge);
//ERT�������
en_result_t Bt_M23_ETRInput_Config(en_bt_unit_t enUnit, stc_bt_m23_etr_input_config_t* pstcConfig);
//ɲ��BK�������
en_result_t Bt_M23_BrakeInput_Config(en_bt_unit_t enUnit, stc_bt_m23_bk_input_config_t* pstcConfig);
//����ADC����
en_result_t Bt_M23_TrigADC_Config(en_bt_unit_t enUnit, stc_bt_m23_adc_trig_config_t* pstcConfig);
//��������
en_result_t Bt_M23_DT_Config(en_bt_unit_t enUnit, stc_bt_m23_dt_config_t* pstcConfig);
//�ظ���������
en_result_t Bt_M23_SetValidPeriod(en_bt_unit_t enUnit, uint8_t u8ValidPeriod);
//OCREF�������
en_result_t Bt_M23_OCRefClr(en_bt_unit_t enUnit, stc_bt_m23_OCREF_Clr_config_t* pstcConfig);
//ʹ��DMA����
en_result_t Bt_M23_EnDMA(en_bt_unit_t enUnit, stc_bt_m23_trig_dma_config_t* pstcConfig);
//����Ƚ�A�������
en_result_t Bt_M23_EnSwTrigCapCmpA(en_bt_unit_t enUnit);
//����Ƚ�B�������
en_result_t Bt_M23_EnSwTrigCapCmpB(en_bt_unit_t enUnit);
//�������ʹ��
en_result_t Bt_M23_EnSwUev(en_bt_unit_t enUnit);
//�������ʹ��
en_result_t Bt_M23_EnSwTrig(en_bt_unit_t enUnit);
//���ɲ��ʹ��
en_result_t Bt_M23_EnSwBk(en_bt_unit_t enUnit);


//@} // BtGroup

#ifdef __cplusplus
#endif


#endif /* __BT_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/


