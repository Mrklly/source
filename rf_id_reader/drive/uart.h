#ifndef __UART_H
#define	__UART_H

#include "stm8s.h"
#include "stm8s_clk.h"
#include <stdio.h>
#include <string.h>

#include "global_variables.h"


#define RS485_PIN        GPIO_PIN_1
#define RS485_PORT       GPIOD


#define		RS485_Send()		RS485_PORT->ODR |= (uint8_t)RS485_PIN;
#define		RS485_Recv()		RS485_PORT->ODR &= (uint8_t)(~RS485_PIN);

void USART_Configuration(uint32_t baud);
void UART_send_byte(uint8_t byte);
void UART_Send(uint8_t *Buffer, uint16_t Length);
uint8_t UART_Recive(void);
void s_printf(const char *str);
void uart_printf(char* fmt,...);
void rs485_send_buf(uint8_t *buf, uint8_t len);

#endif /* __UART_H */
