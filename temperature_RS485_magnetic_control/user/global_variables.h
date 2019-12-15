#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIABLES_H_

#include "stm8s.h"
#include "wwdg.h"


#ifndef NULL
#define NULL	((void*)0)
#endif


#define RX_BUFF_LEN	 	0x10
#define KET_COUNT		5

extern volatile u16 systick_counter;
extern volatile u8 g_set_addr_baud;


typedef void (*mod_hander)(void);
typedef struct _mode_t_ {
    u8				type;
    mod_hander		hander;
} mode;

typedef void (*btn_hander)(u8 shirt);
typedef struct _com_io_t_ {
    GPIO_TypeDef 		*key_port;
    GPIO_Pin_TypeDef	 key_pin;
    btn_hander			 hander;
} com_io;

typedef struct _link_dat_t_ {
    u8		soh_h;
    u8		soh_l;
    u8		sl_addr;
    u8		fun;
    u8		st_addr;
    u8		reg_cnt;
    u8		buff[4];
} link_dat;

typedef struct _btn_ctrl_t_ {
    u8		shirt;
    u8		up;
    u8		down;
} btn_ctrl;

typedef enum _fan_status_t_ {
    FAN_CLZ = 0x00,
    FAN_MIN = 0x01,
    FAN_MID = 0x02,
    FAN_MAX = 0x03,
    FAN_AUTO = 0x04
} fan_status;

typedef enum _temperature_status_t_ {
    TEMP_MODE_COOL = 0x00,
    TEMP_MODE_WARM = 0x01,
    TEMP_MODE_FAN  = 0x02,
    TEMP_MODE_DRY  = 0x03,
    TEMP_MODE_AUTO = 0x04,
    TEMP_MODE_NONE = 0x05
} tempe_status;

typedef enum _lcd_backlight_status_t_ {
    BACKLIGHT_OFF = 0x00,
    BACKLIGHT_ON = 0x01
} lcd_backlight_status;

typedef enum _device_baudrate_t_ {
    BAUDRATE_ERR  = 0x00,
    BAUDRATE9600  = 0x01,
    BAUDRATE19200 = 0x02,
    BAUDRATE38400 = 0x03
}device_baudrate;

typedef enum _btn_setting_t_ {
    set_addr,
    set_baud
} btn_setting;

typedef struct _device_settings_t_ {
    btn_setting set_type;
    u8  dev_addr;
    device_baudrate baud;
} device_settings;


typedef struct _temp_dev_t_ {
    device_baudrate baud;
    u8		addr;		/* 地址 */
    struct {
        u8	mod:1;		/* 模式 */
        u8	res:1;		/* 保留 */
        u8	wind:1;		/* 风速 */
        u8	power:1;	/* 电源 */
        u8	T_up:1;		/* 增温 */
        u8	T_down:1;	/* 减温 */
        u8	res_1:2;		/* 保留 */
    } sw_sta;	/* 按键状态 */
    u8		set_temp;	/* 用户设定温度 */
    float		room_temp;	/* 当前温度 */
    union {
        u8	mod;
        struct {
            u8	mod:3;		/*0：无;1：热风;2：冷风 3:送风 4：除温 5：自动*/
            u8	wind:2;		/*0：无;1：低; 2：中;3：高; */
            u8	wind_auto:1;/*0：无1：自动 */
            u8	valve:1;
            u8	power:1;
        } mode;
    };
} temp_dev;


extern const com_io g_io[KET_COUNT];
extern btn_ctrl  g_btn[KET_COUNT];
extern u8 g_rx_buff[RX_BUFF_LEN];
extern u8 g_uart_baud_changed;
extern u8 g_led_setting_flag;
extern u8 g_saving_flag;
extern temp_dev	g_dev;
extern device_settings g_dev_set;
extern mode g_mod[6];
extern volatile uint8_t g_mod_backup;
extern volatile uint8_t g_mod_backup_flag;
extern u8 g_lcd_on; //0 为休眠状态
extern u8 g_lcd_delay;
extern u8 g_led_on;
extern u8 g_led_delay;
extern volatile u8 g_idx;
extern float s_temp_bak;

#define BUTTON_MODE     g_btn[0]
#define BUTTON_FAN      g_btn[1]
#define BUTTON_UP       g_btn[2]
#define BUTTON_DOWN     g_btn[3]
#define BUTTON_ONOFF    g_btn[4]


