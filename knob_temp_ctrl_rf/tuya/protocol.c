/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2018-0704, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: protocol.c
**��        ��: �·�/�ϱ����ݴ�����
**ʹ �� ˵ �� :

                  *******�ǳ���Ҫ��һ��Ҫ��Ŷ������********

** 1���û��ڴ��ļ���ʵ�������·�/�ϱ�����
** 2��DP��ID/TYPE�����ݴ���������Ҫ�û�����ʵ�ʶ���ʵ��
** 3������ʼĳЩ�궨�����Ҫ�û�ʵ�ִ���ĺ����ڲ���#err��ʾ,��ɺ�������ɾ����#err
**
**--------------��ǰ�汾�޶�---------------------------------------------------
** ��  ��: v1.0.0
** �ա���: 2018��7��4��
** �衡��: 1:Э�����
**
**-----------------------------------------------------------------------------
******************************************************************************/

#include "zigbee.h"
#include "uart.h"

/******************************************************************************
                                ��ֲ��֪:
1:MCU������while��ֱ�ӵ���mcu_api.c�ڵ�zigbee_uart_service()����
2:����������ʼ����ɺ�,���鲻���йش����ж�,�������ж�,���ж�ʱ������,���жϻ����𴮿����ݰ���ʧ
3:�������ж�/��ʱ���ж��ڵ����ϱ�����
******************************************************************************/

         
/******************************************************************************
                              ��һ��:��ʼ��
1:����Ҫʹ�õ�zigbee����ļ����ļ���include "zigbee.h"
2:��MCU��ʼ���е���mcu_api.c�ļ��е�zigbee_protocol_init()����
3:��MCU���ڵ��ֽڷ��ͺ�������protocol.c�ļ���uart_transmit_output������,��ɾ��#error
4:��MCU���ڽ��պ����е���mcu_api.c�ļ��ڵ�uart_receive_input����,�������յ����ֽ���Ϊ��������
5:��Ƭ������whileѭ�������mcu_api.c�ļ��ڵ�zigbee_uart_service()����
******************************************************************************/

/******************************************************************************
                        1:dp���ݵ��������Ͷ��ձ�
          **��Ϊ�Զ����ɴ���,���ڿ���ƽ̨������޸�����������MCU_SDK**         
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_SWITCH, DP_TYPE_BOOL},
  {DPID_TEMP_SET, DP_TYPE_VALUE},
  {DPID_TEMP_CURRENT, DP_TYPE_VALUE},
  {DPID_MODE, DP_TYPE_ENUM},
  {DPID_INQUIRE, DP_TYPE_BOOL},
  {DPID_WINDSPEED, DP_TYPE_ENUM},
  {DPID_WIND_DIRECTION, DP_TYPE_ENUM},
};


/******************************************************************************
                           2:���ڵ��ֽڷ��ͺ���
�뽫MCU���ڷ��ͺ�������ú�����,�������յ���������Ϊ�������봮�ڷ��ͺ���
******************************************************************************/

/*****************************************************************************
�������� : uart_transmit_output
�������� : �����ݴ���
������� : value:�����յ��ֽ�����
���ز��� : ��
ʹ��˵�� : �뽫MCU���ڷ��ͺ�������ú�����,�������յ���������Ϊ�������봮�ڷ��ͺ���
*****************************************************************************/
extern unsigned char emberSerialWriteByte(unsigned char port, unsigned char dataByte);
void uart_transmit_output(unsigned char value)
{
	LPUart_SendData(LPUART1, value);
//#error "�뽫MCU���ڷ��ͺ�������ú���,��ɾ������"
/*
  //ʾ��:
  extern void Uart_PutChar(unsigned char value);
  Uart_PutChar(value);	                                //���ڷ��ͺ���
*/
}
/******************************************************************************
                           �ڶ���:ʵ�־����û�����
1:APP�·����ݴ���
2:�����ϱ�����
******************************************************************************/

/******************************************************************************
                            1:���������ϱ�����
��ǰ��������ȫ�������ϱ�(�������·�/���ϱ���ֻ�ϱ�)
  ��Ҫ�û�����ʵ�����ʵ��:
  1:��Ҫʵ�ֿ��·�/���ϱ����ݵ��ϱ�
  2:��Ҫʵ��ֻ�ϱ����ݵ��ϱ�
�˺���ΪMCU�ڲ��������
�û�Ҳ�ɵ��ô˺���ʵ��ȫ�������ϱ�
******************************************************************************/

