#ifndef _BSP_UART_H
#define _BSP_UART_H
#include "bsp_it.h"
/*****************************************************************************************************/
#define RS485_PORT              GPIOD
#define RS485_PIN               GPIO_PIN_1                         //RS485芯片控制引脚。

#define	RS485_Send()	        Gpio_SetIO(GpioPortA, GpioPin1);  //发送模式：芯片控制引脚写高。
#define	RS485_Recv()	        Gpio_ClrIO(GpioPortA, GpioPin1);  //接收模式：芯片控制引脚写低。

/**********************************************函数声明***********************************************/
void RS485_Init(void);
void Uart_PortInit(void);
void ErrIntCallback(void);
void PErrIntCallBack(void);
void CtsIntCallBack(void);
void TxIntCallback(void);
void UART_Multi_byte(uint8_t *Buffer,uint8_t Length);
#endif
