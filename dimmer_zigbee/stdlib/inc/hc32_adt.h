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
/** \file adt.h
 **
 ** Headerfile for Advance Timer functions
 ** @link ADT Group Some description @endlink
 **
 **   - 2017-06-16 Alex    First Version
 **
 ******************************************************************************/

#ifndef __ADT_H__
#define __ADT_H__

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"
#include "hc32_it.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup AdtGroup Advance Timer (ADT)
  **
 ******************************************************************************/
//@{

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

 /**
 ******************************************************************************
 ** \brief �����ܶ�ʱ��ѡ��
 *****************************************************************************/
typedef enum en_adt_unit
{
    AdTIM4    = 0u,            ///< TIM4
    AdTIM5    = 1u,            ///< TIM5
    AdTIM6    = 2u,            ///< TIM6
}en_adt_unit_t;

 /**
 ******************************************************************************
 ** \brief ADT CHx�˿ڶ���
 *****************************************************************************/
typedef enum en_adt_CHxX_port
{
    AdtCHxA    = 0u,            ///< CHx A�˿�
    AdtCHxB    = 1u,            ///< CHx B�˿�
}en_adt_CHxX_port_t;

 /**
 ******************************************************************************
 ** \brief ADT TRIG�˿ڶ���
 *****************************************************************************/
typedef enum en_adt_trig_port
{
    AdtTrigA    = 0u,            ///< TIMx ����A�˿�
    AdtTrigB    = 1u,            ///< TIMx ����B�˿�
    AdtTrigC    = 2u,            ///< TIMx ����C�˿�
    AdtTrigD    = 3u,            ///< TIMx ����D�˿�
}en_adt_trig_port_t;

