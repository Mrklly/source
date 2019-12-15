#include "spi_soft.h"

void spi_soft_io_init(void)
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stc_gpio_config_t stcLEDPortCfg;

    DDL_ZERO_STRUCT(stcLEDPortCfg);

    stcLEDPortCfg.enDir  = GpioDirOut;

    Gpio_Init(SPI_SOFT_CSB_PORT,  SPI_SOFT_CSB_PIN,  &stcLEDPortCfg);
    Gpio_Init(SPI_SOFT_FCSB_PORT, SPI_SOFT_FCSB_PIN, &stcLEDPortCfg);
    Gpio_Init(SPI_SOFT_SDIO_PORT, SPI_SOFT_SDIO_PIN, &stcLEDPortCfg);
    Gpio_Init(SPI_SOFT_SCLK_PORT, SPI_SOFT_SCLK_PIN, &stcLEDPortCfg);
    
    stcLEDPortCfg.enDir   = GpioDirIn;
    stcLEDPortCfg.enPuPd  = GpioNoPuPd;
    Gpio_Init(SPI_SOFT_INT_PORT,  SPI_SOFT_INT_PIN,  &stcLEDPortCfg);
    Gpio_Init(SPI_SOFT_MISO_PORT, SPI_SOFT_MISO_PIN, &stcLEDPortCfg);
    
    SPI_SOFT_FCSB_H;
    SPI_SOFT_CSB_H;
    SPI_SOFT_SCLK_L;
    SPI_SOFT_SDIO_L;
}

//void SPI_SOFT_SDIO_IN(void)
//{
//    stc_gpio_config_t stcLEDPortCfg;

//    DDL_ZERO_STRUCT(stcLEDPortCfg);
//
//    stcLEDPortCfg.enDir   = GpioDirIn;
//    stcLEDPortCfg.enPuPd  = GpioPd;
//    Gpio_Init(SPI_SOFT_SDIO_PORT, SPI_SOFT_SDIO_PIN, &stcLEDPortCfg);
//}

//void SPI_SOFT_SDIO_OUT(void)
//{
//    stc_gpio_config_t stcLEDPortCfg;

//    DDL_ZERO_STRUCT(stcLEDPortCfg);
//
//    stcLEDPortCfg.enDir   = GpioDirOut;
//    Gpio_Init(SPI_SOFT_SDIO_PORT, SPI_SOFT_SDIO_PIN, &stcLEDPortCfg);
//    SPI_SOFT_SDIO_L;
//}

void spi_soft_write_byte(uint8_t data)
{
    uint8_t i;

    SPI_SOFT_SDIO_OUT();

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

    SPI_SOFT_SDIO_IN();

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

    return data;
}

