#ifndef _UART_H_
#define _UART_H_

//#include "hc32_uart.h"
#include "hc32_lpuart.h"
#include "hc32_gpio.h"
#include "hc32_sysctrl.h"

#define RS485_EN_PORT       GpioPortB
#define RS485_EN_PIN        GpioPin9

#define RS485_SEND    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + RS485_EN_PORT))) |=   1UL << RS485_EN_PIN
#define RS485_RECV    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + RS485_EN_PORT))) &= ~(1UL << RS485_EN_PIN)


extern uint8_t u8TxData[2];
extern uint8_t u8RxData;
extern uint8_t u8TxCnt;
extern uint8_t u8RxCnt;

void uart_init_gpio(void);
void uart_init(void);
void TxIntCallback(void);
void RxIntCallback(void);
void ErrIntCallback(void);
void PErrIntCallBack(void);
void CtsIntCallBack(void);

void rs485_control_pin_init(void);
void UART_Send(uint8_t *Buffer, uint16_t Length);

#endif
