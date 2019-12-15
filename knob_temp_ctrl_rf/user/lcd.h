#ifndef _LCD_H_
#define _LCD_H_

#include "drv_seglcd.h"
#include "global_variables.h"

#define LCD_RAM_USED    4


#define SEG_T1_ON   lcd_ram[0] |=  (1 << 27)
#define SEG_T1_OF   lcd_ram[0] &= ~(1 << 27)

#define SEG_T2_ON   lcd_ram[0] |=  (1 << 24)
#define SEG_T2_OF   lcd_ram[0] &= ~(1 << 24)

#define SEG_T3_ON   lcd_ram[0] |=  (1 << 25)
#define SEG_T3_OF   lcd_ram[0] &= ~(1 << 25)

#define SEG_T4_ON   lcd_ram[0] |=  (1 << 17)
#define SEG_T4_OF   lcd_ram[0] &= ~(1 << 17)

#define SEG_T5_ON   lcd_ram[0] |=  (1 << 18)
#define SEG_T5_OF   lcd_ram[0] &= ~(1 << 18)

#define SEG_T6_ON   lcd_ram[0] |=  (1 << 19)
#define SEG_T6_OF   lcd_ram[0] &= ~(1 << 19)

#define SEG_T7_ON   lcd_ram[0] |=  (1 << 16)
#define SEG_T7_OF   lcd_ram[0] &= ~(1 << 16)

#define SEG_T8_ON   lcd_ram[0] |=  (1 << 8)
#define SEG_T8_OF   lcd_ram[0] &= ~(1 << 8)

#define SEG_T9_ON    lcd_ram[0] |=  (1 << 9)
#define SEG_T9_OF    lcd_ram[0] &= ~(1 << 9)

#define SEG_T10_ON   lcd_ram[3] |=  (1 << 11)
#define SEG_T10_OF   lcd_ram[3] &= ~(1 << 11)

#define SEG_T11_ON   lcd_ram[0] |=  (1 << 10)
#define SEG_T11_OF   lcd_ram[0] &= ~(1 << 10)

#define SEG_T12_ON   lcd_ram[0] |=  (1 << 0)
#define SEG_T12_OF   lcd_ram[0] &= ~(1 << 0)

#define SEG_T13_ON   lcd_ram[0] |=  (1 << 1)
#define SEG_T13_OF   lcd_ram[0] &= ~(1 << 1)

#define SEG_T14_ON   lcd_ram[0] |=  (1 << 2)
#define SEG_T14_OF   lcd_ram[0] &= ~(1 << 2)

#define SEG_T15_ON   lcd_ram[0] |=  (1 << 11)
#define SEG_T15_OF   lcd_ram[0] &= ~(1 << 11)

#define SEG_T16_ON   lcd_ram[0] |=  (1 << 3)
#define SEG_T16_OF   lcd_ram[0] &= ~(1 << 3)

#define SEG_P1_ON    lcd_ram[1] |=  (1 << 3)
#define SEG_P1_OF    lcd_ram[1] &= ~(1 << 3)

#define SEG_P_ON     lcd_ram[2] |=  (1 << 19)
#define SEG_P_OF     lcd_ram[2] &= ~(1 << 19)

#define SEG_0A       (1 << 16)
#define SEG_0B       (1 << 8)
#define SEG_0C       (1 << 10)
#define SEG_0D       (1 << 19)
#define SEG_0E       (1 << 18)
#define SEG_0F       (1 << 17)
#define SEG_0G       (1 << 9)

#define SEG_1A       (1 << 24)
#define SEG_1B       (1 << 16)
#define SEG_1C       (1 << 18)
#define SEG_1D       (1 << 27)
#define SEG_1E       (1 << 26)
#define SEG_1F       (1 << 25)
#define SEG_1G       (1 << 17)

#define SEG_2A       (1 << 8)
#define SEG_2B       (1 << 0)
#define SEG_2C       (1 << 2)
#define SEG_2D       (1 << 11)
#define SEG_2E       (1 << 10)
#define SEG_2F       (1 << 9)
#define SEG_2G       (1 << 1)

#define SEG_2_L      (SEG_2B | SEG_2C | SEG_2G)
#define SEG_2_H      (SEG_2A | SEG_2D | SEG_2E | SEG_2F)

#define SEG_1_L      0
#define SEG_1_H      0



#define SEG_0(seg)      (SEG_##seg##A | SEG_##seg##B | SEG_##seg##C | SEG_##seg##D | SEG_##seg##E | SEG_##seg##F)
#define SEG_1(seg)      (SEG_##seg##B | SEG_##seg##C)
#define SEG_2(seg)      (SEG_##seg##A | SEG_##seg##B | SEG_##seg##D | SEG_##seg##E | SEG_##seg##G)
#define SEG_3(seg)      (SEG_##seg##A | SEG_##seg##B | SEG_##seg##C | SEG_##seg##D | SEG_##seg##G)
#define SEG_4(seg)      (SEG_##seg##B | SEG_##seg##C | SEG_##seg##F | SEG_##seg##G)
#define SEG_5(seg)      (SEG_##seg##A | SEG_##seg##C | SEG_##seg##D | SEG_##seg##F | SEG_##seg##G)
#define SEG_6(seg)      (SEG_##seg##A | SEG_##seg##C | SEG_##seg##D | SEG_##seg##E | SEG_##seg##F | SEG_##seg##G)
#define SEG_7(seg)      (SEG_##seg##A | SEG_##seg##B | SEG_##seg##C)
#define SEG_8(seg)      (SEG_##seg##A | SEG_##seg##B | SEG_##seg##C | SEG_##seg##D | SEG_##seg##E | SEG_##seg##F | SEG_##seg##G)
#define SEG_9(seg)      (SEG_##seg##A | SEG_##seg##B | SEG_##seg##C | SEG_##seg##D | SEG_##seg##F | SEG_##seg##G)

