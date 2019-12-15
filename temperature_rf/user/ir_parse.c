#include "ir_parse.h"


gree_data_t gree_data;


uint8_t ir_parse(void)
{
    if(!(ir_sta & IR_STA_END)) {
        return 0;
    }

// ir_data[0]: start space, 4.5ms
    if((PULSE_SPACE + PULSE_SPACE_ZONE) < ir_data[0] || ir_data[0] < (PULSE_SPACE - PULSE_SPACE_ZONE)) {
        return 0;
    }

    if(ir_index != 139) {
        ir_sta = 0;
        ir_index = 0;

        printf("IR received error...\n");
        return 0;
    }

    gree_data.data1 = 0;
    gree_data.data2 = 0;
    gree_data.data3 = 0;
    gree_data.data4 = 0;

// ir_data[1] to ir_data[32]: 32 bit data1, LSB first
    for(uint8_t i=0; i<32; i++) {
        gree_data.data1 <<= 1;
        if((PULSE_H - PULSE_H_ZONE) < ir_data[32-i] && ir_data[32-i] < (PULSE_H + PULSE_H_ZONE)) {
            gree_data.data1++;
        }
    }

// ir_data[33] to ir_data[35]: 3bit 010

// ir_data[36]: 1bit, 20ms Pulse

// ir_data[37] to ir_data[60]: 24bit low Pulse

// ir_data[61] to ir_data[68]: 8bit data3
    for(uint8_t i=0; i<8; i++) {
        gree_data.data3 <<= 1;
        if((PULSE_H - PULSE_H_ZONE) < ir_data[68-i] && ir_data[68-i] < (PULSE_H + PULSE_H_ZONE)) {
            gree_data.data3++;
        }
    }

// ir_data[69]: 1bit, 40ms Pulse

// ir_data[70]: start space, 4.5ms

// ir_data[71] to ir_data[102]: 32 bit data1
    for(uint8_t i=0; i<32; i++) {
        gree_data.data2 <<= 1;
        if((PULSE_H - PULSE_H_ZONE) < ir_data[102-i] && ir_data[102-i] < (PULSE_H + PULSE_H_ZONE)) {
            gree_data.data2++;
        }
    }

// ir_data[103] to ir_data[105]: 3bit 010

// ir_data[106]: 1bit, 20ms Pulse

// ir_data[107] to ir_data[126]: 20bit low Pulse

// ir_data[127] to ir_data[138]: 12bit data4
    for(uint8_t i=0; i<12; i++) {
        gree_data.data4 <<= 1;
        if((PULSE_H - PULSE_H_ZONE) < ir_data[138-i] && ir_data[138-i] < (PULSE_H + PULSE_H_ZONE)) {
            gree_data.data4++;
        }
    }

    memset(ir_data, 0, 139);

    ir_sta = 0;
    ir_index = 0;

    if((gree_data.data1 & 0x00ffffff) != (gree_data.data2 & 0x00ffffff)) {

        printf("checksum error...\n");
        return 0;

    } else {

        printf("data1: 0x%lx\n", gree_data.data1);
        printf("data2: 0x%lx\n", gree_data.data2);
        printf("data3: 0x%lx\n", gree_data.data3);
        printf("data4: 0x%lx\n", gree_data.data4);

        return 1;
    }

    return 1;
}

