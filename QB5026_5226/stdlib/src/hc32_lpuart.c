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
/** \file lpuart.c
 **
 ** LPUART function driver API.
 ** @link SampleGroup Some description @endlink
 **
 **   - 2017-05-17  1.0  CJ First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "hc32_lpuart.h"
/**
 ******************************************************************************
 ** \addtogroup LPUartGroup
 ******************************************************************************/
//@{
/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

#define IS_VALID_CH(x)          ((LPUART0 == (x)) ||\
                                 (LPUART1 == (x)))
                                 
#define IS_VALID_CLK(x)         ((LPUart_Pclk==(x))||\
                                (LPUart_Pclk_1==(x))||\
                                (LPUart_Xtl==(x))||\
                                (LPUart_Rcl==(x))) 

#define IS_VALID_IRQSEL(x)      ((LPUartTxIrq == (x)) ||\
                                 (LPUartRxIrq == (x)) ||\
                                 (LPUartFEIrq == (x)) ||\
                                 (LPUartCtsIrq == (x))||\
                                 (LPUartPEIrq == (x)) ||\
                                 (LPUartTxEIrq == (x))) 

#define IS_VALID_MODE(x)        ((LPUartMode0==(x))||\
                                 (LPUartMode1==(x))||\
                                 (LPUartMode2==(x))||\
                                 (LPUartMode3==(x)))

#define IS_VALID_STATUS(x)      ((LPUartCts == (x))||\
                                 (LPUartRC == (x))||\
                                 (LPUartTC == (x))||\
                                 (LPUartPE == (x))||\
                                 (LPUartCtsIf == (x))||\
                                 (LPUartTxe == (x))||\
                                 (LPUartFE == (x)))
/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
static stc_lpuart_instance_data_t* LPUartGetInternDataPtr(uint8_t u8Idx);
static void LPUartInitNvic(uint8_t u8Idx);
static void LPUartDeInitNvic(uint8_t u8Idx);
/******************************************************************************/
/* Local variable definitions ('static')                                       */
/******************************************************************************/
static stc_lpuart_instance_data_t m_astcLPUartInstanceDataLut[] =
{
    {
        LPUART0,
        M0P_LPUART0,       /* pstcInstance */
        {NULL,NULL,NULL,NULL,NULL},     
    },
    {
        LPUART1,
        M0P_LPUART1,       /* pstcInstance */
       {NULL,NULL,NULL,NULL,NULL},    
    },
};
/**
 ******************************************************************************
 ** \brief  LPUART0/1ͨ�� ��ص�ַ��ȡ
 **
 ** \param [in] u8Idxͨ����
 **
 ** \retval ͨ����Ӧ�ĵ�ַ�ṹ
 **
 ******************************************************************************/
static stc_lpuart_instance_data_t* LPUartGetInternDataPtr(uint8_t u8Idx)
{
    stc_lpuart_instance_data_t* pstcData = NULL;
    uint8_t u8i = 0;
    for (u8i = 0; u8i < ARRAY_SZ(m_astcLPUartInstanceDataLut); u8i++)
    {
        if (u8Idx == m_astcLPUartInstanceDataLut[u8i].u32Idx)
        {
            pstcData = &m_astcLPUartInstanceDataLut[u8i];
            break;
        }
    }

    return (pstcData);
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ���ж�ʹ�ܺ�������
 **
 ** \param [in] u8Idxͨ���ţ�enIrqSel����or�����ж�ʹ��
 **
 ** \retval OK���óɹ�
 **\retval ErrorInvalidParameter����ʧ��
 ******************************************************************************/
en_result_t LPUart_EnableIrq(uint8_t u8Idx,
                           en_lpuart_irq_sel_t enIrqSel)
{
    stc_lpuart_instance_data_t* pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_IRQSEL(enIrqSel));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }
    switch(enIrqSel)
    {
        case  LPUartTxIrq:
            pstcData->pstcInstance->SCON_f.TCIE  = 1u;
            break;
        case  LPUartRxIrq:
            pstcData->pstcInstance->SCON_f.RCIE  = 1u;
            break; 
        case  LPUartFEIrq:
            pstcData->pstcInstance->SCON_f.FEIE  = 1u;
            break;
        case  LPUartCtsIrq:
            pstcData->pstcInstance->SCON_f.CTSIE = 1u;
            break;
        case  LPUartPEIrq:
            pstcData->pstcInstance->SCON_f.PEIE  = 1u;
            break;
        case  LPUartTxEIrq:
            pstcData->pstcInstance->SCON_f.TXEIE = 1u;
            break;
        default:
            return (ErrorInvalidParameter);       
    }
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ���жϽ�ֹ��������
 **
 ** \param [in] u8Idxͨ���ţ�enIrqSel����or�����жϽ�ֹ
 **
 ** \retval OK���óɹ�
 **\retval ErrorInvalidParameter����ʧ��
 ******************************************************************************/
