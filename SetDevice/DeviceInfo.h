#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <qstring.h>


class DeviceInfo {

public:
    struct Settings {
        uchar   SOH;          // 1 0xA5
        uchar   def_sta;      // 2 default state at start
        uchar   res;          // 3 reserved
        uint8_t channel;      // 4
        uchar   ctrl_type[4]; // 8 four control type
        uchar   name[16];     // 24 device name, 10~15 is ID
        uint8_t len;          // 25, control len
        uchar   dat[64];      // control info
    };

    Settings currentSettings;
    uint16_t device_number;
    uint16_t device_been_set;

    bool SetByHandFlag;
    bool SetAutoFlag;

    explicit DeviceInfo();
    ~DeviceInfo();

    Settings settings() const;

    void SetDevice();

};



#endif
