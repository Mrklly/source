#ifndef _BSP_E2P_H_
#define _BSP_E2P_H_


#include "bsp_uart.h"
#include "stm8s_uart1.h"
#include "bsp_flash.h"
#include "bsp_timer.h"


//typedef struct _eep_burnning_t_{
//	int8_t		SOH;			// Э��ͷ���̶�ΪA5
//	int8_t		def_sta;		// ����Ĭ��״̬
//	int8_t		res;			// ����
//	int8_t		chl;			// ·��
//	int8_t		ctrl_type[4];		// ��·�����豸����
//	int8_t		name[16];		// �豸����24
//	int8_t		len;			// �ܿ�����25 +len
//	int8_t		dat[64];		// �ܿ���Ϣ
//}eep_burnning;


void Write_RX_TO_EEPROM(void);
void Read_EEPROM_TO_BUFFF(void);
void var_value(void);


#endif
