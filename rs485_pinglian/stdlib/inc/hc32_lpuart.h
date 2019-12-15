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
/** \file uart.h
 **
 ** Headerfile for LPUART functions
 **  
 **
 ** History:
 **   - 2017-05-10   Cathy     First Version
 **
 *****************************************************************************/

#ifndef __LPUART_H__
#define __LPUART_H__
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
 ** \defgroup LPUartGroup Universal Asynchronous Receiver/Transmitter (LPUART)
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
 **\brief LPuartͨ��ѡ��
 ******************************************************************************/

typedef enum en_lpuart_channel
{
    LPUART0 = 0u, ///<����2
    LPUART1 = 1u, ///<����3
}en_lpuart_channel_t;
/**
 ******************************************************************************
 ** \brief lpuart ��sclkʱ��Դѡ��
 ******************************************************************************/

typedef enum en_lpuart_sclk_sel
{
    LPUart_Pclk   = 0u, ///<pclk
    LPUart_Pclk_1 = 1u, ///<pclk
    LPUart_Xtl    = 2u, ///<�ⲿ���پ���
    LPUart_Rcl    = 3u, ///<�ڲ����پ���
} en_lpuart_sclksel_t;

/**
 ******************************************************************************
 ** \brief lpuart������ģʽ��ַ֡/����֡������żУ��
 ******************************************************************************/
typedef enum en_lpuart_mmdorck
{
    LPUartDataOrAddr = 0u, ///<������ģʽʱ��ͨ����дSBUF[8]����֡Ϊ����֡���ַ֡
    LPUartEven       = 1u, ///<�Ƕ�����ģʽżУ��   
    LPUartOdd        = 2u, ///<�Ƕ�����ģʽ��У��    
}en_lpuart_mmdorck_t;

/**
 ******************************************************************************
 ** \brief lpuart������ģʽ����
 ******************************************************************************/
typedef enum en_lpuart_multimode
{
    LPUartNormal = 0u, ///<��������ģʽ
    LPUartMulti  = 1u, ///<����������ģʽ
}en_lpuart_multimode_t;
/**
 ******************************************************************************
 ** \brief lpuart������ģʽ���ӻ���ַ�͵�ַ��������
 ******************************************************************************/

typedef struct stc_lpuart_multimode
{
    en_lpuart_multimode_t enMulti_mode; ///<������ģʽ����
    uint8_t             u8SlaveAddr;  ///<�ӻ���ַ
    uint8_t                u8SaddEn;  ///<�Ӽ���ַ����
}stc_lpuart_multimode_t;

/**
 ******************************************************************************
 ** \brief lpuart ���ֹ���ģʽѡ��
 ******************************************************************************/

typedef enum en_lpuart_mode
{
    LPUartMode0 = 0u, ///<ģʽ0    
    LPUartMode1 = 1u, ///<ģʽ1
    LPUartMode2 = 2u, ///<ģʽ2
    LPUartMode3 = 3u, ///<ģʽ3
} en_lpuart_mode_t;
/**
 ******************************************************************************
 ** \brief lpuart stop����ѡ��
 ******************************************************************************/

typedef enum en_lpuart_stop
{
    LPUart1bit  = 0u, ///<1λֹͣλ 
    LPUart15bit = 1u, ///<1.5λֹͣλ
    LPUart2bit  = 2u, ///<2λֹͣλ
} en_lpuart_stop_t;
/**
 ******************************************************************************
 ** \brief lpuart ����ʹ��
 ******************************************************************************/
typedef enum en_lpuart_func
{
    LPUartTx     = 0u, ///<mode0ģʽ����TX    
    LPUartRx     = 1u, ///<��mode0ģʽ����RX and TX ,mode0ģʽ����RX  
    LPUartDmaTx  = 3u, ///<DMA����ʹ��
    LPUartDmaRx  = 4u, ///<DMA����ʹ��
    LPUartCtsRts = 5u, ///<Ӳ����ʹ��
}en_lpuart_func_t;
/**
 ******************************************************************************
 ** \brief lpuart�ж�ʹ�ܿ���
 ******************************************************************************/
