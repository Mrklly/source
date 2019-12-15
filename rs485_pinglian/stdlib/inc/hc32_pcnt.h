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
/** \file pcnt.h
 **
 ** Headerfile for PCNT functions
 ** @link PCNT Group Some description @endlink 
 **
 ** History:
 **   - 2018-04-15   Devi     First Version
 **
 ******************************************************************************/

#ifndef __PCNT_H__
#define __PCNT_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "interrupts_hc32l136.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup PCNTGroup  (PCNT)
  **
 ******************************************************************************/
//@{

/**
 *******************************************************************************
 ** function prototypes.
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief PCNT S1ͨ������ѡ��
 ** \note
 ******************************************************************************/
typedef enum en_pcnt_s1sel
{
    S1P_Noinvert = 0u, ///<<S1ͨ�����Բ�ȡ��
    S1P_Invert   = 1u, ///<<S1ͨ������ȡ��
}en_pcnt_s1sel_t;

/**
 *******************************************************************************
 ** \brief PCNT S0ͨ������ѡ��
 ** \note
 ******************************************************************************/
typedef enum en_pcnt_s0sel
{
    S0P_Noinvert = 0u, ///<<S0ͨ�����Բ�ȡ��
    S0P_Invert   = 1u, ///<<S0ͨ������ȡ��
}en_pcnt_s0sel_t;

/**
 *******************************************************************************
 ** \brief PCNT ��������ѡ��
 ** \note
 ******************************************************************************/
typedef enum en_pcnt_direcsel
{
    Direct_Add   = 0u, ///<<�Ӽ���
    Direct_Sub   = 1u, ///<<������
}en_pcnt_direcsel_t;

/**
 *******************************************************************************
 ** \brief PCNT ����ʱ��ѡ��
 ** \note
 ******************************************************************************/
typedef enum en_pcnt_clksel
{
    CLK_Pclk   = 1u, ///<<PCLK
    CLK_Xtl    = 2u, ///<<XTL
    CLK_Rcl    = 3u, ///<<RCL     
}en_pcnt_clksel_t;

/**
 *******************************************************************************
 ** \brief PCNT �������ģʽѡ��
 ** \note
 ******************************************************************************/
typedef enum en_pcnt_modesel
{
    Single_Mode   = 1u, ///<<��ͨ���������ģʽ
    Special_Mode  = 2u, ///<<˫ͨ���������������ģʽ
    Double_Mode   = 3u, ///<<˫ͨ�������������ģʽ  
}en_pcnt_modesel_t;

/**
 ******************************************************************************
 ** \brief PCNT ״̬
 *****************************************************************************/
typedef enum en_pcnt_status
{
    PCNT_S1E            = 0x80,   ///<S1ͨ������������
    PCNT_S0E            = 0x40,   ///<S0ͨ������������   
    PCNT_BB             = 0x20,   ///<����������    
    PCNT_FE             = 0x10,   ///<������������������
    PCNT_DIR            = 0x08,   ///<����ı��ж�    
    PCNT_TO             = 0x04,   ///<��ʱ�жϱ�ʶ   
    PCNT_OV             = 0x02,   ///<�����жϱ�ʶ
    PCNT_UF             = 0x01,   ///<�����жϱ�ʶ     
}en_pcnt_status_t;

/**
 ******************************************************************************
 ** \brief PCNT �������ýṹ��
 *****************************************************************************/
typedef struct stc_pcnt_config
{
    en_pcnt_s1sel_t     bS1Sel;          ///< S1ͨ������ѡ��
	  en_pcnt_s0sel_t     bS0Sel;          ///< S0ͨ������ѡ��
    en_pcnt_direcsel_t  u8Direc;         ///< ��������ѡ��    	
	  en_pcnt_clksel_t    u8Clk;           ///< ����ʱ��ѡ��
    en_pcnt_modesel_t   u8Mode;          ///< �������ģʽѡ��
    boolean_t           bFLTEn;          ///< �˲�����ʹ��
    uint8_t             u8FLTDep;        ///< �˲���ֵ
    uint8_t             u8FLTClk;        ///< �˲�ʱ��
    boolean_t           bTOEn;           ///< ��ʱʹ��
    uint16_t            u16TODep;         ///< ��ʱ��ֵ
	  uint8_t             u8IrqStatus;     ///< �жϷ�ʽ
    boolean_t           bIrqEn;           ///< �ж�ʹ��
    func_ptr_t          pfnIrqCb;         ///< �жϻص�����
}stc_pcnt_config_t;

//PCNT �ж�
void Pcnt_IRQHandler(void);

/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
///< PCNT��ʼ��
en_result_t PCNT_Init(stc_pcnt_config_t*  pstcPcntConfig);
///< PCNTȥ��ʼ��
void PCNT_DeInit(void);
///< PCNT ��������
en_result_t PCNT_Parameter(uint8_t start,uint8_t end);
///< PCNT ��ȡPCNT ����
en_pcnt_direcsel_t PCNT_Direction(void);
///< PCNT ��ȡPCNT ����ֵ
uint16_t PCNT_Count(void);
///< PCNT ��ȡPCNT ���ֵ
uint16_t PCNT_TopCount(void);
///< PCNT ����PCNT
void PCNT_Run(boolean_t work);
///< PCNT��ȡ״̬
boolean_t PCNT_GetStatus(en_pcnt_status_t enStatus);
///< PCNT���״̬
void PCNT_ClrStatus(en_pcnt_status_t enStatus);
///< PCNT�����ж�
void PCNT_SetIrqStatus(en_pcnt_status_t enStatus);


//@} // PCNT Group

#ifdef __cplusplus
#endif

#endif /* __PCNT_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


