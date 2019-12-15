#ifndef _THIRD_AGREEMENT_H
#define _THIRD_AGREEMENT_H


#include "bsp_flash.h"
#include "bsp_gpio.h"
#include "bsp_uart.h"


#define AGREEMENT_LEN           8               //���������Э�����ݳ��ȡ�
#define MODIFY_ADDRESS          0x01            //�޸ĵ�ַ���
#define CONTROL_SWITCH          0x02            //���ؿ������
#define SWITCH_STATE            0x03            //��ѯ����״̬��
#define QUERRY_STATE            0x04            //��ѯ��尴�������
#define RESET_CMD               0x06            //�����������ݡ�


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

