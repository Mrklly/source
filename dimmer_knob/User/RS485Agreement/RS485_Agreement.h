#ifndef _RS485_AGREEMENT_H
#define _RS485_AGREEMENT_H
#include <stdbool.h> 
#include "bsp_flash.h"
#include "bsp_uart.h"
#include "bsp_gpio.h" 
#include "bsp_timer.h"
/*****************************************************************************************************/
/*RS485EEPROM地址定义*/
#define AGREEMENT_ADDR                0xFE00          //协议头地址为EEPROM的起始地址
#define DEFSTATE_ADDR                 0xFE01          //默认保留地址
#define LED_SATTE_ADDR                0xFE02          //LED上电状态存储起始地址。
#define KEY_FUNCTION_ADDR             0xFE06          //按键功能起始地址
#define KEY_PARTITION_ADDR            0xFE10          //分区起始地址。
#define PANEL_ADDR                    0xFE18          //存放面板地址。
#define RS485_AGREEMENT_LEN           24              //定义485协议数据长度。
/*配置命令列表*/
#define PANEL_ADDRESS                 0x01            //设置面板地址。
#define PANEL_LED_STATE               0x02            //设置面板灯状态。
#define KEY_AREA_PROPERTIES           0x0a            //设置按键区域属性。
#define SET_KEY_FUNCTION              0x0b            //设置面板按键功能。
#define SEND_DATA_BUS                 0x12            //面板向总线发送数据。
#define READ_PANEL_STATE              0x13            //上位机读取面板状态。
#define RESTORE_FACTORY_SETTING       0xf0            //恢复出厂设置。
#define READ_FACTORY_SETTING       0xf1            //读取出厂配置信息。
/*-------LED灯命令-------*/
#define LED_ON                        0x01
#define LED_OFF                       0x00
typedef struct{
  uint8_t SOH;                                        //协议头。
  uint8_t def_sta;                                    //默认状态
  uint8_t led_state[4];                               //4路LED灯上电状态。
  uint8_t key_function[8];                            //4路按键功能配置。2个字节代表一个按键，共8个字节。
  uint8_t key_partition [4];                          //4路按键区域设置，数据为0时无分区。
  uint8_t Transboundary_zoning [4];                   //4路按键跨区域属性，最多跨8个区域，没有设置区域属性时不起作用。00位没有跨区属性。
  uint8_t panel_add;                                  //面板地址。
}rs458_InitTypedef;
/*广播常用命令列表*/
#define ORDINARY_BUTTON_CMD          0X01             //普通按键。
#define TOTAL_SW_CMD                 0x02             //总开关
#define GENERAL_OPEN_CMD             0x03             //总开
#define GENERAL_CLOSE_COM            0X04             //总关
#define REGIONAL_SW_CMD              0x05             //区域总开关。
#define MULTI_CONTROL_CMD            0X06             //多控开关。
#define DOUBLE_CONTROL_CMD           0X07             //双控开关。
#define DOUBLE_INTERLOCKING_CMD      0X08             //双控互锁。
#define MULTI_INTERLOCKING_CMD       0x09             //多控互锁。
#define POINT_PRESS_CMD              0X0A             //点动开关。
#define POINT_LOCK_CMD               0X0B             //点动自锁。
#define NIGHT_LAMP_CMD               0X0C             //夜灯。
#define SPECIAL_SERVICE_CMD          0X0D             //特殊服务
#define INTER_LOCK_CMD               0X0E             //互锁。
#define CLEAR_DISTURB_CMD            0X0F             //清理勿扰。
#define WINDOW_CURTAINS_CMD          0X11             //窗帘。
/***********上电亮灯**********/
#define PORCH_LAMP_CMMD              0X51             //廊灯，上电亮。同名双控。
#define ELECTRIC_LAMP_CMMD           0X52             //廊灯，上电亮。不双控。
/**********************************************函数声明***********************************************/
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


 