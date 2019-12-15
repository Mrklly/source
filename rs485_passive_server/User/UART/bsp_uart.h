#ifndef _BSP_UART_H
#define _BSP_UART_H
#include "stm8s.h"
#include "bsp_gpio.h"
/*****************************************************************************************************/
#define RS485_PORT              GPIOC
#define RS485_PIN               GPIO_PIN_6                         //RS485芯片控制引脚。

#define RS485_RX_GPIO_PROT           GPIOD
#define RS485_RX_GPIO_PIN            GPIO_PIN_6
#define RS485_RX_GPIO_Mode           GPIO_MODE_IN_PU_NO_IT

#define RS485_TX_GPIO_PROT           GPIOD
#define RS485_TX_GPIO_PIN            GPIO_PIN_5
#define RS485_TX_GPIO_Mode           GPIO_MODE_OUT_PP_LOW_FAST


#define	RS485_Send()	        RS485_PORT->ODR |=  RS485_PIN
#define	RS485_Recv()	        RS485_PORT->ODR &= ~RS485_PIN

#define UART_RX_PIN             GPIO_PIN_6  
#define UART_TX_PIN             GPIO_PIN_5
#define UART_PORT               GPIOD


/**********************************************函数声明***********************************************/
void RS485_Init(void);
void RS485_send_Byte(uint8_t Data);
void uart_send_buf(uint8_t *buf, uint8_t len);

#endif
