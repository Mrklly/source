#ifndef _THIRD_AGREEMENT_H
#define _THIRD_AGREEMENT_H


#include "bsp_flash.h"
#include "bsp_gpio.h"
#include "bsp_uart.h"


#define AGREEMENT_LEN           8               //定义第三方协议数据长度。
#define MODIFY_ADDRESS          0x01            //修改地址命令。
#define CONTROL_SWITCH          0x02            //开关控制命令。
#define SWITCH_STATE            0x03            //查询开关状态。
#define QUERRY_STATE            0x04            //查询面板按键情况。
#define RESET_CMD               0x06            //擦除所有数据。


void protocol_run(void);
void Modify_Address(void);
void KEY_Contr(void);
void SWITCH_State(void);
void protocol_send_key_status(uint8_t *buf_key, uint8_t *buf_sta);
void protocol_param_clear(void);
uint8_t protocol_get_crc(uint8_t *buf, uint8_t len);
void protocol_param_get(void);
void protocol_param_save(void);


#endif

