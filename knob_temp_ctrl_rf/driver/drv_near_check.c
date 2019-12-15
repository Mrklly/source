#include "drv_near_check.h"


void near_check_init(void)
{
    stc_gpio_config_t stcLEDPortCfg;
    DDL_ZERO_STRUCT(stcLEDPortCfg);

    stcLEDPortCfg.enDir  = GpioDirIn;
    stcLEDPortCfg.enPuPd = GpioPu;

#if TEMP_CTRL_KNOB
    Gpio_Init(PORT_NEAR_CHECK, PIN_NEAR_CHECK, &stcLEDPortCfg);
#endif
}