//�Զ������������ϱ�����

/*****************************************************************************
�������� : all_data_update
�������� : ϵͳ����dp����Ϣ�ϴ�,ʵ��APP��mcu����ͬ��
������� : ��
���ز��� : ��
ʹ��˵�� : �˺���SDK�ڲ������;
           MCU����ʵ�ָú����������ϱ�����;����ֻ�ϱ��Ϳ��ϱ����·�������
*****************************************************************************/
void all_data_update(void)
{
  //#error "���ڴ˴�����·����ϱ����ݼ�ֻ�ϱ�����ʾ��,������ɺ�ɾ������"
  /* 
  //�˴���Ϊƽ̨�Զ����ɣ��밴��ʵ�������޸�ÿ�����·����ϱ�������ֻ�ϱ�����
 */
  mcu_dp_bool_update(DPID_SWITCH, g_dev.sta.power); //BOOL�������ϱ�;
  mcu_dp_value_update(DPID_TEMP_SET, g_dev.set_temp); //VALUE�������ϱ�;
  mcu_dp_value_update(DPID_TEMP_CURRENT, (int8_t)(g_dev.room_temp+0.5)); //VALUE�������ϱ�;
  mcu_dp_enum_update(DPID_MODE, g_dev.sta.mode); //ö���������ϱ�;
  mcu_dp_enum_update(DPID_WINDSPEED, g_dev.sta.wind);
//  mcu_dp_enum_update(DPID_WIND_DIRECTION,��ǰ����); //ö���������ϱ�;
//  mcu_dp_fault_update(DPID_FAULT,��ǰ���ϸ澯);

}


/******************************************************************************
                                WARNING!!!    
                            2:���������ϱ�����
�Զ�������ģ�庯��,�������û�����ʵ�����ݴ���
******************************************************************************/


