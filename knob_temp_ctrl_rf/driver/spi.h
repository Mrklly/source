#ifndef _SPI_H_
#define _SPI_H_

#include "hc32_gpio.h"
#include "hc32_spi.h"

#define SPI1_CSB_PORT     GpioPortA
#define SPI1_CSB_PIN      GpioPin0

#define SPI1_FCSB_PORT    GpioPortB
#define SPI1_FCSB_PIN     GpioPin12

#define SPI1_MOSI_PORT    GpioPortB
#define SPI1_MOSI_PIN     GpioPin15

#define SPI1_SCLK_PORT    GpioPortB
#define SPI1_SCLK_PIN     GpioPin13

#define SPI1_MISO_PORT    GpioPortB
#define SPI1_MISO_PIN     GpioPin14

#define SPI1_INT_PORT     GpioPortA
#define SPI1_INT_PIN      GpioPin1

#define SPI1_MOSI_IN()    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PADIR + SPI1_MOSI_PORT))) |=   1 << SPI1_MOSI_PIN
#define SPI1_MOSI_OUT()   *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PADIR + SPI1_MOSI_PORT))) &= ~(1 << SPI1_MOSI_PIN)
#define SPI1_MOSI_IO()    *((uint32_t*)(((uint32_t)(&(M0P_GPIO->PA00_SEL)) + SPI1_MOSI_PORT) + (((uint32_t)SPI1_MOSI_PIN)<<2))) = GpioAf0;
#define SPI1_MOSI_SPI()   *((uint32_t*)(((uint32_t)(&(M0P_GPIO->PA00_SEL)) + SPI1_MOSI_PORT) + (((uint32_t)SPI1_MOSI_PIN)<<2))) = GpioAf1;
#define SPI1_MOSI_H    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + SPI_SOFT_SDIO_PORT))) |=   1 << SPI_SOFT_SDIO_PIN
#define SPI1_MOSI_L    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + SPI_SOFT_SDIO_PORT))) &= ~(1 << SPI_SOFT_SDIO_PIN)


#define SPI1_MISO_IN()    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PADIR + SPI1_MISO_PORT))) |=   1 << SPI1_MISO_PIN
#define SPI1_MISO_OUT()   *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PADIR + SPI1_MISO_PORT))) &= ~(1 << SPI1_MISO_PIN)
#define SPI1_MISO_IO()    *((uint32_t*)(((uint32_t)(&(M0P_GPIO->PA00_SEL)) + SPI1_MISO_PORT) + (((uint32_t)SPI1_MISO_PIN)<<2))) = GpioAf0;
#define SPI1_MISO_SPI()   *((uint32_t*)(((uint32_t)(&(M0P_GPIO->PA00_SEL)) + SPI1_MISO_PORT) + (((uint32_t)SPI1_MISO_PIN)<<2))) = GpioAf1;


void spi_io_init(void);
void spi_function_init(void);
void spi_init(void);
void Spi1IntCallBack(void);

void spi_write_byte(uint8_t data);
uint8_t spi_read_byte(void);
uint8_t spi_rw_data(uint8_t data);
void spi_write_buff(uint8_t *buff, uint8_t length);
void spi_read_buff(uint8_t *buff, uint8_t length);


#endif