/**
 ******************************************************************************
 ** \brief ADTͨ�ÿ��� - Z����������������
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_gconr_zmsk
{
    AdtZMaskDis   = 0u,              ///< Z���������ι�����Ч
    AdtZMask4Cyl  = 1u,              ///< λ�ü���������������4�����������ڵ�Z�����뱻����
    AdtZMask8Cyl  = 2u,              ///< λ�ü���������������8�����������ڵ�Z�����뱻����
    AdtZMask16Cyl = 3u,              ///< λ�ü���������������16�����������ڵ�Z�����뱻����
}en_adt_gconr_zmsk_t;

/**
 ******************************************************************************
 ** \brief ADTͨ�ÿ��� - ����ʱ��ѡ��
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_cnt_ckdiv
{
    AdtClkPClk0        = 0u,         ///< PCLK0
    AdtClkPClk0Div2    = 1u,         ///< PCLK0/2
    AdtClkPClk0Div4    = 2u,         ///< PCLK0/4
    AdtClkPClk0Div8    = 3u,         ///< PCLK0/8
    AdtClkPClk0Div16   = 4u,         ///< PCLK0/16
    AdtClkPClk0Div64   = 5u,         ///< PCLK0/64
    AdtClkPClk0Div256  = 6u,         ///< PCLK0/256
    AdtClkPClk0Div1024 = 7u,         ///< PCLK0/1024
}en_adt_cnt_ckdiv_t;

/**
 ******************************************************************************
 ** \brief ADT����ģʽ
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_cnt_mode
{
    AdtSawtoothMode  = 0u,           ///< ��ݲ�ģʽ
    AdtTriangleModeA = 4u,           ///< ���ǲ�Aģʽ
    AdtTriangleModeB = 5u,           ///< ���ǲ�Bģʽ
}en_adt_cnt_mode_t;

/**
 ******************************************************************************
 ** \brief ADT��������
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_cnt_dir
{
    AdtCntDown = 0u,         ///< �ݼ�����
    AdtCntUp   = 1u,         ///< �ݼӼ���
}en_adt_cnt_dir_t;

/**
 ******************************************************************************
 ** \brief ADT�Ƚϻ�׼
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_compare
{
    AdtCompareA = 0u,            ///< ͨ�ñȽϻ�׼A
    AdtCompareB = 1u,            ///< ͨ�ñȽϻ�׼B
    AdtCompareC = 2u,            ///< ͨ�ñȽϻ�׼C
    AdtCompareD = 3u,            ///< ͨ�ñȽϻ�׼D
}en_adt_compare_t;

/**
 ******************************************************************************
 ** \brief ADT�˿ڿ��� - TIMx���״̬����
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_pconr_disval
{
    AdtTIMxDisValNorm = 0u,          ///< ǿ�������Ч����0~3�б�ѡ�����������ʱ��CHx�˿��������
    AdtTIMxDisValHiZ  = 1u,          ///< ǿ�������Ч����0~3�б�ѡ�����������ʱ��CHx�˿��������̬
    AdtTIMxDisValLow  = 2u,          ///< ǿ�������Ч����0~3�б�ѡ�����������ʱ��CHx�˿�����͵�ƽ
    AdtTIMxDisValHigh = 3u,          ///< ǿ�������Ч����0~3�б�ѡ�����������ʱ��CHx�˿�����ߵ�ƽ
}en_adt_pconr_disval_t;

/**
 ******************************************************************************
 ** \brief ADT�˿ڿ��� - CHxǿ�������Ч����ѡ��
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_pconr_dissel
{
    AdtCHxDisSel0 = 0u,             ///< ѡ��ǿ�������Ч����0
    AdtCHxDisSel1 = 1u,             ///< ѡ��ǿ�������Ч����1
    AdtCHxDisSel2 = 2u,             ///< ѡ��ǿ�������Ч����2
    AdtCHxDisSel3 = 3u,             ///< ѡ��ǿ�������Ч����3
}en_adt_pconr_dissel_t;

/**
 ******************************************************************************
 ** \brief ADT�˿ڿ��� - CHx����ֵƥ��ʱ�˿�״̬�趨
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_pconr_perc
{
    AdtCHxPeriodLow  = 0u,          ///< ����������ֵ������ֵ���ʱ��CHx�˿��������Ϊ�͵�ƽ
    AdtCHxPeriodHigh = 1u,          ///< ����������ֵ������ֵ���ʱ��CHx�˿�����趨Ϊ�ߵ�ƽ
    AdtCHxPeriodKeep = 2u,          ///< ����������ֵ������ֵ���ʱ��CHx�˿�����趨Ϊ��ǰ״̬
    AdtCHxPeriodInv  = 3u,          ///< ����������ֵ������ֵ���ʱ��CHx�˿�����趨Ϊ��ת��ƽ
}en_adt_pconr_perc_t;

/**
 ******************************************************************************
 ** \brief ADT�˿ڿ��� - CHx�Ƚ�ֵƥ��ʱ�˿�״̬�趨
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_pconr_cmpc
{
    AdtCHxCompareLow  = 0u,         ///< ����������ֵ��GCMxR���ʱ��CHx�˿��������Ϊ�͵�ƽ
    AdtCHxCompareHigh = 1u,         ///< ����������ֵ��GCMxR���ʱ��CHx�˿�����趨Ϊ�ߵ�ƽ
    AdtCHxCompareKeep = 2u,         ///< ����������ֵ��GCMxR���ʱ��CHx�˿�����趨Ϊ��ǰ״̬
    AdtCHxCompareInv  = 3u,         ///< ����������ֵ��GCMxR���ʱ��CHx�˿�����趨Ϊ��ת��ƽ
}en_adt_pconr_cmpc_t;

/**
 ******************************************************************************
 ** \brief ADT�˿ڿ��� - CHx�˿����
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_pconr_port_out
{
    AdtCHxPortOutLow  = 0u,     ///< CHx�˿�����趨Ϊ�͵�ƽ
    AdtCHxPortOutHigh = 1u,     ///< CHx�˿�����趨Ϊ�ߵ�ƽ
}en_adt_pconr_port_out_t;

/**
 ******************************************************************************
 ** \brief ADT�˿ڿ��� - CHx�˿ڹ���ģʽѡ��
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_pconr_capc
{
    AdtCHxCompareOutput = 0u,       ///< CHx�˿��趨Ϊ�Ƚ��������
    AdtCHxCompareInput  = 1u,       ///< CHx�˿��趨Ϊ�������빦��
}en_adt_pconr_capc_t;

/**
 ******************************************************************************
 ** \brief ADT�˿ڿ��� - CHx������ʼֹͣ�˿�״̬ѡ��
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_pconr_stastps
{
    AdtCHxStateSelSS   = 0u,    ///< ������ʼ��ֹͣʱ��CHx�˿������STACB��STPCB����
    AdtCHxStateSelKeep = 1u,    ///< ������ʼ��ֹͣʱ��CHx�˿�����趨Ϊ��ǰ״̬
}en_adt_pconr_stastps_t;

/**
 ******************************************************************************
 ** \brief ADT�������� - CHx���������趨
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_dconr_sepa
{
    AdtCHxDtSeperate = 0u,      ///< DTUAR��DTDAR�ֱ��趨
    AdtCHxDtEqual    = 1u,      ///< DTDAR��ֵ��DTUAR��ֵ�Զ����
}en_adt_dconr_sepa_t;

/**
 ******************************************************************************
 ** \brief ADT�˲����� - TRIx/TIMxIx�˿��˲�������׼ʱ��ѡ��
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_fconr_nofick
{
    AdtFltClkPclk0      = 0u,    ///< PCLK0
    AdtFltClkPclk0Div4  = 1u,    ///< PCLK0/4
    AdtFltClkPclk0Div16 = 2u,    ///< PCLK0/16
    AdtFltClkPclk0Div64 = 3u,    ///< PCLK0/64
}en_adt_fconr_nofick_t;

/**
 ******************************************************************************
 ** \brief ADT��Ч���� - TIMx��Ч����ѡ��
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_vperr_pcnts
{
    AdtPeriodCnts0 = 0u,         ///< ��Ч����ѡ������Ч
    AdtPeriodCnts1 = 1u,         ///< ÿ��1��������Чһ��
    AdtPeriodCnts2 = 2u,         ///< ÿ��2��������Чһ��
    AdtPeriodCnts3 = 3u,         ///< ÿ��3��������Чһ��
    AdtPeriodCnts4 = 4u,         ///< ÿ��4��������Чһ��
    AdtPeriodCnts5 = 5u,         ///< ÿ��5��������Чһ��
    AdtPeriodCnts6 = 6u,         ///< ÿ��6��������Чһ��
    AdtPeriodCnts7 = 7u,         ///< ÿ��7��������Чһ��
}en_adt_vperr_pcnts_t;

/**
 ******************************************************************************
 ** \brief ADT��Ч���� - ��������ѡ��
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_vperr_pcnte
{
    AdtPeriodCnteDisable = 0u,       ///< ��Ч����ѡ������Ч
    AdtPeriodCnteMin     = 1u,       ///< ��ݲ������ϡ����������ǲ�������Ϊ��������
    AdtPeriodCnteMax     = 2u,       ///< ��ݲ������ϡ����������ǲ�������Ϊ��������
    AdtPeriodCnteBoth    = 3u,       ///< ��ݲ������ϡ����������ǲ����壬������Ϊ��������
}en_adt_vperr_pcnte_t;

/**
 ******************************************************************************
 ** \brief ADT�˿ڴ������� - ����Դѡ��
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_ttrig_trigxs
{
    AdtTrigxSelP01 = 0u,         ///< P01
    AdtTrigxSelP02 = 1u,         ///< P02
    AdtTrigxSelP03 = 2u,         ///< P03
    AdtTrigxSelP14 = 3u,         ///< P14
    AdtTrigxSelP15 = 4u,         ///< P15
    AdtTrigxSelP23 = 5u,         ///< P23
    AdtTrigxSelP24 = 6u,         ///< P24
    AdtTrigxSelP25 = 7u,         ///< P25
    AdtTrigxSelP26 = 8u,         ///< P26
    AdtTrigxSelP27 = 9u,         ///< P27
    AdtTrigxSelP31 = 10u,        ///< P31
    AdtTrigxSelP32 = 11u,        ///< P32
    AdtTrigxSelP33 = 12u,        ///< P33
    AdtTrigxSelP34 = 13u,        ///< P34
    AdtTrigxSelP35 = 14u,        ///< P35
    AdtTrigxSelP36 = 15u,        ///< P36
}en_adt_ttrig_trigxs_t;

/**
 ******************************************************************************
 ** \brief ADT AOS�������� - AOSx����Դѡ��
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_itrig_iaosxs
{
    AdtAosxTrigSelTim0Int  = 0u,          ///< TIM0_INT
    AdtAosxTrigSelTim1Int  = 1u,          ///< TIM1_INT
    AdtAosxTrigSelTim2Int  = 2u,          ///< TIM2_INT
    AdtAosxTrigSelTim4Int  = 4u,          ///< TIM4_INT
    AdtAosxTrigSelTim5Int  = 5u,          ///< TIM5_INT
    AdtAosxTrigSelTim6Int  = 6u,          ///< TIM6_INT
    AdtAosxTrigSelUart0Int = 7u,          ///< UART0_INT
    AdtAosxTrigSelUart1Int = 8u,          ///< UART1_INT
    AdtAosxTrigSelVc1Int   = 10u,         ///< VC1_INT
    AdtAosxTrigSelVc2Int   = 11u,         ///< VC2_INT
    AdtAosxTrigSelPcaInt   = 13u,         ///< PCA_INT
    AdtAosxTrigSelSpiInt   = 14u,         ///< SPI_INT
    AdtAosxTrigSelAdcInt   = 15u,         ///< ADC_INT
}en_adt_itrig_iaosxs_t;

/**
 ******************************************************************************
 ** \brief ADTӲ��(����/ֹͣ/����/����)�¼�����ѡ��
 **
 ** \note
 ******************************************************************************/

