/****************************************Copyright (c)*************************
**                               版权所有 (C), 2018-0704, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------
**文   件   名: zigbee.h
**描        述: zigbee文件头定义
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
#ifndef __ZIGBEE_H_
#define __ZIGBEE_H_


//=============================================================================
/*定义常量*/
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
//dp数据点类型
//=============================================================================
#define         DP_TYPE_RAW                     0x00				//RAW型
#define         DP_TYPE_BOOL                    0x01	            //布尔型
#define         DP_TYPE_VALUE                   0x02	            //数值型
#define         DP_TYPE_STRING                  0x03				//字符串型
#define         DP_TYPE_ENUM                    0x04				//枚举型
#define         DP_TYPE_FAULT                   0x05				//故障型



//=============================================================================
//zigbee复位状态
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
//下发命令
//=============================================================================
typedef struct {
  unsigned char dp_id;                        //dp序号
  unsigned char dp_type;                          //dp类型
} DOWNLOAD_CMD_S;

#endif



