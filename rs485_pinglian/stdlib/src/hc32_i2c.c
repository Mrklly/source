/*************************************************************************************
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
/** \file I2C.c
 **
 ** WDT function driver API.
 ** @link SampleGroup Some description @endlink
 **
 **   - 2018-03-13  1.0  CJ First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "i2c.h"

/**
 *******************************************************************************
 ** \addtogroup I2cGroup
 ******************************************************************************/
//@{

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
static func_ptr_t pfnI2c0tCallback = NULL;
static func_ptr_t pfnI2c1tCallback = NULL;
/**
 ******************************************************************************
 ** \brief  I2C���ò��������üĴ���
 **
 ** \param [in] u8Tm ����������ֵ
 **
 ** \retval enRet �ɹ���ʧ��
 **
 ******************************************************************************/
 en_result_t I2C_SetBaud(en_i2c_channel_t enCh,uint8_t u8Tm)
 {
     en_result_t enRet = Error;
     if(I2C0 == enCh)
     {
        M0P_I2C0->TM = u8Tm; 
     }
     else
     {
        M0P_I2C1->TM = u8Tm;
     }
     
     enRet = Ok;
     return enRet;
 }
 /**
 ******************************************************************************
 ** \brief  I2C����������غ���
 **
 ** \param [in] enFunc���ܲ���
 **
 ** \retval enRet �ɹ���ʧ��
 **
 ******************************************************************************/
en_result_t I2C_SetFunc(en_i2c_channel_t enCh,en_i2c_func_t enFunc)
{
    en_result_t enRet = Error;
    if(I2C0 == enCh)
    {
        switch(enFunc)
        {
            case I2cMode_En:
                M0P_I2C0->CR_f.ENS = 1;
                break;
            case I2cStart_En:
                M0P_I2C0->CR_f.STA = 1;
                break;
            case I2cStop_En:
                M0P_I2C0->CR_f.STO = 1;
                break;
            case I2cAck_En:
                M0P_I2C0->CR_f.AA = 1;
                break;
            case I2cHlm_En:
                M0P_I2C0->CR_f.H1M = 1;
                break;
            case I2cBaud_En:
                M0P_I2C0->TMRUN = 0x01;
                break;
            default:
                return ErrorInvalidParameter;
        }  
    }
    else
    {
        switch(enFunc)
        {
            case I2cMode_En:
                M0P_I2C1->CR_f.ENS = 1;
                break;
            case I2cStart_En:
                M0P_I2C1->CR_f.STA = 1;
                break;
            case I2cStop_En:
                M0P_I2C1->CR_f.STO = 1;
                break;
            case I2cAck_En:
                M0P_I2C1->CR_f.AA = 1;
                break;
            case I2cHlm_En:
                M0P_I2C1->CR_f.H1M = 1;
                break;
            case I2cBaud_En:
                M0P_I2C1->TMRUN = 0x01;
                break;
            default:
                return ErrorInvalidParameter;
        }  
    }

    enRet = Ok;
    return enRet;
}
 /**
 ******************************************************************************
 ** \brief  I2C���������غ���
 **
 ** \param [in] enFunc���ܲ���
 **
 ** \retval enRet �ɹ���ʧ��
 **
 ******************************************************************************/
 en_result_t I2C_ClearFunc(en_i2c_channel_t enCh,en_i2c_func_t enFunc)
 {
    en_result_t enRet = Error;
    if(I2C0 == enCh)
    {
        switch(enFunc)
        {
            case I2cMode_En:
                M0P_I2C0->CR_f.ENS = 0;
                break;
            case I2cStart_En:
                M0P_I2C0->CR_f.STA = 0;
                break;
            case I2cStop_En:
                M0P_I2C0->CR_f.STO = 0;
                break;
            case I2cAck_En:
                M0P_I2C0->CR_f.AA = 0;
                break;
            case I2cHlm_En:
                M0P_I2C0->CR_f.H1M = 0;
                break;
            case I2cBaud_En:
                M0P_I2C0->TMRUN = 0x00;
                break;
            default:
                return ErrorInvalidParameter;
        }  
    }
    else
    {
        switch(enFunc)
        {
            case I2cMode_En:
                M0P_I2C1->CR_f.ENS = 0;
                break;
            case I2cStart_En:
                M0P_I2C1->CR_f.STA = 0;
                break;
            case I2cStop_En:
                M0P_I2C1->CR_f.STO = 0;
                break;
            case I2cAck_En:
                M0P_I2C1->CR_f.AA = 0;
                break;
            case I2cHlm_En:
                M0P_I2C1->CR_f.H1M = 0;
                break;
            case I2cBaud_En:
                M0P_I2C1->TMRUN = 0x00;
                break;
            default:
                return ErrorInvalidParameter;
        }   
    }
    enRet = Ok;
    return enRet; 
 }
 /**
 ******************************************************************************
 ** \brief  I2C��ȡ�жϱ�Ǻ���
 **
 ** \param ��
 **
 ** \retval bIrq�жϱ��
 **
 ******************************************************************************/
