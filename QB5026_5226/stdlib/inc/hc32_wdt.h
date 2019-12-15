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
/** \file wdt.h
 **
 ** Headerfile for WDT functions
 **  
 **
 ** History:
 **   - 2017-05-10   Cathy     First Version
 **
 *****************************************************************************/

#ifndef __WDT_H__
#define __WDT_H__

#include "hc32_ddl.h"
#include "hc32_it.h"

/**
 ******************************************************************************
 ** \defgroup WdtGroup Watchdog Timer (WDT)
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
 ** \brief wdt�����λ���ж�����
 *****************************************************************************/
typedef enum en_wdt_reset
{
    WRESET_EN    = 0,///<��λʹ��
    WINT_EN      = 1,///<�ж�ʹ��
}en_wdt_reset_t;
/**
 ******************************************************************************
 ** \brief wdt��ǰ����״̬
 *****************************************************************************/

typedef enum en_wdt_status
{
    Stop_Mode    = 0,///<ֹͣģ��
    Run_Mode     = 1,///<��������ģʽ
}en_wdt_status_t;

/**
 ******************************************************************************
 ** \brief wdt�������ã���ʱʱ�� ���жϺ�����
 *****************************************************************************/

typedef struct stc_wdt_config
{
    uint8_t          u8LoadValue;   ///<���ʱ��      
    en_wdt_reset_t   enResetEnable;  ///<�жϻ�λʹ��       
    func_ptr_t       pfnWdtIrqCb;   ///<�жϷ���ӿں���
} stc_wdt_config_t;
//wdt��ʼ��
en_result_t Wdt_Init(stc_wdt_config_t* pstcConfig);
void Wdt_DeInit(void);

//wdt��ʼ��ֹͣ
en_result_t Wdt_Start(void);

//wdt���ʱ�����ü���ǰ����ʱ�䡢״̬��ȡ
void Wdt_WriteWdtLoad(uint8_t u8LoadValue);
uint8_t Wdt_ReadWdtValue(void);
uint8_t Wdt_ReadwdtStatus(void);

//ι������
void Wdt_Feed(void);

//@} // WdtGroup

#ifdef __cplusplus
#endif

#endif /* __WDT_H__ */



