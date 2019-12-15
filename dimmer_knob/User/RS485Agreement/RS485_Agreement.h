#ifndef _RS485_AGREEMENT_H
#define _RS485_AGREEMENT_H
#include <stdbool.h> 
#include "bsp_flash.h"
#include "bsp_uart.h"
#include "bsp_gpio.h" 
#include "bsp_timer.h"
/*****************************************************************************************************/
/*RS485EEPROM��ַ����*/
#define AGREEMENT_ADDR                0xFE00          //Э��ͷ��ַΪEEPROM����ʼ��ַ
#define DEFSTATE_ADDR                 0xFE01          //Ĭ�ϱ�����ַ
#define LED_SATTE_ADDR                0xFE02          //LED�ϵ�״̬�洢��ʼ��ַ��
#define KEY_FUNCTION_ADDR             0xFE06          //����������ʼ��ַ
#define KEY_PARTITION_ADDR            0xFE10          //������ʼ��ַ��
#define PANEL_ADDR                    0xFE18          //�������ַ��
#define RS485_AGREEMENT_LEN           24              //����485Э�����ݳ��ȡ�
/*���������б�*/
#define PANEL_ADDRESS                 0x01            //��������ַ��
#define PANEL_LED_STATE               0x02            //��������״̬��
#define KEY_AREA_PROPERTIES           0x0a            //���ð����������ԡ�
#define SET_KEY_FUNCTION              0x0b            //������尴�����ܡ�
#define SEND_DATA_BUS                 0x12            //��������߷������ݡ�
#define READ_PANEL_STATE              0x13            //��λ����ȡ���״̬��
#define RESTORE_FACTORY_SETTING       0xf0            //�ָ��������á�
#define READ_FACTORY_SETTING       0xf1            //��ȡ����������Ϣ��
/*-------LED������-------*/
#define LED_ON                        0x01
#define LED_OFF                       0x00
typedef struct{
  uint8_t SOH;                                        //Э��ͷ��
  uint8_t def_sta;                                    //Ĭ��״̬
  uint8_t led_state[4];                               //4·LED���ϵ�״̬��
  uint8_t key_function[8];                            //4·�����������á�2���ֽڴ���һ����������8���ֽڡ�
  uint8_t key_partition [4];                          //4·�����������ã�����Ϊ0ʱ�޷�����
  uint8_t Transboundary_zoning [4];                   //4·�������������ԣ�����8������û��������������ʱ�������á�00λû�п������ԡ�
  uint8_t panel_add;                                  //����ַ��
}rs458_InitTypedef;
/*�㲥���������б�*/
#define ORDINARY_BUTTON_CMD          0X01             //��ͨ������
#define TOTAL_SW_CMD                 0x02             //�ܿ���
#define GENERAL_OPEN_CMD             0x03             //�ܿ�
#define GENERAL_CLOSE_COM            0X04             //�ܹ�
#define REGIONAL_SW_CMD              0x05             //�����ܿ��ء�
#define MULTI_CONTROL_CMD            0X06             //��ؿ��ء�
#define DOUBLE_CONTROL_CMD           0X07             //˫�ؿ��ء�
#define DOUBLE_INTERLOCKING_CMD      0X08             //˫�ػ�����
#define MULTI_INTERLOCKING_CMD       0x09             //��ػ�����
#define POINT_PRESS_CMD              0X0A             //�㶯���ء�
#define POINT_LOCK_CMD               0X0B             //�㶯������
#define NIGHT_LAMP_CMD               0X0C             //ҹ�ơ�
#define SPECIAL_SERVICE_CMD          0X0D             //�������
#define INTER_LOCK_CMD               0X0E             //������
#define CLEAR_DISTURB_CMD            0X0F             //�������š�
#define WINDOW_CURTAINS_CMD          0X11             //������
/***********�ϵ�����**********/
#define PORCH_LAMP_CMMD              0X51             //�ȵƣ��ϵ�����ͬ��˫�ء�
#define ELECTRIC_LAMP_CMMD           0X52             //�ȵƣ��ϵ�������˫�ء�
/**********************************************��������***********************************************/
void RS485_Agreement_Config(void);
uint8_t RS485_Parse_check (uint8_t ReceiveLeng,uint8_t *buff);
uint32_t Broadcast_data_packing (uint8_t *buff);
void EEPROM_data_packing(void);
void Broadcast_control_LED(void);
void RS485_config_writeEeprom(uint32_t addr,uint8_t rise,uint8_t length);
void Always_LED_control(uint8_t mode);
void Common_pattern_control(uint8_t *buff);
void Regional_model_control(void);
void Regional_Close_opening(uint8_t state,uint8_t *buff);
void Power_State_Config(void);
bool Second_control(uint8_t key,uint8_t *buff);
void RX485_Prepare_receive(void);
void Linkage_open_close(uint8_t key,uint8_t state);
void Public_Multi_interlocking(uint8_t *buff);
void led_openstate0(uint8_t state,uint8_t key);
void clear_Disturb(uint8_t mode,uint8_t key);
uint8_t Exclusion_function (uint8_t mode,uint8_t key);
void led_closestate0(uint8_t state,uint8_t key);
void eeprom_erase_all(void);
#endif


 