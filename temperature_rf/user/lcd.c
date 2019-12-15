#include "lcd.h"
#include "led.h"
#include "eeprom.h"

volatile uint32_t ram[6] = {0, 0, 0, 0, 0, 0};

const uint32_t lcd_number[6][10] = {
    {SEG_0(0), SEG_1(0), SEG_2(0), SEG_3(0), SEG_4(0), SEG_5(0), SEG_6(0), SEG_7(0), SEG_8(0), SEG_9(0)},
    {SEG_0(1), SEG_1(1), SEG_2(1), SEG_3(1), SEG_4(1), SEG_5(1), SEG_6(1), SEG_7(1), SEG_8(1), SEG_9(1)},
    {SEG_0(2), SEG_1(2), SEG_2(2), SEG_3(2), SEG_4(2), SEG_5(2), SEG_6(2), SEG_7(2), SEG_8(2), SEG_9(2)},
    {SEG_0(3), SEG_1(3), SEG_2(3), SEG_3(3), SEG_4(3), SEG_5(3), SEG_6(3), SEG_7(3), SEG_8(3), SEG_9(3)},
    {SEG_0(4), SEG_1(4), SEG_2(4), SEG_3(4), SEG_4(4), SEG_5(4), SEG_6(4), SEG_7(4), SEG_8(4), SEG_9(4)},
    {SEG_0(5), SEG_1(5), SEG_2(5), SEG_3(5), SEG_4(5), SEG_5(5), SEG_6(5), SEG_7(5), SEG_8(5), SEG_9(5)}
};

void lcd_update(void)
{
    if(!g_lcd_on) {
        return ;
    }

    lcd_ram_all_clean();

    if(g_set_type) {

        lcd_show_setting();

        if(g_ir_study_flag) {
            SEG_T7_ON;          // T7, horizontal lines
            lcd_show_ir_temp();
            lcd_show_mode();
            lcd_show_fan();
        }
        return ;
    }

    lcd_show_all_header();
    lcd_show_mode();
    lcd_show_fan();
    lcd_show_setting_temp();
    lcd_show_room_temp();
}

void lcd_ram_all_clean(void)
{
    for(uint8_t i=0; i<6; i++) {
        ram[i] = 0;
    }
}

void lcd_screen_update(void)
{
    for(uint8_t i=0; i<6; i++) {
        LCD_WriteRam0_7Int32(i, ram[i]);
    }
}

void lcd_screen_clear(void)
{
    for(uint8_t i=0; i<6; i++) {
        LCD_WriteRam0_7Int32(i, 0);
    }
}

void lcd_digit_all_clean(void)
{
    ram[0] &= ~lcd_number[0][8];     // seg number 0

    ram[1] &= ~lcd_number[1][8];     // seg number 1
    ram[2] &= ~lcd_number[1][8];

    ram[2] &= ~lcd_number[2][8];     // seg number 2

    ram[4] &= ~lcd_number[3][8];     // seg number 3
    ram[5] &= ~lcd_number[3][8];

    ram[4] &= ~lcd_number[4][8];     // seg number 4

    ram[3] &= ~lcd_number[5][8];     // seg number 5

    for(uint8_t i = 0; i<6; i++) {
        LCD_WriteRam0_7Int32(i, ram[i]);
    }
}

void lcd_show_all_header(void)
{
    SEG_T1_ON;          // T1 MODE
    SEG_T7_ON;          // T7, horizontal lines
    SEG_T8_ON;          // T8, FAN icon
    SEG_T12_ON;         // T12, setting temp
    SEG_T13_ON;         // T13, setting temp point
    SEG_T14_OF;         // T14
    SEG_T15_ON;         // T15, celsius icon
    SEG_T16_ON;         // T16, room temp
    SEG_T17_ON;         // T17, root temp point
    SEG_T18_OF;         // T18
    SEG_T19_ON;         // T19, celsius icon

    for(uint8_t i = 0; i<6; i++) {
        LCD_WriteRam0_7Int32(i, ram[i]);
    }
}