en_result_t LPUart_DisableIrq(uint8_t u8Idx, 
                            en_lpuart_irq_sel_t enIrqSel)
{
    stc_lpuart_instance_data_t *pstcData = NULL;

    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_IRQSEL(enIrqSel));
        
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }
    switch(enIrqSel)
    {
        case  LPUartTxIrq:
            pstcData->pstcInstance->SCON_f.TCIE  = 0u;
            break;
        case  LPUartRxIrq:
            pstcData->pstcInstance->SCON_f.RCIE  = 0u;
            break; 
        case  LPUartFEIrq:
            pstcData->pstcInstance->SCON_f.FEIE  = 0u;
            break;
        case  LPUartCtsIrq:
            pstcData->pstcInstance->SCON_f.CTSIE = 0u;
            break;
        case  LPUartPEIrq:
            pstcData->pstcInstance->SCON_f.PEIE  = 0u;
            break;
        case  LPUartTxEIrq:
            pstcData->pstcInstance->SCON_f.TXEIE = 0u;
            break;
        default:
            return (ErrorInvalidParameter);       
    }
    
    return Ok;
}
/**
 ******************************************************************************
 ** \brief lpuartͨ��ʱ��Դѡ��
 **
 ** \param [in] u8Idxͨ���ţ�enClk ʱ��Դѡ��
 **
 ** \retval Ok ���óɹ�
 **\retval ErrorInvalidParameter����ʧ��
 ******************************************************************************/
en_result_t LPUart_SelSclk(uint8_t u8Idx,en_lpuart_sclksel_t enClk)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_CLK(enClk));
    switch(enClk)
    {
        case LPUart_Pclk:
        case LPUart_Pclk_1:
        case LPUart_Xtl:
        case LPUart_Rcl:
            pstcData->pstcInstance->SCON_f.SCLKSEL = enClk;
            break;
        default:
            return (ErrorInvalidParameter);
    }
    return Ok;
}
/**
 ******************************************************************************
 ** \brief lpuartͨ��ʱ��Դѡ��
 **
 ** \param [in] u8Idxͨ����
 **
 ** \retval Ok ���óɹ�
 **\retval 
 ******************************************************************************/
uint32_t LPUart_GetSclk(uint8_t u8Idx)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    uint8_t u8Sclksrc;
    uint32_t u32Sclk;
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }
    ASSERT(IS_VALID_CH(u8Idx));
    u8Sclksrc = pstcData->pstcInstance->SCON_f.SCLKSEL;
    switch(u8Sclksrc)
    {
        case 0x00:
        case 0x01:
            u32Sclk = Sysctrl_GetPClkFreq();
            break;
        case 0x02:
            u32Sclk = 32768;
            break;
        case 0x03:
            u32Sclk = 38400;//�˴�����ʹ���ڲ�38.4k
            break; 
        default:
            return 0;
    }
    return u32Sclk;
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ��4��ģʽ����
 **
 ** \param [in] u8Idxͨ���ţ�mode����ģʽ
 **
 ** \retval OK���óɹ�
 **\retval ErrorInvalidParameter����ʧ��
 ******************************************************************************/
