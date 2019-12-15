#ifndef _RF433_H_
#define _RF433_H_


#include "drv_spi_soft.h"
#include "drv_cmt2300a.h"


void rf433_tx(uint8_t *buf, uint8_t len);
uint8_t rf433_rx(uint8_t *buf, uint8_t len);
void rf433_handler(void);


#endif

