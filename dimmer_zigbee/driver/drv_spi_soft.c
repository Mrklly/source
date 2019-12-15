#include "drv_spi_soft.h"

void hal_spi_soft_init(void)
{
    Gpio_InitIOExt(SPI_SOFT_CSB_PORT,  SPI_SOFT_CSB_PIN,  GpioDirOut, FALSE, FALSE, FALSE, FALSE);
    Gpio_InitIOExt(SPI_SOFT_FCSB_PORT, SPI_SOFT_FCSB_PIN, GpioDirOut, FALSE, FALSE, FALSE, FALSE);
    Gpio_InitIOExt(SPI_SOFT_SCLK_PORT, SPI_SOFT_SCLK_PIN, GpioDirOut, FALSE, FALSE, FALSE, FALSE);
    Gpio_InitIOExt(SPI_SOFT_SDIO_PORT, SPI_SOFT_SDIO_PIN, GpioDirOut, FALSE, FALSE, FALSE, FALSE);
    Gpio_InitIOExt(SPI_SOFT_INT_PORT,  SPI_SOFT_INT_PIN,  GpioDirIn,  FALSE, TRUE,  FALSE, FALSE);

    Gpio_InitIOExt(0,                  1,                 GpioDirIn,  FALSE, TRUE,  FALSE, FALSE);         // PA1 & PA2 MUST be the same,

    Gpio_InitIOExt(2,  7,  GpioDirOut,  FALSE, FALSE, FALSE, FALSE);
    Gpio_InitIOExt(3,  1,  GpioDirOut,  FALSE, FALSE, FALSE, FALSE);
}


void spi_soft_write_byte(uint8_t data)
{
    uint8_t i;

//    SPI_SOFT_SDIO_OUT;

    for(i=0; i<8; i++) {
        if(data & 0x80) {
            SPI_SOFT_SDIO_H;
            } else {
            SPI_SOFT_SDIO_L;
        }

        delay_us(2);
        SPI_SOFT_SCLK_H;
        data <<= 1;
        delay_us(1);
        SPI_SOFT_SCLK_L;
    }
}

uint8_t spi_soft_read_byte(void)
{
    uint8_t i, data;

    SPI_SOFT_SDIO_IN;

    SPI_SOFT_SCLK_L;

    for(i=0; i<8; i++) {
        data <<= 1;
        SPI_SOFT_SCLK_H;
        delay_us(2);
        if(SPI_SOFT_SDIO_IN_DATA) {
            data++;
        }
        SPI_SOFT_SCLK_L;
        delay_us(1);
    }

    SPI_SOFT_SDIO_OUT;

    return data;
}

