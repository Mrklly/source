#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H
#include "stm8s.h"
extern float Get_Temp(u16 adc);
extern void temp_init(void);
void get_room_temp();

#define CTRL_PORT_VAVLE     GPIOD
#define CTRL_PIN_VAVLE      GPIO_PIN_2

#define CTRL_PORT_HIGH      GPIOD
#define CTRL_PIN_HIGH       GPIO_PIN_4

#define CTRL_PORT_MID       GPIOD
#define CTRL_PIN_MID        GPIO_PIN_3

#define CTRL_PORT_LIT       GPIOD
#define CTRL_PIN_LIT        GPIO_PIN_7

#define CTRL_VAVLE_OPEN()   CTRL_PORT_VAVLE->ODR |= (uint8_t)CTRL_PIN_VAVLE;
#define CTRL_VAVLE_CLOSE()  CTRL_PORT_VAVLE->ODR &= (uint8_t)(~CTRL_PIN_VAVLE);

#define CTRL_FAN_HIGH()     CTRL_PORT_MID->ODR &= (uint8_t)(~CTRL_PIN_MID);\
                            CTRL_PORT_LIT->ODR &= (uint8_t)(~CTRL_PIN_LIT);\
                            CTRL_PORT_HIGH->ODR |= (uint8_t)CTRL_PIN_HIGH;

#define CTRL_FAN_MID()      CTRL_PORT_HIGH->ODR &= (uint8_t)(~CTRL_PIN_HIGH);\
                            CTRL_PORT_LIT->ODR &= (uint8_t)(~CTRL_PIN_LIT);\
                            CTRL_PORT_MID->ODR |= (uint8_t)CTRL_PIN_MID;

#define CTRL_FAN_LIT()      CTRL_PORT_HIGH->ODR &= (uint8_t)(~CTRL_PIN_HIGH);\
                            CTRL_PORT_MID->ODR &= (uint8_t)(~CTRL_PIN_MID);\
                            CTRL_PORT_LIT->ODR |= (uint8_t)CTRL_PIN_LIT;

#define CTRL_FAN_CLOSE()    CTRL_PORT_HIGH->ODR &= (uint8_t)(~CTRL_PIN_HIGH);\
                            CTRL_PORT_MID->ODR &= (uint8_t)(~CTRL_PIN_MID);\
                            CTRL_PORT_LIT->ODR &= (uint8_t)(~CTRL_PIN_LIT);

#define CTRL_PWR_OFF()      CTRL_FAN_CLOSE();
//CTRL_VAVLE_CLOSE();

void temperature_control(void);
void power_control(void);
void mod_cool(void);
void mod_warm(void);
void mod_fan(void);
void mod_dry(void);
void mod_off(void);
void mod_auto(void);

#endif