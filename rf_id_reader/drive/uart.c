#include "uart.h"
#include <stdarg.h>
#include <stdio.h>

extern void delay (u16 cnt) ;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

#define UART_DeInit			UART1_DeInit
#define	UART_Init			UART1_Init
#define UART_ITConfig		UART1_ITConfig
#define	UART_Cmd			UART1_Cmd
#define UART_WORDLENGTH_8D	UART1_WORDLENGTH_8D
#define UART_STOPBITS_1		UART1_STOPBITS_1
#define	UART_PARITY_NO		UART1_PARITY_NO
#define UART_SYNCMODE_CLOCK_DISABLE	UART1_SYNCMODE_CLOCK_DISABLE
#define UART_MODE_TXRX_ENABLE	UART1_MODE_TXRX_ENABLE
#define UART_ITConfig	UART1_ITConfig
#define UART_Cmd		UART1_Cmd
#define UART_SendData8	UART1_SendData8
#define	UART_GetFlagStatus UART1_GetFlagStatus
#define UART_FLAG_TXE	UART1_FLAG_TXE
#define UART_FLAG_TC	UART1_FLAG_TC
#define UART_FLAG_RXNE 	UART1_FLAG_RXNE
#define UART_ReceiveData8 UART1_ReceiveData8
#define UART_IT_RXNE	UART1_IT_RXNE

void USART_Configuration(uint32_t baud)
{
    (void)UART1->SR;
    (void)UART1->DR;

    UART1->BRR2 = UART1_BRR2_RESET_VALUE;  /* Set UART1_BRR2 to reset value 0x00 */
    UART1->BRR1 = UART1_BRR1_RESET_VALUE;  /* Set UART1_BRR1 to reset value 0x00 */

    UART1->CR1 = UART1_CR1_RESET_VALUE;  /* Set UART1_CR1 to reset value 0x00 */
    UART1->CR2 = UART1_CR2_RESET_VALUE;  /* Set UART1_CR2 to reset value 0x00 */
    UART1->CR3 = UART1_CR3_RESET_VALUE;  /* Set UART1_CR3 to reset value 0x00 */
    UART1->CR4 = UART1_CR4_RESET_VALUE;  /* Set UART1_CR4 to reset value 0x00 */
    UART1->CR5 = UART1_CR5_RESET_VALUE;  /* Set UART1_CR5 to reset value 0x00 */

    UART1->GTR = UART1_GTR_RESET_VALUE;
    UART1->PSCR = UART1_PSCR_RESET_VALUE;

    uint32_t BaudRate_Mantissa = 0, BaudRate_Mantissa100 = 0;

    UART1->CR1 &= (uint8_t)(~UART1_CR1_M);
    UART1->CR1 |= (uint8_t)UART_WORDLENGTH_8D;
    UART1->CR3 &= (uint8_t)(~UART1_CR3_STOP);
    UART1->CR3 |= (uint8_t)UART_STOPBITS_1;
    UART1->CR1 &= (uint8_t)(~(UART1_CR1_PCEN | UART1_CR1_PS  ));
    UART1->CR1 |= (uint8_t)UART_PARITY_NO;
    UART1->BRR1 &= (uint8_t)(~UART1_BRR1_DIVM);
    UART1->BRR2 &= (uint8_t)(~UART1_BRR2_DIVM);
    UART1->BRR2 &= (uint8_t)(~UART1_BRR2_DIVF);

    BaudRate_Mantissa    = ((uint32_t)CLK_GetClockFreq() / ((u32)baud << 4));
    BaudRate_Mantissa100 = (((uint32_t)CLK_GetClockFreq() * 100) / ((u32)baud << 4));
    UART1->BRR2 |= (uint8_t)((uint8_t)(((BaudRate_Mantissa100 - (BaudRate_Mantissa * 100)) << 4) / 100) & (uint8_t)0x0F);
    UART1->BRR2 |= (uint8_t)((BaudRate_Mantissa >> 4) & (uint8_t)0xF0);
    UART1->BRR1 |= (uint8_t)BaudRate_Mantissa;
    UART1->CR2 &= (uint8_t)~(UART1_CR2_TEN | UART1_CR2_REN);
    UART1->CR3 &= (uint8_t)~(UART1_CR3_CPOL | UART1_CR3_CPHA | UART1_CR3_LBCL);
    UART1->CR3 |= (uint8_t)((uint8_t)UART_SYNCMODE_CLOCK_DISABLE & (uint8_t)(UART1_CR3_CPOL | UART1_CR3_CPHA | UART1_CR3_LBCL));
    UART1->CR2 |= (uint8_t)UART1_CR2_TEN;
    UART1->CR2 |= (uint8_t)UART1_CR2_REN;
    UART1->CR3 &= (uint8_t)(~UART1_CR3_CKEN);

    uint8_t uartreg = 0, itpos = 0x00;
    uartreg = (uint8_t)((uint16_t)UART_IT_RXNE >> 0x08);
    itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)UART_IT_RXNE & (uint8_t)0x0F));
    if (uartreg == 0x01) {
        UART1->CR1 |= itpos;
    } else if (uartreg == 0x02) {
        UART1->CR2 |= itpos;
    } else {
        UART1->CR4 |= itpos;
    }

    UART1->CR1 &= (uint8_t)(~UART1_CR1_UARTD);
    
    CFG->GCR |= CFG_GCR_SWD;

    GPIO_Init(RS485_PORT,RS485_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);
    
    RS485_Recv();
}

void UART_send_byte(uint8_t byte)
{
    RS485_Send();
    UART_SendData8((unsigned char)byte);
    /* Loop until the end of transmission */
    //while (UART_GetFlagStatus(UART_FLAG_TXE) == RESET);
    while (UART_GetFlagStatus(UART_FLAG_TC) == RESET);
    RS485_Recv();
}

void UART_Send(uint8_t *Buffer, uint16_t Length)
{
    uint16_t l=0;
    for(; l<Length; l++) {
        UART_send_byte(Buffer[l]);
    }
}

uint8_t UART_Recive(void)
{
    uint8_t UART_Re_Buf;
    while (UART_GetFlagStatus(UART_FLAG_RXNE) == RESET);
    UART_Re_Buf=UART_ReceiveData8();
    return  UART_Re_Buf;
}

uint8_t uart_print_buff[128];
void uart_printf(char* fmt,...)
{
    int cnt = 0;
    int len = 0;
 
    va_list ap;
    va_start(ap,fmt);
    vsprintf((char*)uart_print_buff, fmt, ap);
    va_end(ap);
 
    len = strlen((const char*)uart_print_buff);

    while(len--) {
        RS485_Send();
        UART1->DR = (uart_print_buff[cnt++]);
        while (UART_GetFlagStatus(UART_FLAG_TC) == RESET);
        RS485_Recv();
    }
}


void rs485_send_buf(uint8_t *buf, uint8_t len)
{
    RS485_Send();
    for(uint8_t i=0; i<len; i++) {
        UART1->DR = buf[i];
        while (UART_GetFlagStatus(UART_FLAG_TC) == RESET);
    }
    RS485_Recv();
}