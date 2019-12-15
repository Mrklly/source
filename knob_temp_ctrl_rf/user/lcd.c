#include "drv_led.h"
#include "eeprom.h"
#include "global_variables.h"

#if TEMP_CTRL_KNOB
#include "lcd.h"
#elif TEMP_CTRL_TOUCH
#include "lcd_touch.h"
#endif


volatile uint32_t lcd_ram[LCD_RAM_USED] = {0};


#if TEMP_CTRL_KNOB

const uint32_t lcd_number[3][10] = {
    {SEG_0(0), SEG_1(0), SEG_2(0), SEG_3(0), SEG_4(0), SEG_5(0), SEG_6(0), SEG_7(0), SEG_8(0), SEG_9(0)},
    {SEG_0(1), SEG_1(1), SEG_2(1), SEG_3(1), SEG_4(1), SEG_5(1), SEG_6(1), SEG_7(1), SEG_8(1), SEG_9(1)},
    {SEG_0(2), SEG_1(2), SEG_2(2), SEG_3(2), SEG_4(2), SEG_5(2), SEG_6(2), SEG_7(2), SEG_8(2), SEG_9(2)}
};

#elif TEMP_CTRL_TOUCH

const uint32_t lcd_number[6][10] = {
    {SEG_0(0), SEG_1(0), SEG_2(0), SEG_3(0), SEG_4(0), SEG_5(0), SEG_6(0), SEG_7(0), SEG_8(0), SEG_9(0)},
    {SEG_0(1), SEG_1(1), SEG_2(1), SEG_3(1), SEG_4(1), SEG_5(1), SEG_6(1), SEG_7(1), SEG_8(1), SEG_9(1)},
    {SEG_0(2), SEG_1(2), SEG_2(2), SEG_3(2), SEG_4(2), SEG_5(2), SEG_6(2), SEG_7(2), SEG_8(2), SEG_9(2)},
    {SEG_0(3), SEG_1(3), SEG_2(3), SEG_3(3), SEG_4(3), SEG_5(3), SEG_6(3), SEG_7(3), SEG_8(3), SEG_9(3)},
    {SEG_0(4), SEG_1(4), SEG_2(4), SEG_3(4), SEG_4(4), SEG_5(4), SEG_6(4), SEG_7(4), SEG_8(4), SEG_9(4)},
    {SEG_0(5), SEG_1(5), SEG_2(5), SEG_3(5), SEG_4(5), SEG_5(5), SEG_6(5), SEG_7(5), SEG_8(5), SEG_9(5)}
};

#endif


void lcd_update(void)
{
    if(!g_lcd_on) {
        return ;
    }

    lcd_ram_all_clean();

    if(g_set_type) {

        lcd_show_setting();

        return ;
    }

    if(       g_lcd_func == SHOW_TEMP_ROOM) {
        lcd_show_temp((uint16_t)(10 * g_dev.room_temp));
    } else if(g_lcd_func == SHOW_TEMP_SET) {
        lcd_show_temp((uint16_t)(10 * g_dev.set_temp));
    } else if(g_lcd_func == SHOW_TIMING_TASK) {
        lcd_show_temp((uint16_t)(g_timing_task * 5));
    }

    lcd_show_all_header();
    lcd_show_mode();
    lcd_show_fan();
}

void lcd_ram_all_clean(void)
{
    for(uint8_t i=0; i<LCD_RAM_USED; i++) {
        lcd_ram[i] = 0;
    }
}

void lcd_screen_update(void)
{
    for(uint8_t i=0; i<LCD_RAM_USED; i++) {
        LCD_WriteRam0_7Int32(i, lcd_ram[i]);
    }
}

void lcd_screen_clear(void)
{
    for(uint8_t i=0; i<LCD_RAM_USED; i++) {
        LCD_WriteRam0_7Int32(i, 0);
    }
}

void lcd_digit_all_clean(void)
{
#if TEMP_CTRL_KNOB

    lcd_ram[3] &= ~lcd_number[0][8];     // seg number 0

    lcd_ram[2] &= ~lcd_number[1][8];     // seg number 1

//    lcd_ram[1] |= SEG_2_H;
    lcd_ram[1] &= ~lcd_number[2][8];     // seg number 2
//    lcd_ram[2] |= SEG_2_L;
    lcd_ram[2] &= ~lcd_number[2][8];

#elif TEMP_CTRL_TOUCH

    lcd_clear_num_set();

#endif
}

