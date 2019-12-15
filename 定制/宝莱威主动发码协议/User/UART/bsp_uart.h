#ifndef _BSP_UART_H
#define _BSP_UART_H


#include "stm8s.h"
#include "bsp_gpio.h"

#define RS485_MODE_PORT              GPIOC
#define RS485_MODE_PIN               GPIO_PIN_6

#define RS485_RX_PROT           GPIOD
#define RS485_RX_PIN            GPIO_PIN_6
#define RS485_RX_Mode           GPIO_MODE_IN_PU_NO_IT

#define RS485_TX_PROT           GPIOD
#define RS485_TX_PIN            GPIO_PIN_5
#define RS485_TX_Mode           GPIO_MODE_OUT_PP_LOW_SLOW


#define	RS485_Send()	        RS485_MODE_PORT->ODR |=  RS485_MODE_PIN
#define	RS485_Recv()	        RS485_MODE_PORT->ODR &= ~RS485_MODE_PIN


void RS485_Init(void);
void RS485_send_Byte(uint8_t Data);
void uart_send_buf(uint8_t *buf, uint8_t len);


#endif

