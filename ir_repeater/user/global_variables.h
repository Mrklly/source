#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIABLES_H_


#include "hc32l13x.h"
#include "hc32_gpio.h"


#define KET_COUNT       1


#define DEV_TYPE_THIS   0x22
#define DEV_TYPE_CTRL   0x00
#define DEV_TYPE_PANEL  0x07



#define UART_BUF_LEN     1024      // to support getting setting mac from PC

#define RF433_MAX_SIZE           32
#define RF433_DEFAULT_TX_SIZE    31
#define RF433_DEFAULT_RX_SIZE    31


typedef enum _fan_status_t_ {
    FAN_CLZ = 0x00,
    FAN_MIN = 0x01,
    FAN_MID = 0x02,
    FAN_MAX = 0x03,
    FAN_AUTO = 0x04
} fan_status_t;

typedef enum _temperature_status_t_ {
    TEMP_MODE_COOL = 0x00,
    TEMP_MODE_WARM = 0x01,
    TEMP_MODE_FAN  = 0x02,
    TEMP_MODE_DRY  = 0x03,
    TEMP_MODE_AUTO = 0x04,
    TEMP_MODE_NONE = 0x05
} tempe_status_t;

typedef enum _dev_baud_t_ {
    BAUD_ERR  = 0x00,
    BAUD9600  = 0x01,
    BAUD19200 = 0x02
} dev_baud_t;

typedef union _dev_sta_t_ {
    uint8_t status;
    struct {
        uint8_t	mode:3;		/*0：无;1：热风;2：冷风 3:送风 4：除温 5：自动*/
        uint8_t	wind:2;		/*0：无;1：低; 2：中;3：高; */
        uint8_t	wind_auto:1;/*0：无1：自动 */
        uint8_t	valve:1;
        uint8_t	power:1;
    };
} dev_sta_t;

typedef struct _dev_param_t_{
    uint8_t SOH;
    uint8_t addr;
    uint8_t rf_ch;
    dev_baud_t baud_rate;
    int8_t temp_offset;
    uint8_t mac_this[6];
    uint8_t mac_ctrl[6];
    uint8_t mac_panel[6];
} dev_param_t;

typedef struct _dev_settings_t_{
    uint8_t SOH;          // 1 0xA5
    uint8_t def_sta;      // 2 default state at start
    uint8_t res;          // 3 reserved
    uint8_t channel;      // 4
    uint8_t ctrl_type[4]; // 8 four control type
    uint8_t name[16];     // 24 device name, 10~15 is ID
    uint8_t len;          // 25, control len
    uint8_t dat[64];      // control info
} dev_settings_t;

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


typedef struct _dev_this_t_{
    uint8_t addr;
    uint8_t rf_ch;
    uint8_t timing_task;
    uint8_t set_temp;
    float room_temp;
    int8_t temp_offset;
    uint8_t mac_this[6];
    uint8_t mac_ctrl[6];
    uint8_t mac_panel[6];
    dev_sta_t sta;
    dev_baud_t baud;
    uint8_t flag_fan_ultralow;
} dev_this_t;

typedef struct _btn_ctrl_t_ {
    uint16_t shirt;
    uint8_t up;
    uint8_t down;
} btn_ctrl_t;

typedef enum _pair_flag_t_ {
    PAIR_NONE    = 0x00,
    PAIR_PREPARE = 0x01,
    PAIR_CLEAR   = 0x02,
    PAIR_SLAVE   = 0x03,
    PAIR_MASTER  = 0x04,
    PAIR_END     = 0x05
} pair_flag_t;

typedef struct _gpio_port_pin_t_ {
    en_gpio_port_t port;
    en_gpio_pin_t  pin;
} gpio_port_pin_t;

typedef struct _link_dat_t_ {
    uint8_t		soh_h;
    uint8_t		soh_l;
    uint8_t		sl_addr;
    uint8_t		fun;
    uint8_t		st_addr;
    uint8_t		reg_cnt;
    uint8_t		buff[4];
} link_dat;


extern dev_param_t g_eep;
extern dev_this_t  g_dev;
extern dev_mac g_dev_mac;
extern volatile uint8_t g_mod_backup;
extern volatile uint8_t g_mod_backup_flag;
extern volatile uint8_t g_fan_ultralow;

extern btn_ctrl_t g_btn[KET_COUNT];

extern uint8_t g_uart_buff[UART_BUF_LEN];
extern volatile uint16_t g_uart_idx;

extern volatile uint16_t systick_counter;

extern uint8_t g_rf_buf[RF433_DEFAULT_RX_SIZE];
extern uint8_t g_rf_flag;

extern uint8_t g_ir_study_buf[1024];
extern uint16_t g_ir_idx;
extern uint8_t g_ir_flag;
extern uint8_t g_ir_transfer;


#endif

