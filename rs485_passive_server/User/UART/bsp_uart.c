#include "bsp_uart.h"

/*****************************************************************************************************
*程序名：RS485_Init
***功能：配置串口工作在9600波特率，8位字长，一个停止位，非同步模式。
***参数：无参数。
*返回值：无返回值。
***作者：王利均
***时间：2018年4月26号。
*****************************************************************************************************/
void RS485_Init(void)
{
  GPIO_Init(RS485_PORT,RS485_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );          //配置接收发射模式控制引脚为输出模式。
  UART1_Init(   (u32)9600,                      //串口波特率。
                UART1_WORDLENGTH_8D,            //数据字长8位       
                UART1_STOPBITS_1,               //停止位1位。
                UART1_PARITY_NO,                //无校验位。
                UART1_SYNCMODE_CLOCK_DISABLE,   //关闭同步模式时钟。
                UART1_MODE_TXRX_ENABLE);        //使能串口接收与发射。
  UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);      //使能串口接收中断。
  UART1_Cmd(ENABLE);                            //使能串口1。 
  RS485_Recv();                                 //让RS485回到接收模式。
}
/*****************************************************************************************************
*程序名：RS485_send_Byte
***功能：通过RS485发送一个字节。
***参数：无参数。
*返回值：无返回值。
***作者：王利均
***时间：2018年4月26号。
*****************************************************************************************************/
void RS485_send_Byte(uint8_t Data)
{
  while(UART1_GetFlagStatus(UART1_FLAG_TXE)==RESET);    //等待发送寄存器为空。
  UART1_SendData8((char)Data);                          //调用库函数发送一个字节。
  while(UART1_GetFlagStatus(UART1_FLAG_TC)==RESET);     //等待发送完成。
 
}


void uart_send_buf(uint8_t *buf, uint8_t len)
{
    RS485_Send();
    for(uint8_t i=0; i<len; i++) {
        while(!(UART1->SR & 0x80));
        UART1->DR = buf[i];
        while(!(UART1->SR & 0x40));
    }
    RS485_Recv();
}