typedef enum en_lpuart_irq_sel
{
    LPUartTxIrq  = 0u,  ///<�����ж�ʹ��        
    LPUartRxIrq  = 1u,  ///<�����ж�ʹ��
    LPUartFEIrq  = 3u,  ///<֡�����ж�ʹ��
    LPUartCtsIrq = 4u,  ///<CTS�źŷ�ת�ж�ʹ��
    LPUartPEIrq  = 5u,  ///<��żУ���ж�ʹ��
    LPUartTxEIrq = 6u,  ///<TX���ж�ʹ��   
}en_lpuart_irq_sel_t;
/**
 ******************************************************************************
 ** \brief lpuart���ͽ����жϴ������ӿ�
 ******************************************************************************/

typedef struct stc_lpuart_irq_cb
{
    func_ptr_t pfnTxIrqCb;    ///<�����жϷ�����     
    func_ptr_t pfnRxFEIrqCb;  ///<����֡�����жϷ�����
    func_ptr_t pfnRxIrqCb;    ///<�����жϷ����� 
    func_ptr_t pfnCtsIrqCb;   ///<CTS�źŷ�ת�жϷ����� 
    func_ptr_t pfnPEIrqCb;    ///<��żУ������жϷ����� 
}stc_lpuart_irq_cb_t;
/**
 ******************************************************************************
 ** \brief lpuart ״̬��־λ
 ******************************************************************************/
typedef enum en_lpuart_status
{
    LPUartCts   = 0u,  ///<CTS�źű��
    LPUartRC    = 1u,  ///<����������ɱ��
    LPUartTC    = 2u,  ///<����������ɱ��
    LPUartPE    = 3u,  ///<��żУ�������
    LPUartFE    = 4u,  ///<֡������  
    LPUartCtsIf = 5u,  ///CTS�жϱ�־
    LPUartTxe   = 6u,  ///TXbuff�ձ��
}en_lpuart_status_t;
/**
 ******************************************************************************
 ** \brief lpuart ͨ����ַ���жϺ�����ַ�ṹ
 ******************************************************************************/
typedef struct stc_lpuart_instance_data
{   
    uint32_t               u32Idx;               ///< ͨ����
    M0P_LPUART_TypeDef     *pstcInstance;        ///< ͨ���Ĵ�����ַ
    stc_lpuart_irq_cb_t    stcLPUartInternIrqCb;   ///< ͨ���жϷ�����
} stc_lpuart_instance_data_t;
/**
 ******************************************************************************
 ** \brief lpuart ͨ������������
 ******************************************************************************/
typedef enum en_lpuart_clkdiv
{
    LPUart16Or32Div = 0u,///<ģʽ0��Ч��ģʽ1/3Ϊ16��Ƶ��ģʽ2Ϊ32��Ƶ
    LPUart8Or16Div  = 1u,///<ģʽ0��Ч��ģʽ1/3Ϊ8��Ƶ��ģʽ2Ϊ16��Ƶ
    LPUart4Or8Div   = 2u,///<ģʽ0��Ч��ģʽ1/3Ϊ4��Ƶ��ģʽ2Ϊ8��Ƶ
}en_lpuart_clkdiv_t;
/**
 ******************************************************************************
 ** \brief lpuart ʱ���������
 ******************************************************************************/
typedef struct stc_lpuart_sclk_sel
{
    en_lpuart_clkdiv_t  enSclk_Prs; ///<��Ƶѡ��
    en_lpuart_sclksel_t enSclk_sel; ///<����ʱ��ѡ��
}stc_lpuart_sclk_sel_t;
/**
 ******************************************************************************
 ** \brief lpuart ͨ�������ʼ������
 ******************************************************************************/
typedef struct stc_lpuart_baud
{
    uint32_t             u32Sclk;        ///<sclk
    en_lpuart_mode_t     enRunMode;      ///< ����ģʽ����
    uint32_t             u32Baud;        ///< ������
} stc_lpuart_baud_t;
/**
 ******************************************************************************
 ** \lpuart ��������
 ******************************************************************************/

