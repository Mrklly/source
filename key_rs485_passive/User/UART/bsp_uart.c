#include "bsp_uart.h"


void RS485_Init(void)
{
    GPIO_Init(RS485_MODE_PORT, RS485_MODE_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
    RS485_Send();

//    GPIO_Init(RS485_TX_PROT,   RS485_TX_PIN,   RS485_TX_Mode);
//    GPIO_Init(RS485_RX_PROT,   RS485_RX_PIN,   RS485_RX_Mode);

//    UART1_DeInit();
//    UART1_Init(   (uint32_t)9600,
//        UART1_WORDLENGTH_8D,
//        UART1_STOPBITS_1,
//        UART1_PARITY_NO,
//        UART1_SYNCMODE_CLOCK_DISABLE,
//        UART1_MODE_TXRX_ENABLE);
//    UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);
//    UART1_Cmd(ENABLE);

//    RS485_Recv();

    UART1->CR2 = 0;         // tx rx disable
    UART1->CR3 = 0;         // 1 stop bit
    UART1->CR1 = 0;         // 1 start bit, 8 data bits
    UART1->BRR2 = 0x02;     // baudrate = 9600
    UART1->BRR1 = 0x68;     // 16M/9600 = 0x682
    UART1->CR2 = 0x2c;

    RS485_Recv();
}

void RS485_send_Byte(uint8_t Data)
{
    while((UART1->SR & UART1_FLAG_TXE) == 0x00);    //等待发送寄存器为空。
    UART1->DR = Data;
    while((UART1->SR & UART1_FLAG_TC) == 0x00);     //等待发送完成。
}

void uart_send_buf(uint8_t *buf, uint8_t len)
{
    uint16_t i;
    asm("sim");
    RS485_Send();

    for(i=0; i<len; i++) {
        while((UART1->SR & UART1_FLAG_TXE) == 0x00);
        UART1->DR = buf[i];
        while((UART1->SR & UART1_FLAG_TC) == 0x00);
    }

    RS485_Recv();
    asm("rim");
}


