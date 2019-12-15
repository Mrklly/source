#include "DeviceInfo.h"

DeviceInfo::DeviceInfo()
{
    currentSettings.SOH = 0xA5;
    currentSettings.def_sta = 0;
    currentSettings.res = 0;
    currentSettings.channel = 0;
    currentSettings.len = 0;

    for(int i = 0; i < 4; i++) {
        currentSettings.ctrl_type[i] = 0;
    }

    for(int i = 0; i < 16; i++) {
        currentSettings.name[i] = 0;
    }

    for(int i = 0; i < 64; i++) {
        currentSettings.dat[i] = 0;
    }

    device_number = 0;
    device_been_set = 0;
    SetByHandFlag = 0;
    SetAutoFlag = 0;

}

void DeviceInfo::SetDevice()
{

}
