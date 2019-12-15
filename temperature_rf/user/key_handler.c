#include "key_handler.h"
#include "key.h"
#include "uart.h"
#include "cmt2300a.h"
#include "lcd.h"
#include "led.h"
#include "eeprom.h"
#include "wireless.h"


void btn_hander_mode(uint8_t shirt)
{
    if(BUTTON_MODE.shirt == 1) {
        if(g_dev.mode.mod == TEMP_MODE_FAN) {
            g_dev.mode.mod = TEMP_MODE_COOL;
        } else {    // TODO: Now we only use two modes
            g_dev.mode.mod++;
        }

        if((g_dev.mode.mod == TEMP_MODE_FAN) && (g_dev.mode.wind == FAN_CLZ)) {
            g_dev.mode.wind = FAN_MIN;
        }

        btn_callback(KEY_INDEX_MODE);

    } else if(BUTTON_MODE.shirt == 40) {
        reg_to_controller();
    }
}

void btn_hander_fan(uint8_t shirt)
{
    if(BUTTON_FAN.shirt == 1 ) {
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

        btn_callback(KEY_INDEX_FAN);

    }
}

void btn_hander_up(uint8_t shirt)  // run at every 50 ms
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

    if(BUTTON_UP.shirt == 1 ) {
        btn_callback(KEY_INDEX_UP);
    }
}
void btn_hander_down(uint8_t shirt)
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

    if(BUTTON_DOWN.shirt == 1 ) {
        btn_callback(KEY_INDEX_DOWN);
    }
}

