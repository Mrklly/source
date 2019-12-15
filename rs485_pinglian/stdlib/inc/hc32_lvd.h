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
/** \file lvd.h
 **
 ** Headerfile for Low Voltage Detector functions
 **
 **   - 2017-06-28 Alex    First Version
 **
 ******************************************************************************/

#ifndef __LVD_H__
#define __LVD_H__

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "interrupts_hc32l136.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup LvdGroup Low Voltage Detector (LVD)
 **
 ******************************************************************************/
//@{

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief LVD�����ѹԴ
 **
 ** \note
 ******************************************************************************/
typedef enum en_lvd_input
{
    LvdInputVCC  = 0u,                 ///< Vcc
    LvdInputPC13 = 1u,                ///< PC13
    LvdInputPB08 = 2u,                ///< PB08
    LvdInputPB07 = 3u,                ///< PB07
}en_lvd_input_t;

/**
 ******************************************************************************
 ** \brief LVD����ѹѡ��
 **
 ** \note
 ******************************************************************************/
typedef enum en_lvd_threshold
{
    LvdTH1p8V = 0u,              ///< 1.8V
    LvdTH1p9V = 1u,                  ///< 1.9V
    LvdTH2p0V = 2u,                  ///< 2.0V
    LvdTH2p1V = 3u,                  ///< 2.1V
    LvdTH2p2V = 4u,                  ///< 2.2V
    LvdTH2p3V = 5u,                  ///< 2.3V
    LvdTH2p4V = 6u,                  ///< 2.4V
    LvdTH2p5V = 7u,                  ///< 2.5V
    LvdTH2p6V = 8u,                  ///< 2.6V Ĭ�ϵ�ѹ
    LvdTH2p7V = 9u,                  ///< 2.7V
    LvdTH2p8V = 10u,                  ///< 2.8V
    LvdTH2p9V = 11u,                  ///< 2.9V
    LvdTH3p0V = 12u,                  ///< 3.0V
    LvdTH3p1V = 13u,                  ///< 3.1V
    LvdTH3p2V = 14u,                  ///< 3.2V
    LvdTH3p3V = 15u,                  ///< 3.3V
}en_lvd_threshold_t;

/**
 ******************************************************************************
 ** \brief LVD����˲�ʱ��
 **
 ** \note
 ******************************************************************************/
typedef enum en_lvd_filter_time
{
    LvdFilter30us   = 0u,              ///< 30us
    LvdFilter40us   = 1u,                  ///< 40us
    LvdFilter50us   = 2u,                  ///< 50us
    LvdFilter130us  = 3u,                 ///< 130us
    LvdFilter480us  = 4u,                 ///< 480us
    LvdFilter1800us = 5u,                ///< 1.8ms
    LvdFilter7d3ms  = 6u,                 ///< 7.3ms
    LvdFilter29ms   = 7u,                  ///< 29ms
}en_lvd_filter_time_t;

/**
 ******************************************************************************
 ** \brief LVD�жϴ�����ʽ
 **
 ** \note
 ******************************************************************************/
typedef enum en_lvd_irq_type
{
    LvdIrqHigh  = 0u,                 ///< �ߵ�ƽ����
    LvdIrqRise  = 1u,                 ///< �����ش���
    LvdIrqFall  = 2u,                 ///< �½��ش���
}en_lvd_irq_type_t;

/**
 ******************************************************************************
 ** \brief LVD����
 ** \note
 ******************************************************************************/
typedef struct stc_lvd_config
{
    boolean_t               bLvdReset;      ///< TRUE-����,FALSE-�ж�
    en_lvd_input_t          enInput;        ///< LVD�����ѹԴ
    en_lvd_threshold_t      enThreshold;    ///< LVD����ѹ
    boolean_t               bFilter;        ///< �Ƿ�ʹ������˲�
    en_lvd_filter_time_t    enFilterTime;   ///< ����˲�ʱ��
    en_lvd_irq_type_t       enIrqType;      ///< �жϴ�����ʽ
    func_ptr_t              pfnIrqCbk;      ///< �жϻص�����
}stc_lvd_config_t;


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
//irq enable/disable
en_result_t Lvd_EnableIrq(en_lvd_irq_type_t enType);
void Lvd_DisableIrq(void);

// irq function
void Lvd_ClearIrq(void);
boolean_t Lvd_GetIrqStat(void);

// fliter function
boolean_t Lvd_GetFilterResult(void);

//init/deinit function
void Lvd_Init(stc_lvd_config_t *pstcConfig);
void Lvd_DeInit(void);

//LVD function enable/disable
void Lvd_Enable(void);
void Lvd_Disable(void);

//@} // LvdGroup

#ifdef __cplusplus
}
#endif

#endif /* __LVD_H__ */
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