en_result_t LPUart_SetMode(uint8_t u8Idx,en_lpuart_mode_t enMode)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_MODE(enMode));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    } 
    pstcData->pstcInstance->SCON_f.SM = enMode;
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ��������ģʽ����
 **
 ** \param [in] u8Idxͨ���ţ�stcMultiConfig������ģʽ�ṹ
 **
 ** \retval OK���óɹ�
 **\retval ErrorInvalidParameter����ʧ��
 ******************************************************************************/
en_result_t LPUart_SetMultiMode(uint8_t u8Idx,stc_lpuart_multimode_t* pstcMultiConfig)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }
    if(NULL != pstcMultiConfig)
    {
        pstcData->pstcInstance->SCON_f.ADRDET = pstcMultiConfig->enMulti_mode;
        if(pstcMultiConfig->enMulti_mode == LPUartMulti)
        {
            pstcData->pstcInstance->SADDR = pstcMultiConfig->u8SlaveAddr;
            pstcData->pstcInstance->SADEN = pstcMultiConfig->u8SaddEn;
        }
    }
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ��������ģʽ��������/��ַ֡������żУ������TB8
 **
 ** \param [in] u8Idxͨ���ţ�tb8����or��ַ֡������żУ��
 **
 ** \retval OK���óɹ�
 **\retval ErrorInvalidParameter����ʧ��
 ******************************************************************************/
en_result_t LPUart_SetMMDOrCk(uint8_t u8Idx,en_lpuart_mmdorck_t enTb8)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }
    pstcData->pstcInstance->SCON_f.B8CONT = enTb8;
    return Ok;
}
/**
 ******************************************************************************
 ** \brief ��ȡRB8��ֵ
 **
 ** \param [in] u8Idxͨ����
 **
 ** \retval RB8
 **\retval ErrorInvalidParameter����ʧ��
 ******************************************************************************/
boolean_t LPUart_GetRb8(uint8_t u8Idx)
{
     stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }
    return ((pstcData->pstcInstance->SBUF>>8)&0x01);
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ��������ģʽ�ӻ���ַ���ú���
 **
 ** \param [in] u8Idxͨ���ţ�addr��ַ
 **
 ** \retval OK���óɹ�
 **\retval ErrorInvalidParameter����ʧ��
 ******************************************************************************/
en_result_t LPUart_SetSaddr(uint8_t u8Idx,uint8_t u8Addr)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }
    pstcData->pstcInstance->SADDR = u8Addr;
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ��������ģʽ�ӻ��������ú���
 **
 ** \param [in] u8Idxͨ���ţ�addren��ַ����
 **
 ** \retval OK���óɹ�
 **\retval ErrorInvalidParameter����ʧ��
 ******************************************************************************/
en_result_t LPUart_SetSaddrEn(uint8_t u8Idx,uint8_t u8Addren)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }
    pstcData->pstcInstance->SADEN = u8Addren;
    return Ok;  
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ��ֹͣλ��������
 **
 ** \param [in] u8Idxͨ���ţ�u8Lenֹͣλ����
 **
 ** \retval OK���óɹ�
 **\retval ErrorInvalidParameter����ʧ��
 ******************************************************************************/
en_result_t LPUart_SetStopBit(uint8_t u8Idx,uint8_t u8Len)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    switch(u8Len)
    {
        case LPUart1bit:
        case LPUart15bit:
        case LPUart2bit:
           pstcData->pstcInstance->SCON_f.STOPBIT = u8Len;
           break;
        default:
            return ErrorInvalidParameter;
    }
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  LPUART����Ƶ������
 **
 ** \param [in] u8Idxͨ���ţ�u8Div����Ƶ��
 **
 ** \retval OK���óɹ�
 ******************************************************************************/