void btn_hander_pwn(uint8_t shirt)
{
    if(BUTTON_ONOFF.shirt == 1) {
        if(g_lcd_on) {
            g_mod_backup = g_dev.mod; // backup mod
            g_mod_backup_flag = 1;
            g_dev.mode.power = 0;
            g_dev.mode.wind = FAN_MIN;
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

        btn_callback(KEY_INDEX_POWER);

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
        lcd_close();
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

    if(g_set_type) {
        lcd_status = BACKLIGHT_ON;
        g_lcd_delay = 100;
    }

    if(BACKLIGHT_ON == lcd_status) {
        g_lcd_delay--;
        lcd_open();
    } else {
        g_lcd_delay = 100;
        lcd_close();
    }
}

void set_led_indicator(void)    // run at every 100ms
{
    static unsigned char led_counter = 0;
    static unsigned char led_status = 0;

    if(g_saving_flag || g_erasing_flag) {

        if(led_counter % 2 == 0) {
            LED_INDICATOR_OFF;
        } else {
            LED_INDICATOR_ON;
        }

        led_counter++;

        if(led_counter >= 6) {
//            g_saving_flag = 0;
            g_erasing_flag = 0;
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

    if(g_set_type) {
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
}

void device_setting(void)  // run at every 50 ms
{
    static volatile uint16_t dev_setting_counter = 0;
    volatile uint8_t save_status = 0 , led_flash_counter = 3;
    static uint8_t rf_ch_temp;

//    if(BUTTON_MODE.down==1 &&  BUTTON_FAN.down==0 &&  BUTTON_UP.down==1 &&  BUTTON_DOWN.down==0 &&  BUTTON_ONOFF.down==1 ) {
    if(BUTTON_MODE.down==1 &&  BUTTON_UP.down==1 && BUTTON_ONOFF.down==1 ) {

        if(g_set_type) {
            return ;
        }

        dev_setting_counter = 0;
        rf_ch_temp = g_eep.rf_ch;

        g_set_type = SET_ADDR;
        g_led_on = 1;
        g_lcd_on = 1;
        g_saving_flag = 0;
    }

    if(!g_set_type) {
        return ;
    }

    dev_setting_counter++;
    if(dev_setting_counter < 20) {
        return ;
    }

    if(dev_setting_counter % 4) {
        return ;
    }

    if(BUTTON_MODE.down==0 && BUTTON_FAN.down==0 &&  BUTTON_UP.down==1 && BUTTON_DOWN.down==0 && BUTTON_ONOFF.down==0) {
        if((g_set_type == SET_ADDR) && (g_dev.addr < 99)) {
            g_dev.addr++;
        } else if(g_set_type == SET_BAUD) {
            if(BUTTON_UP.shirt > 0x04) {
                return ;
            }
            if(g_dev.baud < BAUD19200) {
                g_dev.baud++;
            } else {
                g_dev.baud = BAUD9600;
            }
        } else if(g_set_type == SET_RF_CH) {
                g_eep.rf_ch++;
        } else if(g_set_type == CLR_EEP) {
            if(g_erasing_index < CLR_ALL) {
                g_erasing_index++;
            } else {
                g_erasing_index = CLR_ID_THIS;
            }
            
        } else if(g_set_type == IR_STUDY) {

        }
    } if(BUTTON_MODE.down==0 && BUTTON_FAN.down==0 &&  BUTTON_UP.down==0 && BUTTON_DOWN.down==1 && BUTTON_ONOFF.down==0) {
        if((g_set_type == SET_ADDR) && (g_dev.addr > 0)) {
            g_dev.addr--;
        }else if(g_set_type == SET_BAUD) {
            if(BUTTON_DOWN.shirt > 0x04) {
                return ;
            }
            if(g_dev.baud > BAUD9600) {
                g_dev.baud--;
            } else {
                g_dev.baud = BAUD19200;
            }
        } else if(g_set_type == SET_RF_CH) {
                g_eep.rf_ch--;
        } else if(g_set_type == CLR_EEP) {
            if(g_erasing_index > CLR_ID_THIS) {
                g_erasing_index--;
            } else {
                g_erasing_index = CLR_ALL;
            }
        } else if(g_set_type == IR_STUDY) {

        }
    } else if(BUTTON_MODE.down==0 && BUTTON_FAN.down==1 &&  BUTTON_UP.down==0 && BUTTON_DOWN.down==0 && BUTTON_ONOFF.down==0) {
        if(BUTTON_FAN.shirt > 44) {
            return ;
        } else if(BUTTON_FAN.shirt > 40) {
//            if(!g_erasing_flag) {
//                g_erasing_flag = 1;
//                printf("erasing eeprom...\n");
//                eeprom_erase_all();
//            }
        } else if(BUTTON_FAN.shirt > 0x04) {
            return ;
        }
        if(g_set_type < IR_RECV) {    // BUTTON FAN changes setting type
            g_set_type++;
        } else if (g_set_type == IR_RECV) {
            g_set_type = SET_ADDR;
        }

        if(       g_set_type == IR_STUDY) {
            g_ir_study_flag = 1;
            g_ir_rf_send = 0;
            g_dev.mode.power = 0;
            g_dev.mode.mod = TEMP_MODE_COOL;
            g_dev.mode.wind = FAN_MIN;
            g_dev.set_temp = 16;
        } else if(g_set_type == IR_SEND) {
            g_ir_study_flag = 0;
            g_ir_rf_flag    = 1;
        } else if(g_set_type == IR_RECV) {
            g_ir_study_flag = 0;
            g_ir_rf_flag    = 1;
        } else {
            g_ir_study_flag = 0;
            g_ir_rf_flag    = 0;
        }
    } else if(BUTTON_MODE.down==1 &&  BUTTON_FAN.down==0 &&  BUTTON_UP.down==0 &&  BUTTON_DOWN.down==0 &&  BUTTON_ONOFF.down==0) {
        if(BUTTON_MODE.shirt > 0x04) {
            return ;
        }

        if(g_set_type == SET_ADDR || g_set_type == SET_BAUD || g_set_type == SET_RF_CH) {

            if(g_saving_flag) {
                return ;
            }

            g_saving_flag = 1;

            if((g_eep.addr == g_dev.addr) && (g_eep.baud_rate == g_dev.baud) && (g_eep.rf_ch == rf_ch_temp)) {
                return ;
            }

            if(g_eep.baud_rate != g_dev.baud) {
                g_eep.baud_rate = g_dev.baud;
                g_uart_baud_changed = 1;
            } else {
                g_uart_baud_changed = 0;
            }

            g_eep.addr      = g_dev.addr;
            g_eep.baud_rate = g_dev.baud;

            eeprom_set_param(EEP_ADDR_THIS, (uint8_t *)(&g_eep), sizeof(g_eep));

        } else if(g_set_type == CLR_EEP) {

            g_erasing_flag = 1;
            eeprom_clr_part(g_erasing_index);

        } else if(g_set_type == IR_STUDY) {
            
        } else if(g_set_type == IR_SEND) {
            g_ir_rf_send    = 1;
            g_ir_rf_recv    = 0;
        } else if(g_set_type == IR_RECV) {
            g_ir_rf_send    = 0;
            g_ir_rf_recv    = 1;
        }
    } else if(BUTTON_MODE.down==0 && BUTTON_FAN.down==0 &&  BUTTON_UP.down==0 && BUTTON_DOWN.down==0 && BUTTON_ONOFF.down==1) {
        if(BUTTON_ONOFF.shirt < 0x08) {

            dev_setting_counter = 0;
            rf_ch_temp = 0;

            g_dev.mode.power = 0;
            g_dev.mode.mod = TEMP_MODE_COOL;
            g_dev.mode.wind = FAN_MIN;
            g_dev.set_temp = 25;

            g_erasing_index = CLR_ID_THIS;
            g_ir_study_flag = 0;
            g_set_type = SET_NONE;

            g_lcd_on = 1;   // TODO: called after key_handler(), maybe ugly
            g_lcd_delay = 100;
            g_led_on = 1;
            g_led_delay = 100;
        }
    }
}