void lcd_show_all_header(void)
{
    SEG_T1_ON;          // T1 MODE
    SEG_T7_ON;          // T7, horizontal lines

#if TEMP_CTRL_KNOB

    if(       g_lcd_func == SHOW_TEMP_ROOM) {
        SEG_T8_ON;          // T8, Room icon
        SEG_T9_OF;          // T9, Setting icon

        if(g_timing_task) {
            SEG_T10_ON;     // T10, Timer icon
        } else {
            SEG_T10_OF;
        }

        SEG_P_ON;           // P, Point icon
        SEG_P1_ON;          // P1, Celsius icon
    } else if(g_lcd_func == SHOW_TEMP_SET) {
        SEG_T8_OF;
        SEG_T9_ON;

        if(g_timing_task) {
            SEG_T10_ON;         // T10, Timer icon
        } else {
            SEG_T10_OF;
        }

        SEG_P_ON;
        SEG_P1_ON;
    } else if(g_lcd_func == SHOW_TIMING_TASK) {
        SEG_T8_OF;
        SEG_T9_OF;
        SEG_T10_ON;
        SEG_P_ON;
        SEG_P1_OF;
    }

    SEG_T11_ON;         // T11, horizontal lines
    SEG_T12_ON;         // T12, Fan icon

#elif TEMP_CTRL_TOUCH

    SEG_T8_ON;          // T8, FAN icon
    SEG_T12_ON;         // T12, setting temp
    SEG_T13_ON;         // T13, setting temp point
    SEG_T14_OF;         // T14
    SEG_T15_ON;         // T15, celsius icon
    SEG_T16_ON;         // T16, room temp
    SEG_T17_ON;         // T17, root temp point
    SEG_T18_OF;         // T18
    SEG_T19_ON;         // T19, celsius icon

#endif
}

void lcd_show_all_digit(void)
{
    uint8_t i=0;

    for(; i<10; i++) {

#if TEMP_CTRL_KNOB

        lcd_ram[3] |= lcd_number[0][i];     // seg number 0

        lcd_ram[2] |= lcd_number[1][i];     // seg number 1

        lcd_ram[1] |= lcd_number[2][i];     // seg number 2
        lcd_ram[1] &= ~SEG_2_H;
        lcd_ram[2] |= lcd_number[2][i];
        lcd_ram[2] &= ~SEG_2_L;

#elif TEMP_CTRL_TOUCH

        lcd_ram[0] |= lcd_number[0][i];     // seg number 0

        lcd_ram[1] |= lcd_number[1][i];     // seg number 1
        lcd_ram[1] &=~SEG_1_L;
        lcd_ram[2] |= lcd_number[1][i];
        lcd_ram[2] &=~SEG_1_H;

        lcd_ram[2] |= lcd_number[2][i];     // seg number 2

        lcd_ram[4] |= lcd_number[3][i];     // seg number 3
        lcd_ram[4] &= (~SEG_3_H);
        lcd_ram[5] |= lcd_number[3][i];
        lcd_ram[5] &= (~SEG_3_L);

        lcd_ram[4] |= lcd_number[4][i];     // seg number 4

        lcd_ram[3] |= lcd_number[5][i];     // seg number 5

#endif

        for(uint8_t j = 0; j<LCD_RAM_USED; j++) {
            LCD_WriteRam0_7Int32(j, lcd_ram[j]);
        }

        delay_ms(500);

        lcd_digit_all_clean();
    }
}

void lcd_show_mode(void)
{
    switch(g_dev.sta.mode) {
    case TEMP_MODE_COOL:
        SEG_T2_ON;      // T2
        SEG_T3_OF;
        SEG_T4_OF;
        SEG_T5_OF;
        SEG_T6_OF;
        break;
    case TEMP_MODE_WARM:
        SEG_T2_OF;      // T3
        SEG_T3_ON;
        SEG_T4_OF;
        SEG_T5_OF;
        SEG_T6_OF;
        break;
    case TEMP_MODE_FAN:
        SEG_T2_OF;      // T4
        SEG_T3_OF;
        SEG_T4_ON;
        SEG_T5_OF;
        SEG_T6_OF;
        break;
    case TEMP_MODE_DRY:
        SEG_T2_OF;      // T5
        SEG_T3_OF;
        SEG_T4_OF;
        SEG_T5_ON;
        SEG_T6_OF;
        break;
    case TEMP_MODE_AUTO:
        SEG_T2_OF;      // T6
        SEG_T3_OF;
        SEG_T4_OF;
        SEG_T5_OF;
        SEG_T6_ON;
        break;
    default:
        break;
    }
}

void lcd_show_mode_all(void)
{
    SEG_T2_ON;
    SEG_T3_ON;
    SEG_T4_ON;
    SEG_T5_ON;
    SEG_T6_ON;
}


