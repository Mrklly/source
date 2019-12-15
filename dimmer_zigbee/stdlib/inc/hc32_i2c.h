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
/** \file i2c.h
 **
 ** Headerfile for I2C functions
 **  
 **
 ** History:
 **   - 2017-06-13   CJ    First Version
 **
 *****************************************************************************/

#ifndef __I2C_H__
#define __I2C_H__

#include "ddl.h"
#include "interrupts_hc32f005.h"

/**
 *******************************************************************************
 ** \defgroup I2cGroup Inter-Integrated Circuit (I2C)
 **
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
 ** \brief I2C��������
 *****************************************************************************/
typedef enum en_i2c_func
{
    I2cMode_En  = 0, ///<I2Cģ��ʹ��
    I2cStart_En = 1, ///<��ʼ�ź�
    I2cStop_En  = 2, ///<�����ź�
    I2cAck_En   = 3, ///<Ӧ���ź�
    I2cHlm_En   = 4, ///<����ʹ��
    I2cBaud_En  = 5, ///<������ʹ��
}en_i2c_func_t; 
/**
 ******************************************************************************
 ** \brief I2C�ӻ���ַ����
 *****************************************************************************/
typedef struct stc_i2c_addr
{
    uint8_t Addr;       ///<�ӻ�ģʽ��ַ
    uint8_t Gc;         ///<�㲥��ַʹ��

}stc_i2c_addr_t;
/**
 ******************************************************************************
 ** \brief I2C��ʼ�����ýṹ
 *****************************************************************************/
typedef struct stc_i2c_config
{
    en_i2c_func_t   enFunc;      ///<����ʹ��
    uint8_t         u8Tm;        ///<�����ʼ���������
    stc_i2c_addr_t  stcSlaveAddr;///<�ӻ���ַ
    func_ptr_t      pfnI2cCb;    ///<�жϷ�����ָ��
    boolean_t       bTouchNvic;  ///<�Ƿ�ʹ��NVIC
}stc_i2c_config_t;

/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 *****************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 *****************************************************************************/
 //I2C��ʼ������
 en_result_t I2C_Init(stc_i2c_config_t* pstcI2CCfg);
  //I2Cģ��رպ���
 en_result_t I2C_DeInit(void);
 //���ò��������üĴ���
 en_result_t I2C_SetBaud(uint8_t u8Tm);
 //I2C�������ú���
 en_result_t I2C_SetFunc(en_i2c_func_t enFunc);
  //I2C�����������
 en_result_t I2C_ClearFunc(en_i2c_func_t enFunc);
 //��ȡ�жϱ��SI
 boolean_t I2C_GetIrq(void);
  //����жϱ��SI
 en_result_t I2C_ClearIrq(void);
 //��ȡ״̬
 uint8_t I2C_GetState(void);
 //д�ӻ���ַ����
 en_result_t I2C_WriteSlaveAddr(stc_i2c_addr_t *pstcSlaveAddr);
 //�����ͺ���
 en_result_t I2C_MasterWriteData(uint8_t u8DevAddr,uint8_t u8Addr,uint8_t *pu8Data,uint32_t u32Len);
  //�ӷ��ͺ���
 en_result_t I2C_SlaveWriteData(uint8_t *pu8Data,uint32_t *u32Len);
 //�ֽ�д����
 en_result_t I2C_WriteByte(uint8_t u8Data);
 //�����պ���
 en_result_t I2C_MasterReadData(uint8_t u8DevAddr,uint8_t *pu8Data,uint8_t u8Addr,uint32_t u32Len);
  //�����պ���
 en_result_t I2C_SlaveReadData(uint8_t *pu8Data,uint32_t *pu32Len);
  //�ֽڶ�����
uint8_t I2C_ReadByte(void);
 
//@} // I2cGroup

#ifdef __cplusplus
#endif

#endif /* __I2C_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/


