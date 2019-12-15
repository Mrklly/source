#ifndef _BSP_SYSTEM_H
#define _BSP_SYSTEM_H

#include "hc32_sysctrl.h"
#include "hc32_flash.h"

#include "bsp_it.h"
void system_init(void);
void clock_output(void);
#endif