void lcd_show_fan(void)
{
    static uint16_t counter = 0;
    switch(g_dev.sta.wind) {   // T9 T10 T11

#if TEMP_CTRL_KNOB

    case FAN_CLZ:
        SEG_T13_OF;
        SEG_T14_OF;
        SEG_T15_OF;
        break;
    case FAN_MIN:
        SEG_T13_ON;
        SEG_T14_OF;
        SEG_T15_OF;
        break;
    case FAN_MID:
        SEG_T13_ON;
        SEG_T14_ON;
        SEG_T15_OF;
        break;
    case FAN_MAX:
        SEG_T13_ON;
        SEG_T14_ON;
        SEG_T15_ON;
        break;

#elif TEMP_CTRL_TOUCH

    case FAN_CLZ:
        SEG_T9_OF;
        SEG_T10_OF;
        SEG_T11_OF;
        break;
    case FAN_MIN:
        SEG_T9_ON;
        SEG_T10_OF;
        SEG_T11_OF;
        break;
    case FAN_MID:
        SEG_T9_ON;
        SEG_T10_ON;
        SEG_T11_OF;
        break;
    case FAN_MAX:
        SEG_T9_ON;
        SEG_T10_ON;
        SEG_T11_ON;
        break;

#endif

    default:
        break;
    }

#if TEMP_CTRL_KNOB

    if(g_dev.sta.wind_auto) {
        SEG_T16_ON;
    } else {
        SEG_T16_OF;
    }

#endif

    if(g_fan_ultralow && g_dev.sta.wind == FAN_MIN) {
        counter++;
        if(      counter == 10) {
            counter = 0;
        }else if(counter > 5) {
#if TEMP_CTRL_KNOB
            SEG_T13_ON;
        } else {
            SEG_T13_OF;
        }
#elif TEMP_CTRL_TOUCH
            SEG_T9_ON;
        } else {
            SEG_T9_OF;
        }
#endif
    }
}

void lcd_show_fan_all(void)
{
#if TEMP_CTRL_KNOB
    SEG_T13_ON;
    SEG_T14_ON;
    SEG_T15_ON;
    SEG_T16_ON;
#elif TEMP_CTRL_TOUCH
    SEG_T9_ON;
    SEG_T10_ON;
    SEG_T11_ON;
#endif
}

void lcd_show_all(void)
{
    lcd_ram_all_clean();

#if TEMP_CTRL_KNOB

    SEG_T1_ON;          // T1 MODE
    SEG_T2_ON;
    SEG_T3_ON;
    SEG_T4_ON;
    SEG_T5_ON;
    SEG_T6_ON;
    SEG_T7_ON;          // T7, horizontal lines
    SEG_T8_ON;          // T8, Room icon
    SEG_T9_ON;
    SEG_T10_ON;         // T10, Timer icon
    SEG_T11_ON;         // T11, horizontal lines
    SEG_T12_ON;         // T12, Fan icon
    SEG_T13_ON;
    SEG_T14_ON;
    SEG_T15_ON;
    SEG_T16_ON;
    SEG_P_ON;           // P, Point icon
    SEG_P1_ON;          // P1, Celsius icon

    lcd_show_temp(888);

#elif TEMP_CTRL_TOUCH


#endif

    for(uint8_t j = 0; j<LCD_RAM_USED; j++) {
        LCD_WriteRam0_7Int32(j, lcd_ram[j]);
    }
}

void lcd_show_setting_temp(void)
{
#if TEMP_CTRL_KNOB


#elif TEMP_CTRL_TOUCH

    uint16_t setting_temp = g_dev.set_temp * 10;
    uint8_t digit[3] = {0, 0, 0};

    digit[0] = setting_temp / 100 % 10;
    digit[1] = setting_temp / 10 % 10;
    digit[2] = setting_temp % 10;

    lcd_ram[0] |= lcd_number[0][digit[0]];     // seg number 0

    lcd_ram[1] |= lcd_number[1][digit[1]];     // seg number 1
    lcd_ram[1] &=~SEG_1_L;
    lcd_ram[2] |= lcd_number[1][digit[1]];
    lcd_ram[2] &=~SEG_1_H;

    lcd_ram[2] |= lcd_number[2][digit[2]];     // seg number 2

#endif
}

void lcd_show_room_temp(void)
{
    uint16_t room_temp = (uint16_t)(g_dev.room_temp * 10);
    uint8_t digit[3] = {0, 0, 0};

    digit[0] = room_temp / 100 % 10;
    digit[1] = room_temp / 10 % 10;
    digit[2] = room_temp % 10;

#if TEMP_CTRL_KNOB

    lcd_ram[3] |= lcd_number[0][digit[0]];     // seg number 0

    lcd_ram[2] |= lcd_number[1][digit[1]];     // seg number 1

    lcd_ram[1] |= lcd_number[2][digit[2]];     // seg number 2
    lcd_ram[1] &= ~SEG_2_H;
    lcd_ram[2] |= lcd_number[2][digit[2]];
    lcd_ram[2] &= ~SEG_2_L;

#elif TEMP_CTRL_TOUCH

    lcd_ram[4] |= lcd_number[3][digit[0]];     // seg number 3
    lcd_ram[4] &= ~SEG_3_H;
    lcd_ram[5] |= lcd_number[3][digit[0]];
    lcd_ram[5] &= ~SEG_3_L;

    lcd_ram[4] |= lcd_number[4][digit[1]];     // seg number 4

    lcd_ram[3] |= lcd_number[5][digit[2]];     // seg number 5

#endif
}

