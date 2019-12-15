#include "bsp_it.h"
#include "global_variables.h"


void timer2_int(void)
{
    if(M0P_TIM2_MODE0->IFR_f.UIF) {

        systick_counter++;

        M0P_TIM2_MODE0->ICLR_f.UIF = 0;
    }
}

void Gpio_IRQHandler(uint8_t u8Param)
{

}

void RxIntCallback(void)
{
    if(g_uart_idx > UART_BUF_LEN) {
        g_uart_idx = 0;
        return ;
    }

//    g_uart_buff[g_uart_idx++] = Uart_ReceiveData(UARTCH1);
    g_uart_buff[g_uart_idx++] = M0P_UART1->SBUF_f.DATA;
}