en_result_t LPUart_SetClkDiv(uint8_t u8Idx,en_lpuart_clkdiv_t enClkDiv)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    pstcData->pstcInstance->SCON_f.OVER = enClkDiv;
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  LPUART�����ʼ���ֵ
 **
 ** \param [in] u8Idxͨ���ţ�stcBaud���㲨����ֵ����
 **
 ** \retval SCNT����ֵ
 ******************************************************************************/
uint16_t LPUart_CalScnt(uint8_t u8Idx,stc_lpuart_baud_t *pstcBaud)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
	uint16_t u16Scnt = 0;
    uint8_t u8Over = 0;
    ASSERT(IS_VALID_CH(u8Idx));

    pstcData = LPUartGetInternDataPtr(u8Idx);
    u8Over = pstcData->pstcInstance->SCON_f.OVER;
    if(u8Over == 3)
    {
        return 0;//test
    } 
    if(LPUartMode0 == pstcBaud->enRunMode)
    {
        return 0;//test
    }
    if((LPUartMode1 == pstcBaud->enRunMode)||(LPUartMode3 == pstcBaud->enRunMode))
    {
        u8Over = 1<<(4-u8Over);            
        u16Scnt = pstcBaud->u32Sclk/(pstcBaud->u32Baud*u8Over);
    }
    else
    {
        u8Over = 1<<(5-u8Over);
        u16Scnt = pstcBaud->u32Sclk/u8Over; 
    }   
    return u16Scnt;
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ������������
 **
 ** \param [in] u8Idxͨ���ţ�u32pclkʱ��Դ��stcBaud���������ýṹ
 **
 ** \retval ��ʱ������ֵ
 ** \retval 0,��ȡֵʧ��
 ******************************************************************************/
en_result_t LPUart_SetBaud(uint8_t u8Idx,uint16_t u16Scnt)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    pstcData->pstcInstance->SCNT = u16Scnt;
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ�������ʻ�ȡ
 **
 ** \param [in] u8Idxͨ����,u8Mode����ģʽ
 **
 ** \retval ������
 ******************************************************************************/
uint32_t LPUart_GetBaud(uint8_t u8Idx,uint8_t u8Mode,uint32_t u32Pclk)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    uint32_t u32Baud = 0;
    uint8_t u8Over = 0;
    uint16_t u16Scnt = 0;
    ASSERT(IS_VALID_CH(u8Idx));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    u8Over = pstcData->pstcInstance->SCON_f.OVER;
    switch(u8Mode)
    {
        case LPUartMode0:
            u32Baud = u32Pclk/12;
            break;
        case LPUartMode1:
        case LPUartMode3:
            u16Scnt = pstcData->pstcInstance->SCNT;
            u8Over = 1<<(4-u8Over); 
            u32Baud = u32Pclk/(u8Over*u16Scnt);
            break;
        case LPUartMode2:
            u8Over = 1<<(5-u8Over); 
            u32Baud = u32Pclk/u8Over;
            break;
        default:
          return 0;//test
    }
    return u32Baud;
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ�����ͻ���յȹ���ʹ������
 **
 ** \param [in] u8Idxͨ���ţ�enFunc����
 **
 ** \retval OK���óɹ�
 **\retval ErrorInvalidParameter����ʧ��
 ******************************************************************************/