#ifndef size_of
#   define size_of(_obj)                (sizeof(_obj)/sizeof((_obj)[0]))
#endif

#ifndef top_of
#   define top_of(_obj)                 (&(_obj)[size_of(_obj)-1])
#endif


/* 以大端格式读取数值内存数值 */
#ifndef GET_BE_UINT16
#   define GET_BE_UINT16(pt)            ( ((uint16)__vat(pt,0) <<  8) \
                                        | ((uint8 )__vat(pt,1) <<  0) )

#   define GET_BE_UINT24(pt)            ( ((uint32)__vat(pt,0) << 16) \
                                        | ((uint16)__vat(pt,1) <<  8) \
                                        | ((uint8 )__vat(pt,2) <<  0) )

#   define GET_BE_UINT32(pt)            ( ((uint32)__vat(pt,0) << 24) \
                                        | ((uint32)__vat(pt,1) << 16) \
                                        | ((uint16)__vat(pt,2) <<  8) \
                                        | ((uint8 )__vat(pt,3) <<  0) )

#endif /* #ifndef GET_BE_UINT16 */
#ifndef __vat
#   define __vat(_base,_offset)         (((uint8*)(_base))[_offset])
#endif
/* 以大端格式填充内存 */
#ifndef PUT_BE_UINT16
#   define PUT_BE_UINT16(pt,val)    do{ __vat(pt,0) = (uint8)((uint16)(val) >>  8); \
                                        __vat(pt,1) = (uint8)((uint8 )(val) >>  0); \
                                       }while(0)

#   define PUT_BE_UINT24(pt,val)    do{ __vat(pt,0) = (uint8)((uint32)(val) >> 16); \
                                        __vat(pt,1) = (uint8)((uint16)(val) >>  8); \
                                        __vat(pt,2) = (uint8)((uint8 )(val) >>  0); \
                                       }while(0)

#   define PUT_BE_UINT32(pt,val)    do{ __vat(pt,0) = (uint8)((uint32)(val) >> 24); \
                                        __vat(pt,1) = (uint8)((uint32)(val) >> 16); \
                                        __vat(pt,2) = (uint8)((uint16)(val) >>  8); \
                                        __vat(pt,3) = (uint8)((uint8 )(val) >>  0); \
                                       }while(0)

#endif /* #ifndef PUT_BE_UINT16 */

/* 以小端格式读取数值内存数值 */
#ifndef GET_LE_UINT16
#   define GET_LE_UINT16(pt)            ( ((uint8 )__vat(pt,0) <<  0) \
                                        | ((uint16)__vat(pt,1) <<  8) )

#   define GET_LE_UINT24(pt)            ( ((uint8 )__vat(pt,0) <<  0) \
                                        | ((uint16)__vat(pt,1) <<  8) \
                                        | ((uint32)__vat(pt,2) << 16) )

#   define GET_LE_UINT32(pt)            ( ((uint8 )__vat(pt,0) <<  0) \
                                        | ((uint16)__vat(pt,1) <<  8) \
                                        | ((uint32)__vat(pt,2) << 16) \
                                        | ((uint32)__vat(pt,3) << 24) )


#endif /* #ifndef GET_LE_UINT16 */

/* 以小端格式填充内存数据 */
#ifndef PUT_LE_UINT16
#   define PUT_LE_UINT16(pt,val)    do{ __vat(pt,0) = (uint8)((uint8 )(val) >>  0); \
                                        __vat(pt,1) = (uint8)((uint16)(val) >>  8); \
                                       }while(0)

#   define PUT_LE_UINT24(pt,val)    do{ __vat(pt,0) = (uint8)((uint8 )(val) >>  0); \
                                        __vat(pt,1) = (uint8)((uint16)(val) >>  8); \
                                        __vat(pt,2) = (uint8)((uint32)(val) >> 16); \
                                       }while(0)

#   define PUT_LE_UINT32(pt,val)    do{ __vat(pt,0) = (uint8)((uint8 )(val) >>  0); \
                                        __vat(pt,1) = (uint8)((uint16)(val) >>  8); \
                                        __vat(pt,2) = (uint8)((uint32)(val) >> 16); \
                                        __vat(pt,3) = (uint8)((uint32)(val) >> 24); \
                                       }while(0)
#endif

void delay (u16 cnt);
void delay_ms(u16 cnt);
unsigned int crc16_modbus(u8 *buf, u8 len);


#endif
