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
/*****************************************************************************/
/** \file bt.h
 **
 ** ������ʱ�����ݽṹ��API����
 ** @link BT Group Some description @endlink
 **
 ** History:
 **   - 2017-05-10   Lux     V1.0
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
 ** \brief ���Կ������������ض��� (GATE_P)
 *****************************************************************************/
typedef enum en_bt_gatep
{
    BtPositive = 0u,            ///< �ߵ�ƽ��Ч
    BtOpposite = 1u,            ///< �͵�ƽ��Ч
}en_bt_gatep_t;

/**
 ******************************************************************************
 ** \brief ��ʱ���ſ����������ض��� (GATE)
 *****************************************************************************/
typedef enum en_bt_gate
{
    BtGateDisable = 0u,            ///< ���ſ�
    BtGateEnable  = 1u,            ///< �ſ�ʹ��
}en_bt_gate_t;

/**
 ******************************************************************************
 ** \brief TIM Ԥ��Ƶѡ�� (PRS)
 *****************************************************************************/
typedef enum en_bt_timclkdiv
{
    BtPCLKDiv1   = 0u,           ///< Div 1
    BtPCLKDiv2   = 1u,           ///< Div 2
    BtPCLKDiv4   = 2u,           ///< Div 4
    BtPCLKDiv8   = 3u,           ///< Div 8
    BtPCLKDiv16  = 4u,           ///< Div 16
    BtPCLKDiv32  = 5u,           ///< Div 32
    BtPCLKDiv64  = 6u,           ///< Div 64
    BtPCLKDiv256 = 7u,           ///< Div 256
}en_bt_timclkdiv_t;

/**
 ******************************************************************************
 ** \brief ��ת���ʹ�����������ض��� (TOG_EN)
 *****************************************************************************/
typedef enum en_bt_toggle
{
    BtTogDisable = 0u,            ///< ��ת�����ֹ
    BtTogEnable  = 1u,            ///< ��ת���ʹ��
}en_bt_toggle_t;

/**
 ******************************************************************************
 ** \brief ����/��ʱ������ѡ�����������ض��� (CT)
 *****************************************************************************/
typedef enum en_bt_function
{
    BtTimer   = 0u,            ///< ��ʱ������
    BtCounter = 1u,            ///< ����������
}en_bt_function_t;


/**
 ******************************************************************************
 ** \brief ��ʱ������ģʽ���������ض��� (MD)
 *****************************************************************************/
typedef enum en_bt_mode
{
    BtMode1  = 0u,         ///< 32λ������/��ʱ��
    BtMode2  = 1u,         ///< �Զ���װ��16λ������/��ʱ��
}en_bt_mode_t;

/**
 ******************************************************************************
 ** \brief ��ʱ�����п������������ض��� (TR)
 *****************************************************************************/
typedef enum en_bt_start
{
    BtTRDisable = 0u,            ///< ֹͣ
    BtTREnable  = 1u,            ///< ����
}en_bt_start_t;

/**
 ******************************************************************************
 ** \brief Timer ���ýṹ�嶨�� 
 *****************************************************************************/
typedef struct stc_bt_config
{
    en_bt_gatep_t     enGateP;          ///< �ſؼ��Կ���
    en_bt_gate_t      enGate;           ///< �ſ�ʹ��
    en_bt_timclkdiv_t enPRS;            ///< Ԥ��Ƶ����
    en_bt_toggle_t    enTog;            ///< ��ת���ʹ��
    en_bt_function_t  enCT;             ///< ��ʱ/��������ѡ��
    en_bt_mode_t      enMD;             ///< ����ģʽ����
    
    func_ptr_t        pfnTim0Cb;        ///< Timer0�жϷ���ص�����[void function(void)]
    func_ptr_t        pfnTim1Cb;        ///< Timer1�жϷ���ص�����[void function(void)]
    func_ptr_t        pfnTim2Cb;        ///< Timer2�жϷ���ص�����[void function(void)]
}stc_bt_config_t;


/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 *****************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 *****************************************************************************/
//�ж���غ��� 
//�жϱ�־��ȡ
boolean_t Bt_GetIntFlag(en_bt_unit_t enUnit);
//�жϱ�־���
en_result_t Bt_ClearIntFlag(en_bt_unit_t enUnit);
//�ж�ʹ��/��ֹ
en_result_t Bt_EnableIrq (en_bt_unit_t enUnit);
en_result_t Bt_DisableIrq(en_bt_unit_t enUnit);

//��ʼ������ع��ܲ���
//timer���ü���ʼ��
en_result_t Bt_Init(en_bt_unit_t enUnit, stc_bt_config_t* pstcConfig);
//Lptimer ����/ֹͣ
en_result_t Bt_Run(en_bt_unit_t enUnit);
en_result_t Bt_Stop(en_bt_unit_t enUnit);
//����ֵ����
en_result_t Bt_ARRSet(en_bt_unit_t enUnit, uint16_t u16Data);
//16λ����ֵ����/��ȡ
en_result_t Bt_Cnt16Set(en_bt_unit_t enUnit, uint16_t u16Data);
uint16_t Bt_Cnt16Get(en_bt_unit_t enUnit);
//32λ����ֵ����/��ȡ
en_result_t Bt_Cnt32Set(en_bt_unit_t enUnit, uint32_t u32Data);
uint32_t Bt_Cnt32Get(en_bt_unit_t enUnit); 

//@} // BtGroup

#ifdef __cplusplus
#endif


#endif /* __BT_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/