typedef enum en_adt_hw_trig
{
    AdtHwTrigAos0        = 0u,       ///< ��AOS�����¼�����0��Ч
    AdtHwTrigAos1        = 1u,       ///< ��AOS�����¼�����1��Ч
    AdtHwTrigAos2        = 2u,       ///< ��AOS�����¼�����2��Ч
    AdtHwTrigAos3        = 3u,       ///< ��AOS�����¼�����3��Ч
    AdtHwTrigTimxARise   = 4u,       ///< CHxA�˿��ϲ�����������
    AdtHwTrigTimxAFall   = 5u,       ///< CHxA�˿��ϲ������½���
    AdtHwTrigTimxBRise   = 6u,       ///< CHxB�˿��ϲ�����������
    AdtHwTrigTimxBFall   = 7u,       ///< CHxB�˿��ϲ������½���
    AdtHwTrigTimTriARise = 8u,       ///< TIMTRIA�˿��ϲ�����������
    AdtHwTrigTimTriAFall = 9u,       ///< TIMTRIA�˿��ϲ������½���
    AdtHwTrigTimTriBRise = 10u,      ///< TIMTRIB�˿��ϲ�����������
    AdtHwTrigTimTriBFall = 11u,      ///< TIMTRIB�˿��ϲ������½���
    AdtHwTrigTimTriCRise = 12u,      ///< TIMTRIC�˿��ϲ�����������
    AdtHwTrigTimTriCFall = 13u,      ///< TIMTRIC�˿��ϲ������½���
    AdtHwTrigTimTriDRise = 14u,      ///< TIMTRID�˿��ϲ�����������
    AdtHwTrigTimTriDFall = 15u,      ///< TIMTRID�˿��ϲ������½���
    AdtHwTrigEnd         = 16u,
}en_adt_hw_trig_t;

