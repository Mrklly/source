#include "spi.h"

void spi_init(void)
{
    SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_2, SPI_MODE_MASTER,\
             SPI_CLOCKPOLARITY_HIGH, SPI_CLOCKPHASE_2EDGE, \
             SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
    SPI_Cmd(ENABLE);
}

u8 spi_wr_byte(u8 dat)
{
    while(SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET) {
    }

    SPI_SendData(dat);
    while(SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET)  {
    }
    return SPI_ReceiveData();
}

