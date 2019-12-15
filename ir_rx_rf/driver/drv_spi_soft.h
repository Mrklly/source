#ifndef _DRV_SPI_SOFT_H_
#define _DRV_SPI_SOFT_H_

#include "hc32_gpio.h"

#define SPI_SOFT_CSB_PORT       2
#define SPI_SOFT_CSB_PIN        6

#define SPI_SOFT_FCSB_PORT      2
#define SPI_SOFT_FCSB_PIN       4

#define SPI_SOFT_SCLK_PORT      2
#define SPI_SOFT_SCLK_PIN       5

#define SPI_SOFT_SDIO_PORT      2
#define SPI_SOFT_SDIO_PIN       7

#define SPI_SOFT_INT_PORT       0
#define SPI_SOFT_INT_PIN        2
//#define SPI_SOFT_INT_IN_DATA    Gpio_GetIO(SPI_SOFT_INT_PORT, SPI_SOFT_INT_PIN)
#define SPI_SOFT_INT_IN_DATA    M0P_GPIO->P0IN_f.P02


#define SPI_SOFT_CSB_H     *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + SPI_SOFT_CSB_PORT * GPIO_GPSZ))  |= (1UL << SPI_SOFT_CSB_PIN)
#define SPI_SOFT_CSB_L     *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + SPI_SOFT_CSB_PORT * GPIO_GPSZ))  &=~(1UL << SPI_SOFT_CSB_PIN)

#define SPI_SOFT_FCSB_H    *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + SPI_SOFT_FCSB_PORT * GPIO_GPSZ)) |= (1UL << SPI_SOFT_FCSB_PIN)
#define SPI_SOFT_FCSB_L    *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + SPI_SOFT_FCSB_PORT * GPIO_GPSZ)) &=~(1UL << SPI_SOFT_FCSB_PIN)

#define SPI_SOFT_SCLK_H    *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + SPI_SOFT_SCLK_PORT * GPIO_GPSZ)) |= (1UL << SPI_SOFT_SCLK_PIN)
#define SPI_SOFT_SCLK_L    *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + SPI_SOFT_SCLK_PORT * GPIO_GPSZ)) &=~(1UL << SPI_SOFT_SCLK_PIN)

#define SPI_SOFT_SDIO_H    *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + SPI_SOFT_SDIO_PORT * GPIO_GPSZ)) |= (1UL << SPI_SOFT_SDIO_PIN)
#define SPI_SOFT_SDIO_L    *((volatile uint32_t *)((uint32_t)&M0P_GPIO->P0OUT + SPI_SOFT_SDIO_PORT * GPIO_GPSZ)) &=~(1UL << SPI_SOFT_SDIO_PIN)


//#define SPI_SOFT_SDIO_IN         Gpio_InitIOExt(SPI_SOFT_SDIO_PORT, SPI_SOFT_SDIO_PIN, GpioDirIn, FALSE, FALSE, FALSE, FALSE);  Gpio_InitIOExt(3, 1, GpioDirIn, FALSE, FALSE, FALSE, FALSE)
//#define SPI_SOFT_SDIO_OUT        Gpio_InitIOExt(SPI_SOFT_SDIO_PORT, SPI_SOFT_SDIO_PIN, GpioDirOut, FALSE, FALSE, FALSE, FALSE); Gpio_InitIOExt(3, 1, GpioDirOut, FALSE, FALSE, FALSE, FALSE)
//#define SPI_SOFT_SDIO_IN_DATA    Gpio_GetIO( SPI_SOFT_SDIO_PORT, SPI_SOFT_SDIO_PIN)

#define SPI_SOFT_SDIO_IN       M0P_GPIO->P2ADS_f.P27 = 0;\
                               M0P_GPIO->P27_SEL_f.SEL = 0;\
                               M0P_GPIO->P2DIR_f.P27 = 1;\
                               M0P_GPIO->P2PU_f.P27 = 0;\
                               M0P_GPIO->P2PD_f.P27 = 0;\
                               M0P_GPIO->P2DR_f.P27 = 0

#define SPI_SOFT_SDIO_OUT      M0P_GPIO->P2ADS_f.P27 = 0;\
                               M0P_GPIO->P27_SEL_f.SEL = 0;\
                               M0P_GPIO->P2DIR_f.P27 = 0;\
                               M0P_GPIO->P2OUT_f.P27 = 0;\
                               M0P_GPIO->P2PU_f.P27 = 0;\
                               M0P_GPIO->P2PD_f.P27 = 0;\
                               M0P_GPIO->P2DR_f.P27 = 0

#define SPI_SOFT_SDIO_IN_DATA  M0P_GPIO->P2IN_f.P27


void hal_spi_soft_init(void);
void spi_soft_write_byte(uint8_t data);
uint8_t spi_soft_read_byte(void);

#endif