/**
 ******************************************************************************
 ** \brief ADTӲ��(�ݼ�/�ݼ�)�¼�����ѡ��
 **
 ** \note
 ******************************************************************************/

typedef enum en_adt_hw_cnt
{
    AdtHwCntTimxALowTimxBRise   = 0u,    ///< CHxA�˿�Ϊ�͵�ƽʱ��CHxB�˿��ϲ�����������
    AdtHwCntTimxALowTimxBFall   = 1u,    ///< CHxA�˿�Ϊ�͵�ƽʱ��CHxB�˿��ϲ������½���
    AdtHwCntTimxAHighTimxBRise  = 2u,    ///< CHxA�˿�Ϊ�ߵ�ƽʱ��CHxB�˿��ϲ�����������
    AdtHwCntTimxAHighTimxBFall  = 3u,    ///< CHxA�˿�Ϊ�ߵ�ƽʱ��CHxB�˿��ϲ������½���
    AdtHwCntTimxBLowTimxARise   = 4u,    ///< CHxB�˿�Ϊ�͵�ƽʱ��CHxA�˿��ϲ�����������
    AdtHwCntTimxBLowTimxAFall   = 5u,    ///< CHxB�˿�Ϊ�͵�ƽʱ��CHxA�˿��ϲ������½���
    AdtHwCntTimxBHighTimxARise  = 6u,    ///< CHxB�˿�Ϊ�ߵ�ƽʱ��CHxA�˿��ϲ�����������
    AdtHwCntTimxBHighTimxAFall  = 7u,    ///< CHxB�˿�Ϊ�ߵ�ƽʱ��CHxA�˿��ϲ������½���
    AdtHwCntTimTriARise         = 8u,    ///< TIMTRIA�˿��ϲ�����������
    AdtHwCntTimTriAFall         = 9u,    ///< TIMTRIA�˿��ϲ������½���
    AdtHwCntTimTriBRise         = 10u,   ///< TIMTRIB�˿��ϲ�����������
    AdtHwCntTimTriBFall         = 11u,   ///< TIMTRIB�˿��ϲ������½���
    AdtHwCntTimTriCRise         = 12u,   ///< TIMTRIC�˿��ϲ�����������
    AdtHwCntTimTriCFall         = 13u,   ///< TIMTRIC�˿��ϲ������½���
    AdtHwCntTimTriDRise         = 14u,   ///< TIMTRID�˿��ϲ�����������
    AdtHwCntTimTriDFall         = 15u,   ///< TIMTRID�˿��ϲ������½���
    AdtHwCntAos0                = 16u,   ///< ��AOS�����¼�����0��Ч
    AdtHwCntAos1                = 17u,   ///< ��AOS�����¼�����1��Ч
    AdtHwCntAos2                = 18u,   ///< ��AOS�����¼�����2��Ч
    AdtHwCntAos3                = 19u,   ///< ��AOS�����¼�����3��Ч
    AdtHwCntMax                 = 20u,
}en_adt_hw_cnt_t;

