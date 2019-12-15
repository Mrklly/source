#include "led.h"

void led_init(void)
{
    GPIO_Init(LED_INDICATOR_PORT,LED_INDICATOR_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );
    GPIO_Init(LCD_BACKLIGHT_PORT,LCD_BACKLIGHT_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );
}

typedef struct _lcd_t_ {
    u8		led_l;
    u8		led_h;
} lcd;
/* addr 6 */
#define LED_T1		(1 << 3)
#define LED_T2		(1 << 2)
#define LED_T3		(1 << 1)
#define LED_T4		(1 << 0)
/* addr 20 */
#define LED_T5		(1 << 0)
#define LED_T6		(1 << 1)
#define LED_T7		(1 << 2)
#define LED_T8		(1 << 3)
#define LED_T9		(1 << 6)
#define LED_T10		(1 << 5)
#define LED_T11		(1 << 4)
/* addr 18 */
#define LED_T12		(1 << 3)
/* addr 14 */
#define LED_T13		(1 << 3)
/* addr 16 */
#define LED_T14		(1 << 3)
/* addr 12 */
#define LED_T15		(1 << 3)
#define LED_T16		(1 << 2)
#define LED_T18		(1 << 1)
#define LED_T19		(1 << 0)
/* addr 8 */
#define LED_T17		(1 << 4)



lcd		g_lcd[6][10] = {
    {{LED1_DIGITAL_0_0},{LED1_DIGITAL_0_1},{LED1_DIGITAL_0_2},{LED1_DIGITAL_0_3},{LED1_DIGITAL_0_4},{LED1_DIGITAL_0_5},{LED1_DIGITAL_0_6},{LED1_DIGITAL_0_7},{LED1_DIGITAL_0_8},{LED1_DIGITAL_0_9}},	/* addr 18、19 */
    {{LED1_DIGITAL_0_0},{LED1_DIGITAL_0_1},{LED1_DIGITAL_0_2},{LED1_DIGITAL_0_3},{LED1_DIGITAL_0_4},{LED1_DIGITAL_0_5},{LED1_DIGITAL_0_6},{LED1_DIGITAL_0_7},{LED1_DIGITAL_0_8},{LED1_DIGITAL_0_9}},	/* addr 16、17 */
    {{LED1_DIGITAL_0_0},{LED1_DIGITAL_0_1},{LED1_DIGITAL_0_2},{LED1_DIGITAL_0_3},{LED1_DIGITAL_0_4},{LED1_DIGITAL_0_5},{LED1_DIGITAL_0_6},{LED1_DIGITAL_0_7},{LED1_DIGITAL_0_8},{LED1_DIGITAL_0_9}},	/* addr 14、15 */

    {   {LED1_DIGITAL_3_0_0,LED1_DIGITAL_3_0_1},{LED1_DIGITAL_3_1_0,LED1_DIGITAL_3_1_1},{LED1_DIGITAL_3_2_0,LED1_DIGITAL_3_2_1},{LED1_DIGITAL_3_3_0,LED1_DIGITAL_3_3_1},{LED1_DIGITAL_3_4_0,LED1_DIGITAL_3_4_1},	/* addr 11、6 */
        {LED1_DIGITAL_3_5_0,LED1_DIGITAL_3_5_1},{LED1_DIGITAL_3_6_0,LED1_DIGITAL_3_6_1},{LED1_DIGITAL_3_7_0,LED1_DIGITAL_3_7_1},{LED1_DIGITAL_3_8_0,LED1_DIGITAL_3_8_1},{LED1_DIGITAL_3_9_0,LED1_DIGITAL_3_9_1},
    },

    {   {LED1_DIGITAL_4_0_0,LED1_DIGITAL_4_0_1},{LED1_DIGITAL_4_1_0,LED1_DIGITAL_4_1_1},{LED1_DIGITAL_4_2_0,LED1_DIGITAL_4_2_1},{LED1_DIGITAL_4_3_0,LED1_DIGITAL_4_3_1},{LED1_DIGITAL_4_4_0,LED1_DIGITAL_4_4_1},	/* addr 9、10 */
        {LED1_DIGITAL_4_5_0,LED1_DIGITAL_4_5_1},{LED1_DIGITAL_4_6_0,LED1_DIGITAL_4_6_1},{LED1_DIGITAL_4_7_0,LED1_DIGITAL_4_7_1},{LED1_DIGITAL_4_8_0,LED1_DIGITAL_4_8_1},{LED1_DIGITAL_4_9_0,LED1_DIGITAL_4_9_1},
    },

    {   {LED1_DIGITAL_5_0_0,LED1_DIGITAL_5_0_1},{LED1_DIGITAL_5_1_0,LED1_DIGITAL_5_1_1},{LED1_DIGITAL_5_2_0,LED1_DIGITAL_5_2_1},{LED1_DIGITAL_5_3_0,LED1_DIGITAL_5_3_1},{LED1_DIGITAL_5_4_0,LED1_DIGITAL_5_4_1},	/* addr 12、8 */
        {LED1_DIGITAL_5_5_0,LED1_DIGITAL_5_5_1},{LED1_DIGITAL_5_6_0,LED1_DIGITAL_5_6_1},{LED1_DIGITAL_5_7_0,LED1_DIGITAL_5_7_1},{LED1_DIGITAL_5_8_0,LED1_DIGITAL_5_8_1},{LED1_DIGITAL_5_9_0,LED1_DIGITAL_5_9_1},
    },
};

