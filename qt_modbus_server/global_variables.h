#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

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

typedef enum _button_up_down_ {
    BUTTON_UP   = 0x00,
    BUTTON_DOWN = 0x01
} button_up_down;

#endif // GLOBAL_VARIABLES_H