#if TEMP_CTRL_KNOB

void lcd_show_num(uint16_t num)
{
    uint8_t digit[3] = {0, 0, 0};

    digit[0] = num / 100 % 10;
    digit[1] = num / 10 % 10;
    digit[2] = num % 10;

    if(digit[0]) {
        lcd_ram[3] |= lcd_number[0][digit[0]]; // seg number 0
    } else {
        lcd_ram[3] &=~lcd_number[0][8];
    }

    lcd_ram[2] |= lcd_number[1][digit[1]];     // seg number 1

    lcd_ram[1] |= lcd_number[2][digit[2]];     // seg number 2
    lcd_ram[1] &= ~SEG_2_H;
    lcd_ram[2] |= lcd_number[2][digit[2]];
    lcd_ram[2] &= ~SEG_2_L;
}

void lcd_clear_num(void)
{
    lcd_ram[3] &=~lcd_number[0][8];     // seg number 0

    lcd_ram[2] &=~lcd_number[1][8];     // seg number 1

    lcd_ram[1] &=~lcd_number[2][8];     // seg number 2
    lcd_ram[1] &=~SEG_2_H;
    lcd_ram[2] &=~lcd_number[2][8];
    lcd_ram[2] &=~SEG_2_L;
}

#elif TEMP_CTRL_TOUCH

void lcd_show_num_set(uint16_t num)
{
    uint8_t digit[3] = {0, 0, 0};

    digit[0] = num / 100 % 10;
    digit[1] = num / 10 % 10;
    digit[2] = num % 10;

    if(digit[0]) {
        lcd_ram[0] |= lcd_number[0][digit[0]]; // seg number 0
    } else {
        lcd_ram[0] &=~lcd_number[0][8];
    }

    lcd_ram[1] |= lcd_number[1][digit[1]];     // seg number 1
    lcd_ram[1] &=~SEG_1_L;
    lcd_ram[2] |= lcd_number[1][digit[1]];
    lcd_ram[2] &=~SEG_1_H;

    lcd_ram[2] |= lcd_number[2][digit[2]];     // seg number 2
}

void lcd_clear_num_set(void)
{
    lcd_ram[0] &= ~lcd_number[0][8];     // seg number 0

    lcd_ram[1] &= ~lcd_number[1][8];     // seg number 1
    lcd_ram[2] &= ~lcd_number[1][8];

    lcd_ram[2] &= ~lcd_number[2][8];     // seg number 2
}


void lcd_show_num_room(uint16_t num)
{
    uint8_t digit[3] = {0, 0, 0};

    digit[0] = num / 100 % 10;
    digit[1] = num / 10 % 10;
    digit[2] = num % 10;

    if(digit[0]) {
        lcd_ram[4] |= lcd_number[3][digit[0]];     // seg number 3
        lcd_ram[4] &=~SEG_3_H;
        lcd_ram[5] |= lcd_number[3][digit[0]];
        lcd_ram[5] &=~SEG_3_L;
    } else {
        lcd_ram[4] &=~lcd_number[3][digit[0]];
        lcd_ram[5] &=~lcd_number[3][digit[0]];
    }

    lcd_ram[4] |= lcd_number[4][digit[1]];     // seg number 4

    lcd_ram[3] |= lcd_number[5][digit[2]];     // seg number 5
}

void lcd_clear_num_room(void)
{
    lcd_ram[4] &= ~lcd_number[3][8];     // seg number 3
    lcd_ram[5] &= ~lcd_number[3][8];

    lcd_ram[4] &= ~lcd_number[4][8];     // seg number 4

    lcd_ram[3] &= ~lcd_number[5][8];     // seg number 5
}

#endif

void lcd_show_temp(uint16_t temp)
{
#if TEMP_CTRL_KNOB

    lcd_show_num(temp);

#elif TEMP_CTRL_TOUCH

    lcd_show_setting_temp();
    lcd_show_room_temp();

#endif
}

