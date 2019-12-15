#ifndef __PWM_H
#define __PWM_H

#include "stm8s.h"

typedef struct _gradual_t_ {
    u16 		zkb;
    u16			v;
} gradual;

extern gradual			gra;
#define DIM_DEVICE      1000

void PWM_Init(void);
extern void gradual_hander(void);


#endif


