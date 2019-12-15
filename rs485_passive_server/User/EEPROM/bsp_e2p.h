#ifndef _BSP_E2P_H
#define _BSP_E2P_H
#include "bsp_uart.h"
#include "stm8s_uart1.h"
#include "bsp_flash.h"
#include "bsp_timer.h"
/*****************************************************************************************************/
#define     COM_EEPROM_ADDR     0x40A5          //内部EEPROM地址
typedef struct _eep_burnning_t_{
	int8_t		SOH;			// 协议头，固定为A5
	int8_t		def_sta;		// 启动默认状态
	int8_t		res;			// 保留
	int8_t		chl;			// 路数
	int8_t		ctrl_type[4];		// 四路控制设备类型
	int8_t		name[16];		// 设备名称24
	int8_t		len;			// 受控数量25 +len
	int8_t		dat[64];		// 受控信息
}eep_burnning;

/**********************************************函数声明***********************************************/
void Write_RX_TO_EEPROM(void);
void Read_EEPROM_TO_BUFFF(void);
void var_value(void);
#endif