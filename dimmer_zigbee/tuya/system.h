/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2018-0704, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: system.c
**��        ��: zigbee���ݴ�����
**ʹ �� ˵ �� : �û�������ĸ��ļ�ʵ������
**
**
**--------------��ǰ�汾�޶�---------------------------------------------------
** ��  ��: v1.0.0
** �ա���: 2018��7��4��
** �衡��: 1:Э�����
**
**-----------------------------------------------------------------------------
******************************************************************************/
#ifndef __SYSTEM_H_
#define __SYSTEM_H_


#include "stdint.h"


#ifdef SYSTEM_GLOBAL
#define SYSTEM_EXTERN
#else
#define SYSTEM_EXTERN   extern
#endif

//=============================================================================
//֡���ֽ�˳��
//=============================================================================
#define         HEAD_FIRST                      0
#define         HEAD_SECOND                     1
#define         PROTOCOL_VERSION                2
#define         SEQ_HIGH                        3
#define         SEQ_LOW                         4
#define         FRAME_TYPE                      5
#define         LENGTH_HIGH                     6
#define         LENGTH_LOW                      7
#define         DATA_START                      8

//=============================================================================
//����֡����
//=============================================================================
#define         PRODUCT_INFO_CMD                0x01                            //��Ʒ��Ϣ
#define         ZIGBEE_STATE_CMD                0x02                            //zigbee����״̬    
#define         ZIGBEE_CFG_CMD                  0x03                            //����zigbeeģ��
#define         ZIGBEE_DATA_REQ_CMD             0x04                            //zigbee�����·�����
#define         DATA_RSP_SYNC_CMD               0x05                            //MCU״̬�����ϱ�
#define         DATA_RSP_ASYNC_CMD              0x06                            //MCU״̬�����ϱ�
#define         FUNC_TEST_CMD                   0x08                            //zigbeeģ�鹦�ܲ���
#ifdef SERIAL_PROTOCOL_SCENE_ENABLE
#define         SCENE_SWITCH_NUM_GET_CMD        0x09                            //��ȡ�������ظ����������ڳ������أ�
#define         SCENE_SWITCH_ID_REPORT_CMD      0x0A                            //��ȡ�����ϱ�ID�������ڳ������أ�
#endif
#define         MCU_OTA_VER_REQ_CMD             0x0B                            //OTA �汾����
#define         MCU_OTA_NOTIFY_CMD              0x0C                            //OTA ����֪ͨ
#define         MCU_OTA_DATA_REQ_CMD            0x0D                            //OTA �̼���������
#define         MCU_OTA_END_CMD                 0x0E                            //OTA ����ϱ�
#define         TIME_GET_CMD                    0x24                            //zigbeeģ��ʱ��ͬ��

//=============================================================================
#define SERIAL_PROTOCOL_VER          0x02                                            //Э��汾��
#define PROTOCOL_HEAD                0x09                                            //�̶�Э��ͷ����
#define FIRST_FRAME_HEAD             0x55
#define SECOND_FRAME_HEAD            0xaa
#define SERIAL_PROTOCOL_HDR          (FIRST_FRAME_HEAD<<8 | SECOND_FRAME_HEAD)   ///<Э����ͷ����


//=============================================================================
extern unsigned char zigbee_queue_buf[PROTOCOL_HEAD + ZIGBEE_UART_QUEUE_LMT];        //���ڶ��л���
extern unsigned char zigbee_uart_rx_buf[PROTOCOL_HEAD + ZIGBEE_UART_RECV_BUF_LMT];   //���ڽ��ջ���
extern unsigned char zigbee_uart_tx_buf[PROTOCOL_HEAD + ZIGBEE_UART_SEND_BUF_LMT];   //���ڷ��ͻ���


extern unsigned char *queue_in;
extern unsigned char *queue_out;
extern volatile uint8_t queue_total_data;

/*****************************************************************************
�������� : set_zigbee_uart_byte
�������� : дzigbee_uart�ֽ�
������� : dest:��������ʵ��ַ;
           byte:д���ֽ�ֵ
���ز��� : д����ɺ���ܳ���
*****************************************************************************/
unsigned short set_zigbee_uart_byte(unsigned short dest, unsigned char byte);

/*****************************************************************************
�������� : set_zigbee_uart_buffer
�������� : дzigbee_uart_buffer
������� : dest:Ŀ���ַ
           src:Դ��ַ
           len:���ݳ���
���ز��� : ��
*****************************************************************************/
unsigned short set_zigbee_uart_buffer(unsigned short dest, unsigned char *src, unsigned short len);

/*****************************************************************************
�������� : zigbee_uart_write_frame
�������� : ��zigbee���ڷ���һ֡����
������� : fr_type:֡����
           len:���ݳ���
���ز��� : ��
*****************************************************************************/
void zigbee_uart_write_frame(unsigned char fr_type, unsigned short len);

/*****************************************************************************
�������� : get_check_sum
�������� : ����У���
������� : pack:����Դָ��
           pack_len:����У��ͳ���
���ز��� : У���
*****************************************************************************/
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len);

/*****************************************************************************
�������� : get_queue_total_data
�������� : ��ȡ�������ַ�
������� : NULL
���ز��� : char
*****************************************************************************/
unsigned short get_queue_total_data(void);

void zigbee_ota_data_req_send(unsigned char* pid, \
                              unsigned char ver, \
                              unsigned int image_offset, \
                              unsigned char req_data_len);

unsigned char Queue_Read_Byte(void);
void data_handle(unsigned short offset);
void seq_num_set(unsigned short seq_num);
unsigned short seq_num_get(void);

#endif
