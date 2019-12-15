#include "checksum.h"
#include "global_variables.h"


uint8_t get_crc(uint8_t *buf, uint8_t len)
{
//    uint8_t temp,i;
//    temp = buf[0];
//
//    for(i=1; i<len; i++) {
//        temp ^= buf[i];
//    }
//
//    return temp;
    
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

uint8_t get_crc_sum(uint8_t *buf, uint8_t len)
{
    uint8_t sum = 0, i;

    for(i = 0; i < len; i++) {
        sum += buf[i];
    }

    return sum;
}
