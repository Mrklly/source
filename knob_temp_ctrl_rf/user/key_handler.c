#include "key_handler.h"
#include "key.h"
#include "uart.h"
#include "cmt2300a.h"
#include "lcd.h"
#include "drv_led.h"
#include "eeprom.h"
#include "wireless.h"


void btn_hander_mode(uint8_t shirt)
{
    if(BUTTON_MODE.shirt == 1) {
        if(g_dev.sta.mode == TEMP_MODE_FAN) {
            g_dev.sta.mode = TEMP_MODE_COOL;
        } else {    // TODO: Now we only use two modes
            g_dev.sta.mode++;
        }

        if(g_dev.sta.mode == TEMP_MODE_FAN) {
            if(g_dev.sta.wind == FAN_CLZ) {
                g_dev.sta.wind = FAN_MIN;
            }

            g_dev.sta.wind_auto = 0;
        }

//        btn_callback(KEY_INDEX_MODE);

    } else if(BUTTON_MODE.shirt == 40) {
//        reg_to_controller();
    }
}

void btn_hander_fan(uint8_t shirt)
{
    if(BUTTON_FAN.shirt == 1 ) {

        if(g_dev.sta.mode == TEMP_MODE_FAN) {
            g_dev.sta.wind_auto = 0;
            if(g_dev.sta.wind == FAN_MAX) {
                g_dev.sta.wind = FAN_MIN;
            } else {
                g_dev.sta.wind++;
            }

            if(g_dev.flag_fan_ultralow && g_dev.sta.wind == FAN_MIN) {
                g_fan_ultralow = 1;
            }

            return ;
        }

        if(g_dev.sta.wind_auto) {
            g_dev.sta.wind_auto = 0;
            g_dev.sta.wind = FAN_CLZ;
            g_fan_ultralow = 0;
        }

        if(g_dev.sta.wind == FAN_MAX) {
            g_dev.sta.wind = FAN_CLZ;
            g_dev.sta.wind_auto = 1;
        } else if(g_dev.sta.wind < FAN_MAX) {
            if(g_fan_ultralow) {
                g_fan_ultralow = 0;
            } else {
                g_dev.sta.wind++;
                if(g_dev.flag_fan_ultralow && g_dev.sta.wind == FAN_MIN) {
                    g_fan_ultralow = 1;
                }
            }
        }

//        btn_callback(KEY_INDEX_FAN);

    }
}

#if TEMP_CTRL_KNOB

void btn_hander_func(uint8_t shirt)  // run at every 50 ms
{
    if(BUTTON_FUNC.shirt == 1 ) {

        if(g_lcd_func == SHOW_TIMING_TASK) {
            g_lcd_func = SHOW_TEMP_ROOM;
        } else {
            g_lcd_func++;
        }

        g_lcd_func_delay = LCD_FUNC_DELAY_TIME;

        if(g_lcd_func == SHOW_TIMING_TASK) {
            g_lcd_func_delay <<= 1;
        }

//        btn_callback(KEY_INDEX_FUNC);
    }
}

#elif TEMP_CTRL_TOUCH

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
//        btn_callback(KEY_INDEX_UP);
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
//        btn_callback(KEY_INDEX_DOWN);
    }
}

#endif

