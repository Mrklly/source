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
/******************************************************************************/
/** \file Gpio.c
 **
 ** GPIO driver API.
 ** @link Driver Group Some description @endlink
 **
 **   - 2017-04-29  1.0  Ronnie     First version for Device Driver Library of
 **                                 Module.
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_gpio.h"

/**
 *******************************************************************************
 ** \addtogroup GpioGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IS_VALID_PIN(port,pin)      (  )
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')         *
 ******************************************************************************/

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/


/**
 *******************************************************************************
 ** \brief GPIO ģ�⹦������
 **
 ** \param [in]  u8Port          IO Port��
 ** \param [in]  u8Pin           IO Pin��
 ** \param [in]  bFlag           TRUE :     ģ�⹦��
 **                              FALSE:     ���ֹ���
 **
 ** \retval Ok         ���óɹ�
 **         ����ֵ     ����ʧ��
 ******************************************************************************/
en_result_t Gpio_SetAnalog(uint8_t u8Port, uint8_t u8Pin, boolean_t bFlag)
{
    setBit((uint32_t)&M0P_GPIO->P0ADS + u8Port * GPIO_GPSZ, u8Pin, bFlag);

    return Ok;
}


/**
 *******************************************************************************
 ** \brief GPIO ��ʼ��IO
 **
 ** \param [in]  u8Port          IO Port��
 ** \param [in]  u8Pin           IO Pin��
 ** \param [in]  enDir           IO ��������������
 **
 ** \retval Ok         ���óɹ�
 **         ����ֵ     ����ʧ��
 ******************************************************************************/
en_result_t Gpio_InitIO(uint8_t u8Port, uint8_t u8Pin,
                        en_gpio_dir_t enDir)
{
    //force open clock
    M0P_CLOCK->PERI_CLKEN_f.GPIO = 1;
    //force set mode, ignore result.
    Gpio_SetAnalog(u8Port, u8Pin, FALSE);

    //fn
    *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P01_SEL + u8Port * GPIO_GPSZ - 4 + u8Pin * 4)) = 0;
    //ADS
    //setBit((uint32_t)&M0P_GPIO->ADS0 + u8Port * GPIO_GPSZ, u8Pin, 0);
    //dir
    setBit((uint32_t)&M0P_GPIO->P0DIR + u8Port * GPIO_GPSZ, u8Pin, enDir);
    //dr
    setBit((uint32_t)&M0P_GPIO->P0PU + u8Port * GPIO_GPSZ, u8Pin, FALSE);
    setBit((uint32_t)&M0P_GPIO->P0PD + u8Port * GPIO_GPSZ, u8Pin, FALSE);
    setBit((uint32_t)&M0P_GPIO->P0OD + u8Port * GPIO_GPSZ, u8Pin, FALSE);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief   GPIO ��ʼ��2 
 ** 
 ** \param   [in]  u8Port             IO Port��
 ** \param   [in]  u8Pin              IO Pin��
 ** \param   [in]  enDir              IO ��������������
 ** \param   [in]  bPullup            ��������
 ** \param   [in]  bPulldown          ��������
 ** \param   [in]  bOdr               ��©����
 ** \param   [in]  bDrive             �������� 
 **                                   0 = ��
 **                                   1 = ��
 ** \retval     Ok         ���óɹ�
 **             ����ֵ     ����ʧ��
 ******************************************************************************/
