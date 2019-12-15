#include "key_handler.h"
#include "led.h"
#include "key.h"
#include "global_variables.h"


const com_io g_io[5] = {
    {KEY_PORT_MODE,KEY_PIN_MODE,btn_hander_mode},	/* MODE */
    {KEY_PORT_FAN,KEY_PIN_FAN,btn_hander_fan},			/* FAN */
    {KEY_PORT_UP,KEY_PIN_UP,btn_hander_up},				/* UP */
    {KEY_PORT_DOWN,KEY_PIN_DOWN,btn_hander_down},	/* DOWN */
    {KEY_PORT_ON_OFF,KEY_PIN_ON_OFF,btn_hander_pwn},	/* ON/OFF */
};


void btn_hander_mode(u8	shirt)
{
    if(BUTTON_MODE.shirt == 1) {
        if(g_dev.mode.mod == TEMP_MODE_FAN) {
            g_dev.mode.mod = TEMP_MODE_COOL;
        } else {    // TODO: Now we only use two modes
            g_dev.mode.mod++;
        }
    }

    if((g_dev.mode.mod == TEMP_MODE_FAN) && (g_dev.mode.wind == FAN_CLZ)) {
        g_dev.mode.wind = FAN_MIN;
    }
}

void btn_hander_fan(u8	shirt)
{
    if(BUTTON_FAN.shirt == 1 && (g_dev.mode.mod != TEMP_MODE_DRY)) {

        if(g_dev.mode.mod == TEMP_MODE_FAN) {
            g_dev.mode.wind_auto = 0;
            if(g_dev.mode.wind == FAN_MAX) {
                g_dev.mode.wind = FAN_MIN;
            } else {
                g_dev.mode.wind++;
            }
            return ;
        }

        if(g_dev.mode.wind == FAN_MAX) {
            g_dev.mode.wind = FAN_CLZ;
        } else {
            g_dev.mode.wind++;
        }
        if(g_dev.mode.wind == FAN_CLZ) {
            g_dev.mode.wind_auto = 1;
        } else {
            g_dev.mode.wind_auto = 0;
        }
    }
}

#define FIRST_POWERON_OPEN_DEVICE   1

void btn_hander_pwn(u8	shirt)
{

#if FIRST_POWERON_OPEN_DEVICE

    if(BUTTON_ONOFF.shirt == 1) {
        if(g_lcd_on) {
            g_mod_backup = g_dev.mod; // backup mod
            g_mod_backup_flag = 1;
            g_dev.mode.power = 0;
            g_dev.mode.wind = 0;
            g_dev.mode.valve = 0;
            g_led_on = 0;
            g_lcd_on = 0;
        } else {
            if(g_mod_backup_flag) {
                g_mod_backup_flag = 0;
                g_dev.mod = g_mod_backup;
            }
            g_dev.mode.power = 1;
            g_lcd_on = 1;
            g_led_on = 1;
        }
    }

#else

    if(BUTTON_ONOFF.shirt == 1) {
        if(g_lcd_on) {
            if(g_dev.mode.power) {
                g_mod_backup = g_dev.mod; // backup mod
                g_dev.mode.power = 0;
                g_lcd_on = 0;
                g_led_on = 0;
            } else {
                g_dev.mod = g_mod_backup;
                g_dev.mode.power = 1;
            }
        } else {
            g_lcd_on = 1;
            g_led_on = 1;
        }
    }

#endif

}

void btn_hander_up(u8	shirt)  // run at every 50 ms
{
    if(g_dev.set_temp < 32) {
        if(BUTTON_UP.shirt == 1) {  // when first pressed, add 1,or check it whether long pressed
            g_dev.set_temp++;
        } else if(BUTTON_UP.shirt >= 12) {     // long pressed
            if(g_dev.set_temp >= 32) {
                return;
            }
            g_dev.set_temp++;
        }
    }
}
void btn_hander_down(u8	shirt)
{
    if(g_dev.set_temp > 16) {
        if(BUTTON_DOWN.shirt == 1) {
            g_dev.set_temp--;
        } else if(BUTTON_DOWN.shirt >= 12) {
            if(g_dev.set_temp <= 16) {
                return;
            }
            g_dev.set_temp--;
        }
    }
}

