#include "drv_it.h"
#include "drv_dimmer.h"


void Gpio_IRQHandler(uint8_t u8Param)
{
    irq_zero_cross();
}