typedef struct stc_lpuart_config
{
    en_lpuart_mode_t       enRunMode;      ///< ����ģʽ����
    stc_lpuart_sclk_sel_t* pstcLpuart_clk; ///<ʱ��Դ����
    en_lpuart_stop_t       enStopBit;      ///<ֹͣλ����
    stc_lpuart_multimode_t* pstcMultiMode; ///<������ģʽ����
    stc_lpuart_irq_cb_t*  pstcIrqCb;       ///<�жϷ�����          
    boolean_t           bTouchNvic;      ///<NVIC�ж�ʹ��    
} stc_lpuart_config_t;
//�ж�������ú���
en_result_t LPUart_EnableIrq(uint8_t u8Idx,
                           en_lpuart_irq_sel_t enIrqSel);
en_result_t LPUart_DisableIrq(uint8_t u8Idx,
                            en_lpuart_irq_sel_t enIrqSel);
// �ܳ�ʼ������
en_result_t LPUart_Init(uint8_t u8Idx, 
                      stc_lpuart_config_t* pstcConfig);
//LPUARTģ�鹤��ģʽ���ú���
en_result_t LPUart_SetMode(uint8_t u8Idx,en_lpuart_mode_t enMode);
//LPUARTģ�鹤��ģʽ���ú���
en_result_t LPUart_SetMultiMode(uint8_t u8Idx,stc_lpuart_multimode_t* pstcMultiConfig);
//LPUARTͨ��������ģʽ��������/��ַ֡������żУ������TB8
en_result_t LPUart_SetMMDOrCk(uint8_t u8Idx,en_lpuart_mmdorck_t enTb8);
//�ӻ���ַ����
en_result_t LPUart_SetSaddr(uint8_t u8Idx,uint8_t u8Addr);
//�ӻ���ַ��������
en_result_t LPUart_SetSaddrEn(uint8_t u8Idx,uint8_t u8Addren);
//ֹͣλ����
en_result_t LPUart_SetStopBit(uint8_t u8Idx,uint8_t u8Len);
//���ݼĴ���bit8λ��ȡ
boolean_t LPUart_GetRb8(uint8_t u8Idx);
//ʱ��Դѡ��
en_result_t LPUart_SelSclk(uint8_t u8Idx,en_lpuart_sclksel_t enClk);
//ʱ��Դʱ�ӻ�ȡ
uint32_t LPUart_GetSclk(uint8_t u8Idx);
//������Ƶ����
en_result_t LPUart_SetClkDiv(uint8_t u8Idx,en_lpuart_clkdiv_t enClkDiv);
//����������ֵ����
uint16_t LPUart_CalScnt(uint8_t u8Idx,stc_lpuart_baud_t *pstcBaud);
//����������
en_result_t LPUart_SetBaud(uint8_t u8Idx,uint16_t u16Scnt);
//��ȡ������
uint32_t LPUart_GetBaud(uint8_t u8Idx,uint8_t u8Mode,uint32_t u32Pclk);
                             
//����ʹ�ܺͽ�ֹ
en_result_t LPUart_EnableFunc(uint8_t u8Idx, en_lpuart_func_t enFunc);
en_result_t LPUart_DisableFunc(uint8_t u8Idx, en_lpuart_func_t enFunc);
//ĳ��״̬bit��ȡ����
boolean_t LPUart_GetStatus(uint8_t u8Idx,en_lpuart_status_t enStatus);
//ĳ��״̬bit״̬λ�����
en_result_t LPUart_ClrStatus(uint8_t u8Idx,en_lpuart_status_t enStatus);
//����״̬�Ĵ�����ȡ
uint8_t LPUart_GetIsr(uint8_t u8Idx);
//����״̬�Ĵ������
en_result_t LPUart_ClrIsr(uint8_t u8Idx);
//���ݲ�ѯ��ʽ�ķ��Ͳ���
en_result_t LPUart_SendData(uint8_t u8Idx, uint8_t u8Data);
//���ݲ�ѯ��ʽ�Ľ��ղ���
uint8_t LPUart_ReceiveData(uint8_t u8Idx);

//@} // LPUartGroup

#ifdef __cplusplus
#endif

#endif /* __UART_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/