void lcd_show_num_minus(int8_t num)
{
    uint8_t digit[2] = {0, 0};

    if((num > 99) || (num < -99)) {

        HIDE_DIGIT_POINT;

#if TEMP_CTRL_KNOB

        lcd_ram[3] |= SEG_SHOW_E(0);     // seg number 0

        lcd_ram[2] |= SEG_SHOW_r(1);     // seg number 1

        lcd_ram[1] |= SEG_SHOW_r(2);     // seg number 2
        lcd_ram[1] &=~SEG_2_H;
        lcd_ram[2] |= SEG_SHOW_r(2);
        lcd_ram[2] &=~SEG_2_L;

#elif TEMP_CTRL_TOUCH

        lcd_ram[4] |= SEG_SHOW_E(3);     // seg number 3
        lcd_ram[4] &= ~SEG_3_H;
        lcd_ram[5] |= SEG_SHOW_E(4);
        lcd_ram[5] &= ~SEG_3_L;

        lcd_ram[4] |= SEG_SHOW_r(5);     // seg number 4

        lcd_ram[3] |= SEG_SHOW_r(5);     // seg number 5

#endif

        return ;
    }

    digit[0] = num / 10 % 10;
    digit[1] = num % 10;

    SHOW_DIGIT_MINUS;                          // seg number 0

#if TEMP_CTRL_KNOB

    lcd_ram[2] |= lcd_number[1][digit[0]];     // seg number 1

    lcd_ram[1] |= lcd_number[2][digit[1]];     // seg number 2
    lcd_ram[1] &= ~SEG_2_H;
    lcd_ram[2] |= lcd_number[2][digit[1]];
    lcd_ram[2] &= ~SEG_2_L;

#elif TEMP_CTRL_TOUCH

    lcd_ram[4] |= lcd_number[4][digit[0]];     // seg number 4

    lcd_ram[3] |= lcd_number[5][digit[1]];     // seg number 5

#endif
}

static void lcd_show_ir_temp(void)
{
    uint16_t setting_temp = g_dev.set_temp;
    uint8_t digit[2] = {0, 0};

#if TEMP_CTRL_KNOB

    if(g_lcd_func_delay) {
        return ;
    }

#endif

    digit[0] = setting_temp / 10 % 10;
    digit[1] = setting_temp % 10;

#if TEMP_CTRL_KNOB

    lcd_ram[2] |= lcd_number[1][digit[0]];     // seg number 1

    lcd_ram[1] |= lcd_number[2][digit[1]];     // seg number 2
    lcd_ram[1] &= ~SEG_2_H;
    lcd_ram[2] |= lcd_number[2][digit[1]];
    lcd_ram[2] &= ~SEG_2_L;

#elif TEMP_CTRL_TOUCH

    lcd_ram[4] |= lcd_number[3][digit[0]];     // seg number 3
    lcd_ram[4] &= (~SEG_3_H);
    lcd_ram[5] |= lcd_number[3][digit[0]];
    lcd_ram[5] &= (~SEG_3_L);

    lcd_ram[4] |= lcd_number[4][digit[1]];     // seg number 4

#endif
}

static void lcd_show_addr(void)
{
#if TEMP_CTRL_KNOB

    lcd_ram[3] |= SEG_SHOW_A(0);     // seg number 0

    lcd_ram[2] |= SEG_SHOW_d(1);     // seg number 1

    lcd_ram[1] |= SEG_SHOW_r(2);     // seg number 2
    lcd_ram[1] &=~SEG_2_H;
    lcd_ram[2] |= SEG_SHOW_r(2);
    lcd_ram[2] &=~SEG_2_L;

#elif TEMP_CTRL_TOUCH

    lcd_ram[0] |= SEG_SHOW_A(0);     // seg number 0

    lcd_ram[1] |= SEG_SHOW_d(1);     // seg number 1
    lcd_ram[1] &=~SEG_1_L;
    lcd_ram[2] |= SEG_SHOW_d(1);
    lcd_ram[2] &=~SEG_1_H;

    lcd_ram[2] |= SEG_SHOW_r(2);     // seg number 2

#endif
}

static void lcd_show_baud(void)
{
#if TEMP_CTRL_KNOB

    lcd_ram[3] |= SEG_SHOW_b(0);     // seg number 0

    lcd_ram[2] |= SEG_SHOW_U(1);     // seg number 1

    lcd_ram[1] |= SEG_SHOW_d(2);     // seg number 2
    lcd_ram[1] &=~SEG_2_H;
    lcd_ram[2] |= SEG_SHOW_d(2);
    lcd_ram[2] &=~SEG_2_L;

#elif TEMP_CTRL_TOUCH

    lcd_ram[0] |= SEG_SHOW_b(0);     // seg number 0

    lcd_ram[1] |= SEG_SHOW_U(1);     // seg number 1
    lcd_ram[1] &=~SEG_1_L;
    lcd_ram[2] |= SEG_SHOW_U(1);
    lcd_ram[2] &=~SEG_1_H;

    lcd_ram[2] |= SEG_SHOW_d(2);     // seg number 2

#endif
}

static void lcd_show_temp_offset(void)
{
#if TEMP_CTRL_KNOB

    lcd_ram[3] |= SEG_SHOW_O(0);     // seg number 0

    lcd_ram[2] |= SEG_SHOW_F(1);     // seg number 1

    lcd_ram[1] |= SEG_SHOW_S(2);     // seg number 2
    lcd_ram[1] &=~SEG_2_H;
    lcd_ram[2] |= SEG_SHOW_S(2);
    lcd_ram[2] &=~SEG_2_L;

#elif TEMP_CTRL_TOUCH

    lcd_ram[0] |= SEG_SHOW_O(0);     // seg number 0

    lcd_ram[1] |= SEG_SHOW_F(1);     // seg number 1
    lcd_ram[1] &=~SEG_1_L;
    lcd_ram[2] |= SEG_SHOW_F(1);
    lcd_ram[2] &=~SEG_1_H;

    lcd_ram[2] |= SEG_SHOW_S(2);     // seg number 2

#endif
}

