#ifndef _INFRARED_H_
#define _INFRARED_H_

#include "hc32f005.h"
#include "hc32_ddl.h"
#include "drv_infrared.h"

#include "global_variables.h"


#define PULSE_LOW       562     // 562
#define PULSE_HIGH      1686    // 1686
#define PULSE_START     9000    // 9000
#define PULSE_SPACE     4500    // 4500


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


void ir_bit_low(void);
void ir_bit_high(void);
void ir_bit_stop(uint8_t bit);
void ir_data_low(uint8_t bit);
void ir_data_high(uint8_t bit);
void ir_bit_start(void);
void ir_send_byte(uint8_t data);
void ir_send_bit(uint16_t data, uint8_t bit);
void ir_send_word(uint32_t data);
void ir_send_pulse(uint8_t bit, uint16_t time);
void ir_send_pulse_buf(uint16_t *buf, uint16_t len);
void ir_send_data_buf(uint8_t *buf, uint8_t len);
void ir_test(void);
void gree_control(void);
void ir_send_gree(void);


#endif

