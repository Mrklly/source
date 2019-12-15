#ifndef _BSP_UART_H_
#define _BSP_UART_H_


#include "stm8s.h"
#include "bsp_gpio.h"


#define RS485_PORT              GPIOC
#define RS485_PIN               GPIO_PIN_6                         //RS485芯片控制引脚。

#define RS485_RX_GPIO_PROT           GPIOD
#define RS485_RX_GPIO_PIN            GPIO_PIN_6
#define RS485_RX_GPIO_Mode           GPIO_MODE_IN_PU_NO_IT

#define RS485_TX_GPIO_PROT           GPIOD
#define RS485_TX_GPIO_PIN            GPIO_PIN_5
#define RS485_TX_GPIO_Mode           GPIO_MODE_OUT_PP_LOW_FAST


#define	RS485_Send()	        GPIO_WriteHigh(GPIOC,GPIO_PIN_6);  //发送模式：芯片控制引脚写高。
#define	RS485_Recv()	        GPIO_WriteLow(GPIOC,GPIO_PIN_6);   //接收模式：芯片控制引脚写低。

#define UART_RX_PIN             GPIO_PIN_6  
#define UART_TX_PIN             GPIO_PIN_5
#define UART_PORT               GPIOD
#define RX_BUFF_LEN	        128            //接收缓存大小100个字。


extern uint8_t ReceiveFlag;


void RS485_Init(void);
void RS485_send_Byte(uint8_t Data);
void uart_send_buf(uint8_t *buf, uint8_t len);


#endif

