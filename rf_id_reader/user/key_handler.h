#ifndef _KEY_HANDLER_H_
#define _KEY_HANDLER_H_

#include "stm8s.h"

#include "drv_key.h"
#include "global_variables.h"


typedef struct _key_status_t_ {
    u8		shirt;
    u8		up;
    u8		down;
} key_status_t;


extern key_status_t g_key_status[KEY_COUNT];


void key_check(void);


#endif
