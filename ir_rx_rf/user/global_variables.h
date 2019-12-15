#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIABLES_H_

#include "hc32f005.h"


#ifndef NULL
#define NULL	((void*)0)
#endif


typedef enum _device_baudrate_t_ {
    BAUDRATE_ERR  = 0x00,
    BAUDRATE9600  = 0x01,
    BAUDRATE19200 = 0x02
}device_baudrate;

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

extern volatile uint8_t rf433_flag;
extern uint8_t mm[32];
extern temp_dev g_dev;
extern dev_mac g_dev_mac;
extern volatile uint8_t modbus_status;
extern volatile uint8_t modbus_flag;
extern volatile uint16_t g_time_delay;
extern volatile uint8_t g_time_flag;


//void delay_us(uint16_t cnt);
//void delay_ms(uint16_t cnt);
unsigned int crc16_modbus(uint8_t *buf, uint8_t len);


#endif