static void lcd_show_led_timeout(void)
{
#if TEMP_CTRL_KNOB

    lcd_ram[3] |= SEG_SHOW_L(0);     // seg number 0

    lcd_ram[2] |= SEG_SHOW_E(1);     // seg number 1

    lcd_ram[1] |= SEG_SHOW_d(2);     // seg number 2
    lcd_ram[1] &=~SEG_2_H;
    lcd_ram[2] |= SEG_SHOW_d(2);
    lcd_ram[2] &=~SEG_2_L;

#elif TEMP_CTRL_TOUCH

    lcd_ram[0] |= SEG_SHOW_L(0);     // seg number 0

    lcd_ram[1] |= SEG_SHOW_E(1);     // seg number 1
    lcd_ram[1] &=~SEG_1_L;
    lcd_ram[2] |= SEG_SHOW_E(1);
    lcd_ram[2] &=~SEG_1_H;

    lcd_ram[2] |= SEG_SHOW_d(2);     // seg number 2

#endif
}

static void lcd_show_fan_ultra_low(void)
{
#if TEMP_CTRL_KNOB

    lcd_ram[3] |= SEG_SHOW_F(0);     // seg number 0

    lcd_ram[2] |= SEG_SHOW_A(1);     // seg number 1

    lcd_ram[1] |= SEG_SHOW_N(2);     // seg number 2
    lcd_ram[1] &=~SEG_2_H;
    lcd_ram[2] |= SEG_SHOW_N(2);
    lcd_ram[2] &=~SEG_2_L;

#elif TEMP_CTRL_TOUCH

    lcd_ram[0] |= SEG_SHOW_F(0);     // seg number 0

    lcd_ram[1] |= SEG_SHOW_A(1);     // seg number 1
    lcd_ram[1] &=~SEG_1_L;
    lcd_ram[2] |= SEG_SHOW_A(1);
    lcd_ram[2] &=~SEG_1_H;

    lcd_ram[2] |= SEG_SHOW_N(2);     // seg number 2

#endif
}

static void lcd_show_zigbee(void)
{
#if TEMP_CTRL_KNOB

    lcd_ram[3] |= lcd_number[0][2];  // seg number 0

    lcd_ram[2] |= SEG_SHOW_I(1);     // seg number 1

    lcd_ram[1] |= SEG_SHOW_G(2);     // seg number 2
    lcd_ram[1] &=~SEG_2_H;
    lcd_ram[2] |= SEG_SHOW_G(2);
    lcd_ram[2] &=~SEG_2_L;

#elif TEMP_CTRL_TOUCH

    lcd_ram[0] |= lcd_number[0][2];  // seg number 0

    lcd_ram[1] |= SEG_SHOW_I(1);     // seg number 1
    lcd_ram[1] &=~SEG_1_L;
    lcd_ram[2] |= SEG_SHOW_I(1);
    lcd_ram[2] &=~SEG_1_H;

    lcd_ram[2] |= SEG_SHOW_G(2);     // seg number 2

#endif
}

static void lcd_show_rf_ch(void)
{
#if TEMP_CTRL_KNOB

    lcd_ram[3] |= SEG_SHOW_r(0);     // seg number 0

    lcd_ram[2] |= SEG_SHOW_F(1);     // seg number 1

    lcd_ram[1] |= SEG_SHOW_C(2);     // seg number 2
    lcd_ram[1] &=~SEG_2_H;
    lcd_ram[2] |= SEG_SHOW_C(2);
    lcd_ram[2] &=~SEG_2_L;

#elif TEMP_CTRL_TOUCH

    lcd_ram[0] |= SEG_SHOW_r(0);     // seg number 0

    lcd_ram[1] |= SEG_SHOW_F(1);     // seg number 1
    lcd_ram[1] &=~SEG_1_L;
    lcd_ram[2] |= SEG_SHOW_F(1);
    lcd_ram[2] &=~SEG_1_H;

    lcd_ram[2] |= SEG_SHOW_C(2);     // seg number 2

#endif
}

static void lcd_show_pipe_type(void)
{
#if TEMP_CTRL_KNOB

    lcd_ram[3] |= SEG_SHOW_P(0);     // seg number 0

    lcd_ram[2] |= SEG_SHOW_I(1);     // seg number 1

    lcd_ram[1] |= SEG_SHOW_P(2);     // seg number 2
    lcd_ram[1] &=~SEG_2_H;
    lcd_ram[2] |= SEG_SHOW_P(2);
    lcd_ram[2] &=~SEG_2_L;

#elif TEMP_CTRL_TOUCH

    lcd_ram[0] |= SEG_SHOW_P(0);     // seg number 0

    lcd_ram[1] |= SEG_SHOW_I(1);     // seg number 1
    lcd_ram[1] &=~SEG_1_L;
    lcd_ram[2] |= SEG_SHOW_I(1);
    lcd_ram[2] &=~SEG_1_H;

    lcd_ram[2] |= SEG_SHOW_P(2);     // seg number 2

#endif
}

