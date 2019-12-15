#ifndef _DRV_SPI_H_
#define _DRV_SPI_H_


#include "hc32_gpio.h"
#include "hc32_spi.h"


#define PORT_SPI0_CS      GpioPortA
#define PIN_SPI0_CS       GpioPin4

#define PORT_SPI0_MOSI    GpioPortA
#define PIN_SPI0_MOSI     GpioPin7

#define PORT_SPI0_CLK     GpioPortA
#define PIN_SPI0_CLK      GpioPin5

#define PORT_SPI0_MISO    GpioPortA
#define PIN_SPI0_MISO     GpioPin6

#define PORT_SPI0_WP      GpioPortB
#define PIN_SPI0_WP       GpioPin0

#define PORT_SPI0_HOLD    GpioPortB
#define PIN_SPI0_HOLD     GpioPin1


#define SPI0_WP_HIGH      M0P_GPIO->PBOUT_f.PB00 = 1
#define SPI0_WP_LOW       M0P_GPIO->PBOUT_f.PB00 = 0

#define SPI0_HOLD_HIGH    M0P_GPIO->PBOUT_f.PB01 = 1
#define SPI0_HOLD_LOW     M0P_GPIO->PBOUT_f.PB01 = 0


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

