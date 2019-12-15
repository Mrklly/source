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
/** \file interrupts_hc32l136.c
 **
 ** Interrupt management
 ** @link Driver Group Some description @endlink
 **
 **   - 2018-04-15  1.0  Lux     First version.
 **
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"
#include "hc32_it.h"
#include "hc32_gpio.h"

__WEAKDEF void Gpio_IRQHandler(uint8_t u8Param);
__WEAKDEF void Dma_IRQHandler(uint8_t u8Param);
__WEAKDEF void Uart_IRQHandler(uint8_t u8Param);
__WEAKDEF void LpUart_IRQHandler(uint8_t u8Param);
__WEAKDEF void Spi_IRQHandler(uint8_t u8Param);
__WEAKDEF void I2c_IRQHandler(uint8_t u8Param);
__WEAKDEF void Tim_IRQHandler(uint8_t u8Param);
__WEAKDEF void Tim3_IRQHandler(uint8_t u8Param);
__WEAKDEF void Adt_IRQHandler(uint8_t u8Param);
__WEAKDEF void LpTim_IRQHandler(uint8_t u8Param);
__WEAKDEF void Pca_IRQHandler(uint8_t u8Param);
__WEAKDEF void Wdt_IRQHandler(uint8_t u8Param);
__WEAKDEF void Vc_IRQHandler(uint8_t u8Param);
__WEAKDEF void Rtc_IRQHandler(uint8_t u8Param);
__WEAKDEF void Adc_IRQHandler(uint8_t u8Param);
__WEAKDEF void Pcnt_IRQHandler(uint8_t u8Param);
__WEAKDEF void Lvd_IRQHandler(uint8_t u8Param);
__WEAKDEF void Lcd_IRQHandler(uint8_t u8Param);
__WEAKDEF void EfRam_IRQHandler(uint8_t u8Param);
__WEAKDEF void ClkTrim_IRQHandler(uint8_t u8Param);

/**
 *******************************************************************************
 ** \brief NVIC �ж�ʹ��
 **
 ** \param [in]  enIrq          �жϺ�ö������
 ** \param [in]  enLevel        �ж����ȼ�ö������
 ** \param [in]  bEn            �жϿ���
 ** \retval    Ok       ���óɹ�
 **            ����ֵ   ����ʧ��
 ******************************************************************************/
void EnableNvic(IRQn_Type enIrq, en_irq_level_t enLevel, boolean_t bEn)
{
    NVIC_ClearPendingIRQ(enIrq);
    NVIC_SetPriority(enIrq, enLevel);
    if (TRUE == bEn)
    {
        NVIC_EnableIRQ(enIrq);
    }
    else
    {
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
        ;
    }
}

/**
 *******************************************************************************
 ** \brief GPIO PortA �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void PORTA_IRQHandler(void)
{
    Gpio_IRQHandler(GpioPortA);
}

/**
 *******************************************************************************
 ** \brief GPIO PortB �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void PORTB_IRQHandler(void)
{
    Gpio_IRQHandler(GpioPortB);
}

/**
 *******************************************************************************
 ** \brief GPIO PortC �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void PORTC_IRQHandler(void)
{
    Gpio_IRQHandler(GpioPortC);
}

/**
 *******************************************************************************
 ** \brief GPIO PortD �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void PORTD_IRQHandler(void)
{
    Gpio_IRQHandler(GpioPortD);
}

/**
 *******************************************************************************
 ** \brief DMAC  �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void DMAC_IRQHandler(void)
{
    Dma_IRQHandler(0);
}


/**
 *******************************************************************************
 ** \brief UART0 ����0 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void UART0_IRQHandler(void)
{
    Uart_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief UART1 ����1 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void UART1_IRQHandler(void)
{
    Uart_IRQHandler(1);
}

/**
 *******************************************************************************
 ** \brief LPUART0 �͹��Ĵ���0 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void LPUART0_IRQHandler(void)
{
    LpUart_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief LPUART1 �͹��Ĵ���1 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void LPUART1_IRQHandler(void)
{
    LpUart_IRQHandler(1);
}

/**
 *******************************************************************************
 ** \brief SPI0 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void SPI0_IRQHandler(void)
{
    Spi_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief SPI1 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void SPI1_IRQHandler(void)
{
    Spi_IRQHandler(1);
}

/**
 *******************************************************************************
 ** \brief I2C0 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void I2C0_IRQHandler(void)
{
    I2c_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief I2C1 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void I2C1_IRQHandler(void)
{
    I2c_IRQHandler(1);
}

/**
 *******************************************************************************
 ** \brief TIM0 ����ʱ��0 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void TIM0_IRQHandler(void)
{
    Tim_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief TIM1 ����ʱ��1 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void TIM1_IRQHandler(void)
{
    Tim_IRQHandler(1);
}

/**
 *******************************************************************************
 ** \brief TIM2 ����ʱ��2 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void TIM2_IRQHandler(void)
{
    Tim_IRQHandler(2);
}

/**
 *******************************************************************************
 ** \brief TIM3 ����ʱ��3 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void TIM3_IRQHandler(void)
{
    Tim3_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief LPTIM �͹���ʱ�� �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void LPTIM_IRQHandler(void)
{
    LpTim_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief TIM4 �߼�ʱ��4 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void TIM4_IRQHandler(void)
{
    Adt_IRQHandler(4);
}

/**
 *******************************************************************************
 ** \brief TIM5 �߼�ʱ��5 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void TIM5_IRQHandler(void)
{
    Adt_IRQHandler(5);
}

/**
 *******************************************************************************
 ** \brief TIM6 �߼�ʱ��6 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void TIM6_IRQHandler(void)
{
    Adt_IRQHandler(6);
}

/**
 *******************************************************************************
 ** \brief PCA �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void PCA_IRQHandler(void)
{
    Pca_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief WDT �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void WDT_IRQHandler(void)
{
    Wdt_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief RTC �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void RTC_IRQHandler(void)
{
    Rtc_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief ADC �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void ADC_IRQHandler(void)
{
    Adc_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief PCNT �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void PCNT_IRQHandler(void)
{
    Pcnt_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief ��ѹ�Ƚ�0 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void VC0_IRQHandler(void)
{
    Vc_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief ��ѹ�Ƚ�1 �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void VC1_IRQHandler(void)
{
    Vc_IRQHandler(1);
}

/**
 *******************************************************************************
 ** \brief �͵�ѹ��� �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void LVD_IRQHandler(void)
{
    Lvd_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief LCD �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void LCD_IRQHandler(void)
{
    Lcd_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief RAM �жϴ�����
 ** 
 ** \retval
 ******************************************************************************/
void EF_RAM_IRQHandler(void)
{
    EfRam_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief ʱ��У׼ �жϴ�����
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
