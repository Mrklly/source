#include "knob_handler.h"
#include "global_variables.h"


void knob_handler(void)
{
    if(g_knob_direction < DIRECTION_CW) {
        return ;
    }

    if(g_knob_direction == DIRECTION_CW) {


    } else if(g_knob_direction == DIRECTION_CCW) {

    }

    g_knob_direction = DIRECTION_NONE;
}