void btn_hander_pwr(uint8_t shirt)
{
    if(BUTTON_PWR.shirt == 1) {
        if(g_lcd_on) {
            g_mod_backup = g_dev.sta.status; // backup mode
            g_mod_backup_flag = 1;
            g_dev.sta.power = 0;
            g_dev.sta.wind = FAN_MIN;
            g_dev.sta.valve = 0;
            g_led_on = 0;
            g_led_delay = 0;
            g_lcd_on = 0;
            g_lcd_delay = 0;
            g_knob_direction = DIRECTION_NONE;
            g_lcd_func = SHOW_TEMP_ROOM;
            g_lcd_func_delay = 0;
            g_timing_task = 0;
            g_counter_min = 0;
            g_counter_sec = 0;
        } else {
            if(g_mod_backup_flag) {
                g_mod_backup_flag = 0;
                g_dev.sta.status = g_mod_backup;
            }
            g_dev.sta.power = 1;
            g_lcd_on = 1;
            g_led_on = 1;
        }

//        btn_callback(KEY_INDEX_PWR);

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

    if((!g_dev.sta.power) && (!g_lcd_on)) {
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
        if(0 < g_lcd_delay && g_lcd_delay < 250*10) {
            g_lcd_delay--;
        }
        if(g_lcd_func_delay > 0) {
            g_lcd_func_delay--;
        }
        lcd_open();
    } else {
        g_lcd_delay = 100;
        lcd_close();
    }

    if(g_lcd_func_delay == 0) {
        g_lcd_func = SHOW_TEMP_ROOM;
    }
}

void device_setting(void)  // run at every 50 ms
{
    static volatile uint16_t dev_setting_counter = 0;
    volatile uint8_t save_status = 0 , led_flash_counter = 3;
    static uint8_t rf_ch_temp;

//    if(BUTTON_MODE.down==1 &&  BUTTON_FAN.down==0 &&  BUTTON_UP.down==1 &&  BUTTON_DOWN.down==0 &&  BUTTON_ONOFF.down==1 ) {
    if(BUTTON_MODE.down && BUTTON_PWR.down ) {

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

    if(BUTTON_FAN.down) {

#if TEMP_CTRL_KNOB
        if(BUTTON_MODE.down || BUTTON_FUNC.down) {
            return ;
        }
#elif TEMP_CTRL_TOUCH
        if(BUTTON_MODE.down || BUTTON_UP.down) {
            return ;
        }
#endif

        if(BUTTON_FAN.shirt > 44) {
            return ;
        } else if(BUTTON_FAN.shirt > 40) {

        } else if(BUTTON_FAN.shirt > 0x04) {
            return ;
        }

        g_lcd_func_delay = 0;

        if(g_set_type < IR_RECV) {    // BUTTON FAN changes setting type
            g_set_type++;
        } else if (g_set_type == IR_RECV) {
            g_set_type = SET_ADDR;
        }

        if(g_dev_type == DEV_TYPE_RS485) {
            if(g_set_type == SET_RF_CH) {
                g_set_type = SET_ADDR;
            }
        }

        if(       g_set_type == IR_STUDY) {
            g_lcd_func_delay = LCD_FUNC_DELAY_TIME;
            g_ir_study_flag = 1;
            g_ir_rf_send = 0;
            g_dev.sta.power = 0;
            g_dev.sta.mode = TEMP_MODE_COOL;
            g_dev.sta.wind = FAN_MIN;
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
    } else if(BUTTON_MODE.down) {

        if(BUTTON_FAN.down) {
            return ;
        }

        if(BUTTON_MODE.shirt > 0x04) {
            return ;
        }

        if(SET_ADDR <= g_set_type && g_set_type <= SET_FAN_ULTRA) {

            if(g_saving_flag) {
                return ;
            }

            g_saving_flag = 1;

//            if((g_eep.addr == g_dev.addr) && (g_eep.baud_rate == g_dev.baud) && (g_eep.rf_ch == rf_ch_temp)) {
//                return ;
//            }

            if(g_eep.baud_rate != g_dev.baud) {
                g_eep.baud_rate = g_dev.baud;
                g_uart_baud_changed = 1;
                uart_init(g_dev.baud);
            } else {
                g_uart_baud_changed = 0;
            }

            g_eep.addr      = g_dev.addr;
            g_eep.baud_rate = g_dev.baud;
            g_eep.pipe_type = g_dev.pipe_type;
            g_eep.temp_offset = g_dev.temp_offset;
            g_eep.led_timeout = g_dev.led_timeout;
            g_eep.flag_fan_ultralow = g_dev.flag_fan_ultralow;

            eeprom_set_param(EEP_ADDR_THIS, (uint8_t *)(&g_eep), sizeof(g_eep));

        } else if(g_set_type == SET_ZIGBEE) {

            if(g_saving_flag) {
                return ;
            }

            g_saving_flag = 1;

            if(g_zig_ctrl == ZIG_CLR) {

                mcu_reset_zigbee();

            } else if(g_zig_ctrl == ZIG_REG) {

                mcu_network_start();

            }

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

    } else if(BUTTON_PWR.down) {

#if TEMP_CTRL_KNOB
        if(BUTTON_MODE.down || BUTTON_FUNC.down) {
            return ;
        }
#elif TEMP_CTRL_TOUCH
        if(BUTTON_MODE.down || BUTTON_UP.down) {
            return ;
        }
#endif
        if(BUTTON_PWR.shirt > 0x04) {
            return ;
        }

        dev_setting_counter = 0;
        rf_ch_temp = 0;

        g_dev.sta.power = 1;
        g_dev.sta.mode = TEMP_MODE_COOL;
        g_dev.sta.wind = FAN_MIN;
        g_dev.set_temp = 25;

        g_erasing_index = CLR_ID_THIS;
        g_ir_study_flag = 0;
        g_set_type = SET_NONE;

        g_lcd_on = 1;   // TODO: called after key_handler(), maybe ugly
        g_lcd_delay = g_dev.led_timeout * 10;
        g_led_on = 1;
        g_led_delay = g_dev.led_timeout * 10;

#if TEMP_CTRL_KNOB
    }
#elif TEMP_CTRL_TOUCH
    } else if(BUTTON_UP.down) {
        if(BUTTON_FAN.down || BUTTON_PWR.down) {
            return ;
        }
//        if(BUTTON_UP.shirt >> 4 == 0) {
//            return ;
//        }

        key_knob_up();

    } else if(BUTTON_DOWN.down) {
        if(BUTTON_FAN.down || BUTTON_UP.down) {
            return ;
        }
//        if(BUTTON_DOWN.shirt >> 4 == 0) {
//            return ;
//        }

        key_knob_down();
    }
#endif
}

void key_knob_up(void)
{
    switch(g_set_type) {
    case SET_NONE:
        if(g_dev.set_temp < 30) {
            g_dev.set_temp++;
        }
        g_lcd_func = SHOW_TEMP_SET;
        break ;

    case SET_ADDR:
        if(g_dev.addr < 99) {
           g_dev.addr++;
        }
        break ;

    case SET_BAUD:
        if(g_dev.baud == BAUD19200) {
           g_dev.baud =  BAUD9600;
        } else if(g_dev.baud == BAUD9600) {
           g_dev.baud =  BAUD19200;
        }
        break ;

    case SET_TEMP_OFFSET:
        if(g_dev.temp_offset < 127) {
            g_dev.temp_offset++;
        }
        break ;

    case SET_PIPE_TYPE:
        if(g_dev.pipe_type == PIPE_DOUB) {
           g_dev.pipe_type =  PIPE_QUAD;
        } else if(g_dev.pipe_type == PIPE_QUAD) {
           g_dev.pipe_type =  PIPE_DOUB;
        } else {
           g_dev.pipe_type =  PIPE_DOUB;
        }
        break ;

    case SET_LED_TIMEOUT:
        g_dev.led_timeout++;
        if(g_dev.led_timeout < 5){
            g_dev.led_timeout = 5;
        }
        break ;

    case SET_FAN_ULTRA:
        if(g_dev.flag_fan_ultralow) {
           g_dev.flag_fan_ultralow = 0;
        } else {
            g_dev.flag_fan_ultralow = 1;
        }
        break ;

    case SET_ZIGBEE:
        if(g_zig_ctrl == ZIG_CLR) {
            g_zig_ctrl = ZIG_REG;
        } else if(g_zig_ctrl == ZIG_CLR) {
            g_zig_ctrl = ZIG_REG;
        } else {
            g_zig_ctrl = ZIG_CLR;
        }
        break ;

    case SET_RF_CH:
        g_eep.rf_ch++;
        break ;

    case CLR_EEP:
        if(g_erasing_index == CLR_ALL) {
            g_erasing_index = CLR_ID_THIS;
        } else {
            g_erasing_index++;
        }
        break ;
    case IR_STUDY:
        break ;
    case IR_SEND:
        break ;
    case IR_RECV:
        break ;
    default :
        break ;
    }
}

void key_knob_down(void)
{
    switch(g_set_type) {
    case SET_NONE:
        if(g_dev.set_temp > 16) {
            g_dev.set_temp--;
        }
        g_lcd_func = SHOW_TEMP_SET;

        break ;

    case SET_ADDR:
        if(g_dev.addr > 1) {
           g_dev.addr--;
        }
        break ;

    case SET_BAUD:
        if(g_dev.baud == BAUD19200) {
           g_dev.baud =  BAUD9600;
        } else if(g_dev.baud == BAUD9600) {
           g_dev.baud =  BAUD19200;
        }
        break ;

    case SET_TEMP_OFFSET:
        if(g_dev.temp_offset > -128) {
            g_dev.temp_offset--;
        }
        break ;

    case SET_PIPE_TYPE:
        if(g_dev.pipe_type == PIPE_DOUB) {
           g_dev.pipe_type =  PIPE_QUAD;
        } else if(g_dev.pipe_type == PIPE_QUAD) {
           g_dev.pipe_type =  PIPE_DOUB;
        }
        break ;

    case SET_LED_TIMEOUT:
        g_dev.led_timeout--;
        if(g_dev.led_timeout < 5) {     // minimum 5s
            g_dev.led_timeout = 255;
        }
        break ;

    case SET_FAN_ULTRA:
        if(g_dev.flag_fan_ultralow) {
           g_dev.flag_fan_ultralow = 0;
        } else {
            g_dev.flag_fan_ultralow = 1;
        }
        break ;

    case SET_ZIGBEE:
        if(g_zig_ctrl == ZIG_CLR) {
            g_zig_ctrl = ZIG_REG;
        } else if(g_zig_ctrl == ZIG_CLR) {
            g_zig_ctrl = ZIG_REG;
        } else {
            g_zig_ctrl = ZIG_CLR;
        }
        break ;

    case SET_RF_CH:
        g_eep.rf_ch--;
        break ;

    case CLR_EEP:
        if(g_erasing_index == CLR_ID_THIS) {
            g_erasing_index = CLR_ALL;
        } else {
            g_erasing_index--;
        }
        break ;

    case IR_STUDY:
        break ;
    case IR_SEND:
        break ;
    case IR_RECV:
        break ;
    default :
        break ;
    }
}