void lcd_show_all_digit(void)
{
    uint8_t i=0;

    for(; i<10; i++) {
        ram[0] |= lcd_number[0][i];     // seg number 0

        ram[1] |= lcd_number[1][i];     // seg number 1
        ram[1] &=~SEG_1_L;
        ram[2] |= lcd_number[1][i];
        ram[2] &=~SEG_1_H;

        ram[2] |= lcd_number[2][i];     // seg number 2

        ram[4] |= lcd_number[3][i];     // seg number 3
        ram[4] &= (~SEG_3_H);
        ram[5] |= lcd_number[3][i];
        ram[5] &= (~SEG_3_L);

        ram[4] |= lcd_number[4][i];     // seg number 4

        ram[3] |= lcd_number[5][i];     // seg number 5

        for(uint8_t j = 0; j<6; j++) {
            LCD_WriteRam0_7Int32(j, ram[j]);
        }

        delay_ms(500);

        lcd_digit_all_clean();

    }
}

void lcd_show_mode(void)
{
    switch(g_dev.mode.mod) {
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

    LCD_WriteRam0_7Int32(0, ram[0]);
    LCD_WriteRam0_7Int32(4, ram[4]);
}

void lcd_show_fan(void)
{
    switch(g_dev.mode.wind) {   // T9 T10 T11
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
    default:
        break;
    }

    LCD_WriteRam0_7Int32(0, ram[0]);
}

void lcd_show_setting_temp(void)
{
    uint16_t setting_temp = g_dev.set_temp * 10;
    uint8_t digit[3] = {0, 0, 0};

    digit[0] = setting_temp / 100 % 10;
    digit[1] = setting_temp / 10 % 10;
    digit[2] = setting_temp % 10;

    ram[0] |= lcd_number[0][digit[0]];     // seg number 0

    ram[1] |= lcd_number[1][digit[1]];     // seg number 1
    ram[1] &=~SEG_1_L;
    ram[2] |= lcd_number[1][digit[1]];
    ram[2] &=~SEG_1_H;

    ram[2] |= lcd_number[2][digit[2]];     // seg number 2

    for(uint8_t i = 0; i<3; i++) {
        LCD_WriteRam0_7Int32(i, ram[i]);
    }
}

void lcd_show_room_temp(void)
{
    uint16_t room_temp = (uint16_t)(g_dev.room_temp * 10);
    uint8_t digit[3] = {0, 0, 0};

    digit[0] = room_temp / 100 % 10;
    digit[1] = room_temp / 10 % 10;
    digit[2] = room_temp % 10;

    ram[4] |= lcd_number[3][digit[0]];     // seg number 3
    ram[4] &= (~SEG_3_H);
    ram[5] |= lcd_number[3][digit[0]];
    ram[5] &= (~SEG_3_L);

    ram[4] |= lcd_number[4][digit[1]];     // seg number 4

    ram[3] |= lcd_number[5][digit[2]];     // seg number 5

    for(uint8_t i = 3; i<6; i++) {
        LCD_WriteRam0_7Int32(i, ram[i]);
    }
}

static void lcd_show_ir_temp(void)
{
    uint16_t setting_temp = g_dev.set_temp;
    uint8_t digit[2] = {0, 0};

    digit[0] = setting_temp / 10 % 10;
    digit[1] = setting_temp % 10;

    ram[4] |= lcd_number[3][digit[0]];     // seg number 3
    ram[4] &= (~SEG_3_H);
    ram[5] |= lcd_number[3][digit[0]];
    ram[5] &= (~SEG_3_L);

    ram[4] |= lcd_number[4][digit[1]];     // seg number 4

    for(uint8_t i = 4; i<6; i++) {
        LCD_WriteRam0_7Int32(i, ram[i]);
    }
}

static void lcd_show_setting_num(uint8_t order, uint8_t value)
{
    ram[0] |= SEG_SHOW_S(0);     // seg number 0

    ram[1] |= SEG_SHOW_E(1);     // seg number 1
    ram[1] &=~SEG_1_L;
    ram[2] |= SEG_SHOW_E(1);
    ram[2] &=~SEG_1_H;

    ram[2] |= lcd_number[2][order];     // seg number 2

    uint8_t digit[3] = {0, 0, 0};
    digit[0] = value / 100 % 10;
    digit[1] = value / 10 % 10;
    digit[2] = value % 10;

    ram[4] |= lcd_number[3][digit[0]];     // seg number 3
    ram[4] &= (~SEG_3_H);
    ram[5] |= lcd_number[3][digit[0]];
    ram[5] &= (~SEG_3_L);

    ram[4] |= lcd_number[4][digit[1]];     // seg number 4

    ram[3] |= lcd_number[5][digit[2]];     // seg number 5
}

static void lcd_show_clr(void)
{
    ram[0] |= SEG_SHOW_C(0);     // seg number 0

    ram[1] |= SEG_SHOW_L(1);     // seg number 1
    ram[1] &=~SEG_1_L;
    ram[2] |= SEG_SHOW_L(1);
    ram[2] &=~SEG_1_H;

    ram[2] |= SEG_SHOW_R(2);     // seg number 2

    uint8_t digit[3] = {0, 0, 0};
    digit[0] = g_erasing_index / 100 % 10;
    digit[1] = g_erasing_index / 10 % 10;
    digit[2] = g_erasing_index % 10;

    ram[4] |= lcd_number[3][digit[0]];     // seg number 3
    ram[4] &= (~SEG_3_H);
    ram[5] |= lcd_number[3][digit[0]];
    ram[5] &= (~SEG_3_L);

    ram[4] |= lcd_number[4][digit[1]];     // seg number 4

    ram[3] |= lcd_number[5][digit[2]];     // seg number 5
}

static void lcd_show_ir_study(void)
{
    ram[0] |= SEG_SHOW_S(0);     // seg number 0

    ram[1] |= SEG_SHOW_d(1);     // seg number 1
    ram[1] &=~SEG_1_L;
    ram[2] |= SEG_SHOW_d(1);
    ram[2] &=~SEG_1_H;

    ram[2] |= SEG_SHOW_Y(2);     // seg number 2

    ram[3] = ram[4] = ram[5] = 0;
}

static void lcd_show_ir_send(void)
{
    ram[0] |= SEG_SHOW_I(0);           // seg number 0

    ram[1] |= SEG_SHOW_r(1);           // seg number 1
    ram[1] &=~SEG_1_L;
    ram[2] |= SEG_SHOW_r(1);
    ram[2] &=~SEG_1_H;

    ram[2] |= SEG_SHOW_S(2);           // seg number 2

    ram[3] = ram[4] = ram[5] = 0;
}

static void lcd_show_ir_recv(void)
{
    ram[0] |= SEG_SHOW_I(0);           // seg number 0

    ram[1] |= SEG_SHOW_r(1);           // seg number 1
    ram[1] &=~SEG_1_L;
    ram[2] |= SEG_SHOW_r(1);
    ram[2] &=~SEG_1_H;

    ram[2] |= SEG_SHOW_r(2);           // seg number 2

    ram[3] = ram[4] = ram[5] = 0;
}

void lcd_show_setting(void)
{
    uint8_t temp, value;

    switch(g_set_type) {
        case SET_ADDR:
            temp = 0;
            value = g_dev.addr;
            lcd_show_setting_num(temp, value);
            break;
        case SET_BAUD:
            temp = 1;
            if(g_dev.baud == BAUD9600) {
                value = 96;
            } else if(g_dev.baud == BAUD19200) {
                value = 192;
            } else {
                g_dev.baud = BAUD_ERR;
                value = 0;
            }
            lcd_show_setting_num(temp, value);
            break;
        case SET_RF_CH:
            temp = 2;
            value = g_eep.rf_ch;
            lcd_show_setting_num(temp, value);
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

    for(uint8_t i = 0; i<6; i++) {
        LCD_WriteRam0_7Int32(i, ram[i]);
    }
}

void lcd_all_auto_test(void)
{
    g_dev.room_temp = 24.6;
    g_dev.set_temp = 26;
    g_dev.mode.mod = 3;
    g_dev.mode.wind = 3;

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

        g_dev.mode.mod++;
        if(g_dev.mode.mod >= 4) {
           g_dev.mode.mod = 0;
        }

        g_dev.mode.wind++;
        if(g_dev.mode.wind >= 3) {
           g_dev.mode.mod = 0;
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
