#ifndef _DRV_SEGLCD_H_
#define _DRV_SEGLCD_H_

#include "hc32_lcd.h"

#define HEADER_MODE (1 << 19)

void seglcd_port_clear(void);
void seglcd_init(void);

#endif