en_result_t LPUart_EnableFunc(uint8_t u8Idx, en_lpuart_func_t enFunc)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }
    switch(enFunc)
    {
        case LPUartTx:
        case LPUartRx:
            pstcData->pstcInstance->SCON_f.REN = 1u;
            break;
        case LPUartDmaTx:
            pstcData->pstcInstance->SCON_f.DMATXEN = 1u;
            break;
        case LPUartDmaRx:
            pstcData->pstcInstance->SCON_f.DMARXEN = 1u;
            break;
        case LPUartCtsRts:
            pstcData->pstcInstance->SCON_f.CTSEN = 1u;
            pstcData->pstcInstance->SCON_f.RTSEN = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ�����ͻ���յȹ��ܽ�ֹ����
 **
 ** \param [in] u8Idxͨ���ţ�enFunc����
 **
 ** \retval OK���óɹ�
 **\retval ErrorInvalidParameter����ʧ��
 ******************************************************************************/
en_result_t LPUart_DisableFunc(uint8_t u8Idx, en_lpuart_func_t enFunc)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }
    switch(enFunc)
    {
        case LPUartTx:
        case LPUartRx:
            pstcData->pstcInstance->SCON_f.REN = 0u;
            break;
        case LPUartDmaTx:
            pstcData->pstcInstance->SCON_f.DMATXEN = 0u;
            break;
        case LPUartDmaRx:
            pstcData->pstcInstance->SCON_f.DMARXEN = 0u;
            break;
        case LPUartCtsRts:
            pstcData->pstcInstance->SCON_f.CTSEN = 0u;
            pstcData->pstcInstance->SCON_f.RTSEN = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ��ͨ��״̬��ȡ
 **
 ** \param [in] u8Idxͨ����
 **
 ** \retval ״ֵ̬isr
 ******************************************************************************/
uint8_t LPUart_GetIsr(uint8_t u8Idx)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    return(pstcData->pstcInstance->ISR);
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ��ͨ��״̬ȫ�����
 **
 ** \param [in] u8Idxͨ����
 **
 **\retval Ok
 ******************************************************************************/
en_result_t LPUart_ClrIsr(uint8_t u8Idx)
{
    en_result_t enRet = Error;
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    pstcData->pstcInstance->ICR = 0;
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ��ͨ��״̬��ȡ
 **
 ** \param [in] u8Idxͨ���ţ�enStatus��ȡ�ĸ�״̬
 **
 ** \retval ״ֵ̬
 **\retval ErrorInvalidParameter��ȡʧ��
 ******************************************************************************/
boolean_t LPUart_GetStatus(uint8_t u8Idx,en_lpuart_status_t enStatus)
{
    boolean_t bStatus=FALSE;
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_STATUS(enStatus));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;//4���û�ֻ���ж�0��1
    }
    switch(enStatus)
    {
        case LPUartCts:
            bStatus = (pstcData->pstcInstance->ISR_f.CTS == 1) ? TRUE : FALSE;
            break;
        case LPUartRC:
            bStatus = (pstcData->pstcInstance->ISR_f.RC == 1) ? TRUE : FALSE;
            break;
        case LPUartTC:
            bStatus = (pstcData->pstcInstance->ISR_f.TC == 1) ? TRUE : FALSE;
            break;
        case LPUartPE:
            bStatus = (pstcData->pstcInstance->ISR_f.PE == 1) ? TRUE : FALSE;
            break;
        case LPUartFE:
            bStatus = (pstcData->pstcInstance->ISR_f.FE == 1) ? TRUE : FALSE;
            break; 
        case LPUartCtsIf:
            bStatus = (pstcData->pstcInstance->ISR_f.CTSIF == 1) ? TRUE : FALSE;
            break;
        case LPUartTxe:
            bStatus = (pstcData->pstcInstance->ISR_f.TXE == 1) ? TRUE : FALSE;
            break;        
        default:
            break; 
    }
    return bStatus;
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ��ͨ��״̬���
 **
 ** \param [in] u8Idxͨ���ţ�enStatus����ĸ�״̬
 **
 ** \retval ״ֵ̬
 **\retval ErrorInvalidParameter���ʧ��
 ******************************************************************************/
en_result_t LPUart_ClrStatus(uint8_t u8Idx,en_lpuart_status_t enStatus)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    ASSERT(IS_VALID_STATUS(enStatus));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }
    switch(enStatus)
    {
        case LPUartCts:
            pstcData->pstcInstance->ICR_f.CTSIFCF = 0;
            break;
        case LPUartRC:
            pstcData->pstcInstance->ICR_f.RCCF = 0;
            break;
        case LPUartTC:
            pstcData->pstcInstance->ICR_f.TCCF = 0;
            break;
        case LPUartPE:
            pstcData->pstcInstance->ICR_f.PECF = 0;
            break;
        case LPUartFE:
            pstcData->pstcInstance->ICR_f.FECF = 0;
            break;   
        default:
            break; 
    }
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ���������ݺ���,��ѯ��ʽ���ô˺������жϷ�ʽ���Ͳ�����
 **
 ** \param [in] u8Idxͨ���ţ�Data��������
 **
 ** \retval Ok���ͳɹ�
 **\retval ErrorInvalidParameter����ʧ��
 ******************************************************************************/