/**
 ******************************************************************************
 ** \brief ADT�˿�ɲ�����Կ���
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_ptbrk_polarity
{
    AdtPtBrkHigh = 0u,       ///< �˿�ɲ�����Ըߵ�ƽ��Ч
    AdtPtBrkLow  = 1u,       ///< �˿�ɲ�����Ե͵�ƽ��Ч
}en_adt_ptbrk_polarity_t;

/**
 ******************************************************************************
 ** \brief ADT PWMչƵ����ѡ��
 **
 ** \note
 ******************************************************************************/
typedef enum en_adt_pwm_dither_type
{
    AdtPwmDitherUnderFlow = 0u,      ///< PWMչƵ���������
    AdtPwmDitherOverFlow  = 1u,       ///< PWMչƵ���������
}en_adt_pwm_dither_type_t;

/**
 ******************************************************************************
 ** \brief ADT�ж�����
 **
 ** \note
 ******************************************************************************/

typedef enum en_adt_irq_type
{
    AdtCMAIrq  = 0u,     ///< ����ƥ��A���򲶻����룩�ж�
    AdtCMBIrq  = 1u,     ///< ����ƥ��B���򲶻����룩�ж�
    AdtCMCIrq  = 2u,     ///< ����ƥ��C�ж�
    AdtCMDIrq  = 3u,     ///< ����ƥ��D�ж�
    AdtOVFIrq  = 6u,     ///< ����ƥ���ж�
    AdtUDFIrq  = 7u,     ///< ����ƥ���ж�
    AdtDTEIrq  = 8u,     ///< ����ʱ������ж�
}en_adt_irq_type_t;

/**
 ******************************************************************************
 ** \brief ADT����ͬ������
 ** \note
 ******************************************************************************/
typedef struct stc_adt_sw_sync
{
    boolean_t               bAdTim4;      ///< Timer 4
    boolean_t               bAdTim5;      ///< Timer 5
    boolean_t               bAdTim6;      ///< Timer 6

}stc_adt_sw_sync_t;

/**
 ******************************************************************************
 ** \brief ADT AOS��������
 ** \note
 ******************************************************************************/
typedef struct stc_adt_aos_trig_cfg
{
    en_adt_itrig_iaosxs_t   enAos0TrigSrc;      ///< AOS0����Դѡ��
    en_adt_itrig_iaosxs_t   enAos1TrigSrc;      ///< AOS1����Դѡ��
    en_adt_itrig_iaosxs_t   enAos2TrigSrc;      ///< AOS2����Դѡ��
    en_adt_itrig_iaosxs_t   enAos3TrigSrc;      ///< AOS3����Դѡ��
}stc_adt_aos_trig_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT �жϴ�������
 ** \note
 ******************************************************************************/
typedef struct stc_adt_irq_trig_cfg
{
    boolean_t   bAdtUnderFlowTrigEn;      ///< ����ƥ��ʹ�ܴ���ADC
    boolean_t   bAdtOverFlowTrigEn;       ///< ����ƥ��ʹ�ܴ���ADC
    boolean_t   bAdtCntMatchDTrigEn;      ///< ����ƥ��Dʹ�ܴ���ADC
    boolean_t   bAdtCntMatchCTrigEn;      ///< ����ƥ��Cʹ�ܴ���ADC
    boolean_t   bAdtCntMatchBTrigEn;      ///< ����ƥ��Bʹ�ܴ���ADC
    boolean_t   bAdtCntMatchATrigEn;      ///< ����ƥ��Aʹ�ܴ���ADC
}stc_adt_irq_trig_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT Trig�˿�����
 ** \note
 ******************************************************************************/
typedef struct stc_adt_port_trig_cfg
{
    en_adt_ttrig_trigxs_t   enTrigSrc;      ///< ����Դѡ��
    boolean_t               bFltEn;         ///< ����Դ���������˲�ʹ��
    en_adt_fconr_nofick_t   enFltClk;       ///< �˲�������׼ʱ��
}stc_adt_port_trig_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT Z���������ι�������
 ** \note
 ******************************************************************************/
