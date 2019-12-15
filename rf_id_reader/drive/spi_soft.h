#ifndef _SPI_SOFT_H_
#define _SPI_SOFT_H_

#include "stm8s.h"

#define RF433_CSB_PORT		GPIOC
#define RF433_CSB_PIN       GPIO_PIN_6

#define RF433_FCSB_PORT		GPIOC
#define RF433_FCSB_PIN      GPIO_PIN_4

#define RF433_SCLK_PORT		GPIOC
#define RF433_SCLK_PIN      GPIO_PIN_5

#define RF433_SDIO_PORT		GPIOC
#define RF433_SDIO_PIN      GPIO_PIN_7

#define RF433_INT_PORT		GPIOA
#define RF433_INT_PIN       GPIO_PIN_2
#define RF433_INT_IN_DATA   (RF433_INT_PORT->IDR & RF433_INT_PIN)

#define RF433_CSB_H     RF433_CSB_PORT->ODR |=   RF433_CSB_PIN
#define RF433_CSB_L     RF433_CSB_PORT->ODR &=  ~RF433_CSB_PIN

#define RF433_FCSB_H    RF433_FCSB_PORT->ODR |=   RF433_FCSB_PIN
#define RF433_FCSB_L    RF433_FCSB_PORT->ODR &=  ~RF433_FCSB_PIN

#define RF433_SCLK_H    RF433_SCLK_PORT->ODR |=   RF433_SCLK_PIN
#define RF433_SCLK_L    RF433_SCLK_PORT->ODR &=  ~RF433_SCLK_PIN

#define RF433_SDIO_H    RF433_SDIO_PORT->ODR |=   RF433_SDIO_PIN
#define RF433_SDIO_L    RF433_SDIO_PORT->ODR &=  ~RF433_SDIO_PIN

#define RF433_SDIO_IN   RF433_SDIO_PORT->DDR &= ~RF433_SDIO_PIN;RF433_SDIO_PORT->CR1 |=  RF433_SDIO_PIN
#define RF433_SDIO_OUT  RF433_SDIO_PORT->DDR |=  RF433_SDIO_PIN;RF433_SDIO_PORT->ODR &= ~RF433_SDIO_PIN
#define RF433_SDIO_IN_DATA      (RF433_SDIO_PORT->IDR & RF433_SDIO_PIN)

void rf433_io_init(void);
void Reg_WriteByte(uint8_t addre, uint8_t data);
uint8_t Reg_ReadByte(uint8_t addre);
void FIFO_WriteByte(uint8_t data);
uint8_t FIFO_ReadByte(void);
void FIFO_Write_x(uint8_t *data, uint8_t length);
void FIFO_Read_x(uint8_t *data, uint8_t length);

#endif
