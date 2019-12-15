/******************************************************************************
*Copyright(C)2017, Huada Semiconductor Co.,Ltd All rights reserved.
*
* This software is owned and published by:
* Huada Semiconductor Co.,Ltd("HDSC").
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

/** \file trim.c
 **
 ** Common API of trim.
 ** @link trimGroup Some description @endlink
 **
 **   - 2017-05-16
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_trim.h"
/**
 *******************************************************************************
 ** \addtogroup TrimGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
#define IS_VALID_TRIMINT(x)             (RefStop      == (x) ||\
                                         CalCntOf     == (x) ||\
                                         XTAL32MFault == (x))


/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static func_ptr_t pfnTrimCallback = NULL;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *****************************************************************************
 ** \brief Trim�жϱ�־��ȡ
 **
 **
 ** \param [in]  enIntType           �ж����ͣ�RefStop��CalCntOf��XTAL32MFault��
 ** 
 ** \retval TRUE or FALSE                                      
 *****************************************************************************/
boolean_t Trim_GetIntFlag(en_trim_inttype_t enIntType)
{
    boolean_t bRetVal = FALSE;
  
    ASSERT(IS_VALID_TRIMINT(enIntType));
    
    switch (enIntType)
    {
        case RefStop:
            bRetVal = M0P_CLK_TRIM->IFR_f.STOP ? TRUE : FALSE;
            break;    
        case CalCntOf: 
            bRetVal = M0P_CLK_TRIM->IFR_f.CALCNT_OF ? TRUE : FALSE;
            break;        
        case XTAL32MFault: 
            bRetVal = M0P_CLK_TRIM->IFR_f.XTH_FAULT ? TRUE : FALSE;
            break;    
        default:
            bRetVal = FALSE;
            break;
    }
    
    return bRetVal;     
}

/**
 *****************************************************************************
 ** \brief Trim�жϱ�־���
 **
 **
 ** \param [in]  enIntType           �ж����ͣ�RefStop��CalCntOf��XTAL32MFault��
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_ClearIntFlag(en_trim_inttype_t enIntType)
{
    en_result_t enResult = Error;
  
    ASSERT(IS_VALID_TRIMINT(enIntType));
    
    switch (enIntType)
    {
        case RefStop:
            M0P_CLK_TRIM->CR_f.TRIM_START = FALSE;
            enResult = Ok;
            break;    
        case CalCntOf:
            M0P_CLK_TRIM->CR_f.TRIM_START = FALSE;
            enResult = Ok;
            break;       
        case XTAL32MFault: 
            M0P_CLK_TRIM->ICLR_f.XTH_FAULT_CLR = FALSE;
            enResult = Ok;
            break;    
        default:
            enResult = Error;
            break;
    }
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief Trim�жϷ�����
 **
 **
 ** \param [in]  u8Param           == 0
 ** 
 ** \retval NULL                                     
 *****************************************************************************/
void ClkTrim_IRQHandler(uint8_t u8Param)
{
    pfnTrimCallback();
}

/**
 *****************************************************************************
 ** \brief Trim�ж�ʹ��
 **
 **
 **
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_EnableIrq (void)
{
    en_result_t enResult = Error;

    M0P_CLK_TRIM->CR_f.IE = TRUE;
    enResult = Ok;
    
    return enResult; 
}

/**
 *****************************************************************************
 ** \brief Trim�жϽ�ֹ
 **
 **
 **
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_DisableIrq(void)
{
    en_result_t enResult = Error;

    M0P_CLK_TRIM->CR_f.IE = FALSE;
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief Trim��ʼ������
 **
 **
 ** \param [in]  pstcConfig           Trim���ýṹ��ָ��
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_Init(stc_trim_config_t* pstcConfig)
{
    en_result_t enResult = Error;

    M0P_CLK_TRIM->CR_f.MON_EN     = pstcConfig->enMON;
    M0P_CLK_TRIM->CR_f.CALCLK_SEL = pstcConfig->enCALCLK;
    M0P_CLK_TRIM->CR_f.REFCLK_SEL = pstcConfig->enREFCLK;
    
    pfnTrimCallback               = pstcConfig->pfnTrimCb;
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief TrimУ׼/�����������
 **
 **
 **
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_Run(void)
{
    en_result_t enResult = Error;

    M0P_CLK_TRIM->CR_f.TRIM_START = TRUE;
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief TrimУ׼/���ֹͣ
 **
 **
 **
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_Stop(void)
{
    en_result_t enResult = Error;

    M0P_CLK_TRIM->CR_f.TRIM_START = FALSE;
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief Trim�ο���������ֵ����
 **
 **
 ** \param [in]  u32Data           �ο���������ֵ
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_RefConSet(uint32_t u32Data)
{
    en_result_t enResult = Error;

    M0P_CLK_TRIM->REFCON_f.RCNTVAL = u32Data;
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief TrimУ׼��������ֵ����
 **
 **
 ** \param [in]  u32Data           У׼��������ֵ
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_CalConSet(uint32_t u32Data)
{
    en_result_t enResult = Error;

    M0P_CLK_TRIM->CALCON_f.CCNTVAL= u32Data;
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief Trim�ο�����������ֵ��ȡ
 **
 **
 ** 
 ** 
 ** \retval u32Data                �ο�����������ֵ                      
 *****************************************************************************/
uint32_t Trim_RefCntGet(void)
{
    uint32_t u32Data = 0;

    u32Data = M0P_CLK_TRIM->REFCNT_f.REFCNT;
    
    return u32Data;
}

/**
 *****************************************************************************
 ** \brief TrimУ׼����������ֵ��ȡ
 **
 **
 ** 
 ** 
 ** \retval u32Data                У׼����������ֵ                      
 *****************************************************************************/
uint32_t Trim_CalCntGet(void)
{
    uint32_t u32Data = 0;

    u32Data = M0P_CLK_TRIM->CALCNT_f.CALCNT;
    
    return u32Data;
}

//@} // TrimGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
