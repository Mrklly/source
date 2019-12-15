#ifndef _EEPROM_H_
#define _EEPROM_H_


#include "hc32_flash.h"


#define EEP_ADDR_THIS            0xFE00
#define EEP_ADDR_THIS_SOH        0xA5
#define EEP_ADDR_SOH_CLR         0xFF


typedef struct _dev_param_t_{
  uint8_t SOH;                                        //协议头。
  uint8_t def_sta;                                    //默认状态
  uint8_t led_state[4];                               //4路LED灯上电状态。
  uint8_t key_function[8];                            //4路按键功能配置。2个字节代表一个按键，共8个字节。
  uint8_t key_partition [4];                          //4路按键区域设置，数据为0时无分区。
  uint8_t Transboundary_zoning [4];                   //4路按键跨区域属性，最多跨8个区域，没有设置区域属性时不起作用。00位没有跨区属性。
  uint8_t panel_add;                                  //面板地址。
} dev_param_t;


extern dev_param_t dev_param;


void eeprom_get_all(void);
void eeprom_erase_all(void);

void eeprom_get_param(uint16_t addr, uint8_t *buf, uint8_t len);
void eeprom_set_param(uint16_t addr, uint8_t *buf, uint8_t len);


#endif

