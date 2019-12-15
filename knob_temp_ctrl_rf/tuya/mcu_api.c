/****************************************Copyright (c)*************************
**                               ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                                 http://www.tuya.com
**
**--------------�ļ���Ϣ-------------------------------------------------------*/

#include "zigbee.h"

/*****************************************************************************
�������� : hex_to_bcd
�������� : hexתbcd
������� : Value_H:���ֽ�/Value_L:���ֽ�
���ز��� : bcd_value:ת����ɺ�����
*****************************************************************************/
unsigned char hex_to_bcd(unsigned char Value_H,unsigned char Value_L)
{
  unsigned char bcd_value;
  
  if((Value_H >= '0') && (Value_H <= '9'))
    Value_H -= '0';
  else if((Value_H >= 'A') && (Value_H <= 'F'))
    Value_H = Value_H - 'A' + 10;
  else if((Value_H >= 'a') && (Value_H <= 'f'))
    Value_H = Value_H - 'a' + 10;
   
  bcd_value = Value_H & 0x0f;
  
  bcd_value <<= 4;
  if((Value_L >= '0') && (Value_L <= '9'))
    Value_L -= '0';
  else if((Value_L >= 'A') && (Value_L <= 'F'))
    Value_L = Value_L - 'a' + 10;
  else if((Value_L >= 'a') && (Value_L <= 'f'))
    Value_L = Value_L - 'a' + 10;
  
  bcd_value |= Value_L & 0x0f;

  return bcd_value;
}
/*****************************************************************************
�������� : my_strlen
�������� : ���ַ�������
������� : src:Դ��ַ
���ز��� : len:���ݳ���
*****************************************************************************/
unsigned long my_strlen(unsigned char *str)  
{
  unsigned long len = 0;
  if(str == NULL)
  { 
    return 0;
  }
  
  for(len = 0; *str ++ != '\0'; )
  {
    len ++;
  }
  
  return len;
}
/*****************************************************************************
�������� : my_memset
�������� : ��src��ָ�ڴ������ǰcount���ֽ����ó��ַ�c
������� : src:Դ��ַ
           ch:�����ַ�
           count:�������ݳ���
���ز��� : src:���ݴ�������Դ��ַ
*****************************************************************************/
void *my_memset(void *src,unsigned char ch,unsigned short count)
{
  unsigned char *tmp = (unsigned char *)src;
  
  if(src == NULL)
  {
    return NULL;
  }
  
  while(count --)
  {
    *tmp ++ = ch;
  }
  
  return src;
}
/*****************************************************************************
�������� : mymemcpy
�������� : �ڴ濽��
������� : dest:Ŀ���ַ
           src:Դ��ַ
           count:���ݿ�������
���ز��� : src:���ݴ�������Դ��ַ
*****************************************************************************/
void *my_memcpy(void *dest, const void *src, unsigned short count)  
{  
  unsigned char *pdest = (unsigned char *)dest;  
  const unsigned char *psrc  = (const unsigned char *)src;  
  unsigned short i;
  
  if(dest == NULL || src == NULL)
  { 
    return NULL;
  }
  
  if((pdest <= psrc) || (pdest > psrc + count))
  {  
    for(i = 0; i < count; i ++)
    {  
      pdest[i] = psrc[i];  
    }  
  }
  else
  {
    for(i = count; i > 0; i --)
    {  
      pdest[i - 1] = psrc[i - 1];  
    }  
  }  
  
  return dest;  
}
/*****************************************************************************
�������� : my_strcpy
�������� : �ڴ濽��
������� : s1:Ŀ���ַ
           s2:Դ��ַ
���ز��� : ���ݴ�������Դ��ַ
*****************************************************************************/
char *my_strcpy(char *dest, const char *src)  
{
  char *p = dest;
  while(*src!='\0')
  {
    *dest++ = *src++;
  }
  *dest = '\0';
  return p;
}
/*****************************************************************************
�������� : my_strcmp
�������� : �ڴ濽��
������� : s1:�ַ���1
           s2:�ַ���2
���ز��� : ��С�Ƚ�ֵ��0:s1=s2; -1:s1<s2; 1:s1>s2
*****************************************************************************/
int my_strcmp(char *s1 , char *s2)
{
  while( *s1 && *s2 && *s1 == *s2 )
  {
    s1++;
    s2++;
  }
  return *s1 - *s2;
}
/*****************************************************************************
�������� : int_to_byte
�������� : ��int���Ͳ���ĸ��ֽ�
������� : number:4�ֽ�ԭ����;value:������ɺ�4�ֽ�����
���ز��� :��
****************************************************************************/
void int_to_byte(unsigned long number,unsigned char value[4])
{
  value[0] = number >> 24;
  value[1] = number >> 16;
  value[2] = number >> 8;
  value[3] = number & 0xff;
}
/*****************************************************************************
�������� : byte_to_int
�������� : ��4�ֽںϲ�Ϊ1��32bit����
������� : value:4�ֽ�����
���ز��� : number:�ϲ���ɺ��32bit����
****************************************************************************/
unsigned long byte_to_int(const unsigned char value[4])
{
  unsigned long nubmer = 0;

  nubmer = (unsigned long)value[0];
  nubmer <<= 8;
  nubmer |= (unsigned long)value[1];
  nubmer <<= 8;
  nubmer |= (unsigned long)value[2];
  nubmer <<= 8;
  nubmer |= (unsigned long)value[3];
  
  return nubmer;
}