typedef struct stc_adt_zmask_cfg
{
    en_adt_gconr_zmsk_t     enZMaskCycle;              ///< Z���������μ�������ѡ��
    boolean_t               bFltPosCntMaksEn;          ///< Z������ʱ�����������ڣ�λ�ü����������㹦�ܲ����Σ�FALSE��������(TRUE)
    boolean_t               bFltRevCntMaksEn;          ///< Z������ʱ�����������ڣ���ת�������ļ������ܲ����Σ�FALSE��������(TRUE)
}stc_adt_zmask_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT TIMxX�˿�����
 ** \note
 ******************************************************************************/
typedef struct stc_adt_TIMxX_port_cfg
{
    en_adt_pconr_capc_t     enCap;      ///< �˿ڹ���ģʽ
    boolean_t               bOutEn;     ///< ���ʹ��
    en_adt_pconr_perc_t     enPerc;     ///< ����ֵƥ��ʱ�˿�״̬
    en_adt_pconr_cmpc_t     enCmpc;     ///< �Ƚ�ֵƥ��ʱ�˿�״̬
    en_adt_pconr_stastps_t  enStaStp;   ///< ������ʼֹͣ�˿�״̬ѡ��
    en_adt_pconr_port_out_t enStaOut;   ///< ������ʼ�˿����״̬
    en_adt_pconr_port_out_t enStpOut;   ///< ����ֹͣ�˿����״̬
    en_adt_pconr_disval_t   enDisVal;   ///< ǿ�������Чʱ���״̬����
    en_adt_pconr_dissel_t   enDisSel;   ///< ǿ�������Ч����ѡ��
    boolean_t               bFltEn;     ///< �˿ڲ��������˲�ʹ��
    en_adt_fconr_nofick_t   enFltClk;   ///< �˿��˲�������׼ʱ��
}stc_adt_CHxX_port_cfg_t;

/**
 ******************************************************************************
 ** \brief ADTɲ���˿�����
 ** \note
 ******************************************************************************/
typedef struct stc_adt_break_port_cfg
{
    boolean_t               bPortEn;    ///< �˿�ʹ��
    en_adt_ptbrk_polarity_t enPol;      ///< ����ѡ��
}stc_adt_break_port_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT��Ч����3����
 ** \note
 ******************************************************************************/
typedef struct stc_adt_disable_3_cfg
{
    stc_adt_break_port_cfg_t    stcBrkPtCfg[16];    ///< ɲ���˿�����
    boolean_t                   bFltEn;             ///< ɲ���˿��˲�ʹ��
    en_adt_fconr_nofick_t       enFltClk;           ///< �˲�������׼ʱ��
    boolean_t                   bSwBrk;             ///< ����ɲ��
}stc_adt_disable_3_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT��Ч����1����
 ** \note
 ******************************************************************************/
typedef struct stc_adt_disable_1_cfg
{
    boolean_t   bTim6OutSH;     ///< TIM6���ͬ��
    boolean_t   bTim5OutSH;     ///< TIM5���ͬ��
    boolean_t   bTim4OutSH;     ///< TIM4���ͬ��
    boolean_t   bTim6OutSL;     ///< TIM6���ͬ��
    boolean_t   bTim5OutSL;     ///< TIM5���ͬ��
    boolean_t   bTim4OutSL;     ///< TIM4���ͬ��
}stc_adt_disable_1_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT PWMչƵ��������
 ** \note
 ******************************************************************************/
typedef struct stc_adt_pwm_dither_cfg
{
    en_adt_pwm_dither_type_t    enAdtPDType;    ///< PWMչƵ����ѡ��
    boolean_t   bTimxBPDEn;                     ///< PWMͨ��BչƵʹ��
    boolean_t   bTimxAPDEn;                     ///< PWMͨ��AչƵʹ��
}stc_adt_pwm_dither_cfg_t;


/**
 ******************************************************************************
 ** \brief ADT������������
 ** \note
 ******************************************************************************/
typedef struct stc_adt_basecnt_cfg
{
    en_adt_cnt_mode_t       enCntMode;      ///< ����ģʽ
    en_adt_cnt_dir_t        enCntDir;       ///< ��������
    en_adt_cnt_ckdiv_t      enCntClkDiv;    ///< ����ʱ��ѡ��
}stc_adt_basecnt_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT����״̬
 ** \note
 ******************************************************************************/
