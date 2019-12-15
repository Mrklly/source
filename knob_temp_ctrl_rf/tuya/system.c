/****************************************Copyright (c)*************************
**                               版权所有 (C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------*/
#define SYSTEM_GLOBAL

#include "zigbee.h"
#include "protocol.h"
#include "global_variables.h"


extern const DOWNLOAD_CMD_S download_cmd[];

#ifdef SUPPORT_MCU_FIRM_UPDATE
     unsigned char x=0;
     unsigned char update_mcu_status=1;
	 unsigned char PID[8];
	 unsigned char DATA[64];
	 unsigned int IMAGE_OFFSET=0;
#endif

volatile unsigned char zigbee_queue_buf[PROTOCOL_HEAD + ZIGBEE_UART_QUEUE_LMT];        //串口队列缓存
volatile unsigned char zigbee_uart_rx_buf[PROTOCOL_HEAD + ZIGBEE_UART_RECV_BUF_LMT];   //串口接收缓存
volatile unsigned char zigbee_uart_tx_buf[PROTOCOL_HEAD + ZIGBEE_UART_SEND_BUF_LMT];   //串口发送缓存

volatile unsigned char *queue_in;
volatile unsigned char *queue_out;
volatile unsigned short queue_total_data;


static volatile unsigned short global_seq_num;
/*****************************************************************************
函数名称 : seq_num_set
功能描述 : 设置序列号
输入参数 : seq_num：序列号
返回参数 : 写入完成后的总长度
*****************************************************************************/
static void seq_num_set(unsigned short seq_num)
{
    global_seq_num = seq_num;
}
/*****************************************************************************
函数名称 : seq_num_get
功能描述 : 获取序列号
输入参数 : 无
返回参数 : 序列号
*****************************************************************************/
static unsigned short seq_num_get(void)
{
    return global_seq_num;
}

/*****************************************************************************
函数名称 : set_zigbee_uart_byte
功能描述 : 写zigbee_uart字节
输入参数 : dest:缓存区其实地址;
           byte:写入字节值
返回参数 : 写入完成后的总长度
*****************************************************************************/
unsigned short set_zigbee_uart_byte(unsigned short dest, unsigned char byte)
{
  unsigned char *obj = (unsigned char *)zigbee_uart_tx_buf + DATA_START + dest;
  
  *obj = byte;
  dest += 1;
  
  return dest;
}

/*****************************************************************************
函数名称 : set_zigbee_uart_buffer
功能描述 : 写zigbee_uart_buffer
输入参数 : dest:目标地址
           src:源地址
           len:数据长度
返回参数 : 无
*****************************************************************************/
unsigned short set_zigbee_uart_buffer(unsigned short dest, unsigned char *src, unsigned short len)
{
  unsigned char *obj = (unsigned char *)zigbee_uart_tx_buf + DATA_START + dest;
  
  my_memcpy(obj,src,len);
  
  dest += len;
  return dest;
}

/*****************************************************************************
函数名称 : zigbee_uart_write_data
功能描述 : 向zigbee uart写入连续数据
输入参数 : in:发送缓存指针
           len:数据发送长度
返回参数 : 无
*****************************************************************************/
static void zigbee_uart_write_data(unsigned char *in, unsigned short len)
{
  if((NULL == in) || (0 == len))
  {
    return;
  }
  
  while(len --)
  {
    uart_transmit_output(*in);
    in ++;
  }
}

/*****************************************************************************
函数名称 : get_check_sum
功能描述 : 计算校验和
输入参数 : pack:数据源指针
           pack_len:计算校验和长度
返回参数 : 校验和
*****************************************************************************/
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len)
{
  unsigned short i;
  unsigned char check_sum = 0;
  
  for(i = 0; i < pack_len; i ++)
  {
    check_sum += *pack ++;
  }
  
  return check_sum;
}

