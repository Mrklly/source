#include "temperature.h"
#include "math.h"
#include "global_variables.h"
#include "led.h"
#include "key_handler.h"

#define Rr			10000
const float Rp = 10000.0;
const float T2 = (273.15+25.0);
const float Bx = 3950.0;
const float Ka = 273.15;

void temp_init(void)
{
    GPIO_Init(CTRL_PORT_VAVLE,CTRL_PIN_VAVLE,GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(CTRL_PORT_HIGH,CTRL_PIN_HIGH,GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(CTRL_PORT_MID,CTRL_PIN_MID,GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(CTRL_PORT_LIT,CTRL_PIN_LIT,GPIO_MODE_OUT_PP_LOW_FAST);
    //SET_POWER_ON();
}

float Get_Rt(u16 adc_value)
{
    float Rt;
    Rt = (float)1024/(float)(adc_value);
    Rt = Rt - 1;
    Rt = Rr / (Rt);
    return Rt;
}


#define 	TEMP_AMEND		(0)
float Get_Temp(u16 adc_value)
{
    float Rt;
    float temp;

    Rt = Get_Rt(adc_value);
    temp = Rt/Rp;
    temp = log(temp);
    temp /= Bx;
    temp += (1/T2);
    temp = 1/(temp);
    temp -= Ka;
    temp += TEMP_AMEND;
    return temp -4;    //考虑温控面板自身发热的温差，在检测室内温度时适当减去4 度。
}

void get_room_temp()
{
    unsigned int adc_value, adc_max = 0, adc_min = 0xffff;
    static unsigned int adc_sum;
    static unsigned char adc_counter;

    adc_value = ADC1_GetConversionValue();
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
        s_temp_bak = Get_Temp(adc_sum);
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
}

void mod_cool(void)
{
    float  gap;
    static unsigned char fan_auto_delay = 20;    // delay 2s

    gap = g_dev.room_temp - (g_dev.set_temp);
    if(g_dev.mode.wind_auto){
        fan_auto_delay--;

        if(fan_auto_delay == 0) {
            fan_auto_delay = 20;
            
            if(gap >= 2) {
                g_dev.mode.wind = (u8)FAN_MAX;
            } else if(gap >= 1) {
                g_dev.mode.wind = (u8)FAN_MID;
            } else if(gap > 0) {
                g_dev.mode.wind = (u8)FAN_MIN;
            } else {
                g_dev.mode.wind = (u8)FAN_MIN;
            }
//            else if(gap < -0.5) {
//                g_dev.mode.wind = (u8)FAN_CLZ;
//            }
        } else if(gap > 0) {

        }
    }else {
        fan_auto_delay = 20;
    }

    if(gap > 0.5) {
        g_dev.mode.valve = 1;
    } else if(gap < 0) {
        g_dev.mode.valve = 0;
    }

    return;
}

void mod_warm(void)
{
    float  gap;
    static unsigned char fan_auto_delay = 20;    // delay 1.5s

    gap = g_dev.set_temp - (g_dev.room_temp);
    if(g_dev.mode.wind_auto) {
        fan_auto_delay--;
        
        if(fan_auto_delay == 0) {
            fan_auto_delay = 20;

            if(gap >= 2) {
                g_dev.mode.wind = (u8)FAN_MAX;
            } else if(gap >= 1) {
                g_dev.mode.wind = (u8)FAN_MID;
            } else if(gap > 0) {
                g_dev.mode.wind = (u8)FAN_MIN;
            } else {
                g_dev.mode.wind = (u8)FAN_MIN;
            }
//            else if(gap < -0.5) {
//                g_dev.mode.wind = (u8)FAN_CLZ;
//            }
        } else if(gap > 0) {
        
        }
    } else {
        fan_auto_delay = 20;
    }

    if(gap > 0.5) {
        g_dev.mode.valve = 1;
    } else if(gap < 0) {
        g_dev.mode.valve = 0;
    }

    return;
}
void mod_fan(void)
{
    g_dev.mode.valve = 0;
    return;
}
void mod_dry(void)
{
    float  gap;

    g_dev.mode.wind = FAN_MIN;
    gap = g_dev.room_temp - (g_dev.set_temp);

    if(gap < 0) {
        g_dev.mode.valve = 0;
    } else {
        g_dev.mode.valve = 1;
    }
    return;
}
void mod_off(void)
{
    g_dev.mode.power = 0;
    g_lcd_on = 0;
    return;
}

void mod_auto(void)
{

}

mode 	g_mod[6]= {
    {TEMP_MODE_COOL,mod_cool},
    {TEMP_MODE_WARM,mod_warm},
    {TEMP_MODE_FAN,mod_fan},
    {TEMP_MODE_DRY,mod_dry},
    {TEMP_MODE_AUTO,mod_auto},
    {TEMP_MODE_NONE,mod_off},
};

void temperature_control(void)  // every 100ms to run
{
    if(!g_dev.mode.power) {
        return ;
    }
    switch(g_dev.mode.mod){
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
    static uint8_t back_mod = 0;
    
    if(!g_dev.mode.power) {
        back_mod = g_dev.mod;
        g_dev.mode.valve = 0;
        g_dev.mode.wind = 0;
    } else {
        if(back_mod) {
            g_dev.mod = back_mod;
            g_dev.mode.power = 1;
        }
    }
}