lcd show_setting[3][4] = {
    {{LED_1_DIGITAL_A | LED_1_DIGITAL_C | LED_1_DIGITAL_D | LED_1_DIGITAL_F | LED_1_DIGITAL_G},{LED_1_DIGITAL_A | LED_1_DIGITAL_D | LED_1_DIGITAL_E | LED_1_DIGITAL_F | LED_1_DIGITAL_G},{LED1_DIGITAL_0_0},{LED1_DIGITAL_0_1}},
    {{LED_1_DIGITAL_A | LED_1_DIGITAL_C | LED_1_DIGITAL_D | LED_1_DIGITAL_F | LED_1_DIGITAL_G},{LED_1_DIGITAL_A | LED_1_DIGITAL_D | LED_1_DIGITAL_E | LED_1_DIGITAL_F | LED_1_DIGITAL_G},{LED1_DIGITAL_0_0},{LED1_DIGITAL_0_1}},
    {{LED_1_DIGITAL_A | LED_1_DIGITAL_C | LED_1_DIGITAL_D | LED_1_DIGITAL_F | LED_1_DIGITAL_G},{LED_1_DIGITAL_A | LED_1_DIGITAL_D | LED_1_DIGITAL_E | LED_1_DIGITAL_F | LED_1_DIGITAL_G},{LED1_DIGITAL_0_0},{LED1_DIGITAL_0_1}}
};

lcd show_off[2] =
    {{LED_1_DIGITAL_A | LED_1_DIGITAL_B | LED_1_DIGITAL_C | LED_1_DIGITAL_D | LED_1_DIGITAL_E | LED_1_DIGITAL_F},{LED_1_DIGITAL_A | LED_1_DIGITAL_E | LED_1_DIGITAL_F | LED_1_DIGITAL_G}};

void lcd_show_setting(btn_setting set_type)
{
    u8 temp;

    if(set_type == set_addr) {
        temp = 2;
    } else if(set_type == set_baud) {
        temp = 3;
    } else {
        return ;
    }
    HT1621_WR_OneByte(18,show_setting[0][0].led_l);
    HT1621_WR_OneByte(16,show_setting[1][1].led_l);
    HT1621_WR_OneByte(14,show_setting[2][temp].led_l);
}

void lcd_show_off()
{
    lcd_clear();
    HT1621_WR_OneByte(18,show_off[0].led_l);
    HT1621_WR_OneByte(16,show_off[1].led_l);
    HT1621_WR_OneByte(14,show_off[1].led_l);
}

