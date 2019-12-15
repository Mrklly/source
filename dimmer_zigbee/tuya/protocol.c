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
#include "global_variables.h"
#include "hc32f005.h"
#include "hc32_uart.h"


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
const DOWNLOAD_CMD_S download_cmd[] = {
    {DPID_LED_SWITCH_1, DP_TYPE_BOOL},
    {DPID_BRIGHT_VALUE_1, DP_TYPE_VALUE},
    {DPID_BRIGHTNESS_MIN_1, DP_TYPE_VALUE},
    {DPID_LED_TYPE_1, DP_TYPE_ENUM},
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
    M0P_UART1->ICR_f.TICLR = 0;

    M0P_UART1->SBUF = value;

    while (TRUE != M0P_UART1->ISR_f.TI);

    M0P_UART1->ICR_f.TICLR = 0;
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
    static uint8_t led_sta_last = 1, dimming_last = 0;

    if(led_sta_last != led_sta[1]) {
        mcu_dp_bool_update(DPID_LED_SWITCH_1, led_sta[1]);
        led_sta_last = led_sta[1];
    }

    if(dimming_last != Chopping1_channel_cont) {
        mcu_dp_value_update(DPID_BRIGHT_VALUE_1, ((25 - Chopping1_channel_cont) << 2) * 10);
        dimming_last = Chopping1_channel_cont;
    }

//  Chopping1_channel_cont = 26 - ((bright_value_1 / 10 + 3) >> 2);

//    mcu_dp_value_update(DPID_BRIGHTNESS_MIN_1, 10); //VALUE�������ϱ�;
//    mcu_dp_enum_update(DPID_LED_TYPE_1, 2); //ö���������ϱ�;

}


/******************************************************************************
                                WARNING!!!
                            2:���������ϱ�����
�Զ�������ģ�庯��,�������û�����ʵ�����ݴ���
******************************************************************************/


/*****************************************************************************
�������� : dp_download_led_switch_1_handle
�������� : ���DPID_LED_SWITCH_1�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:ZIGBEE_SUCCESS/ʧ�ܷ���:ZIGBEE_ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_led_switch_1_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:��/1:��
    unsigned char led_switch_1;

    led_switch_1 = mcu_get_dp_download_bool(value, length);

    led_sta[1] = led_switch_1;

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_bool_update(DPID_LED_SWITCH_1, led_switch_1);

    if(ret == ZIGBEE_SUCCESS) {
        return ZIGBEE_SUCCESS;
    } else {
        return ZIGBEE_ERROR;
    }
}
/*****************************************************************************
�������� : dp_download_bright_value_1_handle
�������� : ���DPID_BRIGHT_VALUE_1�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:ZIGBEE_SUCCESS/ʧ�ܷ���:ZIGBEE_ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_bright_value_1_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪVALUE
    unsigned char ret;
    unsigned long bright_value_1;

    bright_value_1 = mcu_get_dp_download_value(value, length);

//    Chopping1_channel_cont = 26 - ((bright_value_1 / 10 + 3) >> 2);

//    Chopping1_channel_cont = 26 - ((bright_value_1 / 10 + 4) >> 2);

    Chopping1_channel_cont = 24 - ((bright_value_1 / 10 - 1) >> 2);


    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_value_update(DPID_BRIGHT_VALUE_1, bright_value_1);

    if(ret == ZIGBEE_SUCCESS) {
        return ZIGBEE_SUCCESS;
    } else {
        return ZIGBEE_ERROR;
    }
}
/*****************************************************************************
�������� : dp_download_brightness_min_1_handle
�������� : ���DPID_BRIGHTNESS_MIN_1�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:ZIGBEE_SUCCESS/ʧ�ܷ���:ZIGBEE_ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_brightness_min_1_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪVALUE
    unsigned char ret;
    unsigned long brightness_min_1;

    brightness_min_1 = mcu_get_dp_download_value(value, length);
    /*
    //VALUE�������ݴ���

    */

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_value_update(DPID_BRIGHTNESS_MIN_1, brightness_min_1);

    if(ret == ZIGBEE_SUCCESS) {
        return ZIGBEE_SUCCESS;
    } else {
        return ZIGBEE_ERROR;
    }
}
/*****************************************************************************
�������� : dp_download_led_type_1_handle
�������� : ���DPID_LED_TYPE_1�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:ZIGBEE_SUCCESS/ʧ�ܷ���:ZIGBEE_ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_led_type_1_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪENUM
    unsigned char ret;
    unsigned char led_type_1;

    led_type_1 = mcu_get_dp_download_enum(value, length);

    switch(led_type_1) {
    case 0:

        break;

    case 1:

        break;

    case 2:

        break;

    default:

        break;
    }

    //������DP���ݺ�Ӧ�з���
    ret = mcu_dp_enum_update(DPID_LED_TYPE_1, led_type_1);

    if(ret == ZIGBEE_SUCCESS) {
        return ZIGBEE_SUCCESS;
    } else {
        return ZIGBEE_ERROR;
    }
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
    if(1 == local_or_utc) {
        //��ȷ���յ�zigbeeģ�鷵�صı���ʱ������

    } else {
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
void zigbee_test_result(bool result, unsigned char rssi)
{
    //#error "������ʵ��zigbee���ܲ��Գɹ�/ʧ�ܴ���,��ɺ���ɾ������"
    if(result == 0) {
        //����ʧ��
    }

    else if(rssi == 0x00) {
        //��ָ���ŵ���δ��⵽��zigbee dongle,����dongle�ŵ�
    } else {
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
unsigned char dp_download_handle(unsigned char dpid, const unsigned char value[], unsigned short length)
{
    /*********************************
    ��ǰ����������·�/���ϱ����ݵ���
    ���庯������Ҫʵ���·����ݴ���
    �������Ҫ��������������APP��,����APP����Ϊ�·�ʧ��
    ***********************************/
    unsigned char ret = 0;

    switch(dpid) {
    case DPID_LED_SWITCH_1:
        //���ش�����
        ret = dp_download_led_switch_1_handle(value, length);
        break;

    case DPID_BRIGHT_VALUE_1:
        //����ֵ������
        ret = dp_download_bright_value_1_handle(value, length);
        break;

    case DPID_BRIGHTNESS_MIN_1:
        //��С���ȴ�����
        ret = dp_download_brightness_min_1_handle(value, length);
        break;

    case DPID_LED_TYPE_1:
        //��Դ���ʹ�����
        ret = dp_download_led_type_1_handle(value, length);
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
