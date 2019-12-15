#ifndef _BSP_SYSTEM_H_
#define _BSP_SYSTEM_H_


#include "hc32_sysctrl.h"
#include "hc32_flash.h"

#include "bsp_it.h"


void system_init(void);
void clock_output(void);
void systick_init(void);
void SysTick_Handler(void);


#endif