/*****************************************************************************
函数名称 : zigbee_uart_write_frame
功能描述 : 向zigbee串口发送一帧数据
输入参数 : fr_type:帧类型
           len:数据长度
返回参数 : 无
*****************************************************************************/
void zigbee_uart_write_frame(unsigned char fr_type, unsigned short len)
{
  unsigned char check_sum = 0;
  unsigned short seq_num = seq_num_get();
  
  zigbee_uart_tx_buf[HEAD_FIRST] = 0x55;
  zigbee_uart_tx_buf[HEAD_SECOND] = 0xaa;
  zigbee_uart_tx_buf[PROTOCOL_VERSION] = SERIAL_PROTOCOL_VER;
  zigbee_uart_tx_buf[SEQ_HIGH] = seq_num >> 8 ;
  zigbee_uart_tx_buf[SEQ_LOW] = seq_num & 0xff;
  zigbee_uart_tx_buf[FRAME_TYPE] = fr_type;
  zigbee_uart_tx_buf[LENGTH_HIGH] = len >> 8;
  zigbee_uart_tx_buf[LENGTH_LOW] = len & 0xff;

  seq_num++;
  seq_num_set(seq_num);
  len += PROTOCOL_HEAD ;
  check_sum = get_check_sum((unsigned char *)zigbee_uart_tx_buf, len - 1);
  zigbee_uart_tx_buf[len - 1] = check_sum;

  zigbee_uart_write_data((unsigned char *)zigbee_uart_tx_buf, len);
}

/*****************************************************************************
函数名称 : get_update_dpid_index
功能描述 : 获取制定DPID在数组中的序号
输入参数 : dpid:dpid
返回参数 : index:dp序号
*****************************************************************************/
static unsigned char get_dowmload_dpid_index(unsigned char dpid)
{
  unsigned char index;
  unsigned char total = get_download_cmd_total();
  
  for(index = 0; index < total; index ++)
  {
    if(download_cmd[index].dp_id == dpid)
    {
      break;
    }
  }
  
  return index;
}

/*****************************************************************************
函数名称 : get_queue_total_data
功能描述 : 读取队列内数据
输入参数 : 无
返回参数 : 无
*****************************************************************************/
unsigned short get_queue_total_data(void)
{
    if(queue_total_data == 0)
    {
        zigbee_protocol_init();
    }
    return (queue_total_data);
}


/*****************************************************************************
函数名称 : Queue_Read_Byte
功能描述 : 读取队列1字节数据
输入参数 : 无
返回参数 : 无
*****************************************************************************/
unsigned char Queue_Read_Byte(void)
{
    unsigned char value = 0;

    if(queue_total_data > 0)
    {
        //  有数据
        if(queue_out >= (unsigned char *)(zigbee_queue_buf + sizeof(zigbee_queue_buf)))
        {
            //  数据已经到末尾
            queue_out = (unsigned char *)(zigbee_queue_buf);
        }
        value = *queue_out++;
    	queue_total_data--;
    }
    return value;
}

/*****************************************************************************
函数名称 : zigbee_time_convert
功能描述 : 网络时间同步转换（UTC/local->年月日时分秒）
输入参数 : time：UTC时间或者本地时间
					 convert_time：转换之后的年月日时间
返回参数 : 转换结果指针
*****************************************************************************/
static unsigned char* zigbee_time_convert(unsigned long time, time_t* convert_time)
{
    const char days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    unsigned int pass4_year = 0, hours_per_year = 0;;

    if(0 == time || NULL == convert_time)
    {
        return NULL;
    }

    convert_time->second = time%60;
    time /= 60;
    convert_time->minute = time%60;
    time /= 60;

    pass4_year = time/(1461L * 24L);  /*365*4+1*/
    convert_time->year = (pass4_year << 2) + 1970;

    time %= (1461L * 24L);

    for( ; ; )
    {
        hours_per_year = 365 * 24;
        if((convert_time->year & 3) == 0) /*闰年多一天*/
        {
            hours_per_year += 24;
        }

        if(time < hours_per_year)
        {
            break;
        }

        convert_time->year++;
        time -= hours_per_year;
    }

    convert_time->hour = time % 24;
    time /= 24;
    time++; /*假定为闰年,矫正闰年误差， 计算月份、日期*/
    if((convert_time->year & 3) == 0 )
    {
        if(time > 60)
        {
            time --;
        }
        else
        {
            if(time == 60)
            {
                convert_time->month = 1;
                convert_time->day = 29;
                return (unsigned char*)convert_time;
            }
        }
    }

    /*计算月日*/
    for(convert_time->month = 0; days[convert_time->month] <time; convert_time->month++)
    {
        time -= days[convert_time->month];
    }
    convert_time->month++;
    convert_time->day = time;
    return (unsigned char*)convert_time;
}