/*****************************************************************************
�������� : dp_download_switch_handle
�������� : ���DPID_SWITCH�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:ZIGBEE_SUCCESS/ʧ�ܷ���:ZIGBEE_ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_switch_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪBOOL
  unsigned char ret;
  //0:��/1:��
  unsigned char switch1;
  
  switch1 = mcu_get_dp_download_bool(value,length);
  if(switch1 == 0) {
    g_dev.sta.power = 0;

    g_lcd_on = 0;
    g_lcd_delay = 0;
    g_led_on = 0;
    g_lcd_delay = 0;
  } else {
    g_dev.sta.power = 1;

    g_lcd_on = 1;
    g_lcd_delay = g_dev.led_timeout * 10;

    g_led_on = 1;
    g_led_delay = g_dev.led_timeout * 10;
  }
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_bool_update(DPID_SWITCH,switch1);
  if(ret == ZIGBEE_SUCCESS)
    return ZIGBEE_SUCCESS;
  else
    return ZIGBEE_ERROR;
}
/*****************************************************************************
�������� : dp_download_temp_set_handle
�������� : ���DPID_TEMP_SET�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:ZIGBEE_SUCCESS/ʧ�ܷ���:ZIGBEE_ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_temp_set_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪVALUE
  unsigned char ret;
  unsigned long temp_set;
  
  temp_set = mcu_get_dp_download_value(value,length);

  g_dev.set_temp = temp_set;
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_value_update(DPID_TEMP_SET,temp_set);
  if(ret == ZIGBEE_SUCCESS)
    return ZIGBEE_SUCCESS;
  else
    return ZIGBEE_ERROR;
}
/*****************************************************************************
�������� : dp_download_mode_handle
�������� : ���DPID_MODE�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:ZIGBEE_SUCCESS/ʧ�ܷ���:ZIGBEE_ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_mode_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪENUM
  unsigned char ret;
  unsigned char mode;
  
  mode = mcu_get_dp_download_enum(value,length);
  switch(mode)
  {
    case 0:
      g_dev.sta.mode = TEMP_MODE_WARM;
      break;

    case 1:
      g_dev.sta.mode = TEMP_MODE_FAN;
      break;

    case 2:
      g_dev.sta.mode = TEMP_MODE_COOL;
      break;

    case 3:
      g_dev.sta.mode = TEMP_MODE_AUTO;
      break;

    default:
      break;
  }
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_enum_update(DPID_MODE,mode);
  if(ret == ZIGBEE_SUCCESS)
    return ZIGBEE_SUCCESS;
  else
    return ZIGBEE_ERROR;
}

/*****************************************************************************
�������� : dp_download_inquire_handle
�������� : ���DPID_INQUIRE�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ֻ�·�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_inquire_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪBOOL
  unsigned char ret;
  //0:��/1:��
  unsigned char inquire;
  
  inquire = mcu_get_dp_download_bool(value,length);
  if(inquire == 0)
  {
    //���ع�
  }
  else
  {
    //���ؿ�
  }
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_bool_update(DPID_INQUIRE,inquire);
  if(ret == ZIGBEE_SUCCESS)
    return ZIGBEE_SUCCESS;
  else
    return ZIGBEE_ERROR;
}

/*****************************************************************************
�������� : dp_download_windspeed_handle
�������� : ���DPID_WINDSPEED�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_windspeed_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪENUM
  unsigned char ret;
  unsigned char windspeed;

  windspeed = mcu_get_dp_download_enum(value,length);

  g_dev.sta.wind_auto = 0;

  switch(windspeed)
  {
    case 0:
      g_dev.sta.wind = FAN_MIN;
      break;

    case 1:
      g_dev.sta.wind = FAN_MID;
      break;

    case 2:
      g_dev.sta.wind = FAN_MAX;
      break;

    case 3:
      g_dev.sta.wind = FAN_CLZ;
      g_dev.sta.wind_auto = 1;
      break;

    case 4:
      break;

    case 5:
      break;

    default:
      break;
  }

  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_enum_update(DPID_WINDSPEED,windspeed);
  if(ret == ZIGBEE_SUCCESS)
    return ZIGBEE_SUCCESS;
  else
    return ZIGBEE_ERROR;
}

/*****************************************************************************
�������� : dp_download_wind_direction_handle
�������� : ���DPID_WIND_DIRECTION�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_wind_direction_handle(const unsigned char value[], unsigned short length)
{
  //ʾ��:��ǰDP����ΪENUM
  unsigned char ret;
  unsigned char wind_direction;
  
  wind_direction = mcu_get_dp_download_enum(value,length);
  switch(wind_direction)
  {
    case 0:
      
      break;
      
    case 1:
      
      break;
      
    case 2:
      
      break;
      
    case 3:
      
      break;
      
    default:
      
      break;
  }
  
  //������DP���ݺ�Ӧ�з���
  ret = mcu_dp_enum_update(DPID_WIND_DIRECTION,wind_direction);
  if(ret == ZIGBEE_SUCCESS)
    return ZIGBEE_SUCCESS;
  else
    return ZIGBEE_ERROR;
}

/******************************************************************************
                                WARNING!!!                     
�˴���ΪSDK�ڲ�����,�밴��ʵ��dp����ʵ�ֺ����ڲ�����
******************************************************************************/

/*****************************************************************************
�������� : mcu_write_rtctime
�������� : MCUУ�Ա���RTCʱ��
������� : local_or_utc: true��local time�� false��UTCtime
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void mcu_write_rtctime(bool local_or_utc, time_t* time)
{
  //#error "���������RTCʱ��д�����,��ɾ������"
  if(1 == local_or_utc)
  {
    //��ȷ���յ�zigbeeģ�鷵�صı���ʱ������ 
	 
  }
  else
  {
  	//��ȷ���յ�zigbeeģ�鷵�ص�UTCʱ������ 
  }
}

#ifdef ZIGBEE_TEST_ENABLE
/*****************************************************************************
�������� : zigbee_test_result
�������� : zigbee���ܲ��Է���
������� : result:zigbee���ܲ��Խ��;0:ʧ��/1:�ɹ�
           rssi:���Գɹ���ʾzigbee�ź�ǿ��/����ʧ�ܱ�ʾ��������
���ز��� : ��
ʹ��˵�� : MCU��Ҫ����ʵ�ָù���
*****************************************************************************/
void zigbee_test_result(bool result,unsigned char rssi)
{
    //#error "������ʵ��zigbee���ܲ��Գɹ�/ʧ�ܴ���,��ɺ���ɾ������"
    if(result == 0)
    {
        //����ʧ��
    }

    else if(rssi == 0x00)
    {
        //��ָ���ŵ���δ��⵽��zigbee dongle,����dongle�ŵ�
    }
    else
    {
        //���Գɹ�
        //rssiΪ�ź�ǿ��(0-100, 0�ź���100�ź���ǿ)
    }
  
}
#endif