/*****************************************************************************
�������� : mcu_reset_zigbee
�������� : MCU����reset zigbee,ͬ���reset���ܣ�ֻ�����ջ���ݣ� ������Ϣ����
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void mcu_reset_zigbee(void)
{
  unsigned short length = 0;

  length = set_zigbee_uart_byte(length,0);
  zigbee_uart_write_frame(ZIGBEE_CFG_CMD, length);
}

/*****************************************************************************
�������� : mcu_network_start
�������� : MCU��������zigbee����ģʽ�����¿�ʼ��������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void mcu_network_start(void)
{
  unsigned short length = 0;

  length = set_zigbee_uart_byte(length,1);
  zigbee_uart_write_frame(ZIGBEE_CFG_CMD, length);
}

/*****************************************************************************
�������� : mcu_dp_raw_update
�������� : raw��dp�����ϴ�
������� : dpid:id��
           value:��ǰdpֵָ��
           len:���ݳ���
���ز��� : ��
*****************************************************************************/
unsigned char mcu_dp_raw_update(unsigned char dpid,const unsigned char value[],unsigned short len)
{
  unsigned short length = 0;
  
  length = set_zigbee_uart_byte(length,dpid);
  length = set_zigbee_uart_byte(length,DP_TYPE_RAW);
  //
  length = set_zigbee_uart_byte(length,len / 0x100);
  length = set_zigbee_uart_byte(length,len % 0x100);
  //
  length = set_zigbee_uart_buffer(length,(unsigned char *)value,len);
  
  zigbee_uart_write_frame(DATA_RSP_ASYNC_CMD,length);
  
  return ZIGBEE_SUCCESS;
}
/*****************************************************************************
�������� : mcu_dp_bool_update
�������� : bool��dp�����ϴ�
������� : dpid:id��
           value:��ǰdpֵ
���ز��� : ��
*****************************************************************************/
unsigned char mcu_dp_bool_update(unsigned char dpid,unsigned char value)
{
  unsigned short length = 0;
  
  length = set_zigbee_uart_byte(length,dpid);
  length = set_zigbee_uart_byte(length,DP_TYPE_BOOL);
  //
  length = set_zigbee_uart_byte(length,0);
  length = set_zigbee_uart_byte(length,1);
  //
  if(value == FALSE)
  {
    length = set_zigbee_uart_byte(length,FALSE);
  }
  else
  {
    length = set_zigbee_uart_byte(length,1);
  }
  
  zigbee_uart_write_frame(DATA_RSP_ASYNC_CMD,length);
  
  return ZIGBEE_SUCCESS;
}
/*****************************************************************************
�������� : mcu_dp_value_update
�������� : value��dp�����ϴ�
������� : dpid:id��
           value:��ǰdpֵ
���ز��� : ��
*****************************************************************************/
unsigned char mcu_dp_value_update(unsigned char dpid,unsigned long value)
{
  unsigned short length = 0;
  
  length = set_zigbee_uart_byte(length,dpid);
  length = set_zigbee_uart_byte(length,DP_TYPE_VALUE);
  //
  length = set_zigbee_uart_byte(length,0);
  length = set_zigbee_uart_byte(length,4);
  //
  length = set_zigbee_uart_byte(length,value >> 24);
  length = set_zigbee_uart_byte(length,value >> 16);
  length = set_zigbee_uart_byte(length,value >> 8);
  length = set_zigbee_uart_byte(length,value & 0xff);
  
  zigbee_uart_write_frame(DATA_RSP_ASYNC_CMD,length);
  
  return ZIGBEE_SUCCESS;
}
/*****************************************************************************
�������� : mcu_dp_string_update
�������� : rstring��dp�����ϴ�
������� : dpid:id��
           value:��ǰdpֵָ��
           len:���ݳ���
���ز��� : ��
*****************************************************************************/
unsigned char mcu_dp_string_update(unsigned char dpid,const unsigned char value[],unsigned short len)
{
  unsigned short length = 0;
  
  length = set_zigbee_uart_byte(length,dpid);
  length = set_zigbee_uart_byte(length,DP_TYPE_STRING);
  //
  length = set_zigbee_uart_byte(length,len / 0x100);
  length = set_zigbee_uart_byte(length,len % 0x100);
  //
  length = set_zigbee_uart_buffer(length,(unsigned char *)value,len);
  
  zigbee_uart_write_frame(DATA_RSP_ASYNC_CMD,length);
  
  return ZIGBEE_SUCCESS;
}
/*****************************************************************************
�������� : mcu_dp_enum_update
�������� : enum��dp�����ϴ�
������� : dpid:id��
           value:��ǰdpֵ
���ز��� : ��
*****************************************************************************/
unsigned char mcu_dp_enum_update(unsigned char dpid,unsigned char value)
{
  unsigned short length = 0;
  
  length = set_zigbee_uart_byte(length,dpid);
  length = set_zigbee_uart_byte(length,DP_TYPE_ENUM);
  //
  length = set_zigbee_uart_byte(length,0);
  length = set_zigbee_uart_byte(length,1);
  //
  length = set_zigbee_uart_byte(length,value);
  
  zigbee_uart_write_frame(DATA_RSP_ASYNC_CMD,length);
  
  return ZIGBEE_SUCCESS;
}
/*****************************************************************************
�������� : mcu_dp_fault_update
�������� : fault��dp�����ϴ�
������� : dpid:id��
           value:��ǰdpֵ
���ز��� : ��
*****************************************************************************/
unsigned char mcu_dp_fault_update(unsigned char dpid,unsigned long value)
{
  unsigned short length = 0;

  length = set_zigbee_uart_byte(length,dpid);
  length = set_zigbee_uart_byte(length,DP_TYPE_FAULT);
  //
  length = set_zigbee_uart_byte(length,0);
  
  if((value | 0xff) == 0xff)
  {
    length = set_zigbee_uart_byte(length,1);
    length = set_zigbee_uart_byte(length,value);
  }
  else if((value | 0xffff) == 0xffff)
  {
    length = set_zigbee_uart_byte(length,2);
    length = set_zigbee_uart_byte(length,value >> 8);
    length = set_zigbee_uart_byte(length,value & 0xff);
  }
  else
  {
    length = set_zigbee_uart_byte(length,4);
    length = set_zigbee_uart_byte(length,value >> 24);
    length = set_zigbee_uart_byte(length,value >> 16);
    length = set_zigbee_uart_byte(length,value >> 8);
    length = set_zigbee_uart_byte(length,value & 0xff);
  }    
  
  zigbee_uart_write_frame(DATA_RSP_ASYNC_CMD,length);

  return ZIGBEE_SUCCESS;
}
/*****************************************************************************
�������� : mcu_get_dp_download_bool
�������� : mcu��ȡbool���·�dpֵ
������� : value:dp���ݻ�������ַ
           length:dp���ݳ���
���ز��� : bool:��ǰdpֵ
*****************************************************************************/
unsigned char mcu_get_dp_download_bool(const unsigned char value[],unsigned short len)
{
  return(value[0]);
}
/*****************************************************************************
�������� : mcu_get_dp_download_enum
�������� : mcu��ȡenum���·�dpֵ
������� : value:dp���ݻ�������ַ
           length:dp���ݳ���
���ز��� : enum:��ǰdpֵ
*****************************************************************************/
unsigned char mcu_get_dp_download_enum(const unsigned char value[],unsigned short len)
{
  return(value[0]);
}
/*****************************************************************************
�������� : mcu_get_dp_download_value
�������� : mcu��ȡvalue���·�dpֵ
������� : value:dp���ݻ�������ַ
           length:dp���ݳ���
���ز��� : value:��ǰdpֵ
*****************************************************************************/
unsigned long mcu_get_dp_download_value(const unsigned char value[],unsigned short len)
{
  return(byte_to_int(value));
}

