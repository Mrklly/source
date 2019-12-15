#ifndef _THIRD_AGREEMENT_H_
#define _THIRD_AGREEMENT_H_


#include "bsp_flash.h"
#include "bsp_gpio.h"
#include "bsp_uart.h"


#define MODIFY_ADDRESS          0x01            //�޸ĵ�ַ���
#define CONTROL_SWITCH          0x02            //���ؿ������
#define SWITCH_STATE            0x03            //��ѯ����״̬��
#define QUERRY_STATE            0x04            //��ѯ��尴�������
#define RESET_CMD               0x06            //�����������ݡ�


/*������Э���б�ṹ��*/
typedef struct Protocol_list{
    uint8_t     Beginning_agreement ;   //Э��ͷ���̶�Ϊ0x55��
    uint8_t     Device_address ;        //�����豸��ַ��
    uint8_t     LED_channel;            //�����ж���·LED��
    uint8_t     KEY_channel;           // �����ж��ٰ�����      
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
void Modify_Address(void);
void KEY_Contr(void);
void SWITCH_State(void);
void protocol_send_key_status(uint8_t *buf_key, uint8_t *buf_sta);
void rest_config(void);
uint8_t protocol_get_crc(uint8_t *buf, uint8_t len);
void protocol_param_get(void);
void protocol_param_save(void);


#endif

