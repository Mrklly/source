#ifndef _BSP_UART_H_
#define _BSP_UART_H_


#include "bsp_it.h"


#define RS485_PORT              GpioPortA
#define RS485_PIN               GpioPin1

#define	RS485_Send()	        GPIO_HIGH(RS485_PORT, RS485_PIN)
#define	RS485_Recv()	        GPIO_LOW(RS485_PORT, RS485_PIN)


void RS485_Init(void);
void Uart_PortInit(void);
void ErrIntCallback(void);
void PErrIntCallBack(void);
void CtsIntCallBack(void);
void TxIntCallback(void);
void uart_send_buf(uint8_t *buf, uint8_t len);


#endif

