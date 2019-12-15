#ifndef _EEPROM_H_
#define _EEPROM_H_


#include "stm8s.h"


#define EEP_ADDR_THIS            0x4000
#define EEP_ADDR_THIS_SOH        0xA5
#define EEP_ADDR_SOH_CLR         0x00


typedef struct _dev_param_t_{
  uint8_t SOH;                                        //Э��ͷ��
  uint8_t def_sta;                                    //Ĭ��״̬
  uint8_t led_state[4];                               //4·LED���ϵ�״̬��
  uint8_t key_function[8];                            //4·�����������á�2���ֽڴ���һ����������8���ֽڡ�
  uint8_t key_partition [4];                          //4·�����������ã�����Ϊ0ʱ�޷�����
  uint8_t Transboundary_zoning [4];                   //4·�������������ԣ�����8������û��������������ʱ�������á�00λû�п������ԡ�
  uint8_t panel_add;                                  //����ַ��
} dev_param_t;


#define EEP_ADDR_START   ((dev_param_t *) EEP_ADDR_THIS)


extern dev_param_t dev_param;


void eeprom_get_all(void);
void eeprom_erase_all(void);

void eeprom_get_param(uint16_t addr, uint8_t *buf, uint8_t len);
void eeprom_set_param(uint16_t addr, uint8_t *buf, uint8_t len);


#endif

