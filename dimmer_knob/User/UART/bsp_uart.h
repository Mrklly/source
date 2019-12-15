#ifndef _BSP_UART_H
#define _BSP_UART_H
#include "bsp_it.h"
/*****************************************************************************************************/
#define RS485_PORT              GPIOD
#define RS485_PIN               GPIO_PIN_1                         //RS485оƬ�������š�

#define	RS485_Send()	        Gpio_SetIO(GpioPortA, GpioPin1);  //����ģʽ��оƬ��������д�ߡ�
#define	RS485_Recv()	        Gpio_ClrIO(GpioPortA, GpioPin1);  //����ģʽ��оƬ��������д�͡�

/**********************************************��������***********************************************/
void RS485_Init(void);
void Uart_PortInit(void);
void ErrIntCallback(void);
void PErrIntCallBack(void);
void CtsIntCallBack(void);
void TxIntCallback(void);
void UART_Multi_byte(uint8_t *Buffer,uint8_t Length);
#endif
