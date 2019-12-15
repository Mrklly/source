#include "drv_infrared.h"
#include "hc32_clk.h"


#define RCL_CR_TRIM_38_4K_VAL       (*((volatile uint16_t*) (0x00100C20ul)))


void hal_ir_init(void)
{
    M0P_CLOCK->PERI_CLKEN_f.GPIO = 1;

    M0P_CLOCK->RCL_CR_f.TRIM = RCL_CR_TRIM_38_4K_VAL;

    M0P_CLOCK->SYSCTRL2 = 0x5A5A;
    M0P_CLOCK->SYSCTRL2 = 0xA5A5;
    M0P_CLOCK->SYSCTRL0_f.RCL_EN = 1;

    M0P_GPIO->P2ADS_f.P23 = 0;
    M0P_GPIO->P23_SEL_f.SEL = 7;
    M0P_GPIO->P2DIR_f.P23 = 0;

    M0P_GPIO->CTRL1_f.IR_POL = 1;
    M0P_GPIO->P2OUT_f.P23 = 1;
}

