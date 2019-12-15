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
 ** Headerfile for UART functions
 **  
 **
 ** History:
 **   - 2017-05-10   Cathy     First Version
 **
 *****************************************************************************/

#ifndef __UART_H__
#define __UART_H__
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
 ** \defgroup UartGroup Universal Asynchronous Receiver/Transmitter (UART)
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
 **\brief uartͨ��ѡ��
 ******************************************************************************/

typedef enum en_uart_channel
{
    UARTCH0 = 0u, ///<����0
    UARTCH1 = 1u, ///<����1
}en_uart_channel_t;


/**
 ******************************************************************************
 ** \brief uart������ģʽ��ַ֡/����֡������żУ��
 ******************************************************************************/
typedef enum en_uart_mmdorck
{
    Data = 0u, ///<������ģʽʱ������֡TB8���   
    Addr = 1u, ///<������ģʽʱ����ַ֡TB8���   
}en_uart_mmdorck_t;
/**
 ******************************************************************************
 ** \brief uart��żУ��
 ******************************************************************************/
typedef enum en_uart_check
{
    Even = 0u, ///<żУ��
    Odd  = 1u, ///<��У��
}en_uart_check_t;
/**
 ******************************************************************************
 ** \brief uart������ģʽ����
 ******************************************************************************/
typedef enum en_uart_multimode
{
    UartNormal = 0u, ///<��������ģʽ
    UartMulti  = 1u, ///<����������ģʽ
}en_uart_multimode_t;
/**
 ******************************************************************************
 ** \brief uart������ģʽ���ӻ���ַ�͵�ַ��������
 ******************************************************************************/

typedef struct stc_uart_multimode
{
    en_uart_multimode_t enMulti_mode; ///<������ģʽ����
    uint8_t             u8SlaveAddr;  ///<�ӻ���ַ
    uint8_t                u8SaddEn;  ///<�Ӽ���ַ����
}stc_uart_multimode_t;

/**
 ******************************************************************************
 ** \brief uart ���ֹ���ģʽѡ��
 ******************************************************************************/

typedef enum en_uart_mode
{
    UartMode0 = 0u, ///<ģʽ0    
    UartMode1 = 1u, ///<ģʽ1
    UartMode2 = 2u, ///<ģʽ2
    UartMode3 = 3u, ///<ģʽ3
} en_uart_mode_t;
/**
 ******************************************************************************
 ** \brief uart ���ͽ��չ���ʹ��
 ******************************************************************************/
typedef enum en_uart_func
{
    UartTx = 0u, ///<mode0ģʽ����TX    
    UartRx = 1u, ///<��mode0ģʽ����RX and TX ,mode0ģʽ����RX  
}en_uart_func_t;
/**
 ******************************************************************************
 ** \brief uart���ͽ����ж�ʹ��
 ******************************************************************************/
typedef enum en_uart_irq_sel
{
    UartTxIrq = 0u,  ///<�����ж�ʹ��        
    UartRxIrq = 1u,  ///<�����ж�ʹ��
}en_uart_irq_sel_t;
/**
 ******************************************************************************
 ** \brief uart���ͽ����жϴ������ӿ�
 ******************************************************************************/

typedef struct stc_uart_irq_cb
{
    func_ptr_t pfnTxIrqCb;    ///<�����жϷ�����     
    func_ptr_t pfnRxErrIrqCb; ///<���մ����жϷ�����
    func_ptr_t pfnRxIrqCb;    ///<�����жϷ�����     
}stc_uart_irq_cb_t;
/**
 ******************************************************************************
 ** \brief uart ״̬��־λ
 ******************************************************************************/
typedef enum en_uart_status
{
    UartRFRAMEError = 0u,  ///<֡������
    UartRxFull      = 1u,  ///<����������ɱ��
    UartTxEmpty     = 2u,  ///<����������ɱ��
   
}en_uart_status_t;
/**
 ******************************************************************************
 ** \brief uart ͨ����ַ���жϺ�����ַ�ṹ
 ******************************************************************************/