en_result_t Gpio_InitIOExt(uint8_t u8Port, uint8_t u8Pin,
                           en_gpio_dir_t enDir,
                           boolean_t bPullup,
                           boolean_t bPulldown,
                           boolean_t bOdr,
                           boolean_t bDrive)
{
    //force open clock
    M0P_CLOCK->PERI_CLKEN_f.GPIO = 1;
    //force set mode, ignore result.
    Gpio_SetAnalog(u8Port, u8Pin, FALSE);

    //fn
    *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P01_SEL + u8Port * GPIO_GPSZ - 4 + u8Pin * 4)) = 0;
    //ADS
    //setBit((uint32_t)&M0P_GPIO->ADS0 + u8Port * GPIO_GPSZ, u8Pin, 0);
    //dir
    setBit((uint32_t)&M0P_GPIO->P0DIR + u8Port * GPIO_GPSZ, u8Pin, enDir);
    //dr
    setBit((uint32_t)&M0P_GPIO->P0DR + u8Port * GPIO_GPSZ, u8Pin, bDrive);

    setBit((uint32_t)&M0P_GPIO->P0PU + u8Port * GPIO_GPSZ, u8Pin, bPullup);
    setBit((uint32_t)&M0P_GPIO->P0PD + u8Port * GPIO_GPSZ, u8Pin, bPulldown);
    setBit((uint32_t)&M0P_GPIO->P0OD + u8Port * GPIO_GPSZ, u8Pin, bOdr);

    return Ok;
}


/**
 *******************************************************************************
 ** \brief GPIO IO���ֵ�趨
 **
 ** \param [in]  u8Port          IO Port��
 ** \param [in]  u8Pin           IO Pin��
 ** \param [in]  bVal            �����ƽ�ߵ�
 **
 ** \retval Ok         ���óɹ�
 **         ����ֵ     ����ʧ��
 ******************************************************************************/
void Gpio_SetIO(uint8_t u8Port, uint8_t u8Pin, boolean_t bVal)
{
    bVal = !!bVal;
    setBit((uint32_t)&M0P_GPIO->P0OUT + u8Port * GPIO_GPSZ, u8Pin, bVal);
}



/**
 *******************************************************************************
 ** \brief GPIO IO����ֵ��ȡ
 **
 ** \param [in]  u8Port          IO Port��
 ** \param [in]  u8Pin           IO Pin��
 **
 ** \retval boolean_t            IO��ƽ�ߵ�
 ******************************************************************************/
boolean_t Gpio_GetIO(uint8_t u8Port, uint8_t u8Pin)
{
    return getBit((uint32_t)&M0P_GPIO->P0IN + u8Port * GPIO_GPSZ, u8Pin);
}

