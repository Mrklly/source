#ifndef __KEY_H
#define	__KEY_H

#include "stm8s.h"


void KEY_Init(void);
extern u8 KEY_Down(GPIO_TypeDef* GPIOx,GPIO_Pin_TypeDef  GPIO_Pin,u8 *press_counter);
extern void button_check(void);
void button_handler(void);
extern void AddrMod_Set(void);

#endif /* __KEY_H */

