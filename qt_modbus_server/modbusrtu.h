#ifndef MODBUSRTU_H
#define MODBUSRTU_H

#include <QtCore/QtGlobal>
#include <QByteArray>
#include <QObject>

#include "global_variables.h"

#define CTRL_FUN_RAED           0x03
#define CTRL_FUN_WRITE          0x10

#define FS_ERASE_PART           0x20        // 270 * 1024 bytes that contains ir data
#define FS_ERASE_ALL            0x21        // 8M bytes, it takes too long
#define FS_READ_ONE             0x22        // e.g. power=1, temp=16, fan=mid
#define FS_READ_PART            0x23        // 270 * 1024 bytes that contains ir data
#define FS_READ_WHOLE           0x24        // 8M bytes, it takes too long
#define FS_WRITE_ONE            0x25        // e.g. power=1, temp=16, fan=mid
#define FS_WRITE_PART           0x26        // 270 * 1024 bytes that contains ir data
#define FS_WRITE_WHOLE          0x27        // 8M bytes, it takes too long


class ModbusRtu : public QObject
{
    Q_OBJECT
public:
    explicit ModbusRtu(QObject *parent = 0);
    ~ModbusRtu() {}

    struct _modbus_rtu_data_ {
        unsigned char sync_head1;
        unsigned char sync_head2;
        unsigned char rtu_address;
        unsigned char function;
        unsigned char start_address;
        unsigned char register_number;
        QByteArray data;
        unsigned char crc_high;
        unsigned char crc_low;
    };

    float room_temp;
    uint8_t set_temp;
    union {
        uint8_t device_status;
        struct {
            uint8_t mod:3;
            uint8_t wind:2;
            uint8_t wind_auto:1;
            uint8_t valve:1;
            uint8_t power:1;
        }mode;
    };

    uint8_t g_fan_ultralow;

    unsigned char byte_counter;
    _modbus_rtu_data_ data;

    unsigned int modbus_crc16(unsigned char *buf, int len);

private slots:


signals:


private:



};


#endif // MODBUSRTU_H