typedef struct stc_uart_instance_data
{   
    uint32_t               u32Idx;               ///< ͨ����
    M0P_UART_TypeDef       *pstcInstance;        ///< ͨ���Ĵ�����ַ
    stc_uart_irq_cb_t      stcUartInternIrqCb;   ///< ͨ���жϷ�����
} stc_uart_instance_data_t;
/**
 ******************************************************************************
 ** \brief uart ͨ������������
 ******************************************************************************/
typedef struct stc_uart_baud_config
{
    uint8_t u8Mode;   ///<���ֹ���ģʽ
    boolean_t bDbaud; ///<�Ƿ�˫��������
    uint32_t u32Baud; ///<������
}stc_uart_baud_config_t;
/**
 ******************************************************************************
 ** \uart ��������
 ******************************************************************************/

typedef struct stc_uart_config
{
    en_uart_mode_t       enRunMode;      ///< ����ģʽ����
    stc_uart_multimode_t* pstcMultiMode; ///<������ģʽ����
    stc_uart_irq_cb_t*  pstcIrqCb;       ///<�жϷ�����          
    boolean_t           bTouchNvic;      ///<NVIC�ж�ʹ��    
} stc_uart_config_t;
//�ж�������ú���
en_result_t Uart_EnableIrq(uint8_t u8Idx,
                           en_uart_irq_sel_t enIrqSel);
en_result_t Uart_DisableIrq(uint8_t u8Idx,
                            en_uart_irq_sel_t enIrqSel);
//void Uart_IrqHandler(uint8_t u8Idx);
// �ܳ�ʼ������
en_result_t Uart_Init(uint8_t u8Idx, 
                      stc_uart_config_t* pstcConfig);
en_result_t Uart_DeInit(uint8_t u8Idx, boolean_t bTouchNvic);
//����ģʽ���á�������ģʽ�´ӻ���ַ�͵�ַ��������
en_result_t Uart_SetMode(uint8_t u8Idx,en_uart_mode_t enMode);
en_result_t Uart_SetMultiMode(uint8_t u8Idx,stc_uart_multimode_t* pstcMultiConfig);
en_result_t Uart_SetMMDOrCk(uint8_t u8Idx,en_uart_mmdorck_t enTb8);
en_result_t Uart_SetSaddr(uint8_t u8Idx,uint8_t u8Addr);
en_result_t Uart_SetSaddrEn(uint8_t u8Idx,uint8_t u8Addren);
//��ȡRb8λ
boolean_t Uart_GetRb8(uint8_t u8Idx);
//����TB8
en_result_t Uart_SetTb8(uint8_t u8Idx,en_uart_check_t enCheck,uint8_t u8Data);

en_result_t Uart_CheckEvenOrOdd(uint8_t u8Idx,en_uart_check_t enCheck,uint8_t u8Recv);

// ����������
uint16_t Uart_SetBaudRate(uint8_t u8Idx,uint32_t u32pclk,stc_uart_baud_config_t* pstcBaud);
                             
// ���ͻ����ʹ�ܺͽ�ֹ
en_result_t Uart_EnableFunc(uint8_t u8Idx, en_uart_func_t enFunc);
en_result_t Uart_DisableFunc(uint8_t u8Idx, en_uart_func_t enFunc);
//״̬λ�Ļ�ȡ�����
boolean_t Uart_GetStatus(uint8_t u8Idx,en_uart_status_t enStatus);
en_result_t Uart_ClrStatus(uint8_t u8Idx,en_uart_status_t enStatus);
//���ݲ�ѯ��ʽ���շ�����
//en_result_t  Uart_MultiSendFirstData(uint8_t U8Addr);
en_result_t Uart_SendData(uint8_t u8Idx, uint8_t u8Data);
uint8_t Uart_ReceiveData(uint8_t u8Idx);

//@} // UartGroup

#ifdef __cplusplus
#endif

#endif /* __UART_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/



