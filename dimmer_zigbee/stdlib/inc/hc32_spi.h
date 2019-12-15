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
* HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED   , ARISING BY LAW OR OTHERWISE,
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
/** \file spi.h
 **
 ** Headerfile for SPI functions
 **  
 **
 ** History:
 **   - 2017-05-17  1.0  Devi     First Version
 **
 *****************************************************************************/
#ifndef __SPI_H__
#define __SPI_H__

/******************************************************************************
 * Include files
 *****************************************************************************/
#include "ddl.h"
#include "interrupts_hc32f005.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

//@{

/******************************************************************************
 * Global type definitions
 *****************************************************************************/
/**
 ******************************************************************************
 ** \brief SPI ����ʹ������
 ******************************************************************************/ 
typedef enum en_spi_en
{
    SpiEnable     = 1u,  ///< SPIģ��ʹ��
    SpiDisable    = 0u,  ///< SPIģ���ֹ
}en_spi_en_t;
/**
 ******************************************************************************
 ** \brief SPI ģʽ����
 ******************************************************************************/ 
typedef enum en_spi_mode
{
    SpiMaster = 1u,  ///<����
    SpiSlave  = 0u,  ///<�ӻ�
}en_spi_mode_t;
/**
 ******************************************************************************
 ** \brief SPI ʱ�Ӽ�������
 ******************************************************************************/ 
typedef enum en_spi_cpol
{
    Spicpollow   = 0u,  ///<����Ϊ��
    Spicpolhigh  = 1u,  ///<����Ϊ��
}en_spi_cpol_t;
/**
 ******************************************************************************
 ** \brief SPI ʱ����λ����
 ******************************************************************************/ 
typedef enum en_spi_cpha
{
    Spicphafirst   = 0u,  ///<��һ���ز���
    Spicphasecond  = 1u,  ///<�ڶ����ز���
}en_spi_cpha_t;
/**
 ******************************************************************************
 ** \brief SPI ʱ�ӷ�Ƶ����
 *****************************************************************************/
typedef enum en_spi_clk_div
{
    SpiClkDiv2   = 0u,   ///<2��Ƶ       
    SpiClkDiv4   = 1u,   ///<4��Ƶ            
    SpiClkDiv8   = 2u,   ///<8��Ƶ            
    SpiClkDiv16  = 3u,   ///<16��Ƶ            
    SpiClkDiv32  = 4u,   ///<32��Ƶ            
    SpiClkDiv64  = 5u,   ///<64��Ƶ            
    SpiClkDiv128 = 6u,   ///<128��Ƶ            
}en_spi_clk_div_t;

/**
 ******************************************************************************
 ** \brief SPI Ƭѡ�ŵ�ƽѡ��
 *****************************************************************************/
typedef enum en_spi_cspin
{
    SpiCsLow  = 0u,      ///<Ƭѡ�͵�ƽ         
    SpiCsHigh = 1u,      ///<Ƭѡ�ߵ�ƽ            
}en_spi_cspin_t;

/**
 ******************************************************************************
 ** \brief SPI ״̬
 *****************************************************************************/
typedef enum en_spi_status
{
    SpiIf              = 0x80,   ///<��������жϱ�־    
    SpiWcol            = 0x40,   ///<д��ͻ��־    
    SpiSserr           = 0x20,   ///<�ӻ�ģʽ�����־    
    SpiMdf             = 0x10,   ///<����ģʽ�����־    
}en_spi_status_t;
/**
 ******************************************************************************
 ** \brief SPI �������ýṹ��
 *****************************************************************************/
typedef struct stc_spi_config
{
    boolean_t           bMasterMode;     ///< ����ģʽѡ��
    uint8_t             u8BaudRate;      ///< ����������
    boolean_t           bCPOL;           ///< ʱ�Ӽ���ѡ��
    boolean_t           bCPHA;           ///< ʱ����λѡ��
    boolean_t           bIrqEn;          ///< �ж�ʹ��
    func_ptr_t          pfnIrqCb;        ///< �жϻص�����
}stc_spi_config_t;

//SPI �ж�
void Spi_IRQHandler(void);

//SPI ��ȡ״̬      
boolean_t Spi_GetStatus(en_spi_status_t enStatus);

//SPI��ʼ������
en_result_t Spi_Init(stc_spi_config_t* pstcSpiConfig);
//SPI�رպ���
en_result_t Spi_DeInit(void);
//SPI ���������͵ĵ�ƽ
void Spi_SetCS(boolean_t bFlag);
//SPI ��������
en_result_t Spi_SendData(uint8_t u8Data);
//SPI ��������
uint8_t Spi_ReceiveData(void);

//@} // Spi Group

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/