en_result_t LPUart_SendData(uint8_t u8Idx, uint8_t u8Data)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }
    LPUart_ClrStatus(u8Idx,LPUartTC);
    pstcData->pstcInstance->SBUF_f.DATA = u8Data;
    while(FALSE == LPUart_GetStatus(u8Idx,LPUartTC))
    {}
    LPUart_ClrStatus(u8Idx,LPUartTC);
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ���������ݺ���
 **
 ** \param [in] u8Idxͨ����
 **
 ** \retval ��������
 **\retval ErrorInvalidParameter����ʧ��
 ******************************************************************************/
uint8_t LPUart_ReceiveData(uint8_t u8Idx)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }
    return (pstcData->pstcInstance->SBUF_f.DATA);
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ���жϴ�����
 **
 ** \param [in] u8Paramͨ����
 **
 ** \retval ��
 **
 ******************************************************************************/
void LpUart_IRQHandler(uint8_t u8Param)
{
    stc_lpuart_instance_data_t *pstcData = NULL;
    pstcData = LPUartGetInternDataPtr(u8Param);
    if (NULL == pstcData)
    {
        return;
    }
    if(1 == pstcData->pstcInstance->ISR_f.FE)
    {
        LPUart_ClrStatus(u8Param,LPUartFE);
        if(NULL != pstcData->stcLPUartInternIrqCb.pfnRxFEIrqCb)
        {
            pstcData->stcLPUartInternIrqCb.pfnRxFEIrqCb();
        }
        return;//֡�����򲻽��к������ݴ���
    }
    if(1 == pstcData->pstcInstance->ISR_f.PE)
    {
        LPUart_ClrStatus(u8Param,LPUartPE);
        if(NULL != pstcData->stcLPUartInternIrqCb.pfnPEIrqCb)
        {
            pstcData->stcLPUartInternIrqCb.pfnPEIrqCb();
        }
        return;//����żУ������򲻽��к������ݴ���
    }
    if(1 == pstcData->pstcInstance->ISR_f.CTSIF)
    {
        LPUart_ClrStatus(u8Param,LPUartCts);
        if(NULL != pstcData->stcLPUartInternIrqCb.pfnCtsIrqCb)
        {
            pstcData->stcLPUartInternIrqCb.pfnCtsIrqCb();
        }
    }
    if(1 == pstcData->pstcInstance->ISR_f.RC)
    {
        LPUart_ClrStatus(u8Param,LPUartRC);
        if(NULL != pstcData->stcLPUartInternIrqCb.pfnRxIrqCb)
        {
            pstcData->stcLPUartInternIrqCb.pfnRxIrqCb();
        }
    }
    if(1 == pstcData->pstcInstance->ISR_f.TC)
    {
        LPUart_ClrStatus(u8Param,LPUartTC);
        if(NULL != pstcData->stcLPUartInternIrqCb.pfnTxIrqCb)
        {
            pstcData->stcLPUartInternIrqCb.pfnTxIrqCb();
        }
    }
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ��ʹ���ں�NVIC�ж�
 **
 ** \param [in] u8Idxͨ����
 **
 ** \retval ��
 **
 ******************************************************************************/
static void LPUartInitNvic(uint8_t u8Idx)
{
    IRQn_Type enIrqIndex;
    
    ASSERT(IS_VALID_CH(u8Idx));;
    enIrqIndex = (IRQn_Type)(LPUART0_IRQn + u8Idx);
    NVIC_ClearPendingIRQ(enIrqIndex);
    NVIC_SetPriority(enIrqIndex,IrqLevel3);
    NVIC_EnableIRQ(enIrqIndex);
    
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ����ֹ�ں�NVIC�ж�
 **
 ** \param [in] u8Idxͨ����
 **
 ** \retval ��
 **
 ******************************************************************************/
static void LPUartDeInitNvic(uint8_t u8Idx)
{
    IRQn_Type enIrqIndex;

    ASSERT(IS_VALID_CH(u8Idx));
    enIrqIndex = (IRQn_Type)(LPUART0_IRQn + u8Idx);
    NVIC_ClearPendingIRQ(enIrqIndex);
    NVIC_SetPriority(enIrqIndex,IrqLevel3);
    NVIC_DisableIRQ(enIrqIndex);
    
}
/**
 ******************************************************************************
 ** \brief  LPUARTͨ����ʼ������
 **
 ** \param [in] u8Idxͨ���ţ�pstcConfig��ʼ���ṹ��
 **
 ** \retval OK���óɹ�
 **\retval ErrorInvalidParameter����ʧ��
 ******************************************************************************/
en_result_t LPUart_Init(uint8_t u8Idx,stc_lpuart_config_t* pstcConfig)
{
    en_result_t enRet = Error;
    stc_lpuart_instance_data_t *pstcData = NULL;
    ASSERT(IS_VALID_CH(u8Idx));
    pstcData = LPUartGetInternDataPtr(u8Idx);
    if (NULL == pstcData)
    {
        return ErrorInvalidParameter;
    }
    if(NULL == pstcConfig)
    {
        return ErrorInvalidParameter;
    }
    if(NULL != pstcConfig->pstcLpuart_clk)
    {
        LPUart_SelSclk(u8Idx,pstcConfig->pstcLpuart_clk->enSclk_sel);
        LPUart_SetClkDiv(u8Idx,pstcConfig->pstcLpuart_clk->enSclk_Prs);
    }
    enRet = LPUart_SetMode(u8Idx,pstcConfig->enRunMode);
    enRet = LPUart_SetStopBit(u8Idx,pstcConfig->enStopBit);
    if(NULL != pstcConfig->pstcMultiMode)
    {
        enRet = LPUart_SetMultiMode(u8Idx,pstcConfig->pstcMultiMode);
    }
    if(NULL != pstcConfig->pstcIrqCb)
    {
        pstcData->stcLPUartInternIrqCb.pfnRxFEIrqCb = pstcConfig->pstcIrqCb->pfnRxFEIrqCb;
        pstcData->stcLPUartInternIrqCb.pfnRxIrqCb = pstcConfig->pstcIrqCb->pfnRxIrqCb;
        pstcData->stcLPUartInternIrqCb.pfnTxIrqCb = pstcConfig->pstcIrqCb->pfnTxIrqCb;
        pstcData->stcLPUartInternIrqCb.pfnCtsIrqCb = pstcConfig->pstcIrqCb->pfnCtsIrqCb;
        pstcData->stcLPUartInternIrqCb.pfnPEIrqCb = pstcConfig->pstcIrqCb->pfnPEIrqCb;
    }
    if(pstcConfig->bTouchNvic == TRUE)
    {
        LPUartInitNvic(u8Idx);
    }
    else
    {
        LPUartDeInitNvic(u8Idx);
    }
    enRet = Ok;
    return enRet;
}
//@} // LPUartGroup      
