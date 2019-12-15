#ifndef _IR_PARSE_H_
#define _IR_PARSE_H_


#include "drv_ir.h"
#include "global_variables.h"

typedef struct _gree_data_t {
    union {
        uint32_t data1;
        struct {
            uint32_t mode       :3;
            uint32_t power      :1;
            uint32_t fan_speed  :2;
            uint32_t fan_sweep  :1;
            uint32_t sleep      :1;
            uint32_t temp       :4;
            uint32_t timing     :8;
            uint32_t strong     :1;
            uint32_t led        :1;
            uint32_t health     :1;
            uint32_t dry        :1;
            uint32_t aeration   :1;
            uint32_t reserve0   :7;
        } data1_s;
    };

    union {
        uint32_t data2;
        struct {
            uint32_t fan_updn   :1;
            uint32_t reserve0   :3;
            uint32_t fan_ltrt   :1;
            uint32_t reserve1   :3;
            uint32_t temp_room  :2;
            uint32_t reserve2   :16;
            uint32_t energy     :1;
            uint32_t reserve3   :1;
            uint32_t checksum   :4;
        } data2_s;
    };

    uint8_t data3;
    uint8_t reserve0;   // align the structure
    uint16_t data4;
}gree_data_t;


extern gree_data_t gree_data;


uint8_t ir_parse(void);


#endif