static void lcd_show_clr(void)
{
#if TEMP_CTRL_KNOB

    lcd_ram[3] |= SEG_SHOW_C(0);     // seg number 0

    lcd_ram[2] |= SEG_SHOW_L(1);     // seg number 1

    lcd_ram[1] |= SEG_SHOW_R(2);     // seg number 2
    lcd_ram[1] &=~SEG_2_H;
    lcd_ram[2] |= SEG_SHOW_R(2);
    lcd_ram[2] &=~SEG_2_L;

#elif TEMP_CTRL_TOUCH

    lcd_ram[0] |= SEG_SHOW_C(0);     // seg number 0

    lcd_ram[1] |= SEG_SHOW_L(1);     // seg number 1
    lcd_ram[1] &=~SEG_1_L;
    lcd_ram[2] |= SEG_SHOW_L(1);
    lcd_ram[2] &=~SEG_1_H;

    lcd_ram[2] |= SEG_SHOW_R(2);     // seg number 2

#endif
}

static void lcd_show_ir_study(void)
{
#if TEMP_CTRL_KNOB

    lcd_ram[3] |= SEG_SHOW_S(0);     // seg number 0

    lcd_ram[2] |= SEG_SHOW_d(1);     // seg number 1

    lcd_ram[1] |= SEG_SHOW_Y(2);     // seg number 2
    lcd_ram[1] &=~SEG_2_H;
    lcd_ram[2] |= SEG_SHOW_Y(2);
    lcd_ram[2] &=~SEG_2_L;

#elif TEMP_CTRL_TOUCH

    lcd_ram[0] |= SEG_SHOW_S(0);     // seg number 0

    lcd_ram[1] |= SEG_SHOW_d(1);     // seg number 1
    lcd_ram[1] &=~SEG_1_L;
    lcd_ram[2] |= SEG_SHOW_d(1);
    lcd_ram[2] &=~SEG_1_H;

    lcd_ram[2] |= SEG_SHOW_Y(2);     // seg number 2

    lcd_ram[3] = lcd_ram[4] = lcd_ram[5] = 0;

#endif
}

static void lcd_show_ir_send(void)
{
#if TEMP_CTRL_KNOB

    lcd_ram[3] |= SEG_SHOW_I(0);           // seg number 0

    lcd_ram[2] |= SEG_SHOW_r(1);           // seg number 1

    lcd_ram[1] |= SEG_SHOW_S(2);           // seg number 2
    lcd_ram[1] &=~SEG_2_H;
    lcd_ram[2] |= SEG_SHOW_S(2);
    lcd_ram[2] &=~SEG_2_L;

#elif TEMP_CTRL_TOUCH

    lcd_ram[0] |= SEG_SHOW_I(0);           // seg number 0

    lcd_ram[1] |= SEG_SHOW_r(1);           // seg number 1
    lcd_ram[1] &=~SEG_1_L;
    lcd_ram[2] |= SEG_SHOW_r(1);
    lcd_ram[2] &=~SEG_1_H;

    lcd_ram[2] |= SEG_SHOW_S(2);           // seg number 2

    lcd_ram[3] = lcd_ram[4] = lcd_ram[5] = 0;

#endif
}

static void lcd_show_ir_recv(void)
{
#if TEMP_CTRL_KNOB

    lcd_ram[3] |= SEG_SHOW_I(0);           // seg number 0

    lcd_ram[2] |= SEG_SHOW_r(1);           // seg number 1

    lcd_ram[1] |= SEG_SHOW_r(2);           // seg number 2
    lcd_ram[1] &=~SEG_2_H;
    lcd_ram[2] |= SEG_SHOW_r(2);
    lcd_ram[2] &=~SEG_2_L;

#elif TEMP_CTRL_TOUCH

    lcd_ram[0] |= SEG_SHOW_I(0);           // seg number 0

    lcd_ram[1] |= SEG_SHOW_r(1);           // seg number 1
    lcd_ram[1] &=~SEG_1_L;
    lcd_ram[2] |= SEG_SHOW_r(1);
    lcd_ram[2] &=~SEG_1_H;

    lcd_ram[2] |= SEG_SHOW_r(2);           // seg number 2

    lcd_ram[3] = lcd_ram[4] = lcd_ram[5] = 0;

#endif
}