void valve_control(void)
{
    if(!g_dev.mode.power) {
        CTRL_VAVLE_CLOSE();
        return ;
    }
    
    if(g_dev.mode.valve) {
        CTRL_VAVLE_OPEN();
    } else {
        CTRL_VAVLE_CLOSE();
    }
}

void set_fan(void)
{
    if(!g_dev.mode.power) {
        CTRL_FAN_CLOSE();
        return ;
    }
    switch( (fan_status)g_dev.mode.wind ) {
    case FAN_MAX:
        CTRL_FAN_HIGH();
        break;
    case FAN_MID:
        CTRL_FAN_MID();
        break;
    case FAN_MIN:
        CTRL_FAN_LIT();
        break;
    case FAN_CLZ:
        CTRL_FAN_CLOSE();
        break;
    }
}

void set_lcd_backlight(void) // run at every 100ms
{
    static unsigned char lcd_counter = 0;
    static lcd_backlight_status lcd_status = BACKLIGHT_OFF;
    
    if(g_saving_flag) {

        if(lcd_counter % 2 == 0) {
            LCD_BACKLIGHT_OFF;
        } else {
            LCD_BACKLIGHT_ON;
        }

        lcd_counter++;

        if(lcd_counter >= 6) {
            g_saving_flag = 0;
            lcd_counter = 0;
        }
            
        return ;
    }

    if((!g_dev.mode.power) && (!g_lcd_on)) {
        CloseLCD();
    }

    if((g_lcd_on)) {
        lcd_status = BACKLIGHT_ON;
    } else {
        lcd_status = BACKLIGHT_OFF;
    }

    if(g_lcd_delay == 0) {
        g_lcd_delay = 100;
        g_lcd_on = 0;
        lcd_status = BACKLIGHT_OFF;
    }

    if(g_set_addr_baud) {
        lcd_status = BACKLIGHT_ON;
        g_lcd_delay = 100;
    }

    if(BACKLIGHT_ON == lcd_status) {
        g_lcd_delay--;
        OpenLCD();
    } else {
        g_lcd_delay = 100;
        CloseLCD();
    }

#if 0
    if(g_lcd_delay == 0) {
        g_lcd_delay = 50;
        g_lcd_on = 0;
        lcd_status = BACKLIGHT_OFF;
    }

    if(g_set_addr_baud) {
        lcd_status = BACKLIGHT_ON;
        g_lcd_delay = 50;
    }
    
    if(lcd_status == BACKLIGHT_OFF) {
        g_lcd_delay = 50;
        CloseLCD();
    } else {
        g_lcd_delay--;
        OpenLCD();
    }
#endif
}

void set_led_indicator(void)    // run at every 100ms
{
    static unsigned char led_counter = 0;
    static unsigned char led_status = 0;
    
    if(g_saving_flag) {

        if(led_counter % 2 == 0) {
            LED_INDICATOR_OFF;
        } else {
            LED_INDICATOR_ON;
        }

        led_counter++;

        if(led_counter >= 6) {
//            g_saving_flag = 0;
            led_counter = 0;
        }
            
        return ;
    }

    if((g_led_on)) {
        led_status = 1;
    } else {
        led_status = 0;
    }

    if(g_led_delay == 0) {
        g_led_delay = 100;
        g_led_on = 0;
        led_status = 0;
    }

    if(g_set_addr_baud) {
        led_status = 1;
        g_lcd_delay = 100;
    }

    if(led_status) {
        g_led_delay--;
        LED_INDICATOR_ON;
    } else {
        g_led_delay = 100;
        LED_INDICATOR_OFF;
    }

#if 0
    if((!g_dev.mode.power) && (!g_lcd_on)){
        if(g_led_setting_flag) {
            LED_INDICATOR_ON;
        }
        led_counter--;
        if(led_counter == 0) {
            LED_INDICATOR_OFF;
            led_counter = 30;
        }
        return ;
    }

    if(g_lcd_on || g_led_setting_flag ||(g_set_addr_baud)) {
        LED_INDICATOR_ON;
    } else {
        LED_INDICATOR_OFF;
    }
#endif
}

