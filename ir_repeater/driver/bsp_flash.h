#ifndef _BSP_FLASH_H_
#define _BSP_FLASH_H_


#include "hc32_flash.h"


#define AGREEMENT_ADDR                0xFE00          //协议头地址为EEPROM的起始地址
#define DEFSTATE_ADDR                 0xFE01          //默认保留地址
#define LED_SATTE_ADDR                0xFE02          //LED上电状态存储起始地址。
#define KEY_FUNCTION_ADDR             0xFE06          //按键功能起始地址
#define KEY_PARTITION_ADDR            0xFE10          //分区起始地址。
#define PANEL_ADDR                    0xFE18          //存放面板地址。
#define RS485_AGREEMENT_LEN           24              //定义485协议数据长度。


#define PANEL_ADDRESS                 0x01            //设置面板地址。
#define PANEL_LED_STATE               0x02            //设置面板灯状态。
#define KEY_AREA_PROPERTIES           0x0a            //设置按键区域属性。
#define SET_KEY_FUNCTION              0x0b            //设置面板按键功能。
#define SEND_DATA_BUS                 0x12            //面板向总线发送数据。
#define READ_PANEL_STATE              0x13            //上位机读取面板状态。
#define RESTORE_FACTORY_SETTING       0xf0            //恢复出厂设置。


/*定义第三方设备协议状态在存储器中的存储地址0x40A5+0x5B，前90个地址用于内部协议*/
#define EEPROM_THIRD_ADDR       0x40A5     //存储固定码，0x55
#define EEPROM_CMD_ADDR         0x40A6     //存储设备的命令：0x01
#define EEPROM_DEVICE_ADDR      0x40A7     //存储的设备地址。      
#define EEPROM_LED1_ADDR        0x40A8     //LED1状态存储地址。
#define EEPROM_LED2_ADDR        0x40A9     //LED2状态存储地址。
#define EEPROM_LED3_ADDR        0x40AA     //LED3状态存储地址。
#define EEPROM_LED4_ADDR        0x40AB     //LED4状态存储地址。
#define EEPROM_CHECK_ADDR       0x40AC     //存储校验码地址：和校验（4个灯的状态值之和）。


void flash_init(void);
void flash_interrupt(void);
en_result_t eeprom_write_byte(uint32_t addr, uint8_t data);
en_result_t eeprom_write_half_word(uint32_t addr, uint16_t data);
en_result_t eeprom_write_word(uint32_t addr, uint32_t data);
void flash_read_buf(uint32_t addr, uint8_t *pbuff, uint8_t len);
uint8_t flash_write_buf(uint32_t addr, uint8_t *pdat, uint8_t len);


#endif
