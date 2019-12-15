#include "drv_infrared.h"
#include "bsp_gpio.h"

gree_data_t gree_data;


void hal_ir_init(void)
{
    M0P_SYSCTRL->PERI_CLKEN_f.GPIO = 1;

    M0P_SYSCTRL->RCL_CR_f.TRIM = RCL_CR_TRIM_38400_VAL;

    M0P_SYSCTRL->SYSCTRL2 = 0x5A5A;
    M0P_SYSCTRL->SYSCTRL2 = 0xA5A5;
    M0P_SYSCTRL->SYSCTRL0_f.RCL_EN = 1;

    M0P_GPIO->PBADS_f.PB09 = 0;
    M0P_GPIO->PB09_SEL_f.SEL = 2;
    M0P_GPIO->PBDIR_f.PB09 = 0;

    M0P_GPIO->CTRL1_f.IR_POL = 0;
    M0P_GPIO->PBOUT_f.PB09 = 0;
}



void ir_bit_low(void)
{
    IR_DATA_H;
    delay_us(PULSE_LOW);
    IR_DATA_L;
    delay_us(PULSE_LOW);
    IR_DATA_H;
}

void ir_bit_high(void)
{
    IR_DATA_H;
    delay_us(PULSE_HIGH);
    IR_DATA_L;
    delay_us(PULSE_LOW);
    IR_DATA_H;
}

void ir_bit_stop(uint8_t bit)
{
    if(bit) {
        ir_bit_high();
    } else {
        ir_bit_low();
    }
}

void ir_data_low(uint8_t bit)
{
    for(uint8_t i=0; i<bit; i++) {
        ir_bit_low();
    }
}

void ir_data_high(uint8_t bit)
{
    for(uint8_t i=0; i<bit; i++) {
        ir_bit_high();
    }
}

void ir_bit_start(void)
{
    IR_DATA_L;
    delay_us(PULSE_START);
    IR_DATA_H;
    delay_us(PULSE_SPACE);
    IR_DATA_L;
    delay_us(PULSE_LOW);
}

void ir_send_byte(uint8_t data)
{
    for(uint8_t i = 0; i<8; i++) {
        if(data & 0x01) {
            ir_bit_high();
        } else {
            ir_bit_low();
        }
        data >>= 1;
    }
}

void ir_send_bit(uint16_t data, uint8_t bit)
{
    for(uint8_t i=0; i<bit; i++) {
        if(data & 0x01) {
            ir_bit_high();
        } else {
            ir_bit_low();
        }
        data >>= 1;
    }
}

void ir_send_word(uint32_t data)
{
//    uint8_t temp = 0;
//    uint32_t *p = (uint32_t *)&data;
//
//    temp = (uint8_t)((*p) >> 24);
//    ir_send_byte(temp);
//    temp = (uint8_t)((*p) >> 16);
//    ir_send_byte(temp);
//    temp = (uint8_t)((*p) >> 8);
//    ir_send_byte(temp);
//    temp = (uint8_t)((*p) >> 0);
//    ir_send_byte(temp);

    for(uint8_t i = 0; i<32; i++) {
        if(data & 0x01) {
            ir_bit_high();
        } else {
            ir_bit_low();
        }
        data >>= 1;
    }
}

void ir_send_pulse(uint8_t bit, uint16_t time)
{
    if(bit) {
        IR_DATA_H;
        delay_us(time);
        IR_DATA_L;
        delay_us(PULSE_LOW);
    } else {
        IR_DATA_L;
        delay_us(time);
    }
    IR_DATA_H;
}

void ir_send_pulse_buf(uint16_t *buf, uint16_t len)
{
    IR_DATA_L;
    delay_us(PULSE_START);
    IR_DATA_H;
    delay_us(PULSE_SPACE);
    IR_DATA_L;
    delay_us(PULSE_LOW);

    for(uint16_t i=0; i<len; i++) {
        IR_DATA_H;
        delay_us(buf[i]);
        IR_DATA_L;
        delay_us(PULSE_LOW);
    }
    IR_DATA_H;
}

void ir_send_data_buf(uint8_t *buf, uint8_t len)
{
    IR_DATA_L;
    delay_us(PULSE_START);
    IR_DATA_H;
    delay_us(PULSE_SPACE);
    IR_DATA_L;
    delay_us(PULSE_LOW);

    for(uint8_t i=0; i<len; i++) {
        ir_send_byte(buf[i]);
    }

    IR_DATA_H;
}

void ir_test(void)
{
    uint16_t ir_pulse[] = {1686, 562, 562, 562, 1686, 1686, 562, 562,
                           562, 562, 1686, 1686, 562, 562, 562, 562};

    uint8_t ir_data[] = {0x39, 0x0C, 0x20, 0x50};

    while(1) {
        ir_send_data_buf(ir_data, sizeof(ir_data)/sizeof(ir_data[0]));
        LED_RED_TOG;
        delay_ms(200);

        ir_send_pulse_buf(ir_pulse, sizeof(ir_pulse)/sizeof(ir_pulse[0]));
        LED_RED_TOG;
        delay_ms(200);
    }
}


void gree_control(void)
{
    if(       g_dev.sta.mode ==  TEMP_MODE_COOL) {
        gree_data.data1_s.mode = 1;
    } else if(g_dev.sta.mode ==  TEMP_MODE_WARM) {
        gree_data.data1_s.mode = 4;
    }

    gree_data.data1_s.power     = g_dev.sta.power;
    gree_data.data1_s.fan_speed = g_dev.sta.wind;
    gree_data.data1_s.temp      = g_dev.set_temp - 16;
    if(gree_data.data1_s.temp > 30) {
        gree_data.data1_s.temp = 30;
    }

    gree_data.data2 = gree_data.data1;
    gree_data.data2_s.checksum = 7;

    ir_bit_start();
    ir_send_word(gree_data.data1);
    ir_send_bit(0x02, 3);
    ir_send_pulse(1, 20000);

    ir_send_bit(0, 28);
    ir_send_bit(0x04, 4);

    ir_send_pulse(1, 40000);       // 40ms high pulse

    ir_bit_start();
    ir_send_word(gree_data.data2);
    ir_send_bit(0x02, 3);
    ir_send_pulse(1, 20000);       // 20ms high pulse
    ir_send_bit(0, 20);
    ir_send_bit(0x01, 4);

    ir_send_byte(0x50);
}

void ir_send_gree(void)
{
    ir_bit_start();
    ir_send_word(gree_data.data1);
    ir_send_bit(0x02, 3);
    ir_send_pulse(1, 20000);

    ir_send_bit(0, 24);

    ir_send_byte(gree_data.data3);

    ir_send_pulse(1, 40000);       // 40ms high pulse

    ir_bit_start();
    ir_send_word(gree_data.data2);
    ir_send_bit(0x02, 3);
    ir_send_pulse(1, 20000);       // 20ms high pulse
    ir_send_bit(0, 20);
    ir_send_bit(gree_data.data4, 12);
}

void ir_send_raw(uint8_t *pbuf, uint16_t len)
{
    uint16_t i=0;
    if(len == 0xffff) {
        return ;
    }

    for(; i<len-2; i+=4) {
        IR_DATA_L;
        delay_us(pbuf[i]<<8 | pbuf[i+1]);
        IR_DATA_H;
        delay_us(pbuf[i+2]<<8 | pbuf[i+3]);
    }

    IR_DATA_L;
    delay_us(pbuf[len-2]<<8 | pbuf[len-1]);
    IR_DATA_H;
}

