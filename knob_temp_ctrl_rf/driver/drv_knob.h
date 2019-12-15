#ifndef _DRV_KNOB_H_
#define _DRV_KNOB_H_


#include <stdint.h>
#include "base_types.h"
#include "hc32l136.h"
#include "system_hc32l136.h"
#include "hc32_gpio.h"


#define PORT_KNOB_CHA   GpioPortA
#define PORT_KNOB_CHB   GpioPortA

#define PIN_KNOB_CHA    GpioPin4
#define PIN_KNOB_CHB    GpioPin5

#define IRQ_SOURCE_KNOB PORTA_IRQn

#define KNOB_STA_CHA    M0P_GPIO->PAIN_f.PA04
#define KNOB_STA_CHB    M0P_GPIO->PAIN_f.PA05


void knob_io_init(void);


#endif