en_result_t _GpioEnableIrq(uint8_t u8Port,
                           uint8_t u8Pin,
                           en_gpio_irqtype_t enType,
                           boolean_t bEnable)
{
    //high level
    if (enType & GpioIrqHigh)
    {
        setBit((uint32_t)&M0P_GPIO->P0HIE + u8Port * GPIO_GPSZ, u8Pin, bEnable);
    }
    //low level
    if (enType & GpioIrqLow)
    {
        setBit((uint32_t)&M0P_GPIO->P0LIE + u8Port * GPIO_GPSZ, u8Pin, bEnable);
    }
    //rising
    if (enType & GpioIrqRising)
    {
        setBit((uint32_t)&M0P_GPIO->P0RIE + u8Port * GPIO_GPSZ, u8Pin, bEnable);
    }
    //falling
    if (enType & GpioIrqFalling)
    {
        setBit((uint32_t)&M0P_GPIO->P0FIE + u8Port * GPIO_GPSZ, u8Pin, bEnable);
    }


    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO�ж�ʹ��
 **
 ** \param [in]  u8Port          IO Port��
 ** \param [in]  u8Pin           IO Pin��
 ** \param [in]  enType          �ж�ʹ������
 ** \retval      Ok            ���óɹ�
 ******************************************************************************/
en_result_t Gpio_EnableIrq(uint8_t u8Port,
                           uint8_t u8Pin,
                           en_gpio_irqtype_t enType)
{
    _GpioEnableIrq(u8Port, u8Pin, enType, TRUE);
    return Ok;
}


/**
 *******************************************************************************
 ** \brief GPIO IO�жϳ���
 **
 ** \param [in]  u8Port          IO Port��
 ** \param [in]  u8Pin           IO Pin��
 ** \param [in]  enType          �ж�ʹ������
 ** \retval      Ok            ���óɹ�
 ******************************************************************************/
en_result_t Gpio_DisableIrq(uint8_t u8Port, uint8_t u8Pin, en_gpio_irqtype_t enType)
{
    _GpioEnableIrq(u8Port, u8Pin, enType, FALSE);
    return Ok;
}


/**
 *******************************************************************************
 ** \brief GPIO ���IO�ж�״̬
 **
 ** \param [in]  u8Port          IO Port��
 ** \param [in]  u8Pin           IO Pin��
 **
 ** \retval      IO�ж�״̬����
 ******************************************************************************/
boolean_t Gpio_GetIrqStat(uint8_t u8Port, uint8_t u8Pin)
{
    return getBit((uint32_t)&M0P_GPIO->P0STAT + u8Port * 0x40, u8Pin);
}


/**
 *******************************************************************************
 ** \brief GPIO ���IO�ж�״̬
 **
 ** \param [in]  u8Port          IO Port��
 ** \param [in]  u8Pin           IO Pin��
 **
 ** \retval    Ok       ���óɹ�
 ******************************************************************************/
en_result_t Gpio_ClearIrq(uint8_t u8Port, uint8_t u8Pin)
{
    setBit((uint32_t)&M0P_GPIO->P0ICLR + u8Port * GPIO_GPSZ, u8Pin, 0u);

    return Ok;
}


/**
 *******************************************************************************
 ** \brief GPIO ���ӹ����趨
 **
 ** \param [in]  enFunc          ����
 ** \param [in]  u8val           �趨ֵ
 **
 ** \retval    Ok       ���óɹ�
 ******************************************************************************/
en_result_t Gpio_SetCtrlExt(en_gpio_ctrlext_t enFunc, uint8_t u8val)
{
    en_result_t enRet = Ok;
    
    switch (enFunc)
    {
        case GpioCtrlIESEL:
            u8val = !!u8val;
            M0P_GPIO->CTRL0_f.IESEL = u8val;
            break;

        case GpioCtrlIRPOL:
            u8val = !!u8val;
            M0P_GPIO->CTRL1_f.IR_POL = u8val;
            break;

        case GpioCtrlHclkEn:
            u8val = !!u8val;
            M0P_GPIO->CTRL1_f.HCLK_EN = u8val;
            break;

        case GpioCtrlPclkEn:
            u8val = !!u8val;
            M0P_GPIO->CTRL1_f.PCLK_EN = u8val;
            break;

        case GpioCtrlHclkSel:
          {
            if ( (ClkDiv1 == u8val) || (ClkDiv2 == u8val) || (ClkDiv4 == u8val) || (ClkDiv8 == u8val) )
            {
                u8val = Log2(u8val);
                M0P_GPIO->CTRL1_f.HCLK_SEL = u8val;
            } else
            {
                enRet = ErrorInvalidParameter;
            }
            break;
          }
        case GpioCtrlPclkSel:
          {
            if ( (ClkDiv1 == u8val) || (ClkDiv2 == u8val) || (ClkDiv4 == u8val) || (ClkDiv8 == u8val) )
            {
                u8val = Log2(u8val);
                M0P_GPIO->CTRL1_f.PCLK_SEL = u8val;
            } else
            {
              enRet = ErrorInvalidParameter;
            }
            break;
          }
        default:
          enRet = ErrorInvalidParameter;
          break;
    }

    return enRet;
}


/**
 *******************************************************************************
 ** \brief GPIO SSNѡ������
 **
 ** \param [in]  enSrc          ����Դ�趨
 **
 ** \retval    Ok       ���óɹ�
 **            ����ֵ   ����ʧ��
 ******************************************************************************/
en_result_t Gpio_SetSsnSel(en_gpio_ctrl1src_t enSrc)
{
    switch (enSrc)
    {
        case GpioCtrl1SrcP35: 
        case GpioCtrl1SrcP36: 
        case GpioCtrl1SrcP01: 
        case GpioCtrl1SrcP02: 
        case GpioCtrl1SrcP03: 
        case GpioCtrl1SrcP15: 
        case GpioCtrl1SrcP14: 
        case GpioCtrl1SrcP23: 
        case GpioCtrl1SrcP24: 
        case GpioCtrl1SrcP25: 
        case GpioCtrl1SrcP26: 
        case GpioCtrl1SrcP27: 
        case GpioCtrl1SrcP31: 
        case GpioCtrl1SrcP32: 
        case GpioCtrl1SrcP33: 
            M0P_GPIO->CTRL1_f.SSN_SEL = enSrc;
            break;
        default:
            return ErrorInvalidParameter;
    }
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO EXT CLKѡ������
 **
 ** \param [in]  enSrc          ����Դ�趨
 **
 ** \retval    Ok       ���óɹ�
 **            ����ֵ   ����ʧ��
 ******************************************************************************/
en_result_t Gpio_SetExtClkSel(en_gpio_ctrl1src_t enSrc)
{
    switch (enSrc)
    {
        case GpioCtrl1SrcP35: 
        case GpioCtrl1SrcP36: 
        case GpioCtrl1SrcP01: 
        case GpioCtrl1SrcP02: 
        case GpioCtrl1SrcP03: 
        case GpioCtrl1SrcP15: 
        case GpioCtrl1SrcP14: 
        case GpioCtrl1SrcP23: 
        case GpioCtrl1SrcP24: 
        case GpioCtrl1SrcP25: 
        case GpioCtrl1SrcP26: 
        case GpioCtrl1SrcP27: 
        case GpioCtrl1SrcP31: 
        case GpioCtrl1SrcP32: 
        case GpioCtrl1SrcP33: 
            M0P_GPIO->CTRL1_f.EXT_CLK_SEL = enSrc;
            break;
        default:
            return ErrorInvalidParameter;
    }
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO ��ʱ��x����ʱ���趨
 **
 ** \param [in]  enTarget           ����IO
 ** \param [in]  enType             ��ʱ������
 ** \retval    Ok       ���óɹ�
 **            ����ֵ   ����ʧ��
 ******************************************************************************/
en_result_t Gpio_SetSignalSource(en_gpio_target_t enTarget, en_gpio_timex_in_t enType)
{
    if (enType > GpioUart2RxdIn)
    {
        return ErrorInvalidParameter;
    }

    switch (enTarget)
    {
        case GpioSignalCAP4:
            M0P_GPIO->CTRL2_f.PCA_CAP4_SEL = enType;
            break;
        case GpioSignalCAP3:
            M0P_GPIO->CTRL2_f.PCA_CAP3_SEL = enType;
            break;
        case GpioSignalCAP2:
            M0P_GPIO->CTRL2_f.PCA_CAP2_SEL = enType;
            break;
        case GpioSignalCAP1:
            M0P_GPIO->CTRL2_f.PCA_CAP1_SEL = enType;
            break;
        case GpioSignalCAP0:
            M0P_GPIO->CTRL2_f.PCA_CAP0_SEL = enType;
            break;

        case GpioSignalTm6A:
            M0P_GPIO->CTRL3_f.TM6_A_SEL = enType;
            break;
        case GpioSignalTm5A:
            M0P_GPIO->CTRL3_f.TM5_A_SEL = enType;
            break;
        case GpioSignalTm4A:
            M0P_GPIO->CTRL3_f.TM4_A_SEL = enType;
            break;
        case GpioSignalTm6B:
            M0P_GPIO->CTRL3_f.TM6_B_SEL = enType;
            break;
        case GpioSignalTm5B:
            M0P_GPIO->CTRL3_f.TM5_B_SEL = enType;
            break;
        case GpioSignalTm4B:
            M0P_GPIO->CTRL3_f.TM4_B_SEL = enType;
            break;
        case GpioSignalTm2:
            M0P_GPIO->CTRL4_f.TM2_GATE_SEL = enType;
            break;
        case GpioSignalTm1:
            M0P_GPIO->CTRL4_f.TM1_GATE_SEL = enType;
            break;
        case GpioSignalTm0:
            M0P_GPIO->CTRL4_f.TM0_GATE_SEL = enType;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

//@} // GpioGroup


/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

