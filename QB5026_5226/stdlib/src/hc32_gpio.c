/******************************************************************************
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
/** \file Gpio.c
 **
 ** GPIO driver API.
 ** @link Driver Group Some description @endlink
 **
 **   - 2018-04-22  1.0  Lux First version
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
 ** \brief GPIO ��ʼ��
 **
 ** \param [in]  enPort          IO Port��
 ** \param [in]  enPin           IO Pin��
 ** \param [in]  pstcGpioCfg     IO ���ýṹ��ָ��
 **
 ** \retval Ok         ���óɹ�
 **         ����ֵ     ����ʧ��
 ******************************************************************************/
en_result_t Gpio_Init(en_gpio_port_t enPort, en_gpio_pin_t enPin, stc_gpio_config_t  *pstcGpioCfg)
{
    //����ΪĬ��ֵ,GPIO����
    *((uint32_t*)(((uint32_t)(&(M0P_GPIO->PA00_SEL)) + enPort) + (((uint32_t)enPin)<<2))) = GpioAf0;
    
    //��������
    if(GpioDirIn == pstcGpioCfg->enDir)
    {
        setBit(((uint32_t)&M0P_GPIO->PADIR + enPort), enPin, TRUE);
    }
    else
    {
        setBit(((uint32_t)&M0P_GPIO->PADIR + enPort), enPin, FALSE);
    }
    
    //������������
    if(GpioDrvH == pstcGpioCfg->enDrv)
    {
        setBit(((uint32_t)&M0P_GPIO->PADR + enPort), enPin, FALSE);
    }
    else
    {
        setBit(((uint32_t)&M0P_GPIO->PADR + enPort), enPin, TRUE);
    }
    
    //������������
    if(GpioPu == pstcGpioCfg->enPuPd)
    {
        setBit(((uint32_t)&M0P_GPIO->PAPU + enPort), enPin, TRUE);
        setBit(((uint32_t)&M0P_GPIO->PAPD + enPort), enPin, FALSE);
    }
    else if(GpioPd == pstcGpioCfg->enPuPd)
    {
        setBit(((uint32_t)&M0P_GPIO->PAPU + enPort), enPin, FALSE);
        setBit(((uint32_t)&M0P_GPIO->PAPD + enPort), enPin, TRUE);
    }
    else
    {
        setBit(((uint32_t)&M0P_GPIO->PAPU + enPort), enPin, FALSE);
        setBit(((uint32_t)&M0P_GPIO->PAPD + enPort), enPin, FALSE);
    }
    
    //��©�������
    if(GpioOdDisable == pstcGpioCfg->enOD)
    {
        setBit(((uint32_t)&M0P_GPIO->PAOD + enPort), enPin, FALSE);
    }
    else
    {
        setBit(((uint32_t)&M0P_GPIO->PAOD + enPort), enPin, TRUE);
    }
    
    M0P_GPIO->CTRL2_f.AHB_SEL = pstcGpioCfg->enCtrlMode;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO����ֵ��ȡ
 **
 ** \param [in]  enPort          IO Port��
 ** \param [in]  enPin           IO Pin��
 **
 ** \retval boolean_t            IO��ƽ�ߵ�
 ******************************************************************************/
boolean_t Gpio_GetInputIO(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    return getBit(((uint32_t)&M0P_GPIO->PAIN + enPort), enPin);
}

/**
 *******************************************************************************
 ** \brief GPIO IO Port�������ݻ�ȡ
 **
 ** \param [in]  enPort          IO Port
 **
 ** \retval boolean_t            IO Port����
 ******************************************************************************/
uint16_t    Gpio_GetInputData(en_gpio_port_t enPort)
{
    return (uint16_t)(*((uint32_t *)((uint32_t)&M0P_GPIO->PAIN + enPort))); 
}

/**
 *******************************************************************************
 ** \brief GPIO IO���ֵд��
 **
 ** \param [in]  enPort          IO Port��
 ** \param [in]  enPin           IO Pin��
 ** \param [out] bVal            ���ֵ
 **
 ** \retval en_result_t          Ok      ���óɹ�
 **                              ����ֵ  ����ʧ��
 ******************************************************************************/
en_result_t Gpio_WriteOutputIO(en_gpio_port_t enPort, en_gpio_pin_t enPin, boolean_t bVal)
{
    setBit(((uint32_t)&M0P_GPIO->PAOUT + enPort), enPin, bVal);
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO���ֵ��ȡ
 **
 ** \param [in]  enPort          IO Port��
 ** \param [in]  enPin           IO Pin��
 **
 ** \retval boolean_t            IO��ƽ�ߵ�
 ******************************************************************************/
boolean_t   Gpio_ReadOutputIO(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    return getBit(((uint32_t)&M0P_GPIO->PAOUT + enPort), enPin);
}

/**
 *******************************************************************************
 ** \brief GPIO IO Port���ã���ͬʱ����һ��Port�еĶ��PIN
 **
 ** \param [in]  enPort          IO Port
 ** \param [in]  u16ValMsk       ��Port��16��PIN����ֵ,����Ҫ���õ�PIN��Ӧ��bitд1��Ч   
 **
 ** \retval boolean_t            IO Port����
 ******************************************************************************/
en_result_t Gpio_SetPort(en_gpio_port_t enPort, uint16_t u16ValMsk)
{
    *((uint16_t*)(((uint32_t)&(M0P_GPIO->PABSET)) + enPort)) = u16ValMsk;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO����
 **
 ** \param [in]  enPort          IO Port��
 ** \param [in]  enPin           IO Pin��
 **
 ** \retval en_result_t          Ok      ���óɹ�
 **                              ����ֵ  ����ʧ��
 ******************************************************************************/
en_result_t Gpio_SetIO(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    setBit(((uint32_t)&M0P_GPIO->PABSET + enPort), enPin, TRUE);
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO Port���㣬��ͬʱ����һ��Port�еĶ��PIN
 **
 ** \param [in]  enPort          IO Port
 ** \param [in]  u16ValMsk       ��Port��16��PIN����ֵ,����Ҫ�����PIN��Ӧ��bitд1��Ч
 **
 ** \retval boolean_t            IO Port����
 ******************************************************************************/
en_result_t Gpio_ClrPort(en_gpio_port_t enPort, uint16_t u16ValMsk)
{
    *((uint16_t*)(((uint32_t)&(M0P_GPIO->PABCLR)) + enPort)) = u16ValMsk;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO����
 **
 ** \param [in]  enPort          IO Port��
 ** \param [in]  enPin           IO Pin��
 **
 ** \retval en_result_t          Ok      ���óɹ�
 **                              ����ֵ  ����ʧ��
 ******************************************************************************/
en_result_t Gpio_ClrIO(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    setBit(((uint32_t)&M0P_GPIO->PABCLR + enPort), enPin, TRUE);
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO Port��λ/���㣬��ͬʱ��λ/����һ��Port�еĶ��PIN
 **
 ** \param [in]  enPort       IO Port
** \param [in]  u32ValMsk     ��16bits��ʾ��Port��16��PIN��λ����ֵ,
 **                           ��16bits��ʾ��Port��16��PIN��������ֵ,
**                            ����Ҫ���õ�PIN��Ӧ��bitд1,ͬһ��PIN������ͬʱΪ1,���PIN���㡣                              
 **
 ** \retval en_result_t       Ok      ���óɹ�
 **                           ����ֵ  ����ʧ��
 ******************************************************************************/
en_result_t Gpio_SetClrPort(en_gpio_port_t enPort, uint32_t u32ValMsk)
{
    *((uint32_t*)(((uint32_t)&(M0P_GPIO->PABSETCLR)) + enPort)) = u32ValMsk;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO����Ϊģ�⹦��ģʽ
 **
 ** \param [in]  enPort          IO Port��
 ** \param [in]  enPin           IO Pin��
 **
 ** \retval Ok         ���óɹ�
 **         ����ֵ     ����ʧ��
 ******************************************************************************/
en_result_t Gpio_SetAnalogMode(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    setBit((uint32_t)&M0P_GPIO->PAADS + enPort, enPin, TRUE);

    return Ok;
}

/**
 *******************************************************************************
** \brief GPIO IO���ù�������
 **
 ** \param [in]  enPort    IO Port��
 ** \param [in]  enPin     IO Pin��
 ** \param [in]  enAf      ���ù���ö������ѡ��
 ** \retval Ok             ���óɹ�
 **         ����ֵ         ����ʧ��
 ******************************************************************************/
en_result_t Gpio_SetAfMode(en_gpio_port_t enPort, en_gpio_pin_t enPin, en_gpio_af_t enAf)
{
    *((uint32_t*)(((uint32_t)(&(M0P_GPIO->PA00_SEL)) + enPort) + (((uint32_t)enPin)<<2))) = enAf;
    
    return Ok;
}


static en_result_t _GpioEnableIrq(en_gpio_port_t enPort,
                           en_gpio_pin_t enPin,
                           en_gpio_irqtype_t enType,
                           boolean_t bEnable)
{
    //high level
    if (enType & GpioIrqHigh)
    {
        setBit((uint32_t)&M0P_GPIO->PAHIE + enPort, enPin, bEnable);
    }
    //low level
    if (enType & GpioIrqLow)
    {
        setBit((uint32_t)&M0P_GPIO->PALIE + enPort, enPin, bEnable);
    }
    //rising
    if (enType & GpioIrqRising)
    {
        setBit((uint32_t)&M0P_GPIO->PARIE + enPort, enPin, bEnable);
    }
    //falling
    if (enType & GpioIrqFalling)
    {
        setBit((uint32_t)&M0P_GPIO->PAFIE + enPort, enPin, bEnable);
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO�ж�ʹ��
 **
 ** \param [in]  enPort          IO Port��
 ** \param [in]  enPin           IO Pin��
 ** \param [in]  enType          �ж�ʹ������
 **
 ** \retval      Ok            ���óɹ�
 ******************************************************************************/
en_result_t Gpio_EnableIrq(en_gpio_port_t enPort, en_gpio_pin_t enPin, en_gpio_irqtype_t enType)
{
    _GpioEnableIrq(enPort, enPin, enType, TRUE);
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO�жϹر�
 **
 ** \param [in]  enPort          IO Port��
 ** \param [in]  enPin           IO Pin��
 ** \param [in]  enType          �ж�ʹ������
 **
 ** \retval      Ok            ���óɹ�
 ******************************************************************************/
en_result_t Gpio_DisableIrq(en_gpio_port_t enPort, en_gpio_pin_t enPin, en_gpio_irqtype_t enType)
{
    _GpioEnableIrq(enPort, enPin, enType, FALSE);
    
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
boolean_t Gpio_GetIrqStatus(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    return getBit((uint32_t)&M0P_GPIO->PA_STAT + enPort, enPin);
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
en_result_t Gpio_ClearIrq(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    setBit((uint32_t)&M0P_GPIO->PA_ICLR + enPort, enPin, FALSE);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO �˿ڸ����������á����ж�ģʽ����
 **
 ** \param [in]  enIrqMode          �˿��ж�ģʽ����������Ƿ���Ӧ�жϣ�
 **
 ** \retval    Ok       ���óɹ�
 ******************************************************************************/
en_result_t Gpio_SfIrqModeConfig(en_gpio_sf_irqmode_t enIrqMode)
{
    M0P_GPIO->CTRL0_f.IESEL = enIrqMode;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO �˿ڸ����������á���IR�����������
 **
 ** \param [in]  enIrPolMode          IR�����������ö��
 **
 ** \retval    Ok       ���óɹ�
 ******************************************************************************/
en_result_t Gpio_SfIrPolConfig(en_gpio_sf_irpol_t enIrPolMode)
{
    M0P_GPIO->CTRL1_f.IR_POL = enIrPolMode;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO �˿ڸ����������á���HCLK�������
 **
 ** \param [in]  enGate         HCLK���ʹ��
 ** \param [in]  enDiv          �����Ƶö��ֵ
 **
 ** \retval    Ok       ���óɹ�
 ******************************************************************************/
en_result_t Gpio_SfHClkOutputConfig(en_gpio_sf_hclkout_g_t enGate, en_gpio_sf_hclkout_div_t enDiv)
{
    M0P_GPIO->CTRL1_f.HCLK_EN  = enGate;
    M0P_GPIO->CTRL1_f.HCLK_SEL = enDiv;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO �˿ڸ����������á���PCLK�������
 **
 ** \param [in]  enGate         PCLK���ʹ��
 ** \param [in]  enDiv          �����Ƶö��ֵ
 **
 ** \retval    Ok       ���óɹ�
 ******************************************************************************/
en_result_t Gpio_SfPClkOutputConfig(en_gpio_sf_pclkout_g_t enGate, en_gpio_sf_pclkout_div_t enDiv)
{
    M0P_GPIO->CTRL1_f.PCLK_EN  = enGate;
    M0P_GPIO->CTRL1_f.PCLK_SEL = enDiv;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO �˿ڸ����������á����ⲿʱ��������Դ����
 **
 ** \param [in]  enExtClk         �ⲿʱ���ź���Դѡ��ö��
 **
 ** \retval    Ok       ���óɹ�
 ******************************************************************************/
en_result_t Gpio_SfExtClkConfig(en_gpio_sf_ssn_extclk_t enExtClk)
{
    M0P_GPIO->CTRL1_f.EXT_CLK_SEL  = enExtClk;
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO �˿ڸ����������á���SSN ͨ���ź���Դ����
 **
 ** \param [in]  enSpi         SSN SPIͨ��ѡ��ö��
 ** \param [in]  enSsn         SSN �ź���Դѡ��ö��
 **
 ** \retval    Ok       ���óɹ�
 ******************************************************************************/
en_result_t Gpio_SfSsnConfig(en_gpio_sf_ssnspi_t enSpi, en_gpio_sf_ssn_extclk_t enSsn)
{
    //SPI0
    if(enSpi == GpioSpi0)
    {
        M0P_GPIO->CTRL1_f.SSN0_SEL = enSsn;
    }
    //SPI1
    if(enSpi == GpioSpi1)
    {
        M0P_GPIO->CTRL2_f.SSN1_SEL = enSsn;
    }
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO �˿ڸ����������á���Timer �ſ���������
 **
 ** \param [in]  enTimG       Timer����ѡ��ö��
 ** \param [in]  enSf         Timer��������ѡ��ö��
 **
 ** \retval    Ok       ���óɹ�
 ******************************************************************************/
en_result_t Gpio_SfTimGConfig(en_gpio_sf_tim_g_t enTimG, en_gpio_sf_t enSf)
{
    M0P_GPIO->TIMGS &= (uint32_t)(~(0x07U<<enTimG));
    M0P_GPIO->TIMGS |= (uint32_t)(enSf<<enTimG);
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO �˿ڸ����������á���Timer ETRѡ������
 **
 ** \param [in]  enTimE       Timer����ѡ��ö��
 ** \param [in]  enSf         Timer��������ѡ��ö��
 **
 ** \retval    Ok       ���óɹ�
 ******************************************************************************/
en_result_t Gpio_SfTimEConfig(en_gpio_sf_tim_e_t enTimE, en_gpio_sf_t enSf)
{
    M0P_GPIO->TIMES &= (uint32_t)(~(0x07U<<enTimE));
    M0P_GPIO->TIMES |= (uint32_t)(enSf<<enTimE);
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO �˿ڸ����������á���Timer ��������ѡ������
 **
 ** \param [in]  enTimC       Timer����ѡ��ö��
 ** \param [in]  enSf         Timer��������ѡ��ö��
 **
 ** \retval    Ok       ���óɹ�
 ******************************************************************************/
en_result_t Gpio_SfTimCConfig(en_gpio_sf_tim_c_t enTimC, en_gpio_sf_t enSf)
{
    M0P_GPIO->TIMCPS &= (uint32_t)(~(0x07u<<enTimC));
    M0P_GPIO->TIMCPS |= (uint32_t)(enSf<<enTimC);
    
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO �˿ڸ����������á���PCA ��������ѡ������
 **
 ** \param [in]  enPca        PCA����ѡ��ö��
 ** \param [in]  enSf         PCA��������ѡ��ö��
 **
 ** \retval    Ok       ���óɹ�
 ******************************************************************************/
en_result_t Gpio_SfPcaConfig(en_gpio_sf_pca_t enPca, en_gpio_sf_t enSf)
{
    if(GpioSfPcaCH0 == enPca)
    {
        M0P_GPIO->PCAS_f.PCA_CH0 = enSf;
    }
    
    if(GpioSfPcaECI == enPca)
    {
        M0P_GPIO->PCAS_f.PCA_ECI = enSf;
    }
    
    return Ok;
}


//@} // GpioGroup


/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