void lcd_show_setting_item(void)
{
#if TEMP_CTRL_KNOB

    lcd_clear_num();

#elif TEMP_CTRL_TOUCH

    lcd_clear_num_set();

#endif

    switch(g_set_type) {
    case SET_ADDR:
        lcd_show_addr();
        break;
    case SET_BAUD:
        lcd_show_baud();
        break;
    case SET_TEMP_OFFSET:
        lcd_show_temp_offset();
        break;
    case SET_LED_TIMEOUT:
        lcd_show_led_timeout();
        break;
    case SET_FAN_ULTRA:
        lcd_show_fan_ultra_low();
        break ;
    case SET_ZIGBEE:
        lcd_show_zigbee();
        break ;
    case SET_RF_CH:
        lcd_show_rf_ch();
        break;
    case SET_PIPE_TYPE:
        lcd_show_pipe_type();
        break;
    case CLR_EEP:
        lcd_show_clr();
        break;
    case IR_STUDY:
        lcd_show_ir_study();
        break;
    case IR_SEND:
        lcd_show_ir_send();
        break;
    case IR_RECV:
        lcd_show_ir_recv();
        break;
    default:
        break;
    }
}


void lcd_show_setting_content(void)
{
    volatile uint8_t data_temp = 0;
    volatile int8_t temp_bias;

#if TEMP_CTRL_KNOB

    lcd_clear_num();

#elif TEMP_CTRL_TOUCH

    lcd_clear_num_room();

#endif

    switch(g_set_type) {
    case SET_NONE:
        break ;

    case SET_ADDR:
        data_temp = g_dev.addr;
        break ;

    case SET_BAUD:
        if(g_dev.baud == BAUD9600) {
            data_temp = 96;
        } else if(g_dev.baud == BAUD19200) {
            data_temp = 192;
        } else {
            data_temp = 0;
        }
        break ;

    case SET_TEMP_OFFSET:
        data_temp = g_dev.temp_offset;
        SHOW_DIGIT_POINT;
        if(g_dev.temp_offset < 0) {
            SHOW_DIGIT_MINUS;
            data_temp--;
            data_temp = ~data_temp;
            lcd_show_num_minus(data_temp);
            return ;
        }
        break ;

    case SET_LED_TIMEOUT:
        data_temp = g_dev.led_timeout;
        break ;

    case SET_FAN_ULTRA:
        data_temp = g_dev.flag_fan_ultralow;
        break ;

    case SET_ZIGBEE:
        data_temp = g_zig_ctrl;
        break ;

    case SET_RF_CH:
        data_temp = g_eep.rf_ch;
        break ;

    case SET_PIPE_TYPE:

        data_temp = g_dev.pipe_type;

        break ;

    case CLR_EEP:
        data_temp = g_erasing_index;
        break ;

    case IR_STUDY:
        data_temp = g_dev.set_temp;
        lcd_show_mode();
        lcd_show_fan();
        break ;

    case IR_SEND:
        break ;

    case IR_RECV:
        break ;

    default :
        break ;
    }

#if TEMP_CTRL_KNOB

    lcd_show_temp(data_temp);

#elif TEMP_CTRL_TOUCH

    lcd_show_num_room(data_temp);

#endif
}

void lcd_show_setting(void)
{
    if(g_set_type == IR_STUDY) {
        if(g_lcd_func_delay) {
            lcd_show_ir_study();
        } else {
            lcd_show_mode();
            lcd_show_fan();
            lcd_show_temp(g_dev.set_temp);
        }
        return ;
    }

#if TEMP_CTRL_KNOB

    if(g_lcd_func_delay) {
        lcd_show_setting_content();
    } else {
        lcd_show_setting_item();
    }

#elif TEMP_CTRL_TOUCH

    lcd_show_setting_item();
    lcd_show_setting_content();

#endif
}

void lcd_all_auto_test(void)
{
    g_dev.room_temp = 24.6;
    g_dev.set_temp = 26;
    g_dev.sta.mode = 3;
    g_dev.sta.wind = 3;

    while(1) {

        lcd_show_all_header();

        delay_ms(1000);
        lcd_ram_all_clean();

        lcd_show_all_digit();

        delay_ms(1000);
        lcd_ram_all_clean();

        lcd_show_mode();

        delay_ms(1000);
        lcd_ram_all_clean();

        lcd_show_fan();

        delay_ms(1000);
        lcd_ram_all_clean();

        lcd_show_setting_temp();

        delay_ms(1000);
        lcd_ram_all_clean();

        lcd_show_room_temp();

        delay_ms(1000);
        lcd_ram_all_clean();

        lcd_show_setting();

        delay_ms(1000);
        lcd_ram_all_clean();

        g_dev.sta.mode++;
        if(g_dev.sta.mode >= 4) {
           g_dev.sta.mode = 0;
        }

        g_dev.sta.wind++;
        if(g_dev.sta.wind >= 3) {
           g_dev.sta.mode = 0;
        }

    }
}

void lcd_open(void)
{
    LCD_BACKLIGHT_ON;
    lcd_screen_update();
}

void lcd_close(void)
{
    LCD_BACKLIGHT_OFF;
    lcd_ram_all_clean();
    lcd_screen_clear();
}