void lcd_clear(void)
{
    HT1621_WR_OneByte(6,0);
    HT1621_WR_OneByte(8,0);
    HT1621_WR_OneByte(10,0);
    HT1621_WR_OneByte(12,0);
    HT1621_WR_OneByte(14,0);
    HT1621_WR_OneByte(16,0);
    HT1621_WR_OneByte(18,0);
    HT1621_WR_OneByte(20,0);

}

void lcd_show_settings(device_settings *dev_set)
{
    u8 temp;
    unsigned int val;
    if(dev_set->set_type == set_addr) {
        val = (u8)(dev_set->dev_addr);
        temp = 2;
    } else if(dev_set->set_type == set_baud) {
        if(dev_set->baud == BAUDRATE9600) {
            val = 96;
        } else if (dev_set->baud == BAUDRATE19200) {
            val = 192;
        } else if (dev_set->baud == BAUDRATE38400) {
            val = 384;
        }
        temp = 3;
    } else {
        return ;
    }
    
    HT1621_WR_OneByte(18,show_setting[0][0].led_l);
    HT1621_WR_OneByte(16,show_setting[1][1].led_l);
    HT1621_WR_OneByte(14,show_setting[2][temp].led_l);

    
    HT1621_WR_OneByte(6,g_lcd[3][val / 100 % 10].led_h);
    HT1621_WR_OneByte(8,g_lcd[4][val / 10 % 10].led_l | g_lcd[5][val % 10].led_h);
    HT1621_WR_OneByte(10,g_lcd[3][val / 100 % 10].led_l | g_lcd[4][val / 10 % 10].led_h );
    HT1621_WR_OneByte(12,g_lcd[5][val % 10].led_l);
    
}

void lcd_show(temp_dev	*pdev)
{
    u16 	val = (u16)(pdev->set_temp * 10);
    u8		mod = 0;


    HT1621_WR_OneByte(14,g_lcd[0][val % 10].led_l | LED_T13);
    HT1621_WR_OneByte(16,g_lcd[1][(val / 10 % 10)].led_l | LED_T15);
    HT1621_WR_OneByte(18,g_lcd[2][(val / 100 % 10)].led_l | LED_T12);
    val = (u16)(pdev->room_temp * 10);
    switch(pdev->mode.mod) {
    case TEMP_MODE_COOL:
        mod = LED_T2;
        break;
    case TEMP_MODE_WARM:
        mod = LED_T3;
        break;
    case TEMP_MODE_FAN:
        mod = LED_T4;
        break;
    default:
        break;
    }
    HT1621_WR_OneByte(6,g_lcd[3][val / 100 % 10].led_h | LED_T1 | mod);
    mod = 0;
    switch(pdev->mode.mod) {
    case TEMP_MODE_DRY:
        mod = LED_T5;
        break;
    case TEMP_MODE_AUTO:
        mod = LED_T6;
        break;
    default:
        break;
    }
    switch(pdev->mode.wind) {
    case FAN_CLZ:
        break;
    case FAN_MIN:
        mod |= LED_T9;
        break;
    case FAN_MID:
        mod |= LED_T9 | LED_T10;
        break;
    case FAN_MAX:
        mod |= LED_T9 | LED_T10 | LED_T11;
        break;
    case FAN_AUTO:
        mod |= LED_T11;
        break;
    default:
        break;
    }
    HT1621_WR_OneByte(20,LED_T7 | LED_T8 | mod);
    HT1621_WR_OneByte(8,g_lcd[4][val / 10 % 10].led_l | g_lcd[5][val % 10].led_h | LED_T17);
    HT1621_WR_OneByte(10,g_lcd[3][val / 100 % 10].led_l | g_lcd[4][val / 10 % 10].led_h );
    HT1621_WR_OneByte(12,g_lcd[5][val % 10].led_l | LED_T16 | LED_T19);
}

void OpenLCD()
{
    HT1621_WR_Cmd(LCDON);
    GPIO_WriteHigh(LCD_BACKLIGHT_PORT,LCD_BACKLIGHT_PIN);
}

void CloseLCD()
{
    HT1621_WR_Cmd(LCDOFF);
    GPIO_WriteLow(LCD_BACKLIGHT_PORT,LCD_BACKLIGHT_PIN);
}

