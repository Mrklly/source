#include "drv_wdt.h"


void hal_wdt_init(void)
{
    M0P_SYSCTRL->PERI_CLKEN_f.WDT = 1; 
    M0P_WDT->CON_f.WOV = 0x0B;        // 3.28s
    M0P_WDT->CON_f.WINT_EN = 0;       // reset

    M0P_WDT->RST = 0x1E;
    M0P_WDT->RST = 0xE1;
}

void hal_wdt_feed(void)
{
    M0P_WDT->RST = 0x1E;
    M0P_WDT->RST = 0xE1;
}