/*****************************************************************************
函数名称 : zigbee_ota_ver_req_send
功能描述 : OTA版本请求回复
输入参数 : version：版本号
返回参数 : 无
*****************************************************************************/
static void zigbee_ota_ver_req_send(unsigned char version)
{
    unsigned short length = 0;

    length = set_zigbee_uart_byte(length,version);

    zigbee_uart_write_frame(MCU_OTA_VER_REQ_CMD,length);

    //return SUCCESS;
}
/*****************************************************************************
函数名称 : zigbee_ota_notify_send
功能描述 : OTA升级通知回复
输入参数 : status：状态
返回参数 : 无
*****************************************************************************/
static void zigbee_ota_notify_send(unsigned char status)
{
    unsigned short length = 0;

    length = set_zigbee_uart_byte(length,status);

    zigbee_uart_write_frame(MCU_OTA_NOTIFY_CMD,length);

    //return SUCCESS;
}
/*****************************************************************************
函数名称 : zigbee_ota_data_req_send
功能描述 : OTA固件内容请求
输入参数 : pid：PID
					 ver：版本号
					 image_offset：数据包偏移量
					 req_data_len: 数据包大小
返回参数 : 无
*****************************************************************************/
void zigbee_ota_data_req_send(unsigned char* pid, \
                                            unsigned char ver, \
                                            unsigned int image_offset, \
                                            unsigned char req_data_len)
{
    unsigned short length = 0, pid_len = 8;

    while(pid_len--)
    {
        length = set_zigbee_uart_byte(length,*pid++);
    }
    length = set_zigbee_uart_byte(length,ver);
    length = set_zigbee_uart_byte(length,image_offset >> 24);
    length = set_zigbee_uart_byte(length,image_offset >> 16);
    length = set_zigbee_uart_byte(length,image_offset >> 8);
    length = set_zigbee_uart_byte(length,image_offset & 0xff);

    length = set_zigbee_uart_byte(length,req_data_len);

    zigbee_uart_write_frame(MCU_OTA_DATA_REQ_CMD,length);

  //  return SUCCESS;
}
/*****************************************************************************
函数名称 : zigbee_ota_end_req_send
功能描述 : OTA固件升级结果上报
输入参数 : status：状态
					 pid：PID
					 ver：版本号
返回参数 : 无
*****************************************************************************/
static void zigbee_ota_end_req_send(unsigned char status, \
                                            unsigned char* pid, \
                                            unsigned char ver)
{
    unsigned short length = 0, pid_len = 8;

    length = set_zigbee_uart_byte(length,status);
     while(pid_len--)
    {
        length = set_zigbee_uart_byte(length,*pid++);
    }

    length = set_zigbee_uart_byte(length,ver);
    zigbee_uart_write_frame(MCU_OTA_END_CMD,length);

  //  return SUCCESS;
}





/*****************************************************************************
函数名称 : product_info_cmd_handle
功能描述 : 产品信息获取（PID和版本号）
输入参数 : 无
返回参数 : 无
*****************************************************************************/
static void product_info_cmd_handle()
{
    unsigned char length = 0;

    length = set_zigbee_uart_buffer(length,(unsigned char *)"{\"p\":\"",my_strlen((unsigned char *)"{\"p\":\""));
    length = set_zigbee_uart_buffer(length,(unsigned char *)PRODUCT_KEY,my_strlen((unsigned char *)PRODUCT_KEY));
    length = set_zigbee_uart_buffer(length,(unsigned char *)"\",\"v\":\"",my_strlen((unsigned char *)"\",\"v\":\""));
    length = set_zigbee_uart_buffer(length,(unsigned char *)MCU_VER,my_strlen((unsigned char *)MCU_VER));
    length = set_zigbee_uart_buffer(length,(unsigned char *)"\"}",my_strlen((unsigned char *)"\"}"));

    zigbee_uart_write_frame(PRODUCT_INFO_CMD, length);
}

/*****************************************************************************
函数名称 : zigbee_state_cmd_handle
功能描述 : ZigBee模块网络状态通知
输入参数 : current_state: 当前网络状态
返回参数 : 无
*****************************************************************************/
static void zigbee_state_cmd_handle(unsigned char current_state)
{
    zigbee_uart_write_frame(ZIGBEE_STATE_CMD, 0);
}

