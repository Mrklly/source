#ifndef __SPI_H
#define	__SPI_H

#include "stm8s.h"

void spi_init(void);
u8 spi_wr_byte(u8 dat);

#endif /* __SPI_H */