typedef struct stc_adt_cntstate_cfg
{
    uint16_t         u16Counter;      ///< ��ǰ�������ļ���ֵ
    boolean_t        enCntDir;        ///< ��������
    uint8_t          u8ValidPeriod;   ///< ��Ч���ڼ���
    boolean_t        bDTEF;           ///< ����ʱ������־
    boolean_t        bUDFF;           ///< ����ƥ���־
    boolean_t        bOVFF;           ///< ����ƥ���־
    boolean_t        bCMDF;           ///< ����ƥ��D��־
    boolean_t        bCMCF;           ///< ����ƥ��C��־
    boolean_t        bCMBF;           ///< ����ƥ��B��־
    boolean_t        bCMAF;           ///< ����ƥ��A��־
}stc_adt_cntstate_cfg_t;

/**
 ******************************************************************************
 ** \brief ADT��Ч��������
 ** \note
 ******************************************************************************/
typedef struct stc_adt_validper_cfg
{
    en_adt_vperr_pcnts_t    enValidCnt;     ///< ��Ч����ѡ��
    en_adt_vperr_pcnte_t    enValidCdt;     ///< ��Ч���ڼ�������
    boolean_t               bPeriodD;       ///< ͨ���ź���Ч����ѡ��D
    boolean_t               bPeriodC;       ///< ͨ���ź���Ч����ѡ��C
    boolean_t               bPeriodB;       ///< ͨ���ź���Ч����ѡ��B
    boolean_t               bPeriodA;       ///< ͨ���ź���Ч����ѡ��A
}stc_adt_validper_cfg_t;