/*****************************************************************************
函数名称 : zigbee_cfg_cmd_handle
功能描述 : ZigBee模块配置信息通知
输入参数 : 无
返回参数 : 无
*****************************************************************************/
static void zigbee_cfg_cmd_handle(void)
{
    return;
}

/*****************************************************************************
函数名称 : zigbee_data_point_handle
功能描述 : ZigBee模块dp命令下发处理
输入参数 : value:下发数据源指针
返回参数 : ret:返回数据处理结果
*****************************************************************************/
static unsigned char zigbee_data_point_handle(const unsigned char value[])
{
  unsigned char dp_id,index;
  unsigned char dp_type;
  unsigned char ret;
  unsigned short dp_len;

  dp_id = value[0];
  dp_type = value[1];
  dp_len = value[2] * 0x100;
  dp_len += value[3];


  index = get_dowmload_dpid_index(dp_id);

  if(dp_type != download_cmd[index].dp_type)
  {
    //错误提示
    return FALSE;
  }
  else
  {
    ret = dp_download_handle(dp_id,value + 4,dp_len);
  }
  
  return ret;
}

/*****************************************************************************
函数名称 : zigbee_data_rsp_handle
功能描述 : ZigBee模块状态上报响应
输入参数 : rsp_status:状态上报status
返回参数 : 无
*****************************************************************************/
static void zigbee_data_rsp_handle(unsigned char rsp_status)
{
    //用户自行处理
}

#ifdef SERIAL_PROTOCOL_SCENE_ENABLE
/*****************************************************************************
函数名称 : zigbee_scene_switch_num_get_handle
功能描述 : ZigBee场景开关个数获取
输入参数 : rsp_status:状态上报status
返回参数 : 无
*****************************************************************************/
static void zigbee_scene_switch_num_get_handle(void)
{
    //用户自行处理
}

/*****************************************************************************
函数名称 : zigbee_scene_report_status_handle
功能描述 : ZigBee场景开关执行状态处理
输入参数 : scene_status:场景执行status
返回参数 : 无
*****************************************************************************/
static void zigbee_scene_report_status_handle(unsigned char scene_status)
{
    //用户自行处理
}
#endif

/*****************************************************************************
函数名称 : zigbee_scene_report_status_handle
功能描述 : ZigBee模块RF功能测试结果处理
输入参数 : result:结果返回状态， true是成功， false是失败
输入参数 : rssi:ZigBee模块rssi测试数值（0-100）
返回参数 : 无
*****************************************************************************/
static void zigbee_rf_test_result_handle(bool result, unsigned char rssi)
{
    zigbee_test_result( result, rssi);
}

#ifdef SUPPORT_MCU_FIRM_UPDATE
/*****************************************************************************
函数名称 : zigbee_ota_ver_req_handle
功能描述 : ZigBee模块获取MCU固件版本号
输入参数 : 无
返回参数 : 无
*****************************************************************************/
static void zigbee_ota_ver_req_handle(void)
{
	//用户自行处理，以下代码仅做参考
	/*
	if(x==0)
	{
  zigbee_ota_ver_req_send(NOW_MCU_VER);
	x=1;
	}
	if(x==1)
	{
	zigbee_ota_ver_req_send(UPDATE_MCU_VER);
	}
	*/

}


/*****************************************************************************
函数名称 : zigbee_ota_notify_handle
功能描述 : ZigBee模块通知MCU OTA升级信息
输入参数 : pid： 设备pid信息
输入参数 : version： ota固件版本号
输入参数 : image_size： ota固件大小
输入参数 : image_crc： ota固件校验和
返回参数 : 无
*****************************************************************************/
static void zigbee_ota_notify_handle(unsigned char* pid, \
                                            unsigned char version, \
                                            unsigned int image_size, \
                                            unsigned int image_crc)

{
	//用户自行处理，以下代码仅做参考
	/*
	unsigned char i=0;
	for(i=0;i<8;i++)
	{
		PID[i]=pid[i];
	}

	update_mcu_status=0;
  zigbee_ota_notify_send(update_mcu_status);
	*/

}

