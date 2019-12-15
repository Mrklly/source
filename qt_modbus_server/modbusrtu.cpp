#include "modbusrtu.h"


ModbusRtu::ModbusRtu(QObject *parent) :
    QObject(parent)
{
    data.sync_head1 = 0x55;
    data.sync_head2 = 0xAA;
    data.rtu_address = 0x03;
    data.function = 0;
    data.start_address = 0;
    data.register_number = 0;
    data.data = 0;
    data.crc_high = 0;
    data.crc_low = 0;

    byte_counter = 0;
    room_temp = 0;
    set_temp = 0;
}

unsigned int ModbusRtu::modbus_crc16(unsigned char *buf, int len)
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
