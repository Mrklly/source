#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H


#define PANEL_ADDR_BASE 0x10
#define PANEL_NUMBER_MAX 16


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

typedef enum _fresh_window_state_t_ {
    FRESH_TEXT_RECEIVED = 0x00,
    FRESH_TEXT_SEND     = 0x01,
    FRESH_MODE          = 0x02,
    FRESH_FAN           = 0x03,
    FRESH_TEMP_SETTING  = 0x04,
    FRESH_TEMP_ROOM     = 0x05
} fresh_window_state;

typedef enum _crc_type_t_ {
    CRC8  = 0x00,
    CRC16 = 0x01,
    CRC32 = 0x01,
    CRC_XOR = 0x02
} crc_type_t;

typedef enum _panel_name_t_ {
    NONE  = 0x00,
    QB502 = 0x01,
    QB522 = 0x02,
    QB811 = 0x03
} panel_name_t;

typedef enum _function_t_ {
    FUNC_CONFIG       = 0x00,
    FUNC_TEST_POLLING = 0x01,
    FUNC_TEST_NAMING  = 0x02
} function_t;

typedef enum _protocol_t_ {
    PROT_ACTIVE  = 0x00,
    PROT_PASSIVE = 0x01
} protocol_t;

typedef enum _dev_key_2_type_t_ {
    KEY_2_N = 0,    // key number != 2
    KEY_2_V = 1,    // key number = 2, board has 4 keys, aligned in vertial line, left and right
    KEY_2_H = 2,    // key number = 2, board has 4 keys, aligned in horizontal line, up and down
    KEY_2_6 = 3     // key number = 2, board has 6 keys, using key 2 and 5
} dev_key_2_type_t;


#endif // GLOBAL_VARIABLES_H
