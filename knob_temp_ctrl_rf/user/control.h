#ifndef _CONTROL_H_
#define _CONTROL_H_


#include "drv_led.h"


#if TEMP_CTRL_KNOB

#define VAVLE_WARM_PORT     GpioPortD   // JK5
#define VAVLE_WARM_PIN      GpioPin5

#elif TEMP_CTRL_TOUCH

#define VAVLE_WARM_PORT     GpioPortC   // JK5
#define VAVLE_WARM_PIN      GpioPin13

#endif

#define VAVLE_COLD_PORT     GpioPortC
#define VAVLE_COLD_PIN      GpioPin3

#define CTRL_FAN_HIGH_PORT  GpioPortC
#define CTRL_FAN_HIGH_PIN   GpioPin0

#define CTRL_FAN_MID_PORT   GpioPortC
#define CTRL_FAN_MID_PIN    GpioPin1

#define CTRL_FAN_LOW_PORT   GpioPortC
#define CTRL_FAN_LOW_PIN    GpioPin2


#define VAVLE_COLD_OPEN()     GPIO_HIGH(VAVLE_COLD_PORT, VAVLE_COLD_PIN)
#define VAVLE_COLD_CLOSE()    GPIO_LOW (VAVLE_COLD_PORT, VAVLE_COLD_PIN)

#define VAVLE_WARM_OPEN()     GPIO_HIGH(VAVLE_WARM_PORT, VAVLE_WARM_PIN)
#define VAVLE_WARM_CLOSE()    GPIO_LOW (VAVLE_WARM_PORT, VAVLE_WARM_PIN)

#define CTRL_FAN_HIGH()       GPIO_HIGH(CTRL_FAN_HIGH_PORT, CTRL_FAN_HIGH_PIN);\
                              GPIO_LOW (CTRL_FAN_MID_PORT,  CTRL_FAN_MID_PIN);\
                              GPIO_LOW (CTRL_FAN_LOW_PORT,  CTRL_FAN_LOW_PIN);

#define CTRL_FAN_MID()        GPIO_LOW (CTRL_FAN_HIGH_PORT, CTRL_FAN_HIGH_PIN);\
                              GPIO_HIGH(CTRL_FAN_MID_PORT,  CTRL_FAN_MID_PIN);\
                              GPIO_LOW (CTRL_FAN_LOW_PORT,  CTRL_FAN_LOW_PIN);

#define CTRL_FAN_LOW()        GPIO_LOW (CTRL_FAN_HIGH_PORT, CTRL_FAN_HIGH_PIN);\
                              GPIO_LOW (CTRL_FAN_MID_PORT,  CTRL_FAN_MID_PIN);\
                              GPIO_HIGH(CTRL_FAN_LOW_PORT,  CTRL_FAN_LOW_PIN);

#define CTRL_FAN_CLZ()        GPIO_LOW (CTRL_FAN_HIGH_PORT, CTRL_FAN_HIGH_PIN);\
                              GPIO_LOW (CTRL_FAN_MID_PORT,  CTRL_FAN_MID_PIN);\
                              GPIO_LOW (CTRL_FAN_LOW_PORT,  CTRL_FAN_LOW_PIN);

#define CTRL_PWR_OFF()        CTRL_FAN_CLZ()


void control_init(void);
void control_valve(void);
void control_fan(void);


#endif

