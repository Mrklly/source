#ifndef _THIRD_AGREEMENT_H_
#define _THIRD_AGREEMENT_H_


#include "bsp_flash.h"
#include "bsp_gpio.h"
#include "bsp_uart.h"


#define MODIFY_ADDRESS          0x01            //修改地址命令。
#define CONTROL_SWITCH          0x02            //开关控制命令。
#define SWITCH_STATE            0x03            //查询开关状态。
#define QUERRY_STATE            0x04            //查询面板按键情况。
#define RESET_CMD               0x06            //擦除所有数据。


/*第三方协议列表结构体*/
typedef struct Protocol_list{
    uint8_t     Beginning_agreement ;   //协议头：固定为0x55。
    uint8_t     Device_address ;        //本机设备地址。
    uint8_t     LED_channel;            //代表有多少路LED。
    uint8_t     KEY_channel;           // 代表有多少按键。      
    uint8_t     LED1_state;
    uint8_t     LED2_state;
    uint8_t     LED3_state;
    uint8_t     LED4_state;
    uint8_t     KEY1_state;
    uint8_t     KEY2_state;
    uint8_t     KEY3_state;
    uint8_t     KEY4_state;
}AgreementTypet;


void protocol_run(void);
void protocol_control_out(uint8_t data);
void protocol_clear_buf(void);
void protocol_master_check(void);
uint8_t get_crc(uint8_t *buf, uint8_t len);
uint8_t get_crc8(const void *buf, int len);

void near_check(void);


#endif

