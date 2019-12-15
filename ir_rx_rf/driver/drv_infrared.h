#ifndef _DRV_INFRARED_H_
#define _DRV_INFRARED_H_


#include "hc32f005.h"

#define CARRIER_38K_ON      M0P_GPIO->P2OUT_f.P23 = 1
#define CARRIER_38K_OFF      M0P_GPIO->P2OUT_f.P23 = 0

#define IR_DATA_H       CARRIER_38K_OFF
#define IR_DATA_L       CARRIER_38K_ON


void hal_ir_init(void);


#endif