/******************************************************************************
 * Global definitions
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
en_result_t Adt_ConfigHwCntUp(en_adt_unit_t enAdtUnit, en_adt_hw_cnt_t enAdtHwCntUp);
en_result_t Adt_ClearHwCntUp(en_adt_unit_t enAdtUnit);
en_result_t Adt_ConfigHwCntDwn(en_adt_unit_t enAdtUnit, en_adt_hw_cnt_t enAdtHwCntDwn);
en_result_t Adt_ClearHwCntDwn(en_adt_unit_t enAdtUnit);
en_result_t Adt_ConfigHwStart(en_adt_unit_t enAdtUnit, en_adt_hw_trig_t enAdtHwStart);
en_result_t Adt_ClearHwStart(en_adt_unit_t enAdtUnit);
en_result_t Adt_EnableHwStart(en_adt_unit_t enAdtUnit);
en_result_t Adt_DisableHwStart(en_adt_unit_t enAdtUnit);
en_result_t Adt_ConfigHwStop(en_adt_unit_t enAdtUnit, en_adt_hw_trig_t enAdtHwStop);
en_result_t Adt_ClearHwStop(en_adt_unit_t enAdtUnit);
en_result_t Adt_EnableHwStop(en_adt_unit_t enAdtUnit);
en_result_t Adt_DisableHwStop(en_adt_unit_t enAdtUnit);
en_result_t Adt_ConfigHwClear(en_adt_unit_t enAdtUnit, en_adt_hw_trig_t enAdtHwClear);
en_result_t Adt_ClearHwClear(en_adt_unit_t enAdtUnit);
en_result_t Adt_EnableHwClear(en_adt_unit_t enAdtUnit);
en_result_t Adt_DisableHwClear(en_adt_unit_t enAdtUnit);
en_result_t Adt_ConfigHwCaptureA(en_adt_unit_t enAdtUnit, en_adt_hw_trig_t enAdtHwCaptureA);
en_result_t Adt_ClearHwCaptureA(en_adt_unit_t enAdtUnit);
en_result_t Adt_ConfigHwCaptureB(en_adt_unit_t enAdtUnit, en_adt_hw_trig_t enAdtHwCaptureB);
en_result_t Adt_ClearHwCaptureB(en_adt_unit_t enAdtUnit);

en_result_t Adt_SwSyncStart(stc_adt_sw_sync_t* pstcAdtSwSyncStart);
en_result_t Adt_SwSyncStop(stc_adt_sw_sync_t* pstcAdtSwSyncStop);
en_result_t Adt_SwSyncClear(stc_adt_sw_sync_t* pstcAdtSwSyncClear);
en_result_t Adt_GetSwSyncState(stc_adt_sw_sync_t* pstcAdtSwSyncState);

en_result_t Adt_AosTrigConfig(stc_adt_aos_trig_cfg_t* pstcAdtAosTrigCfg);

en_result_t Adt_IrqTrigConfig(en_adt_unit_t enAdtUnit,
                              stc_adt_irq_trig_cfg_t* pstcAdtIrqTrigCfg);

en_result_t Adt_PortTrigConfig(en_adt_trig_port_t enAdtTrigPort,
                               stc_adt_port_trig_cfg_t* pstcAdtPortTrigCfg);

en_result_t Adt_CHxXPortConfig(en_adt_unit_t enAdtUnit,
                                en_adt_CHxX_port_t enAdtCHxXPort,
                                stc_adt_CHxX_port_cfg_t* pstcAdtCHxXCfg);

en_result_t Adt_EnableBreakPort(uint8_t port, stc_adt_break_port_cfg_t* pstcAdtBrkPtCfg);
void Adt_ClearBrakePort(void);
en_result_t Adt_Disable3Cfg(stc_adt_disable_3_cfg_t* pstcAdtDisable3);
boolean_t Adt_GetPortBrakeFlag(void);
void Adt_ClearPortBrakeFlag(void);
en_result_t Adt_Disable1Cfg(stc_adt_disable_1_cfg_t* pstcAdtDisable1);
boolean_t Adt_GetSameBrakeFlag(void);
void Adt_ClearSameBrakeFlag(void);

en_result_t Adt_PwmDitherConfig(stc_adt_pwm_dither_cfg_t* pstcAdtPwmDitherCfg);

en_result_t Adt_Init(en_adt_unit_t enAdtUnit, stc_adt_basecnt_cfg_t* pstcAdtBaseCntCfg);
en_result_t Adt_DeInit(en_adt_unit_t enAdtUnit);

en_result_t Adt_StartCount(en_adt_unit_t enAdtUnit);
en_result_t Adt_StopCount(en_adt_unit_t enAdtUnit);
en_result_t Adt_SetCount(en_adt_unit_t enAdtUnit, uint16_t u16Value);
en_result_t Adt_ClearCount(en_adt_unit_t enAdtUnit);
en_result_t Adt_GetCntState(en_adt_unit_t enAdtUnit, stc_adt_cntstate_cfg_t* pstcAdtCntState);
en_result_t Adt_SetPeriod(en_adt_unit_t enAdtUnit, uint16_t u16Period);
en_result_t Adt_SetPeriodBuf(en_adt_unit_t enAdtUnit, uint16_t u16PeriodBuf);
en_result_t Adt_ClearPeriodBuf(en_adt_unit_t enAdtUnit);
en_result_t Adt_SetValidPeriod(en_adt_unit_t enAdtUnit,
                               stc_adt_validper_cfg_t* pstcAdtValidPerCfg);
en_result_t Adt_SetCompareValue(en_adt_unit_t enAdtUnit,
                                en_adt_compare_t enAdtCompare,
                                uint16_t u16Compare);
en_result_t Adt_EnableValueBuf(en_adt_unit_t enAdtUnit,
                                 en_adt_CHxX_port_t enAdtCHxXPort,
                                 boolean_t bCompareBufEn);
en_result_t Adt_ClearValueBuf(en_adt_unit_t enAdtUnit,
                                     en_adt_CHxX_port_t enAdtCHxXPort);
en_result_t Adt_GetCaptureValue(en_adt_unit_t enAdtUnit,
                                en_adt_CHxX_port_t enAdtCHxXPort,
                                uint16_t* pu16Capture);
en_result_t Adt_GetCaptureBuf(en_adt_unit_t enAdtUnit,
                                en_adt_CHxX_port_t enAdtCHxXPort,
                                uint16_t* pu16CaptureBuf);

en_result_t Adt_SetDTUA(en_adt_unit_t enAdtUnit,
                        uint16_t u16Value);
en_result_t Adt_SetDTDA(en_adt_unit_t enAdtUnit,
                        uint16_t u16Value);
en_result_t Adt_ConfigDT(en_adt_unit_t enAdtUnit,
                         boolean_t bDTEn,
                         boolean_t bEqual);

en_result_t Adt_ConfigIrq(en_adt_unit_t enAdtUnit,
                          en_adt_irq_type_t enAdtIrq,
                          boolean_t bEn,
                          func_ptr_t pfnIrqCalbak);
en_result_t Adt_GetIrqFlag(en_adt_unit_t enAdtUnit,
                           en_adt_irq_type_t enAdtIrq,
                           boolean_t* pbFlag);
en_result_t Adt_ClearIrqFlag(en_adt_unit_t enAdtUnit,
                             en_adt_irq_type_t enAdtIrq);
en_result_t Adt_ConfigZMask(en_adt_unit_t enAdtUnit,
                            stc_adt_zmask_cfg_t* pstcAdtZMaskCfg);

//@} // ADT Group

#ifdef __cplusplus
}
#endif

#endif /* __ADT_H__ */
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/