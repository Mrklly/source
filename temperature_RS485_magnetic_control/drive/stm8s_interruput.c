
#include "stm8s.h"
#include "stm8s_spi.h"
#include "stm8s_gpio.h"
#include "stm8s_clk.h"
#include "stm8s_exti.h"
#include "stm8s_tim1.h"
#include "led.h"
#include "uart.h"
#include "key.h"
#include "global_variables.h"


u16	Conversion_Value;
#pragma vector=1
__interrupt void TRAP_IRQHandler(void)
{

}
#pragma vector=2
__interrupt void TLI_IRQHandler(void)
{

}
#pragma vector=3
__interrupt void AWU_IRQHandler(void)
{

}
#pragma vector=4
__interrupt void CLK_IRQHandler(void)
{


}
#pragma vector=5
__interrupt void EXTI_PORTA_IRQHandler(void)
{

}
#pragma vector=6
__interrupt void EXTI_PORTB_IRQHandler(void)
{

}
#pragma vector=7
__interrupt void EXTI_PORTC_IRQHandler(void)
{

}
#pragma vector=8
__interrupt void EXTI_PORTD_IRQHandler(void)
{


}
#pragma vector=9
__interrupt void EXTI_PORTE_IRQHandler(void)
{

}
#ifdef STM8S903
#pragma vector=0xA
__interrupt void EXTI_PORTF_IRQHandler(void)
{

}
#endif
#ifdef STM8S208
#pragma vector=0xA
__interrupt void CAN_RX_IRQHandler(void)
{

}
#pragma vector=0xB
__interrupt void CAN_TX_IRQHandler(void)
{

}
#endif
#pragma vector=0xC
__interrupt void SPI_IRQHandler(void)
{

}
#pragma vector=0xD
__interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void)
{
    TIM1->SR1 = 0;
 
    systick_counter++;

//    GPIOC->ODR ^= GPIO_PIN_5;
}
#pragma vector=0xE
__interrupt void TIM1_CAP_COM_IRQHandler(void)
{
}
#ifdef STM8S903
#pragma vector=0xF
__interrupt void TIM5_UPD_OVF_BRK_TRG_IRQHandler(void)
{

}
#pragma vector=0x10
__interrupt void TIM5_CAP_COM_IRQHandler(void)
{

}
#else
#pragma vector=0xF
__interrupt void TIM2_UPD_OVF_BRK_IRQHandler(void)
{

    //TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
}
#pragma vector=0x10
__interrupt void TIM2_CAP_COM_IRQHandler(void)
{

}
#endif
#if defined (STM8S208) || defined(STM8S207) || defined(STM8S105)
#pragma vector=0x11
__interrupt void TIM3_UPD_OVF_BRK_IRQHandler(void)
{

}
#pragma vector=0x12
__interrupt void TIM3_CAP_COM_IRQHandler(void)
{

}
#endif
//#ifndef STM8S105
#if defined (STM8S208) || defined(STM8S207) || defined(STM8S105) || defined(STM8S003)
#pragma vector=0x13
__interrupt void UART1_TX_IRQHandler(void)
{


}
#pragma vector=0x14
__interrupt void UART1_RX_IRQHandler(void)
{
    if(UART1_GetITStatus(UART1_IT_RXNE) != RESET) {
        if(g_idx >= (RX_BUFF_LEN - 1)) {
            g_idx = 0;
            return ;
        }
        g_rx_buff[g_idx++] = (uint8_t)UART1->DR;
    }
}
#endif
#pragma vector=0x15
__interrupt void I2C_IRQHandler(void)
{

}
#ifdef STM8S105
#pragma vector=0x16
__interrupt void UART2_TX_IRQHandler(void)
{

}
#pragma vector=0x17
__interrupt void UART2_RX_IRQHandler(void)
{

#ifdef	STM8S105
    if(UART2_GetITStatus(UART2_IT_RXNE) != RESET) {
    }
#endif
    return;
}
#endif
#if defined(STM8S207) || defined(STM8S208)
#pragma vector=0x16
__interrupt void UART3_TX_IRQHandler(void)
{

}
#pragma vector=0x17
__interrupt void UART3_RX_IRQHandler(void)
{
#if 0
    if(UART3_GetITStatus(UART3_IT_RXNE) != RESET) {
        g_RX_buff[g_r_idx] = UART_Recive();
        g_r_idx++;
        if(g_r_idx >= COM_RX_Lenth) {
            g_r_idx = 0;
        }
    }
#endif
    return;
}
#endif
#if defined(STM8S207) || defined(STM8S208)
#pragma vector=0x18
__interrupt void ADC2_IRQHandler(void)
{

}
#else
#pragma vector=0x18
__interrupt void ADC1_IRQHandler(void)
{
    Conversion_Value = ADC1_GetConversionValue();
    ADC1_ClearITPendingBit(ADC1_IT_AWS2);
}
#endif
#ifdef STM8S903
#pragma vector=0x19
__interrupt void TIM6_UPD_OVF_TRG_IRQHandler(void)
{

}
#else
#pragma vector=0x19
__interrupt void TIM4_UPD_OVF_IRQHandler(void)
{

}
#endif
#pragma vector=0x1A
__interrupt void EEPROM_EEC_IRQHandler(void)
{

}

