#ifndef _DRV_SPI_SOFT_H_
#define _DRV_SPI_SOFT_H_

#include "base_types.h"
#include "hc32l13x.h"
#include "system_hc32l13x.h"
#include "hc32_gpio.h"


#define SPI_SOFT_CSB_PORT       GpioPortA
#define SPI_SOFT_CSB_PIN        GpioPin8

#define SPI_SOFT_FCSB_PORT      GpioPortB
#define SPI_SOFT_FCSB_PIN       GpioPin12

#define SPI_SOFT_SDIO_PORT      GpioPortB
#define SPI_SOFT_SDIO_PIN       GpioPin15

#define SPI_SOFT_MISO_PORT      GpioPortB
#define SPI_SOFT_MISO_PIN       GpioPin14

#define SPI_SOFT_SCLK_PORT      GpioPortB
#define SPI_SOFT_SCLK_PIN       GpioPin13

#define SPI_SOFT_INT_PORT       GpioPortA
#define SPI_SOFT_INT_PIN        GpioPin9

#define SPI1_INT_PORT           GpioPortA
#define SPI1_INT_PIN            GpioPin9

//#define SPI_SOFT_CSB_H     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + SPI_SOFT_CSB_PORT))) |=   1 << SPI_SOFT_CSB_PIN
//#define SPI_SOFT_CSB_L     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + SPI_SOFT_CSB_PORT))) &= ~(1 << SPI_SOFT_CSB_PIN)

//#define SPI_SOFT_FCSB_H    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + SPI_SOFT_FCSB_PORT))) |=   1 << SPI_SOFT_FCSB_PIN
//#define SPI_SOFT_FCSB_L    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + SPI_SOFT_FCSB_PORT))) &= ~(1 << SPI_SOFT_FCSB_PIN)

//#define SPI_SOFT_SDIO_H    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + SPI_SOFT_SDIO_PORT))) |=   1 << SPI_SOFT_SDIO_PIN
//#define SPI_SOFT_SDIO_L    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + SPI_SOFT_SDIO_PORT))) &= ~(1 << SPI_SOFT_SDIO_PIN)

//#define SPI_SOFT_SCLK_H    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + SPI_SOFT_SCLK_PORT))) |=   1 << SPI_SOFT_SCLK_PIN
//#define SPI_SOFT_SCLK_L    *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PAOUT + SPI_SOFT_SCLK_PORT))) &= ~(1 << SPI_SOFT_SCLK_PIN)

//#define SPI_SOFT_SDIO_IN()      *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PADIR + SPI_SOFT_SDIO_PORT))) |=   1 << SPI_SOFT_SDIO_PIN
//#define SPI_SOFT_SDIO_OUT()     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PADIR + SPI_SOFT_SDIO_PORT))) &= ~(1 << SPI_SOFT_SDIO_PIN)

//#define SPI_SOFT_MISO_IN()      *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PADIR + SPI_SOFT_MISO_PORT))) |=   1 << SPI_SOFT_MISO_PIN
//#define SPI_SOFT_MISO_OUT()     *((volatile uint32_t *)(((uint32_t)&M0P_GPIO->PADIR + SPI_SOFT_MISO_PORT))) &= ~(1 << SPI_SOFT_MISO_PIN)

//#define SPI_SOFT_SDIO_IN_DATA  Gpio_GetInputIO(SPI_SOFT_SDIO_PORT, SPI_SOFT_SDIO_PIN)
//#define SPI_SOFT_INT_IN_DATA   Gpio_GetInputIO(SPI_SOFT_INT_PORT, SPI_SOFT_INT_PIN)


#define SPI_SOFT_CSB_H     M0P_GPIO->PAOUT_f.PA08 = 1
#define SPI_SOFT_CSB_L     M0P_GPIO->PAOUT_f.PA08 = 0

#define SPI_SOFT_FCSB_H    M0P_GPIO->PBOUT_f.PB12 = 1
#define SPI_SOFT_FCSB_L    M0P_GPIO->PBOUT_f.PB12 = 0

#define SPI_SOFT_SDIO_H    M0P_GPIO->PBOUT_f.PB15 = 1
#define SPI_SOFT_SDIO_L    M0P_GPIO->PBOUT_f.PB15 = 0

#define SPI_SOFT_SCLK_H    M0P_GPIO->PBOUT_f.PB13 = 1
#define SPI_SOFT_SCLK_L    M0P_GPIO->PBOUT_f.PB13 = 0

#define SPI_SOFT_SDIO_IN()      M0P_GPIO->PBDIR_f.PB15 = 1
#define SPI_SOFT_SDIO_OUT()     M0P_GPIO->PBDIR_f.PB15 = 0

#define SPI_SOFT_MISO_IN()      M0P_GPIO->PBDIR_f.PB14 = 1
#define SPI_SOFT_MISO_OUT()     M0P_GPIO->PBDIR_f.PB14 = 0

#define SPI_SOFT_SDIO_IN_DATA   M0P_GPIO->PBIN_f.PB15
#define SPI_SOFT_INT_IN_DATA    M0P_GPIO->PAIN_f.PA09


//void SPI_SOFT_SDIO_IN(void);
//void SPI_SOFT_SDIO_OUT(void);
void spi_soft_io_init(void);
void spi_soft_write_byte(uint8_t data);
uint8_t spi_soft_read_byte(void);


#endif