/******************************************************************************
                                WARNING!!!                     
���º����û������޸�!!
******************************************************************************/

/*****************************************************************************
�������� : dp_download_handle
�������� : dp�·�������
������� : dpid:DP���
           value:dp���ݻ�������ַ
           length:dp���ݳ���
���ز��� : �ɹ�����:ZIGBEE_SUCCESS/ʧ�ܷ���:ERRO
ʹ��˵�� : �ú����û������޸�
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  /*********************************
  ��ǰ����������·�/���ϱ����ݵ���
  ���庯������Ҫʵ���·����ݴ���
  �������Ҫ��������������APP��,����APP����Ϊ�·�ʧ��
  ***********************************/
  unsigned char ret = 0;
  switch(dpid)
  {
    case DPID_SWITCH:
      //���ش�����
      ret = dp_download_switch_handle(value,length);
      break;
    case DPID_TEMP_SET:
      //�����¶ȴ�����
      ret = dp_download_temp_set_handle(value,length);
      break;
    case DPID_MODE:
      //����ģʽ������
      ret = dp_download_mode_handle(value,length);
      break;
    case DPID_INQUIRE:
      //��ѯ�¿���״̬������
      ret = dp_download_inquire_handle(value,length);
      break;
    case DPID_WINDSPEED:
      //���ٴ�����
      ret = dp_download_windspeed_handle(value,length);
      break;
    case DPID_WIND_DIRECTION:
      //��������
      ret = dp_download_wind_direction_handle(value,length);
      break;

  default:
    break;
  }
  return ret;
}
/*****************************************************************************
�������� : get_download_cmd_total
�������� : ��ȡ����dp�����ܺ�
������� : ��
���ز��� : �·������ܺ�
ʹ��˵�� : �ú����û������޸�
*****************************************************************************/
unsigned char get_download_cmd_total(void)
{
  return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}

void zigbee_auto_send(void)
{
    static dev_sta_t dev_sta_last = 0xff;
    static int8_t dev_temp_set_last = 0;
    static int8_t dev_temp_room_last = 0;
    uint8_t temp_value;

    if(g_zig_net_sta != ZIG_NET_STA_OK) {
        return ;
    }

    if(g_set_type != SET_NONE) {
        return ;
    }

    if(dev_temp_set_last != g_dev.set_temp) {
        dev_temp_set_last = g_dev.set_temp;

        mcu_dp_value_update(DPID_TEMP_SET, g_dev.set_temp);
    }

    if(dev_sta_last.status != g_dev.sta.status) {

        if(dev_sta_last.power != g_dev.sta.power) {
            mcu_dp_bool_update(DPID_SWITCH, g_dev.sta.power);
        }

        if(dev_sta_last.mode != g_dev.sta.mode) {
            switch(g_dev.sta.mode) {
            case TEMP_MODE_WARM:
                temp_value = 0;
                break;

            case TEMP_MODE_FAN:
                temp_value = 1;
                break;

            case TEMP_MODE_COOL:
                temp_value = 2;
                break;

            case TEMP_MODE_AUTO:
                temp_value = 3;
                break;

            default:
                break;
            }

            mcu_dp_enum_update(DPID_MODE, temp_value);
        }

        if(dev_sta_last.wind != g_dev.sta.wind) {
            switch(g_dev.sta.wind) {
            case FAN_CLZ:
                temp_value = dev_sta_last.wind;
                break;

            case FAN_MIN:
                temp_value = 0;
                break;

            case FAN_MID:
                temp_value = 1;
                break;

            case FAN_MAX:
                temp_value = 2;
                break;

            default:
                break;
            }

            mcu_dp_enum_update(DPID_WINDSPEED, temp_value);
        }

        dev_sta_last.status = g_dev.sta.status;
    }

    if(dev_temp_room_last != (int8_t)(g_dev.room_temp + 0.5)) {
        dev_temp_room_last = (int8_t)(g_dev.room_temp + 0.5);
        mcu_dp_value_update(DPID_TEMP_CURRENT, (int8_t)(g_dev.room_temp + 0.5));
    }
}