/*****************************************************************************
�������� :  zigbee_protocol_init
�������� : Э�鴮�ڳ�ʼ������
������� : ��
���ز��� : ��
ʹ��˵�� : ������MCU��ʼ�������е��øú���
*****************************************************************************/
void zigbee_protocol_init(void)
{
//  #error " ����main���������zigbee_protocol_init()���zigbeeЭ���ʼ��,��ɾ������"
  queue_in = (unsigned char *)zigbee_queue_buf;
  queue_out = (unsigned char *)zigbee_queue_buf;
  queue_total_data = 0;
}

/*****************************************************************************
�������� : uart_receive_input
�������� : �����ݴ���
������� : value:�����յ��ֽ�����
���ز��� : ��
ʹ��˵�� : ��MCU���ڽ��պ����е��øú���,�������յ���������Ϊ��������
*****************************************************************************/
void uart_receive_input(unsigned char value)
{
//  #error "���ڴ��ڽ����ж��е���uart_receive_input(value),����������MCU_SDK����,�û����������д���,��ɺ�ɾ������" 

  if(queue_total_data < sizeof(zigbee_queue_buf))
  {
    //���в���
    if(queue_in >= (unsigned char *)(zigbee_queue_buf + sizeof(zigbee_queue_buf)))
    {
      queue_in = (unsigned char *)(zigbee_queue_buf);
    }
    
    *queue_in ++ = value;
    queue_total_data++;
  }
  else
  {
    //��������
  }
}

