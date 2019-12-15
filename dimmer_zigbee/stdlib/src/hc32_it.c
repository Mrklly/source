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
/** \file interrupts_hc32f005.c
 **
 ** Interrupt management
 ** @link Driver Group Some description @endlink
 **
 **   - 2017-04-29  1.0  Ronnie     First version.
 **
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"
#include "hc32_it.h"

#include "drv_led.h"

__WEAKDEF void Gpio_IRQHandler(uint8_t u8Param);
__WEAKDEF void Uart_IRQHandler(uint8_t u8Param);
__WEAKDEF void Spi_IRQHandler(uint8_t u8Param);
__WEAKDEF void I2c_IRQHandler(uint8_t u8Param);
__WEAKDEF void Tim_IRQHandler(uint8_t u8Param);
__WEAKDEF void Adt_IRQHandler(uint8_t u8Param);
__WEAKDEF void Pca_IRQHandler(uint8_t u8Param);
__WEAKDEF void Wdt_IRQHandler(uint8_t u8Param);
__WEAKDEF void Vc_IRQHandler(uint8_t u8Param);
__WEAKDEF void Adc_IRQHandler(uint8_t u8Param);
__WEAKDEF void Lvd_IRQHandler(uint8_t u8Param);
__WEAKDEF void EfRam_IRQHandler(uint8_t u8Param);
__WEAKDEF void ClkTrim_IRQHandler(uint8_t u8Param);

/**
 *******************************************************************************
 ** \brief NVIC �ж�ʹ��
 **
 ** \param [in]  u32Irq         �жϺ�
 ** \param [in]  u8Level        �ж����ȼ�
 ** \param [in]  bEn            �жϿ���
 ** \retval    Ok       ���óɹ�
 **            ����ֵ   ����ʧ��
 ******************************************************************************/
void EnableNvic(uint32_t u32Irq,uint8_t u8Level,boolean_t bEn)
{
    IRQn_Type enIrq = (IRQn_Type)u32Irq;

    NVIC_ClearPendingIRQ(enIrq);
    NVIC_SetPriority(enIrq, u8Level);
    if (TRUE == bEn)
    {
        NVIC_EnableIRQ(enIrq);
    }else{
        NVIC_DisableIRQ(enIrq);
    }
}

/**
 *******************************************************************************
 ** \brief NVIC hardware fault �ж�ʵ��
 **        ���ڵ������Թ���
 **
 ** \retval
 ******************************************************************************/
void HardFault_Handler(void)
{
    volatile int a = 0;

    while( 0 == a)
    {
        LED_1_ON;
        LED_2_ON;
        LED_3_ON;
    }
}

/**
 *******************************************************************************
 ** \brief GPIO Port0 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void PORT0_IRQHandler(void)
{
    Gpio_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief GPIO Port1 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void PORT1_IRQHandler(void)
{
    Gpio_IRQHandler(1);
}

/**
 *******************************************************************************
 ** \brief GPIO Port2 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void PORT2_IRQHandler(void)
{
    Gpio_IRQHandler(2);
}

/**
 *******************************************************************************
 ** \brief GPIO Port3 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void PORT3_IRQHandler(void)
{
    Gpio_IRQHandler(3);
}

/**
 *******************************************************************************
 ** \brief GPIO ����0 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void UART0_IRQHandler(void)
{
    Uart_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief GPIO ����1 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void UART1_IRQHandler(void)
{
    Uart_IRQHandler(1);
}
/**
 *******************************************************************************
 ** \brief GPIO SPI �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void SPI_IRQHandler(void)
{
    Spi_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief GPIO I2C �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void I2C_IRQHandler(void)
{
    I2c_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief GPIO ����ʱ��0 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void TIM0_IRQHandler(void)
{
    Tim_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief GPIO ����ʱ��1 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void TIM1_IRQHandler(void)
{
    Tim_IRQHandler(1);
}

/**
 *******************************************************************************
 ** \brief GPIO ����ʱ��2 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void TIM2_IRQHandler(void)
{
    Tim_IRQHandler(2);
}

/**
 *******************************************************************************
 ** \brief GPIO �߼�ʱ��4 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void TIM4_IRQHandler(void)
{
    Adt_IRQHandler(4);
}

/**
 *******************************************************************************
 ** \brief GPIO �߼�ʱ��5 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void TIM5_IRQHandler(void)
{
    Adt_IRQHandler(5);
}

/**
 *******************************************************************************
 ** \brief GPIO �߼�ʱ��6 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void TIM6_IRQHandler(void)
{
    Adt_IRQHandler(6);
}

/**
 *******************************************************************************
 ** \brief GPIO PCA �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void PCA_IRQHandler(void)
{
    Pca_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief GPIO WDT �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void WDT_IRQHandler(void)
{
    Wdt_IRQHandler(0);
}


/**
 *******************************************************************************
 ** \brief GPIO ADC �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void ADC_IRQHandler(void)
{
    Adc_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief GPIO ��ѹ�Ƚ�0 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void VC0_IRQHandler(void)
{
    Vc_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief GPIO ��ѹ�Ƚ�1 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void VC1_IRQHandler(void)
{
    Vc_IRQHandler(1);
}

/**
 *******************************************************************************
 ** \brief GPIO �͵�ѹ��� �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void LVD_IRQHandler(void)
{
    Lvd_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief GPIO RAM �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void EF_RAM_IRQHandler(void)
{
    EfRam_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief GPIO ʱ��У׼ �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void CLKTRIM_IRQHandler(void)
{
    ClkTrim_IRQHandler(0);
}



/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
