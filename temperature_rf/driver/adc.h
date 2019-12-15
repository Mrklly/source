#ifndef _ADC_H_
#define _ADC_H_

#include "hc32_adc.h"
#include "hc32_gpio.h"

extern volatile uint8_t adc_flag;
extern stc_adc_irq_t stcAdcIrqFlag;
extern uint16_t u16AdcRestult;
extern uint32_t u32AdcRestultAcc;


void AdcContIrqCallback(void);
void adc_init(void);


#endif