boolean_t I2C_GetIrq(en_i2c_channel_t enCh)
{
    boolean_t bIrq = FALSE;
    if(I2C0 == enCh)
    {
        bIrq = M0P_I2C0->CR_f.SI; 
    }
    else
    {
        bIrq = M0P_I2C1->CR_f.SI; 
    }
    
    return bIrq;  
}
/**
 ******************************************************************************
 ** \brief  I2C����жϱ�Ǻ���
 **
 ** \param ��
 **
 ** \retval bIrq�жϱ��
 **
 ******************************************************************************/
en_result_t I2C_ClearIrq(en_i2c_channel_t enCh)
{
    en_result_t enRet = Error;
    if(I2C0 == enCh)
    {
       M0P_I2C0->CR_f.SI = 0; 
    }
    else
    {
       M0P_I2C1->CR_f.SI = 0;  
    }  
    enRet = Ok;
    return enRet; 
}
 /**
 ******************************************************************************
 ** \brief  I2C��ȡ���״̬
 **
 ** \param ��
 **
 ** \retval I2C״̬
 **
 ******************************************************************************/
uint8_t I2C_GetState(en_i2c_channel_t enCh)
{
    uint8_t u8State = 0;
    if(I2C0 == enCh)
    {
        u8State = M0P_I2C0->STAT;  
    }
    else
    {
        u8State = M0P_I2C1->STAT;  
    }
    return u8State;
}
/**
 ******************************************************************************
 ** \brief  I2Cд�ӻ���ַ����
 **
 ** \param u8SlaveAddr�ӻ���ַ
 **
 ** \retval I2Cд�ɹ����״̬
 **
 ******************************************************************************/
 en_result_t I2C_WriteSlaveAddr(en_i2c_channel_t enCh,stc_i2c_addr_t *pstcSlaveAddr)
{
    en_result_t enRet = Error;
    if(I2C0 == enCh)
    {
        M0P_I2C0->ADDR_f.ADR = pstcSlaveAddr->Addr;
        M0P_I2C0->ADDR_f.GC = pstcSlaveAddr->Gc;
    }
    else
    {
        M0P_I2C1->ADDR_f.ADR = pstcSlaveAddr->Addr;
        M0P_I2C1->ADDR_f.GC = pstcSlaveAddr->Gc;   
    }

    enRet = Ok;
    return enRet;     
}
/**
 ******************************************************************************
 ** \brief  �ֽ�д�ӻ�����
 **
 ** \param u8Dataд����
 **
 ** \retval д�����Ƿ�ɹ�
 **
 ******************************************************************************/