/*****************************************************************************
函数名称 : zigbee_ota_data_rsp_handle
功能描述 : ZigBee模块通知MCU OTA升级信息
输入参数 : status: 0 成功， 1 失败
输入参数 : pid： 设备pid信息
输入参数 : version： ota固件版本号
输入参数 : image_offset： ota固件偏移大小
输入参数 : data_len：ota升级数据长度
输入参数 : data： ota升级数据
返回参数 : 无
*****************************************************************************/
static void zigbee_ota_data_rsp_handle(unsigned char status, 
                                    unsigned char* pid, \
                                    unsigned char version, \
                                    unsigned int  image_offset, \
                                    unsigned char data_len, \
                                    unsigned char* data)
{
	//用户自行处理，以下代码仅做参考
	/*
	unsigned  char i;

		for(i=0;i<data_len;i++)
	{
		DATA[IMAGE_OFFSET+i]=data[i];
	}
		IMAGE_OFFSET+=5;
		zigbee_ota_data_req_send(PID,version,IMAGE_OFFSET,5);
	if(data_len<5)
	{
		IMAGE_OFFSET=0;
		zigbee_ota_end_req_send(0,pid,UPDATE_MCU_VER);
	}
//		zigbee_ota_end_req_send(0,pid,UPDATE_MCU_VER);
	*/

}
#endif
/*****************************************************************************
函数名称 : zigbee_time_get_handle
功能描述 : 网络数据同步处理
输入参数 : data：数据指针
输入参数 : data_len：数据长度
返回参数 : 无
*****************************************************************************/
static void zigbee_time_get_handle(unsigned char* data, unsigned char data_len)
{
    unsigned long time1 =0, time2 = 0;
    time_t localTime, utcTime;
    unsigned char i = 0;
    unsigned char* ptr = data;
    
    if (data_len < 8)
    {
        return;
    }

    my_memset(&localTime, 0, sizeof(time_t));
    my_memset(&utcTime, 0, sizeof(time_t));

    for(i=0; i<3; i++) /*UTC time*/
    {
        time1 |= *ptr++;
        time1 = time1 << 8;
    }
    time1 |= *ptr++;

    for(i=0; i<3; i++) /*local time*/
    {
        time2 |= *ptr++;
        time2 = time2 << 8;
    }
    time2 |= *ptr++;

    zigbee_time_convert(time1, &utcTime);
    zigbee_time_convert(time2, &localTime);

    mcu_write_rtctime(TRUE, &utcTime);
    mcu_write_rtctime(FALSE, &localTime);
}

