#ifndef _DRV_IT_H_
#define _DRV_IT_H_


#include "hc32_it.h"


#define GPIO_IRQ_STA(PORT,PIN)    ((((*((volatile uint32_t *)(addr))) >> (offset)) & 1u)>0?1u:0)


#endif
