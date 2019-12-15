#ifndef _BSP_WWDG_H
#define _BSP_WWDG_H

#include "stm8s.h"
#define CounterInit 0x7f
#define window      0x70    // 0x70

void WWDG_Configuration(void) ;
void WWDGReset(void);
void Refresh_WWDG_Window(void);

#endif