/*****************************************************************************
��������  : zigbee_uart_service
��������  : zigbee���ڴ������
������� : ��
���ز��� : ��
ʹ��˵�� : ��MCU������whileѭ���е��øú���
*****************************************************************************/
extern void serialDataReceiveHandler(unsigned char* payload, unsigned char payload_len);

void zigbee_uart_service(void)
{
//    #error "��ֱ����main������while(1){}zigbee_uart_service(),���øú�����Ҫ���κ������ж�,��ɺ�ɾ������" 
    static volatile unsigned short rx_in = 0;
    volatile unsigned short offset = 0;
    volatile unsigned short rx_value_len = 0;     //����֡����
    volatile unsigned short protocol_version = 0, protocol_head_len = PROTOCOL_HEAD;

    while((rx_in < sizeof(zigbee_uart_rx_buf)) && (get_queue_total_data() > 0))
    {
        zigbee_uart_rx_buf[rx_in++] = Queue_Read_Byte();
    }

    if(rx_in < PROTOCOL_HEAD)
    {
        return;
    }

    while((rx_in - offset) >= PROTOCOL_HEAD)
    {
        if(zigbee_uart_rx_buf[offset + HEAD_FIRST] != FIRST_FRAME_HEAD)
        {
            offset++;
            continue;
        }

        if(zigbee_uart_rx_buf[offset + HEAD_SECOND] != SECOND_FRAME_HEAD)
        {
            offset++;
            continue;
        }

        protocol_version = zigbee_uart_rx_buf[offset + PROTOCOL_VERSION];

        if(protocol_version != SERIAL_PROTOCOL_VER) /*version 2 is more than two byte  sequence number than version 1*/
        {
            offset += 2; 
            continue;
        }

        rx_value_len = zigbee_uart_rx_buf[offset + LENGTH_HIGH ] * 0x100;
        rx_value_len += (zigbee_uart_rx_buf[offset + LENGTH_LOW] + PROTOCOL_HEAD);

        if(rx_value_len > sizeof(zigbee_uart_rx_buf) + PROTOCOL_HEAD)
        {
            offset += 3;
            continue;
        }

        if((rx_in - offset) < rx_value_len)
        {
            break;
        }

        //���ݽ������, ��������У��
        if( get_check_sum((unsigned char *)zigbee_uart_rx_buf + offset, rx_value_len - 1) != zigbee_uart_rx_buf[offset + rx_value_len - 1])
        {
            //����У��ʧ��
            offset += 3; 
            continue;
        }
        data_handle(offset);
        offset += rx_value_len;
    }
    rx_in -= offset;

    if(rx_in > 0)
    {
		if((offset == 0)  && (rx_in >= sizeof(zigbee_uart_rx_buf)))
        {
            //������������������offset=0����ʱ�������޷�������ݣ����ǽ������ݳ��Ȳ������򶪵���һ�����ݣ� �����������ѭ��
            offset++;
            rx_in -= offset;
            my_memcpy((char*)zigbee_uart_rx_buf, (const char *)zigbee_uart_rx_buf + offset, rx_in);
        }
		else
		{
			my_memcpy((char*)zigbee_uart_rx_buf, (const char *)zigbee_uart_rx_buf + offset, rx_in);
		}
    }
}

