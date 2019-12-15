#include "zg_register.h"
#include "global_variables.h"
#include "drv_led.h"
#include "mcu_api.h"


void zg_reg_run(void)       // 100ms
{
    static uint16_t count_time = 0;

    if(g_zig_net_sta < ZIG_NET_REG_PRE) {
        LED_NET_OFF;
        count_time = 0;
        return ;
    }

    count_time++;

    if(g_zig_net_sta == ZIG_NET_REG_PRE) {

        if(count_time > 20) {
           count_time = 0;
           LED_NET_OFF;
           g_zig_net_sta = ZIG_NET_STA_NONE;
        }

    } else if(g_zig_net_sta == ZIG_NET_REG_S_3) {

        if(count_time <= 20) {
            g_zig_net_sta = ZIG_NET_REG_ING;
            mcu_network_start();
        } else {
            g_zig_net_sta = ZIG_NET_STA_NONE;
//            mcu_reset_zigbee();
        }

        count_time = 0;

    } else if(g_zig_net_sta == ZIG_NET_REG_ING) {

        if(count_time >= 3000) {      // 5min = 300s
            g_zig_net_sta = ZIG_NET_REG_TOUT;
            mcu_reset_zigbee();
        }
    }

    switch(g_zig_net_sta) {

    case ZIG_NET_STA_NONE:
    case ZIG_NET_REG_OVER:
        count_time = 0;
        LED_NET_OFF;
        break;
    case ZIG_NET_REG_PRE:
        LED_NET_ON;
        break;
    case ZIG_NET_REG_ING:
        if(count_time % 4 == 0) {
            LED_NET_TOG;
        }
        break;
    case ZIG_NET_REG_TOUT:
        count_time = 0;
        LED_NET_ON;
    default :
        break;
    }
}

void zg_reg_start(void)
{
    if(g_zig_net_sta != ZIG_NET_REG_PRE) {

        LED_NET_ON;
        g_zig_net_sta = ZIG_NET_REG_PRE;

    }
}

void zg_reg_pre(void)
{
    if(g_zig_net_sta >= ZIG_NET_REG_PRE && g_zig_net_sta < ZIG_NET_REG_S_4) {

        g_zig_net_sta++;

    }
}

