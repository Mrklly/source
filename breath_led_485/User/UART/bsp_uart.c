#include "bsp_uart.h"


uint8_t ReceiveFlag = 0;


void RS485_Init(void)
{
  GPIO_Init(RS485_PORT,RS485_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );          //���ý��շ���ģʽ��������Ϊ���ģʽ��
  UART1_Init(   (u32)19200,                     //���ڲ����ʡ�
                UART1_WORDLENGTH_8D,            //�����ֳ�8λ       
                UART1_STOPBITS_1,               //ֹͣλ1λ��
                UART1_PARITY_NO,                //��У��λ��
                UART1_SYNCMODE_CLOCK_DISABLE,   //�ر�ͬ��ģʽʱ�ӡ�
                UART1_MODE_TXRX_ENABLE);        //ʹ�ܴ��ڽ����뷢�䡣
  UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);      //ʹ�ܴ��ڽ����жϡ�
  UART1_Cmd(ENABLE);                            //ʹ�ܴ���1�� 
  RS485_Recv();                                 //��RS485�ص�����ģʽ��
}
/*****************************************************************************************************
*��������RS485_send_Byte
***���ܣ�ͨ��RS485����һ���ֽڡ�
***�������޲�����
*����ֵ���޷���ֵ��
***���ߣ�������
***ʱ�䣺2018��4��26�š�
*****************************************************************************************************/
void RS485_send_Byte(uint8_t Data)
{
  while(UART1_GetFlagStatus(UART1_FLAG_TXE)==RESET);    //�ȴ����ͼĴ���Ϊ�ա�
  UART1_SendData8((char)Data);                          //���ÿ⺯������һ���ֽڡ�
  while(UART1_GetFlagStatus(UART1_FLAG_TC)==RESET);     //�ȴ�������ɡ�
 
}

void uart_send_buf(uint8_t *buf, uint8_t len)
{
    RS485_Send();
    for(uint8_t i=0; i<len; i++) {
        while((UART1->SR & (uint8_t)UART1_FLAG_TXE) == RESET);
        UART1->DR = buf[i];
        while((UART1->SR & (uint8_t)UART1_FLAG_TC)  == RESET);
    }
    RS485_Recv();
}

