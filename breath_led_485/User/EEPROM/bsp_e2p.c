#include "bsp_e2p.h"
#include "bsp_flash.h"



void Write_RX_TO_EEPROM(void)
{
  flash_Init();                                       //��ʼ���ڲ�EEPROM��
//  flash_write(COM_EEPROM_ADDR,RS485_RX_buff,89);      //����д�롣
}

void Read_EEPROM_TO_BUFFF(void)
{
  flash_Init();                                       //��ʼ���ڲ�EEPROM��
//  flash_read(COM_EEPROM_ADDR,RS485_RX_buff,8);        //����EEPROM���ݡ�
}

