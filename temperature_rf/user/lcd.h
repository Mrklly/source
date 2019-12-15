#ifndef _LCD_H_
#define _LCD_H_

#include "drv_seglcd.h"
#include "global_variables.h"

#define SEG_T1_ON   ram[4] |=  (1 << 24)
#define SEG_T1_OF   ram[4] &= ~(1 << 24)

#define SEG_T2_ON   ram[4] |=  (1 << 25)
#define SEG_T2_OF   ram[4] &= ~(1 << 25)

#define SEG_T3_ON   ram[4] |=  (1 << 26)
#define SEG_T3_OF   ram[4] &= ~(1 << 26)

#define SEG_T4_ON   ram[4] |=  (1 << 27)
#define SEG_T4_OF   ram[4] &= ~(1 << 27)

#define SEG_T5_ON   ram[0] |=  (1 << 3)
#define SEG_T5_OF   ram[0] &= ~(1 << 3)

#define SEG_T6_ON   ram[0] |=  (1 << 2)
#define SEG_T6_OF   ram[0] &= ~(1 << 2)

#define SEG_T7_ON   ram[0] |=  (1 << 1)
#define SEG_T7_OF   ram[0] &= ~(1 << 1)

#define SEG_T8_ON   ram[0] |=  (1 << 0)
#define SEG_T8_OF   ram[0] &= ~(1 << 0)

#define SEG_T9_ON    ram[0] |=  (1 << 9)
#define SEG_T9_OF    ram[0] &= ~(1 << 9)

#define SEG_T10_ON   ram[0] |=  (1 << 10)
#define SEG_T10_OF   ram[0] &= ~(1 << 10)

#define SEG_T11_ON   ram[0] |=  (1 << 11)
#define SEG_T11_OF   ram[0] &= ~(1 << 11)

#define SEG_T12_ON   ram[0] |=  (1 << 16)
#define SEG_T12_OF   ram[0] &= ~(1 << 16)

#define SEG_T13_ON   ram[2] |=  (1 << 16)
#define SEG_T13_OF   ram[2] &= ~(1 << 16)

#define SEG_T14_ON   ram[3] |=  (1 << 8)
#define SEG_T14_OF   ram[3] &= ~(1 << 8)

#define SEG_T15_ON   ram[1] |=  (1 << 0)
#define SEG_T15_OF   ram[1] &= ~(1 << 0)

#define SEG_T16_ON   ram[3] |=  (1 << 9)
#define SEG_T16_OF   ram[3] &= ~(1 << 9)

#define SEG_T17_ON   ram[3] |=  (1 << 27)
#define SEG_T17_OF   ram[3] &= ~(1 << 27)

#define SEG_T18_ON   ram[3] |=  (1 << 10)
#define SEG_T18_OF   ram[3] &= ~(1 << 10)

#define SEG_T19_ON   ram[3] |=  (1 << 11)
#define SEG_T19_OF   ram[3] &= ~(1 << 11)

#define SEG_0A       (1 << 27)
#define SEG_0B       (1 << 26)
#define SEG_0C       (1 << 25)
#define SEG_0D       (1 << 24)
#define SEG_0E       (1 << 17)
#define SEG_0F       (1 << 19)
#define SEG_0G       (1 << 18)

#define SEG_1A       (1 << 11)
#define SEG_1B       (1 << 10)
#define SEG_1C       (1 << 9)
#define SEG_1D       (1 << 8)
#define SEG_1E       (1 << 1)
#define SEG_1F       (1 << 3)
#define SEG_1G       (1 << 2)

#define SEG_1_L      (SEG_1A | SEG_1B | SEG_1C | SEG_1D)
#define SEG_1_H      (SEG_1E | SEG_1F | SEG_1G)

#define SEG_2A       (1 << 27)
#define SEG_2B       (1 << 26)
#define SEG_2C       (1 << 25)
#define SEG_2D       (1 << 24)
#define SEG_2E       (1 << 17)
#define SEG_2F       (1 << 19)
#define SEG_2G       (1 << 18)

#define SEG_3A       (1 << 16)
#define SEG_3B       (1 << 17)
#define SEG_3C       (1 << 18)
#define SEG_3D       (1 << 19)
#define SEG_3E       (1 << 2)
#define SEG_3F       (1 << 0)
#define SEG_3G       (1 << 1)

#define SEG_3_L      (SEG_3A | SEG_3B | SEG_3C | SEG_3D)
#define SEG_3_H      (SEG_3E | SEG_3F | SEG_3G)

#define SEG_4A       (1 << 0)
#define SEG_4B       (1 << 1)
#define SEG_4C       (1 << 2)
#define SEG_4D       (1 << 3)
#define SEG_4E       (1 << 10)
#define SEG_4F       (1 << 8)
#define SEG_4G       (1 << 9)

#define SEG_5A       (1 << 16)
#define SEG_5B       (1 << 17)
#define SEG_5C       (1 << 18)
#define SEG_5D       (1 << 19)
#define SEG_5E       (1 << 26)
#define SEG_5F       (1 << 24)
#define SEG_5G       (1 << 25)


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


extern const uint32_t lcd_number[6][10];


void lcd_update(void);
void lcd_ram_all_clean(void);
void lcd_screen_update(void);
void lcd_screen_clear(void);
void lcd_digit_all_clean(void);
void lcd_show_all_header(void);
void lcd_show_all_digit(void);
void lcd_show_mode(void);
void lcd_show_fan(void);
void lcd_show_setting_temp(void);
void lcd_show_room_temp(void);
static void lcd_show_ir_temp(void);
static void lcd_show_setting_num(uint8_t order, uint8_t value);
static void lcd_show_clr(void);
static void lcd_show_ir_study(void);
static void lcd_show_ir_send(void);
static void lcd_show_ir_recv(void);
void lcd_show_setting(void);
void lcd_all_auto_test(void);
void lcd_open(void);
void lcd_close(void);



#endif

