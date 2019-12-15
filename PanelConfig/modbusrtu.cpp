#include "modbusrtu.h"


PanelConfig::PanelConfig(QObject *parent) :
    QObject(parent)
{
    config.head = 0x5A;
    config.addr = 0x10;
    config.key_number = 0;
    config.misc = 0;
    config.crc = 0;
}

uint8_t PanelConfig::get_crc8(const uint8_t *buf, int len)
{
    const uint8_t *data = buf;
    unsigned crc = 0;
    int i, j;

    for (j = len; j; j--, data++) {
        crc ^= (*data << 8);
        for(i = 8; i; i--) {
        if (crc & 0x8000)
        crc ^= (0x1070 << 3);
        crc <<= 1;
        }
    }

    return (uint8_t)(crc >> 8);
}

uint16_t PanelConfig::modbus_crc16(uint8_t *buf, int len)
{
    uint16_t crc = 0xFFFF;

      for (int pos = 0; pos < len; pos++) {
        crc ^= (uint16_t)buf[pos];          // XOR byte into least sig. byte of crc

        for (int i = 8; i != 0; i--) {    // Loop over each bit
          if ((crc & 0x0001) != 0) {      // If the LSB is set
            crc >>= 1;                    // Shift right and XOR 0xA001
            crc ^= 0xA001;
          }
          else                            // Else LSB is not set
            crc >>= 1;                    // Just shift right
        }
      }
      // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
      return crc;
}

uint8_t PanelConfig::get_crc_xor(uint8_t *buf, uint8_t len)
{
    uint8_t temp,i;
    temp = buf[0];

    for(i=1; i<len; i++) {
        temp ^= buf[i];
    }

    return temp;
}
