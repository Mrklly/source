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
/** \file trim.h
 **
 ** TRIM ���ݽṹ��API����
 **  
 **
 ** History:
 **   - 2017-05-10   Lux     V1.0
 **
 *****************************************************************************/

#ifndef __TRIM_H__
#define __TRIM_H__

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
 ** \defgroup TrimGroup Clock Trimming (TRIM)
 **
 ******************************************************************************/
//@{

/******************************************************************************
 ** Global pre-processor symbols/macros ('#define')                            
 ******************************************************************************/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

 /**
 ******************************************************************************
 ** \brief ���ģʽʹ��ö���ض���    (MON_EN)
 *****************************************************************************/
typedef enum en_trim_monitor
{
    MonDisable = 0u,            ///< ��ֹ
    MonEnable  = 1u,            ///< ʹ��
}en_trim_monitor_t; 
 
 /**
 ******************************************************************************
 ** \brief ��У׼/���ʱ��ѡ��ö���ض��� (CALCLK_SEL)
 *****************************************************************************/
typedef enum en_trim_calclksel
{
    CALIRC16M  = 0u,            ///< �ڲ�16M
    CALXTAL32M = 1u,            ///< �ⲿ32M
    CALIRC32K  = 2u,            ///< �ڲ�32K
}en_trim_calclksel_t;

/**
 ******************************************************************************
 ** \brief �ο�ʱ��ѡ��ö���ض��� (REFCLK_SEL)
 *****************************************************************************/
typedef enum en_trim_refclksel
{
    RefIRC16M  = 0u,            ///< �ڲ�16M
    RefXTAL32M = 1u,            ///< �ⲿ32M
    RefIRC32K  = 2u,            ///< �ڲ�32K
    RefIRC10K  = 4u,            ///< �ڲ�10K
    ExtClkIn   = 5u,            ///< �ⲿ����ʱ��
}en_trim_refclksel_t;

/**
 ******************************************************************************
 ** \brief �жϱ�־����ö���ض���
 *****************************************************************************/
typedef enum en_trim_inttype
{
    RefStop      = 0u,            ///< �ο�������ֹͣ��־
    CalCntOf     = 1u,            ///< У׼������ֹͣ��־
    XTAL32MFault = 3u,            ///< XTAL32M ʧЧ��־
}en_trim_inttype_t;

/**
 ******************************************************************************
 ** \brief TRIM ���ýṹ�嶨�� 
 *****************************************************************************/
typedef struct stc_trim_config
{
    en_trim_monitor_t    enMON;         ///< ���ģʽ
    en_trim_calclksel_t  enCALCLK;      ///< У׼ʱ��ѡ��
    en_trim_refclksel_t  enREFCLK;      ///< �ο�ʱ��ѡ��
    
    func_ptr_t           pfnTrimCb;     ///< TRIM �жϷ���ص�����[void function(void)]
}stc_trim_config_t;

/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 *****************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 *****************************************************************************/
//�жϲ�����غ���
//�жϱ�־��ȡ
boolean_t Trim_GetIntFlag(en_trim_inttype_t enIntType);
//�жϱ�־���
en_result_t Trim_ClearIntFlag(en_trim_inttype_t enIntType);
//�ж�ʹ��/��ֹ
en_result_t Trim_EnableIrq (void);
en_result_t Trim_DisableIrq(void);

//�������ü���������
//Trim ���ü���ʼ��
en_result_t Trim_Init(stc_trim_config_t* pstcConfig);
//У׼/�������/ֹͣ
en_result_t Trim_Run(void);
en_result_t Trim_Stop(void);
//�ο���������ֵ����
en_result_t Trim_RefConSet(uint32_t u32Data);
//У׼��������ֵ����
en_result_t Trim_CalConSet(uint32_t u32Data);
//�ο�����������ֵ��ȡ
uint32_t Trim_RefCntGet(void);
//У׼����������ֵ��ȡ
uint32_t Trim_CalCntGet(void);

//@} // TrimGroup

#ifdef __cplusplus
#endif


#endif /* __TRIM_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/