/*****************************************************************************
函数名称 : data_handle
功能描述 : 数据帧处理
输入参数 : offset:数据起始位
返回参数 : 无
*****************************************************************************/
void data_handle(unsigned short offset)
{
    unsigned char cmd_type = 0;
    unsigned short total_len = 0, seq_num = 0;

    cmd_type   = zigbee_uart_rx_buf[offset + FRAME_TYPE];
    total_len  = zigbee_uart_rx_buf[offset + LENGTH_HIGH] * 0x100;
    total_len += zigbee_uart_rx_buf[offset + LENGTH_LOW];

    seq_num = zigbee_uart_rx_buf[offset + SEQ_HIGH] << 8;
    seq_num += zigbee_uart_rx_buf[offset + SEQ_LOW];
    seq_num_set(seq_num);

    switch(cmd_type)
    {
        case PRODUCT_INFO_CMD:
            product_info_cmd_handle();
            break;

        case ZIGBEE_STATE_CMD:
            {
                g_zig_net_sta = zigbee_uart_rx_buf[offset + DATA_START];
                zigbee_state_cmd_handle(g_zig_net_sta);

                if(g_set_type == SET_ZIGBEE) {
                    if(g_zig_net_sta == ZIG_NET_STA_OK) {
                        g_set_type = SET_NONE;
                        g_led_on = 1;
                        g_lcd_on = 1;
                    }
                }
            }
            break;

        case ZIGBEE_CFG_CMD:
            zigbee_cfg_cmd_handle();
            break;

        case ZIGBEE_DATA_REQ_CMD:
            {
                unsigned char i = 0, ret;
                unsigned short dp_len = 0;
                
                for(i = 0;i < total_len;)
                {
                  dp_len = zigbee_uart_rx_buf[offset + DATA_START + i + 2] << 8;
                  dp_len += zigbee_uart_rx_buf[offset + DATA_START + i + 3];

                  ret = zigbee_data_point_handle((unsigned char *)zigbee_uart_rx_buf + offset + DATA_START + i);
                  
                  if(ZIGBEE_SUCCESS == ret)
                  {
                    //成功提示
                  }
                  else
                  {
                    //错误提示
                  }
                  i += (dp_len + 4);
                }
            }
            break;

        case DATA_RSP_SYNC_CMD:
        case DATA_RSP_ASYNC_CMD:
            {
                unsigned char rsp_status = zigbee_uart_rx_buf[offset + DATA_START];
                zigbee_data_rsp_handle( rsp_status);
            }
            break;

#ifdef SERIAL_PROTOCOL_SCENE_ENABLE
        case SCENE_SWITCH_NUM_GET_CMD:
            zigbee_scene_switch_num_get_handle();
            break;

        case SCENE_SWITCH_ID_REPORT_CMD:
            {
                unsigned char scene_status = zigbee_uart_rx_buf[offset + DATA_START];
                zigbee_scene_report_status_handle(scene_status);
            }
            break;
#endif

        case FUNC_TEST_CMD:
            {
                bool flag = zigbee_uart_rx_buf[offset + DATA_START];
                unsigned char rssi = zigbee_uart_rx_buf[offset + DATA_START + 1];
                zigbee_rf_test_result_handle(flag, rssi);
            }
            break;
#ifdef SUPPORT_MCU_FIRM_UPDATE
        case MCU_OTA_VER_REQ_CMD:
            {
                zigbee_ota_ver_req_handle();
            }
            break;

        case MCU_OTA_NOTIFY_CMD:
            {
                unsigned char pid[8] = {0};
                unsigned char version = 0, index = 0;
                unsigned int image_size = 0;
                unsigned int image_crc = 0;
                unsigned char receive_len = offset + DATA_START;

                my_memcpy(pid,(const char *)zigbee_uart_rx_buf+receive_len,8);
                receive_len += 8;
                version = zigbee_uart_rx_buf[receive_len];
                receive_len++;

                do
                {
                    image_size += zigbee_uart_rx_buf[receive_len];
                    receive_len++;
                    image_size = image_size << 8;
										index++;
                }
                while (index < 3);

                image_size += zigbee_uart_rx_buf[receive_len];
                receive_len++;

                index = 0;
                do
                {
                    image_crc += zigbee_uart_rx_buf[receive_len];
                    receive_len++;
                    image_size = image_crc << 8;
										index++;
                }
                while (index < 3);
                image_crc += zigbee_uart_rx_buf[receive_len];
                receive_len++;

                zigbee_ota_notify_handle(pid, version, image_size, image_crc);
            }
            break;

        case MCU_OTA_DATA_REQ_CMD:
            {
                unsigned char status = 0;
                unsigned char pid[8] = {0};
                unsigned char version = 0, index = 0;;
                unsigned int image_offset = 0;
                unsigned char receive_len = offset + DATA_START;
                unsigned char data_len = 0;
                unsigned char data[64] = {0};

                status = zigbee_uart_rx_buf[receive_len];
								if(status==0)
									{
										receive_len++;
										//memcpy(pid, &zigbee_uart_rx_buf[receive_len]);
										my_memcpy(pid,(const char *)(zigbee_uart_rx_buf+receive_len),8);
										receive_len += 8;

										version = zigbee_uart_rx_buf[receive_len];
										receive_len++;

										do
										{
												image_offset += zigbee_uart_rx_buf[receive_len];
												receive_len++;
												image_offset = image_offset << 8;
												index++;
										}
										while (index < 3);
										image_offset += zigbee_uart_rx_buf[receive_len];
										receive_len++;
										data_len = total_len+PROTOCOL_HEAD - receive_len;
										my_memcpy(data, (const char *)(zigbee_uart_rx_buf+receive_len),data_len);
										//data_len = total_len - receive_len;

										zigbee_ota_data_rsp_handle(status, \
																				pid, \
																				version, \
																				image_offset, \
																				data_len, \
																				data);
									}
								else
									return;
            }
            break;
#endif
        case TIME_GET_CMD:
            zigbee_time_get_handle((unsigned char *)zigbee_uart_rx_buf + offset + DATA_START, total_len);
            break;

        default:
            break;
    }
}

