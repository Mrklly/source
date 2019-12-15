/****************************************Copyright (c)*************************
**                               版权所有 (C), 2018-0704, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------
**文   件   名: system.c
**描        述: zigbee数据处理函数
**使 用 说 明 : 用户无需关心该文件实现内容
**
**
**--------------当前版本修订---------------------------------------------------
** 版  本: v1.0.0
** 日　期: 2018年7月4日
** 描　述: 1:协议初版
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
//帧的字节顺序
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
//数据帧类型
//=============================================================================
#define         PRODUCT_INFO_CMD                0x01                            //产品信息
#define         ZIGBEE_STATE_CMD                0x02                            //zigbee工作状态    
#define         ZIGBEE_CFG_CMD                  0x03                            //配置zigbee模块
#define         ZIGBEE_DATA_REQ_CMD             0x04                            //zigbee数据下发命令
#define         DATA_RSP_SYNC_CMD               0x05                            //MCU状态主动上报
#define         DATA_RSP_ASYNC_CMD              0x06                            //MCU状态被动上报
#define         FUNC_TEST_CMD                   0x08                            //zigbee模块功能测试
#ifdef SERIAL_PROTOCOL_SCENE_ENABLE
#define         SCENE_SWITCH_NUM_GET_CMD        0x09                            //获取场景开关个数（仅用于场景开关）
#define         SCENE_SWITCH_ID_REPORT_CMD      0x0A                            //获取场景上报ID（仅用于场景开关）
#endif
#define         MCU_OTA_VER_REQ_CMD             0x0B                            //OTA 版本请求
#define         MCU_OTA_NOTIFY_CMD              0x0C                            //OTA 升级通知
#define         MCU_OTA_DATA_REQ_CMD            0x0D                            //OTA 固件内容请求
#define         MCU_OTA_END_CMD                 0x0E                            //OTA 结果上报
#define         TIME_GET_CMD                    0x24                            //zigbee模块时间同步

//=============================================================================
#define SERIAL_PROTOCOL_VER          0x02                                            //协议版本号
#define PROTOCOL_HEAD                0x09                                            //固定协议头长度
#define FIRST_FRAME_HEAD             0x55
#define SECOND_FRAME_HEAD            0xaa
#define SERIAL_PROTOCOL_HDR          (FIRST_FRAME_HEAD<<8 | SECOND_FRAME_HEAD)   ///<协议枕头数据


//=============================================================================
extern unsigned char zigbee_queue_buf[PROTOCOL_HEAD + ZIGBEE_UART_QUEUE_LMT];        //串口队列缓存
extern unsigned char zigbee_uart_rx_buf[PROTOCOL_HEAD + ZIGBEE_UART_RECV_BUF_LMT];   //串口接收缓存
extern unsigned char zigbee_uart_tx_buf[PROTOCOL_HEAD + ZIGBEE_UART_SEND_BUF_LMT];   //串口发送缓存


extern unsigned char *queue_in;
extern unsigned char *queue_out;
extern volatile uint8_t queue_total_data;

/*****************************************************************************
函数名称 : set_zigbee_uart_byte
功能描述 : 写zigbee_uart字节
输入参数 : dest:缓存区其实地址;
           byte:写入字节值
返回参数 : 写入完成后的总长度
*****************************************************************************/
unsigned short set_zigbee_uart_byte(unsigned short dest, unsigned char byte);

/*****************************************************************************
函数名称 : set_zigbee_uart_buffer
功能描述 : 写zigbee_uart_buffer
输入参数 : dest:目标地址
           src:源地址
           len:数据长度
返回参数 : 无
*****************************************************************************/
unsigned short set_zigbee_uart_buffer(unsigned short dest, unsigned char *src, unsigned short len);

/*****************************************************************************
函数名称 : zigbee_uart_write_frame
功能描述 : 向zigbee串口发送一帧数据
输入参数 : fr_type:帧类型
           len:数据长度
返回参数 : 无
*****************************************************************************/
void zigbee_uart_write_frame(unsigned char fr_type, unsigned short len);

/*****************************************************************************
函数名称 : get_check_sum
功能描述 : 计算校验和
输入参数 : pack:数据源指针
           pack_len:计算校验和长度
返回参数 : 校验和
*****************************************************************************/
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len);

/*****************************************************************************
函数名称 : get_queue_total_data
功能描述 : 读取队列内字符
输入参数 : NULL
返回参数 : char
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