#define SEG_MINUS(seg)       (SEG_##seg##G)

#define SEG_SHOW_A(seg)      (SEG_##seg##A | SEG_##seg##B | SEG_##seg##C | SEG_##seg##E | SEG_##seg##F | SEG_##seg##G)

#define SEG_SHOW_b(seg)      (SEG_##seg##C | SEG_##seg##D | SEG_##seg##E | SEG_##seg##F | SEG_##seg##G)
#define SEG_SHOW_u(seg)      (SEG_##seg##C | SEG_##seg##D | SEG_##seg##E)
#define SEG_SHOW_U(seg)      (SEG_##seg##B | SEG_##seg##C | SEG_##seg##D | SEG_##seg##E | SEG_##seg##F)

#define SEG_SHOW_O(seg)      (SEG_##seg##A | SEG_##seg##B | SEG_##seg##C | SEG_##seg##D | SEG_##seg##E | SEG_##seg##F)

#define SEG_SHOW_L(seg)      (SEG_##seg##D | SEG_##seg##E | SEG_##seg##F)

#define SEG_SHOW_F(seg)      (SEG_##seg##A | SEG_##seg##E | SEG_##seg##F | SEG_##seg##G)

#define SEG_SHOW_P(seg)      (SEG_##seg##A | SEG_##seg##B| SEG_##seg##E | SEG_##seg##F | SEG_##seg##G)

#define SEG_SHOW_S(seg)      (SEG_##seg##A | SEG_##seg##C | SEG_##seg##D | SEG_##seg##F | SEG_##seg##G)
#define SEG_SHOW_E(seg)      (SEG_##seg##A | SEG_##seg##D | SEG_##seg##E | SEG_##seg##F | SEG_##seg##G)

#define SEG_SHOW_C(seg)      (SEG_##seg##A | SEG_##seg##D | SEG_##seg##E | SEG_##seg##F)
#define SEG_SHOW_L(seg)      (SEG_##seg##D | SEG_##seg##E | SEG_##seg##F)
#define SEG_SHOW_R(seg)      (SEG_##seg##A | SEG_##seg##B | SEG_##seg##C | SEG_##seg##E | SEG_##seg##F | SEG_##seg##G)
#define SEG_SHOW_r(seg)      (SEG_##seg##A | SEG_##seg##E | SEG_##seg##F)   // lower case
#define SEG_SHOW_I(seg)      (SEG_##seg##E | SEG_##seg##F)

#define SEG_SHOW_T(seg)      (SEG_##seg##A | SEG_##seg##B | SEG_##seg##C)
#define SEG_SHOW_d(seg)      (SEG_##seg##B | SEG_##seg##C | SEG_##seg##D | SEG_##seg##E | SEG_##seg##G)    // lower case
#define SEG_SHOW_Y(seg)      (SEG_##seg##B | SEG_##seg##C | SEG_##seg##D | SEG_##seg##F | SEG_##seg##G)

#define SEG_SHOW_N(seg)      (SEG_##seg##A | SEG_##seg##B | SEG_##seg##C | SEG_##seg##E | SEG_##seg##F)

#define SEG_SHOW_G(seg)      (SEG_##seg##A | SEG_##seg##C | SEG_##seg##D | SEG_##seg##E | SEG_##seg##F)

#define SHOW_DIGIT_POINT     SEG_P_ON
#define HIDE_DIGIT_POINT     SEG_P_OF

#define SHOW_DIGIT_MINUS     lcd_ram[3] |= SEG_MINUS(0)


extern const uint32_t lcd_number[3][10];


void lcd_update(void);
void lcd_ram_all_clean(void);
void lcd_screen_update(void);
void lcd_screen_clear(void);
void lcd_digit_all_clean(void);
void lcd_show_all_header(void);
void lcd_show_all_digit(void);
void lcd_show_mode(void);
void lcd_show_mode_all(void);
void lcd_show_fan(void);
void lcd_show_fan_all(void);
void lcd_show_all(void);
void lcd_show_num(uint16_t num);
void lcd_show_setting_temp(void);
void lcd_show_room_temp(void);
void lcd_show_temp(uint16_t temp);
static void lcd_show_ir_temp(void);
static void lcd_show_setting_num(uint8_t order, uint8_t value);
static void lcd_show_addr(void);
static void lcd_show_baud(void);
static void lcd_show_temp_offset(void);
static void lcd_show_led_timeout(void);
static void lcd_show_fan_ultra_low(void);
static void lcd_show_zigbee(void);
static void lcd_show_rf_ch(void);
static void lcd_show_pipe_type(void);
static void lcd_show_clr(void);
static void lcd_show_ir_study(void);
static void lcd_show_ir_send(void);
static void lcd_show_ir_recv(void);
void lcd_show_setting_item(void);
void lcd_show_setting_content(void);
void lcd_show_setting(void);
void lcd_all_auto_test(void);
void lcd_open(void);
void lcd_close(void);



#endif

