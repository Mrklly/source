#include <math.h>

#include "temperature.h"
#include "global_variables.h"
#include "adc.h"


#define Rr         10000

const float Rp = 10000.0;
const float T2 = (273.15 + 25.0);
const float Bx = 3950.0;
const float Ka = 273.15;


float Get_Rt(uint16_t adc_value)
{
    float Rt;
    Rt = (float)4096/(float)(adc_value);
    Rt = Rt - 1;
    Rt = Rr / (Rt);
    return Rt;
}


float Get_Temp(uint16_t adc_value)
{
    float Rt, temp;

    Rt = Get_Rt(adc_value);
    temp = Rt/Rp;
    temp = log(temp);
    temp /= Bx;
    temp += (1/T2);
    temp = 1/(temp);
    temp -= Ka;
    return temp;
}

void get_room_temp()
{
    uint16_t adc_value, adc_max = 0, adc_min = 0xffff;
    static uint16_t adc_sum;
    static uint16_t adc_counter;
    float s_temp_bak = 0;

    adc_value = u16AdcRestult;
    adc_sum += adc_value;
    if(adc_max < adc_value) {
    adc_max = adc_value;
    }
    if(adc_min > adc_value) {
    adc_min = adc_value;
    }
    if(++adc_counter == 10) {
        adc_sum = adc_sum - adc_max - adc_min;
        adc_sum = adc_sum >> 3;
        if(adc_sum == 0) {
            adc_sum = 1;
        }
        s_temp_bak = Get_Temp(adc_sum) + ((float)(g_dev.temp_offset)/10);
        if(s_temp_bak < 0) {
            s_temp_bak = 0;
        }
        if(s_temp_bak > 99) {
            s_temp_bak = 99;
        }
        if(s_temp_bak - g_dev.room_temp > 0.01 || (s_temp_bak - g_dev.room_temp < -0.01)) {
//            SET_ROOM_TEMP(s_temp_bak);
            g_dev.room_temp = s_temp_bak;
        }
        adc_sum = 0;
        adc_counter = 0;
        adc_max = 0;
        adc_min = 0xffff;
    }

    Adc_ClrAccResult();
    Adc_SQR_Start();
}

void mod_cool(void)
{
    float  gap;
    static unsigned char fan_auto_delay = 20;    // delay 2s

    gap = g_dev.room_temp - (g_dev.set_temp);
    if(g_dev.sta.wind_auto){

        fan_auto_delay--;

        if(fan_auto_delay == 0) {
            fan_auto_delay = 20;
            g_fan_ultralow = 0;

            if(gap >= 2) {
                g_dev.sta.wind = FAN_MAX;
            } else if(gap >= 1) {
                g_dev.sta.wind = FAN_MID;
            } else if(gap > 0) {
                g_dev.sta.wind = FAN_MIN;
            } else {
                g_dev.sta.wind = FAN_MIN;
                if(g_dev.flag_fan_ultralow) {
                    g_fan_ultralow = 1;
                }
            }
        } else if(gap > 0) {

        }
    }else {
        fan_auto_delay = 20;
    }

    if(gap > 0.5) {
        g_dev.sta.valve = 1;
    } else if(gap < 0) {
        g_dev.sta.valve = 0;
    }

    return;
}


void mod_warm(void)
{
    float  gap;
    static unsigned char fan_auto_delay = 20;    // delay 1.5s

    gap = g_dev.set_temp - (g_dev.room_temp);
    if(g_dev.sta.wind_auto) {

        fan_auto_delay--;

        if(fan_auto_delay == 0) {
            fan_auto_delay = 20;
            g_fan_ultralow = 0;

            if(gap >= 2) {
                g_dev.sta.wind = FAN_MAX;
            } else if(gap >= 1) {
                g_dev.sta.wind = FAN_MID;
            } else if(gap > 0) {
                g_dev.sta.wind = FAN_MIN;
            } else {
                g_dev.sta.wind = FAN_MIN;
                if(g_dev.flag_fan_ultralow) {
                    g_fan_ultralow = 1;
                }
            }
        } else if(gap > 0) {

        }
    } else {
        fan_auto_delay = 20;
    }

    if(gap > 0.5) {
        g_dev.sta.valve = 1;
    } else if(gap < 0) {
        g_dev.sta.valve = 0;
    }

}
void mod_fan(void)
{
    g_dev.sta.valve = 0;
}
void mod_dry(void)
{
    float  gap;

    g_dev.sta.wind = FAN_MIN;
    gap = g_dev.room_temp - (g_dev.set_temp);

    if(gap < 0) {
        g_dev.sta.valve = 0;
    } else {
        g_dev.sta.valve = 1;
    }

}
void mod_off(void)
{
    g_dev.sta.power = 0;
    g_lcd_on = 0;
}

void mod_auto(void)
{

}

mode 	g_mod[6]= {
    {TEMP_MODE_COOL,mod_cool},
    {TEMP_MODE_WARM,mod_warm},
    {TEMP_MODE_FAN, mod_fan},
    {TEMP_MODE_DRY, mod_dry},
    {TEMP_MODE_AUTO,mod_auto},
    {TEMP_MODE_NONE,mod_off},
};

void temperature_control(void)  // every 100ms to run
{
    if(!g_dev.sta.power) {
        return ;
    }
    switch(g_dev.sta.mode){
    case TEMP_MODE_COOL:
        mod_cool();
        break;
    case TEMP_MODE_WARM:
        mod_warm();
        break;
    case TEMP_MODE_FAN:
        mod_fan();
        break;
    case TEMP_MODE_DRY:
        mod_dry();
        break;
    case TEMP_MODE_AUTO:
        mod_auto();
        break;
    case TEMP_MODE_NONE:
        break;
    default:
        break;

    }
}

void power_control(void)
{

}

