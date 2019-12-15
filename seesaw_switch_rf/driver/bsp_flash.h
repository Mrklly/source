#ifndef _BSP_FLASH_H_
#define _BSP_FLASH_H_


#include "hc32_flash.h"


#define AGREEMENT_ADDR                0xFE00          //Э��ͷ��ַΪEEPROM����ʼ��ַ
#define DEFSTATE_ADDR                 0xFE01          //Ĭ�ϱ�����ַ
#define LED_SATTE_ADDR                0xFE02          //LED�ϵ�״̬�洢��ʼ��ַ��
#define KEY_FUNCTION_ADDR             0xFE06          //����������ʼ��ַ
#define KEY_PARTITION_ADDR            0xFE10          //������ʼ��ַ��
#define PANEL_ADDR                    0xFE18          //�������ַ��
#define RS485_AGREEMENT_LEN           24              //����485Э�����ݳ��ȡ�


#define PANEL_ADDRESS                 0x01            //��������ַ��
#define PANEL_LED_STATE               0x02            //��������״̬��
#define KEY_AREA_PROPERTIES           0x0a            //���ð����������ԡ�
#define SET_KEY_FUNCTION              0x0b            //������尴�����ܡ�
#define SEND_DATA_BUS                 0x12            //��������߷������ݡ�
#define READ_PANEL_STATE              0x13            //��λ����ȡ���״̬��
#define RESTORE_FACTORY_SETTING       0xf0            //�ָ��������á�


/*����������豸Э��״̬�ڴ洢���еĴ洢��ַ0x40A5+0x5B��ǰ90����ַ�����ڲ�Э��*/
#define EEPROM_THIRD_ADDR       0x40A5     //�洢�̶��룬0x55
#define EEPROM_CMD_ADDR         0x40A6     //�洢�豸�����0x01
#define EEPROM_DEVICE_ADDR      0x40A7     //�洢���豸��ַ��      
#define EEPROM_LED1_ADDR        0x40A8     //LED1״̬�洢��ַ��
#define EEPROM_LED2_ADDR        0x40A9     //LED2״̬�洢��ַ��
#define EEPROM_LED3_ADDR        0x40AA     //LED3״̬�洢��ַ��
#define EEPROM_LED4_ADDR        0x40AB     //LED4״̬�洢��ַ��
#define EEPROM_CHECK_ADDR       0x40AC     //�洢У�����ַ����У�飨4���Ƶ�״ֵ̬֮�ͣ���


void flash_init(void);
void flash_interrupt(void);
en_result_t eeprom_write_byte(uint32_t addr, uint8_t data);
en_result_t eeprom_write_half_word(uint32_t addr, uint16_t data);
en_result_t eeprom_write_word(uint32_t addr, uint32_t data);
void flash_read_buf(uint32_t addr, uint8_t *pbuff, uint8_t len);
uint8_t flash_write_buf(uint32_t addr, uint8_t *pdat, uint8_t len);


#endif
