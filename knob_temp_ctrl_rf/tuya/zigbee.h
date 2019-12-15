/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2018-0704, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: zigbee.h
**��        ��: zigbee�ļ�ͷ����
**ʹ �� ˵ �� : �û�������ĸ��ļ�ʵ������
**
**
**--------------��ǰ�汾�޶�---------------------------------------------------
** ��  ��: v1.0.0
** �ա���: 2018��7��4��
** �衡��: 1:Э�����
**
**-----------------------------------------------------------------------------
******************************************************************************/
#ifndef __ZIGBEE_H_
#define __ZIGBEE_H_


//=============================================================================
/*���峣��*/
//=============================================================================
#ifndef TRUE
#define         TRUE                1
#endif
//
#ifndef FALSE
#define         FALSE               0
#endif
//
#ifndef NULL
#define         NULL                ((void *) 0)
#endif

#ifndef ZIGBEE_SUCCESS
#define         ZIGBEE_SUCCESS             1
#endif

#ifndef ZIGBEE_ERROR
#define         ZIGBEE_ERROR               0
#endif

#ifndef INVALID
#define         INVALID             0xFF
#endif

#ifndef ENABLE
#define         ENABLE                1
#endif
//
#ifndef DISABLE
#define         DISABLE               0
#endif
//=============================================================================
//dp���ݵ�����
//=============================================================================
#define         DP_TYPE_RAW                     0x00				//RAW��
#define         DP_TYPE_BOOL                    0x01	            //������
#define         DP_TYPE_VALUE                   0x02	            //��ֵ��
#define         DP_TYPE_STRING                  0x03				//�ַ�����
#define         DP_TYPE_ENUM                    0x04				//ö����
#define         DP_TYPE_FAULT                   0x05				//������



//=============================================================================
//zigbee��λ״̬
//=============================================================================
#define         RESET_ZIGBEE_ERROR                0
#define         RESET_ZIGBEE_SUCCESS              1


#include "protocol.h"
#include "system.h"
#include "mcu_api.h"

#ifndef bool
#define bool unsigned char
#endif

//=============================================================================
//�·�����
//=============================================================================
typedef struct {
  unsigned char dp_id;                        //dp���
  unsigned char dp_type;                          //dp����
} DOWNLOAD_CMD_S;

#endif



