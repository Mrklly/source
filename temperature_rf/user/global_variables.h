#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIABLES_H_

#include <stdint.h>
#include "base_types.h"
#include "hc32l136.h"
#include "system_hc32l136.h"
#include "hc32_gpio.h"


#ifndef NULL
#define NULL	((void*)0)
#endif

#define RF433_MAX_SIZE        32
#define RF433_DEFAULT_TX_SIZE 31
#define RF433_DEFAULT_RX_SIZE 31

#define IR_DATA_SIZE    256

#define DEV_TYPE_THIS   0x07    // temperature control type is 0x22
#define DEV_ADDR_485    0x03
#define DEV_UART_BAUD   BAUD19200
#define DEV_RF_CH       10

#define RX_BUFF_LEN	 	90

#define KET_COUNT		5

#define KEY_INDEX_MODE   0
#define KEY_INDEX_FAN    1
#define KEY_INDEX_UP     2
#define KEY_INDEX_DOWN   3
#define KEY_INDEX_POWER  4


extern volatile uint16_t systick_counter;
extern volatile uint8_t g_set_type;


typedef void (*mod_hander)(void);
typedef struct _mode_t_ {
    uint8_t				type;
    mod_hander		hander;
} mode;

typedef void (*btn_hander)(uint8_t shirt);
typedef struct _com_io_t_ {
    en_gpio_port_t  key_port;
    en_gpio_pin_t	key_pin;
    btn_hander      hander;
} com_io;


typedef struct _link_dat_t_ {
    uint8_t		soh_h;
    uint8_t		soh_l;
    uint8_t		sl_addr;
    uint8_t		fun;
    uint8_t		st_addr;
    uint8_t		reg_cnt;
    uint8_t		buff[4];
} link_dat;

typedef struct _btn_ctrl_t_ {
    uint8_t		shirt;
    uint8_t		up;
    uint8_t		down;
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
    BAUD_ERR  = 0x00,
    BAUD9600  = 0x01,
    BAUD19200 = 0x02
}device_baudrate;

typedef enum _btn_setting_t_ {
    SET_NONE  = 0,
    SET_ADDR  = 1,
    SET_BAUD  = 2,
    SET_RF_CH = 3,
    CLR_EEP   = 4,
    IR_STUDY  = 5,
    IR_SEND   = 6,
    IR_RECV   = 7
} btn_setting;

typedef enum _clr_type_t_ {
    CLR_ID_THIS       = 0,
    CLR_ID_CTRL       = 1,
    CLR_RF_CH_THIS    = 2,
    CLR_RF_CH_CTRL    = 3,
    CLR_IR            = 4,
    CLR_ALL           = 5
} clr_type;

typedef struct _device_settings_t_ {
    btn_setting set_type;
    uint8_t  dev_addr;
    device_baudrate baud_rate;
} device_settings;


typedef struct _temp_dev_t_ {
    device_baudrate baud;
    uint8_t		addr;		/* 地址 */
    struct {
        uint8_t	mod:1;		/* 模式 */
        uint8_t	res:1;		/* 保留 */
        uint8_t	wind:1;		/* 风速 */
        uint8_t	power:1;	/* 电源 */
        uint8_t	T_up:1;		/* 增温 */
        uint8_t	T_down:1;	/* 减温 */
        uint8_t	res_1:2;		/* 保留 */
    } sw_sta;	/* 按键状态 */
    uint8_t		set_temp;	/* 用户设定温度 */
    float		room_temp;	/* 当前温度 */
    union {
        uint8_t	mod;
        struct {
            uint8_t	mod:3;		/*0：无;1：热风;2：冷风 3:送风 4：除温 5：自动*/
            uint8_t	wind:2;		/*0：无;1：低; 2：中;3：高; */
            uint8_t	wind_auto:1;/*0：无1：自动 */
            uint8_t	valve:1;
            uint8_t	power:1;
        } mode;
    };
} temp_dev;


typedef struct _dev_mac_ {
    uint8_t SOH;          // 1 0xA5
    uint8_t def_sta;      // 2 default state at start
    uint8_t res;          // 3 reserved
    uint8_t channel;      // 4
    uint8_t ctrl_type[4]; // 8 four control type
    uint8_t name[16];     // 24 device name, 10~15 is ID
    uint8_t len;          // 25, control len
//    uint8_t dat[64];      // control info
    uint8_t controller_name[6]; // master controller
    uint8_t frequency;
} dev_mac;


extern btn_ctrl  g_btn[KET_COUNT];
extern uint8_t g_rx_buff[RX_BUFF_LEN];
extern uint8_t g_uart_baud_changed;
extern uint8_t g_saving_flag;
extern uint8_t g_erasing_index;
extern uint8_t g_erasing_flag;
extern uint8_t g_ir_study_flag;
extern uint16_t g_ir_study_index;
extern uint8_t g_ir_rf_flag;
extern uint8_t g_ir_rf_send;
extern uint8_t g_ir_rf_recv;
extern temp_dev	g_dev;
extern dev_mac g_dev_mac;
extern uint8_t g_rf_channel;
extern mode g_mod[6];
extern volatile uint8_t g_mod_backup;
extern volatile uint8_t g_mod_backup_flag;
extern uint8_t g_lcd_on; //0 为休眠状态
extern uint8_t g_lcd_delay;
extern uint8_t g_led_on;
extern uint8_t g_led_delay;
extern volatile uint8_t g_idx;
extern float s_temp_bak;
extern volatile uint8_t rf433_flag;
extern uint8_t mm[RF433_DEFAULT_RX_SIZE];
extern uint8_t rf433_tx_buff[RF433_MAX_SIZE];
extern uint8_t rf433_rx_buff[RF433_MAX_SIZE];
extern uint8_t reg_mode_flag;
extern uint8_t  ir_index;
extern uint16_t ir_data[IR_DATA_SIZE];


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


unsigned int crc16_modbus(uint8_t *buf, uint8_t len);


#endif

