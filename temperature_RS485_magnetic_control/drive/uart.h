#ifndef __UART_H
#define	__UART_H

#include "stm8s.h"
#include "stm8s_clk.h"
#include <stdio.h>
#include <string.h>

#include "global_variables.h"


#define RS485_PIN        GPIO_PIN_7
#define RS485_PORT       GPIOC


#define		RS485_Send()		RS485_PORT->ODR |= (uint8_t)RS485_PIN;
#define		RS485_Recv()		RS485_PORT->ODR &= (uint8_t)(~RS485_PIN);

void USART_Configuration(device_baudrate baud);
void UART_send_byte(uint8_t byte);
void UART_Send(uint8_t *Buffer, uint16_t Length);
uint8_t UART_Recive(void);
int fputc(int ch, FILE *f);
void s_printf(const char *str);

#endif /* __UART_H */
