#ifndef _BSP_UART_H_
#define _BSP_UART_H_


#include "bsp_it.h"


#define RS485_PORT              GpioPortA
#define RS485_PIN               GpioPin1                         //RS485芯片控制引脚。

#define	RS485_Send()	        Gpio_SetIO(RS485_PORT, RS485_PIN);  //发送模式：芯片控制引脚写高。
#define	RS485_Recv()	        Gpio_ClrIO(RS485_PORT, RS485_PIN);  //接收模式：芯片控制引脚写低。


extern uint8_t uart_flag;
extern uint8_t ReceiveFlag;


void RS485_Init(void);
void Uart_PortInit(void);
void ErrIntCallback(void);
void PErrIntCallBack(void);
void CtsIntCallBack(void);
void TxIntCallback(void);
void UART_Multi_byte(uint8_t *Buffer,uint8_t Length);
void uart_send_buf(uint8_t *buf, uint8_t len);


#endif
