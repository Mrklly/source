#ifndef MODBUSRTU_H
#define MODBUSRTU_H

#include <QtCore/QtGlobal>
#include <QByteArray>
#include <QObject>

#include "global_variables.h"


class PanelConfig : public QObject
{
    Q_OBJECT
public:
    explicit PanelConfig(QObject *parent = 0);
    ~PanelConfig() {}

    struct _config_data_t_ {
        uint8_t head;
        uint8_t addr;
        uint8_t key_number;
        union {
            uint8_t misc;
            struct {
                uint8_t reservered :4;
                uint8_t name       :3;
                uint8_t near_sen   :1;
            };
        };
        uint8_t crc;
    };

//    panel_name_t name;

    crc_type_t crc_type;

    function_t function;

    protocol_t protocol;

    _config_data_t_ config;

    uint8_t get_crc8(const uint8_t *buf, int len);
    uint16_t modbus_crc16(uint8_t *buf, int len);
    uint8_t get_crc_xor(uint8_t *buf, uint8_t len);

private slots:


signals:


private:



};


#endif
