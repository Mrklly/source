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
    UartDataOrAddr = 0u, ///<������ģʽʱ��ͨ����дSBUF[8]����֡Ϊ����֡���ַ֡
    UartEven       = 1u, ///<�Ƕ�����ģʽżУ��   
    UartOdd        = 2u, ///<�Ƕ�����ģʽ��У��    
}en_uart_mmdorck_t;

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
 ** \brief uart stop����ѡ��
 ******************************************************************************/

typedef enum en_uart_stop
{
    Uart1bit  = 0u, ///<1λֹͣλ 
    Uart15bit = 1u, ///<1.5λֹͣλ
    Uart2bit  = 2u, ///<2λֹͣλ
} en_uart_stop_t;
/**
 ******************************************************************************
 ** \brief uart ����ʹ��
 ******************************************************************************/
typedef enum en_uart_func
{
    UartTx     = 0u, ///<mode0ģʽ����TX    
    UartRx     = 1u, ///<��mode0ģʽ����RX and TX ,mode0ģʽ����RX  
    UartDmaTx  = 3u, ///<DMA����ʹ��
    UartDmaRx  = 4u, ///<DMA����ʹ��
    UartCtsRts = 5u, ///<Ӳ����ʹ��
}en_uart_func_t;
/**
 ******************************************************************************
 ** \brief uart�ж�ʹ�ܿ���
 ******************************************************************************/
typedef enum en_uart_irq_sel
{
    UartTxIrq  = 0u,  ///<�����ж�ʹ��        
    UartRxIrq  = 1u,  ///<�����ж�ʹ��
    UartFEIrq  = 3u,  ///<֡�����ж�ʹ��
    UartCtsIrq = 4u,  ///<CTS�źŷ�ת�ж�ʹ��
    UartPEIrq  = 5u,  ///<��żУ���ж�ʹ��
    UartTxEIrq = 6u,  ///<TX���ж�ʹ��   
}en_uart_irq_sel_t;
/**
 ******************************************************************************
 ** \brief uart���ͽ����жϴ������ӿ�
 ******************************************************************************/

typedef struct stc_uart_irq_cb
{
    func_ptr_t pfnTxIrqCb;    ///<�����жϷ�����     
    func_ptr_t pfnRxFEIrqCb;  ///<����֡�����жϷ�����
    func_ptr_t pfnRxIrqCb;    ///<�����жϷ����� 
    func_ptr_t pfnCtsIrqCb;   ///<CTS�źŷ�ת�жϷ����� 
    func_ptr_t pfnPEIrqCb;    ///<��żУ������жϷ����� 
}stc_uart_irq_cb_t;
/**
 ******************************************************************************
 ** \brief uart ״̬��־λ
 ******************************************************************************/
typedef enum en_uart_status
{
    UartCts   = 0u,  ///<CTS�źű��
    UartRC    = 1u,  ///<����������ɱ��
    UartTC    = 2u,  ///<����������ɱ��
    UartPE    = 3u,  ///<��żУ�������
    UartFE    = 4u,  ///<֡������  
    UartCtsIf = 5u,  ///<CTS�жϱ��
    UartTxe   = 6u,  ///<TXbuff�ձ��
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
typedef enum en_uart_clkdiv
{
    Uart16Or32Div = 0u,///<ģʽ0��Ч��ģʽ1/3Ϊ16��Ƶ��ģʽ2Ϊ32��Ƶ
    Uart8Or16Div  = 1u,///<ģʽ0��Ч��ģʽ1/3Ϊ8��Ƶ��ģʽ2Ϊ16��Ƶ
}en_uart_clkdiv_t;
/**
 ******************************************************************************
 ** \brief uart ͨ�������ʼ������
 ******************************************************************************/
typedef struct stc_uart_baud
{
    uint32_t             u32Pclk;        ///<pclk
    en_uart_mode_t       enRunMode;      ///< ����ģʽ����
    uint32_t             u32Baud;        ///< ������
} stc_uart_baud_t;
/**
 ******************************************************************************
 ** \uart ��������
 ******************************************************************************/

typedef struct stc_uart_config
{
    en_uart_mode_t       enRunMode;      ///< ����ģʽ����
    en_uart_stop_t       enStopBit;      ///<ֹͣλ����
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
//UARTģ�鹤��ģʽ���ú���
en_result_t Uart_SetMode(uint8_t u8Idx,en_uart_mode_t enMode);
//UARTģ�������ģʽ���ú���
en_result_t Uart_SetMultiMode(uint8_t u8Idx,stc_uart_multimode_t* pstcMultiConfig);
//UARTͨ��������ģʽ��������/��ַ֡������żУ������TB8
en_result_t Uart_SetMMDOrCk(uint8_t u8Idx,en_uart_mmdorck_t enTb8);
//�ӻ���ַ����
en_result_t Uart_SetSaddr(uint8_t u8Idx,uint8_t u8Addr);
//�ӻ���ַ��������
en_result_t Uart_SetSaddrEn(uint8_t u8Idx,uint8_t u8Addren);
//ֹͣλ��������
en_result_t Uart_SetStopBit(uint8_t u8Idx,uint8_t u8Len);
//RB8���ݻ�ȡ
boolean_t Uart_GetRb8(uint8_t u8Idx);

//������Ƶ����
en_result_t Uart_SetClkDiv(uint8_t u8Idx,en_uart_clkdiv_t enClkDiv);
//����������ֵ����
uint16_t Uart_CalScnt(uint8_t u8Idx,stc_uart_baud_t *pstcBaud);
//����������
en_result_t Uart_SetBaud(uint8_t u8Idx,uint16_t u16Scnt);
//��ȡ������
uint32_t Uart_GetBaud(uint8_t u8Idx,uint8_t u8Mode,uint32_t u32Pclk);
                             
// ����ʹ�ܺͽ�ֹ
en_result_t Uart_EnableFunc(uint8_t u8Idx, en_uart_func_t enFunc);
en_result_t Uart_DisableFunc(uint8_t u8Idx, en_uart_func_t enFunc);
//״̬λ�Ļ�ȡ����� 
uint8_t Uart_GetIsr(uint8_t u8Idx);
boolean_t Uart_GetStatus(uint8_t u8Idx,en_uart_status_t enStatus);
en_result_t Uart_ClrIsr(uint8_t u8Idx);
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