en_result_t I2C_WriteByte(en_i2c_channel_t enCh,uint8_t u8Data)
{
    en_result_t enRet = Error;
    if(I2C0 == enCh)
    {
        M0P_I2C0->DATA = u8Data;  
    }
    else
    {
        M0P_I2C1->DATA = u8Data;   
    }
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  �ֽڶ��ӻ�����
 **
 ** \param ��
 **
 ** \retval ��ȡ����
 **
 ******************************************************************************/
uint8_t I2C_ReadByte(en_i2c_channel_t enCh)
{
    uint8_t u8Data = 0;
    if(I2C0 == enCh)
    {
       u8Data = M0P_I2C0->DATA; 
    }
    else
    {
       u8Data = M0P_I2C1->DATA; 
    } 
    return u8Data;
}
 /**
 ******************************************************************************
 ** \brief  �������ͺ���
 **
 ** \param u8Addr�ӻ��ڴ��ַ��pu8Dataд���ݣ�u32Lenд���ݳ���
 **
 ** \retval д�����Ƿ�ɹ�
 **
 ******************************************************************************/
en_result_t I2C_MasterWriteData(en_i2c_channel_t enCh,uint8_t u8DevAddr,uint8_t u8Addr,uint8_t *pu8Data,uint32_t u32Len)
{
    en_result_t enRet = Error;
    uint8_t u8i=0,u8State;
    
    I2C_SetFunc(enCh,I2cStart_En);
	while(1)
	{
		while(0 == I2C_GetIrq(enCh))
		{}
		u8State = I2C_GetState(enCh);
		switch(u8State)
		{
			case 0x08:
				I2C_ClearFunc(enCh,I2cStart_En);
				I2C_WriteByte(enCh,u8DevAddr);//���豸��ַ����
				break;
			case 0x18:
				I2C_WriteByte(enCh,u8Addr);//���豸�ڴ��ַ����
				break;
			case 0x28:	
				I2C_WriteByte(enCh,pu8Data[u8i++]);
				break;
			case 0x20:
			case 0x38:
				I2C_SetFunc(enCh,I2cStart_En);
				break;
			case 0x30:
				I2C_SetFunc(enCh,I2cStop_En);
				break;
			default:
				break;
		}			
		if(u8i>u32Len)
		{
			I2C_SetFunc(enCh,I2cStop_En);//��˳���ܵ�������ֹͣ����
			I2C_ClearIrq(enCh);
			break;
		}
		I2C_ClearIrq(enCh);			
	}
    enRet = Ok;
    return enRet;
}
 /**
 ******************************************************************************
 ** \brief  �ӻ����ͺ���
 **
 ** \param pu8Data�������ݻ��棬u32Len�������ݳ���
 **
 ** \retval ���������Ƿ�ɹ�
 **
 ******************************************************************************/
 en_result_t I2C_SlaveWriteData(en_i2c_channel_t enCh,uint8_t *pu8Data,uint32_t *u32Len)
 {
    uint8_t u8i=0,u8State;
    //
    while(1)
    {

        while(0 == I2C_GetIrq(enCh))
        {}
        u8State = I2C_GetState(enCh);
        switch(u8State)
        {
            case 0xA8:
            case 0xB0:
                I2C_WriteByte(enCh,pu8Data[u8i++]);
                break;
            case 0xB8: 
            case 0xC8:               
                I2C_WriteByte(enCh,pu8Data[u8i++]);            
                break;          
            case 0xF8:
                *u32Len = u8i;
                break;   
            default:
                
                return ErrorInvalidParameter; 
        }
		I2C_ClearIrq(enCh);
    } 
 }
 /**
 ******************************************************************************
 ** \brief  �ӻ����պ���
 **
 ** \param pu8Data�������ݴ�Ż��棬u32Len��������ָ��
 **
 ** \retval ���������Ƿ�ɹ�
 **
 ******************************************************************************/
en_result_t I2C_SlaveReadData(en_i2c_channel_t enCh,uint8_t *pu8Data,uint32_t *pu32Len)
{
    uint8_t u8i=0,u8State;
    while(0 == I2C_GetIrq(enCh))
    {}
    while(1)
   {
        while(0 == I2C_GetIrq(enCh))
        {}
        u8State = I2C_GetState(enCh);
        switch(u8State)
        {
            case 0x60:
            case 0x68:
            case 0x70:
            case 0x78:
              break;
            case 0x80:
            case 0x90:               
                pu8Data[u8i++] = I2C_ReadByte(enCh);               
                break;
            case 0xA0:      
                *pu32Len = u8i;
                break;
            default:
                return ErrorInvalidParameter; 
        }
		I2C_ClearIrq(enCh);
        if(0xA0 == u8State)
        {
          return Ok;
        }
   }
} 

/**
 ******************************************************************************
 ** \brief  �������պ���
 **
 ** \param u8Addr�ӻ��ڴ��ַ��pu8Data�����ݴ�Ż��棬u32Len�����ݳ���
 **
 ** \retval �������Ƿ�ɹ�
 **
 ******************************************************************************/
 en_result_t I2C_MasterReadData(en_i2c_channel_t enCh,uint8_t u8DevAddr,uint8_t *pu8Data,uint8_t u8Addr,uint32_t u32Len)
{
    en_result_t enRet = Error;
    uint8_t u8i=0,u8State;
    
    I2C_SetFunc(enCh,I2cStart_En);
    
	while(1)
	{
		while(0 == I2C_GetIrq(enCh))
        {}
		u8State = I2C_GetState(enCh);
		switch(u8State)
		{
			case 0x08:
				I2C_ClearFunc(enCh,I2cStart_En);
				I2C_WriteByte(enCh,u8DevAddr);
				break;
			case 0x18:
				I2C_WriteByte(enCh,u8Addr);
				break;
			case 0x28:
				I2C_SetFunc(enCh,I2cStart_En);
				break;
			case 0x10:
				I2C_ClearFunc(enCh,I2cStart_En);
				I2C_WriteByte(enCh,u8DevAddr|0x01);//�ӻ���ַ����OK
				break;
			case 0x40:
				if(u32Len>1)
				{
					I2C_SetFunc(enCh,I2cAck_En);
				}
				break;
			case 0x50:
				pu8Data[u8i++] = I2C_ReadByte(enCh);
				if(u8i==u32Len-1)
				{
					I2C_ClearFunc(enCh,I2cAck_En);
				}
				break;
			case 0x58:
				pu8Data[u8i++] = I2C_ReadByte(enCh);
				I2C_SetFunc(enCh,I2cStop_En);
				break;	
			case 0x38:
				I2C_SetFunc(enCh,I2cStart_En);
				break;
			case 0x48:
				I2C_SetFunc(enCh,I2cStop_En);
				I2C_SetFunc(enCh,I2cStart_En);
				break;
			default:
				I2C_SetFunc(enCh,I2cStart_En);//��������״̬�����·�����ʼ����
				break;
		}
		I2C_ClearIrq(enCh);
		if(u8i==u32Len)
		{
			break;
		}
	}
	enRet = Ok;
	return enRet;
}
/**
 ******************************************************************************
 ** \brief  I2Cģ���ʼ��
 **
 ** \param pstcI2CCfg��ʼ�����ýṹ��
 **
 ** \retval ��ʼ���Ƿ�ɹ�
 **
 ******************************************************************************/
en_result_t I2C_Init(en_i2c_channel_t enCh,stc_i2c_config_t *pstcI2CCfg)
{
   en_result_t enRet = Error;
   enRet = I2C_SetFunc(enCh,pstcI2CCfg->enFunc);
   enRet = I2C_SetBaud(enCh,pstcI2CCfg->u8Tm);
   enRet = I2C_WriteSlaveAddr(enCh,&pstcI2CCfg->stcSlaveAddr);
   if(pstcI2CCfg->u8Tm<9)
   {
       I2C_SetFunc(enCh,I2cHlm_En);
   }
   if(NULL!=pstcI2CCfg->pfnI2c0Cb)
   {
		pfnI2c0tCallback = pstcI2CCfg->pfnI2c0Cb;
   }
   if(NULL!=pstcI2CCfg->pfnI2c1Cb)
   {
		pfnI2c1tCallback = pstcI2CCfg->pfnI2c1Cb;
   }
   if(TRUE == pstcI2CCfg->bTouchNvic)
   {
        if(I2C0 == enCh)
        {
            EnableNvic(I2C0_IRQn,IrqLevel3,TRUE);
        }
        else
        {
            EnableNvic(I2C1_IRQn,IrqLevel3,TRUE);
        }  
   }  
   return enRet;
}
/**
 ******************************************************************************
 ** \brief  I2Cģ��رճ�ʼ��
 **
 ** \param ��
 **
 ** \retval �����Ƿ�ɹ�
 **
 ******************************************************************************/
 en_result_t I2C_DeInit(en_i2c_channel_t enCh)
 {
    en_result_t enRet = Error;
    if(I2C0 == enCh)
    {
        M0P_I2C0->CR = 0x00; 
    }
    else
    {
        M0P_I2C1->CR = 0x00; 
    } 
    enRet = Ok;
    return enRet;
 }
 /**
 ******************************************************************************
 ** \brief  I2Cģ���жϴ�����
 **
 ** \param u8Param ������
 **
 ** \retval  ��
 **
 ******************************************************************************/
void I2c_IRQHandler(uint8_t u8Param)
{
    if(I2C0 == u8Param)
    {
		if(NULL != pfnI2c0tCallback)
		{
			pfnI2c0tCallback();
		}    
    }
    else
    {
		if(NULL != pfnI2c1tCallback)
		{
			pfnI2c1tCallback(); 
		}
	}   
}

//@} // I2cGroup
