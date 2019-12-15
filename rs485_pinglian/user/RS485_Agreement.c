#include "RS485_Agreement.h"
uint8_t ReceiveRs485Agreement[RS485_AGREEMENT_LEN];              //定义RS485协议接收缓存。
uint8_t SendRs485Agreement   [RS485_AGREEMENT_LEN];              //定义RS485协议发送缓存。
extern uint8_t ReceiveFlag;                                     //接收标志，1代表有串口数据,0代表无数据。
extern uint8_t uart_flag;                                       //接收计数。
extern uint8_t pwm_flag[4];
uint32_t key_config[4];                                         //按键配置。
uint8_t key_led_state[4]={0,0,0,0};                             //LED临时状态。
rs458_InitTypedef rs485InitStruct;                              //初始化结构体。
uint8_t cont=0;

void RS485_Agreement_Config(void)
{
  if(ReceiveFlag==1)                                                            //有串口数据。
  {
    time2_xms(15);                                                              //延时15MS等待串口接收数据完成。
    Uart_DisableFunc(UARTCH1,UartRx);                                           //关闭串口接收
    if((ReceiveRs485Agreement[0]==0xfe)&&(uart_flag>=ReceiveRs485Agreement[2])) //配置模式
    {  
      if((RS485_Parse_check(ReceiveRs485Agreement[2],ReceiveRs485Agreement)==ReceiveRs485Agreement[ReceiveRs485Agreement[2]+1])&&(ReceiveRs485Agreement[1]==0x55))
      {
        switch(ReceiveRs485Agreement[3])                                   //判断是字命令。
        {
        case PANEL_ADDRESS:                                                //修改面板地址。
          RS485_config_writeEeprom(PANEL_ADDR,4,6);                        //写入面板地址。
          break;
        case PANEL_LED_STATE:                                              //设置面板LED灯初始状态         
          RS485_config_writeEeprom(LED_SATTE_ADDR,4,8);                    //给LED写初始状态。
          break;
        case KEY_AREA_PROPERTIES:                                          //设置按键区域属性。
          RS485_config_writeEeprom(KEY_PARTITION_ADDR,4,12);               //写入区域属性。
          break;
        case SET_KEY_FUNCTION:                                             //设置按键功能。
          RS485_config_writeEeprom(KEY_FUNCTION_ADDR,4,12);                //给按键写入功能。
          break;
        case READ_PANEL_STATE:                                             //上位机读面板状态。
          break;
        case RESTORE_FACTORY_SETTING:                                      //恢复出厂设置。         
          eeprom_erase_all();
          break;
        default:
          break;   
        }
        RX485_Prepare_receive();                                           //为下次接收数据做准备。                         
      }
      else
      {
        RX485_Prepare_receive();                           
      }
    }
    else if((ReceiveRs485Agreement[0]==0xfa)&&(uart_flag==6)&&(ReceiveRs485Agreement[1]==0))        //普通广播。
    {
      if(RS485_Parse_check(4,ReceiveRs485Agreement)==ReceiveRs485Agreement[5])                      //识别校验码。
      {
        Broadcast_control_LED();           //根据广播命令执行功能。
      }
      RX485_Prepare_receive();                               
    }
    else if((ReceiveRs485Agreement[0]==0xfc)&&(uart_flag==6)&&(ReceiveRs485Agreement[1]>0))        //区域广播。
    {
      Regional_model_control();           //区域模式数据处理
      RX485_Prepare_receive();            //为下次接收数据做准备。                      
    }
    else                                 //以上都不成立，代表数据接收错误。
    {
      RX485_Prepare_receive();           //为下次接收数据做准备。
    }
  }
}

uint8_t RS485_Parse_check (uint8_t ReceiveLeng,uint8_t *buff)
{
  uint8_t temp,i;
  temp=buff[1];
  for(i=2;i<=ReceiveLeng;i++)
  {
    temp^=buff[i];
  }
  return temp;
}

void RS485_config_writeEeprom(uint32_t addr,uint8_t rise,uint8_t length)
{
  uint8_t i;
  uint8_t config_data[24];
  flash_read_buf(AGREEMENT_ADDR,config_data,24);                    //读出按键区域配置。
  for(i=0;i<24;i++)
  {
    if(config_data[i]==0xff)
    {
      config_data[i]=0;
    }
  }
  switch(addr)
  {
  case AGREEMENT_ADDR:  
    return;
    break;
  case DEFSTATE_ADDR:
    return;
    break;
  case LED_SATTE_ADDR:  
    for(i=2;i<6;i++)
    {
      config_data[i]=ReceiveRs485Agreement[4+i-2];
    }
    break;
  case KEY_FUNCTION_ADDR:
    for(i=6;i<14;i++)
    {
      config_data[i]=ReceiveRs485Agreement[4+i-6];
    }
    break;
  case KEY_PARTITION_ADDR:
    for(i=14;i<22;i++)
    {
      config_data[i]=ReceiveRs485Agreement[4+i-14];
    }
    break;
  default:
    break;
  }
  UART_Multi_byte(config_data,24);
  __disable_irq();
  Flash_SectorErase(AGREEMENT_ADDR);
  flash_write_buf(AGREEMENT_ADDR,config_data,24);
  __enable_irq();
}

uint32_t Broadcast_data_packing (uint8_t *buff)
{
  uint32_t temp=0;
  temp=(uint32_t)buff[1]<<16|(uint32_t)buff[2]<<8|(uint32_t)buff[3];
  return temp;
}

void EEPROM_data_packing(void)
{
  uint8_t i;
  flash_read_buf(AGREEMENT_ADDR,ReceiveRs485Agreement,24);                    //读出按键区域配置。                      
  rs485InitStruct.SOH=ReceiveRs485Agreement[0];                               //协议头，保留字节。
  rs485InitStruct.def_sta=ReceiveRs485Agreement[1];                           //保留字节。
  for(i=2;i<6;i++)
  {
    rs485InitStruct.led_state[i-2]=ReceiveRs485Agreement[i];                  //将LED状态赋值给结构体变量。4个字节。
  }     
  for(i=6;i<14;i++)
  {
    rs485InitStruct.key_function[i-6]=ReceiveRs485Agreement[i];               //将按键功能赋值给结构体。
  }
  for(i=14;i<18;i++)
  {
    rs485InitStruct.key_partition[i-14]=ReceiveRs485Agreement[i];             //区域属性赋值结构体。
  }
  for(i=18;i<23;i++)
  {
    rs485InitStruct.Transboundary_zoning[i-18]=ReceiveRs485Agreement[i];      //跨区域属性赋值。
  }
  rs485InitStruct.panel_add=ReceiveRs485Agreement[23];                        //面板地址
  key_config[0]=(uint32_t)rs485InitStruct.key_partition[0]<<16|(uint32_t)rs485InitStruct.key_function[0]<<8|(uint32_t)rs485InitStruct.key_function[1];
  key_config[1]=(uint32_t)rs485InitStruct.key_partition[1]<<16|(uint32_t)rs485InitStruct.key_function[2]<<8|(uint32_t)rs485InitStruct.key_function[3];
  key_config[2]=(uint32_t)rs485InitStruct.key_partition[2]<<16|(uint32_t)rs485InitStruct.key_function[4]<<8|(uint32_t)rs485InitStruct.key_function[5];
  key_config[3]=(uint32_t)rs485InitStruct.key_partition[3]<<16|(uint32_t)rs485InitStruct.key_function[6]<<8|(uint32_t)rs485InitStruct.key_function[7];
  key_led_state[0]=rs485InitStruct.led_state[0];                 
  key_led_state[1]=rs485InitStruct.led_state[1];
  key_led_state[2]=rs485InitStruct.led_state[2];
  key_led_state[3]=rs485InitStruct.led_state[3]; 
}

void Broadcast_control_LED(void)
{         
  switch(ReceiveRs485Agreement[2])
  {
  case TOTAL_SW_CMD:                            //接收到总开关命令 0K
    switch(ReceiveRs485Agreement[4])
    {
    case LED_ON:          
      Always_LED_control(1);     
      break;
    case LED_OFF:
      Always_LED_control(0);
      break;
    default:
      break;
    }
    return;
    break;
  case GENERAL_OPEN_CMD:                        //接收到总开命令处理。 OK
    Always_LED_control(1);
    return;
    break;
  case GENERAL_CLOSE_COM:                       //接收到总关命令处理。 OK
    Always_LED_control(0);
    return;
    break;
  case MULTI_INTERLOCKING_CMD:                  //多互锁，总共16组。每组16个按键。   ok  
    Public_Multi_interlocking(ReceiveRs485Agreement); 
    return;
    break;
  case DOUBLE_INTERLOCKING_CMD:                 //双控互锁命令。总共16组。每组16个按键。 ok   
    Public_Multi_interlocking(ReceiveRs485Agreement); 
    return;
    break;
  case CLEAR_DISTURB_CMD:                       //清理勿扰命令。总共16组。每组2个按键。ok 
    Public_Multi_interlocking(ReceiveRs485Agreement);
    return;
    break;
  default:
    break; 
  }
  Common_pattern_control(ReceiveRs485Agreement);
}

void Always_LED_control(uint8_t mode)
{
  uint8_t i;
  for(i=0;i<4;i++)
  {
    while(!Exclusion_function (mode,i))     //排除夜灯，点动，清理勿扰，独立开关，互锁开关。
    {
      led_closestate0(mode,i);              
      break;                                //跳出while,执行下一个FOR。
    } 
  }  
}

void Common_pattern_control(uint8_t *buff)
{
  uint8_t i;
  uint32_t temp=0;
  for(i=0;i<4;i++)
  {
    if((Broadcast_data_packing (buff)==key_config[i]))          //按键对码。
    {
      switch(buff[4])                                           //比对开关属性。
      {
      case LED_ON:
        Linkage_open_close(i+1,1);
        break;
      case LED_OFF:
        Linkage_open_close(i+1,0);
        break;
      default:
        break;
      }
    }
    else if(Second_control(i,buff))                             //判断第二属性。
    {
      temp=(uint32_t)cont<<16|(key_config[i]&0x00ffff);         //生成第二属性键值。
      if(Broadcast_data_packing(buff)==temp)                    //对比按键第二属性是否完全相等。
      {
        led_closestate0(buff[4],i);  
        cont=0;
      }     
    }
  }
}

void Regional_model_control(void)
{
  switch(ReceiveRs485Agreement[2])
  {
  case TOTAL_SW_CMD:                               //总开关命令，0x02。OK
    if(ReceiveRs485Agreement[1]==ReceiveRs485Agreement[3])      
    {
      switch(ReceiveRs485Agreement[4])
      {
      case LED_ON:
        Regional_Close_opening(1,ReceiveRs485Agreement);
        break;
      case LED_OFF:
        Regional_Close_opening(0,ReceiveRs485Agreement); 
        break;
      default:
        break;       
      }
    }
    return;
    break;
  case GENERAL_OPEN_CMD:                                //接收到总开命令处理。
    Regional_Close_opening(1,ReceiveRs485Agreement);
    return;
    break;
  case GENERAL_CLOSE_COM:                               //接收到总关命令处理。
    Regional_Close_opening(0,ReceiveRs485Agreement);
    return;
    break;
  case MULTI_INTERLOCKING_CMD:                  //多互锁，总共16组。每组16个按键。   OK
    Public_Multi_interlocking(ReceiveRs485Agreement); 
    return;
    break;
  case DOUBLE_INTERLOCKING_CMD:                 //双控互锁命令。总共16组。每组16个按键。  ok 
    Public_Multi_interlocking(ReceiveRs485Agreement); 
    return;
    break;
  case CLEAR_DISTURB_CMD:                       //清理勿扰命令。总共16组。每组2个按键。  ok
    Public_Multi_interlocking(ReceiveRs485Agreement);
    return;
    break;
  default:
    break;
  }
  if(ReceiveRs485Agreement[1]>0)                //进入普通模式。
  {
    Common_pattern_control(ReceiveRs485Agreement);
  }
}

void Regional_Close_opening(uint8_t state,uint8_t *buff)
{
  uint8_t i;
  for(i=0;i<4;i++)
  {
    if((rs485InitStruct.key_partition[i]>0)||(rs485InitStruct.Transboundary_zoning[i]>0))//不成立，则接收到的命令与按键无关。
    {
      if(buff[1]==rs485InitStruct.key_partition[i])     //第一模式属性是否相等。
      {  
        while(!Exclusion_function (state,i))            //排除夜灯，点动，清理勿扰。
        {
          led_closestate0(state,i);                     //控制LED灯。
          break;                                       //跳出while,执行下一个FOR.
        } 
      }
      else if(Second_control(i,buff))                  //判断第二属性。
      {
        while(!Exclusion_function (state,i))           //排除夜灯，点动，清理勿扰。
        {
          led_closestate0(state,i);                    //控制LED灯。
          break;                                       //跳出while,执行下一个FOR.
        } 
      }
      else if(buff[2]==GENERAL_CLOSE_COM)             //功能是否总关。      
      {
        
      }
      else if((buff[2]==TOTAL_SW_CMD)&&(state==0))
      {
      
      }
      else                                            //模式属性和跨模式属性都不相等，且具备模式属性。立直接关闭灯。
      {
        Linkage_open_close(i+1,0);
      }
    }
  }
}

void Power_State_Config(void)
{
  uint8_t i, temp[4]={0x00,0x02,0x04,0x06};
  for(i=0;i<4;i++)
  {
    if((rs485InitStruct.key_function[temp[i]]>=0x51)&&(rs485InitStruct.key_function[temp[i]]<=0x71))//在本区段内直接上电亮灯。
    {
      pwm_flag[i]=0x01;
      Linkage_open_close(i+1,1);        //打开LED灯。
    }
    else if(key_led_state[i]==0)        //如果不在区段中，就根据配置信息确定上电状态。
    {
      pwm_flag[i]=0x02;
      Linkage_open_close(i+1,0);
    } 
    else
    {
      pwm_flag[i]=0x01;
      Linkage_open_close(i+1,1);
    }
  }
}

void Linkage_open_close(uint8_t key,uint8_t state) 
{
  switch(key)
  {
  case 1:
    LED_open_Close_Config(0,state);  //LED1输出0
    Relay_open_Close_Config(RELAY_CH1_PORT,RELAY_CH1_PIN,state);  //继电器通道1设置低电平。
    break;
  case 2:
    LED_open_Close_Config(1,state);  //LED2输出0
    Relay_open_Close_Config(RELAY_CH2_PORT,RELAY_CH2_PIN,state);  //继电器通道2设置低电平。
    break;
  case 3:
    LED_open_Close_Config(2,state);  //LED3输出0
    Relay_open_Close_Config(RELAY_CH3_PORT,RELAY_CH3_PIN,state);  //继电器通道3设置低电平。
    break;
  case 4:
    LED_open_Close_Config(3,state);  //LED4输出0
    Relay_open_Close_Config(RELAY_CH4_PORT,RELAY_CH4_PIN,state);  //继电器通道4设置低电平。
    break;
  default:
    break;
  }
}

bool Second_control(uint8_t key,uint8_t *buff)
{
  uint8_t i, temp[9]={0x0,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
  for(i=1;i<9;i++)
  {
    if((rs485InitStruct.Transboundary_zoning[key]&temp[i])==temp[i])
    {
      if(i==buff[1])
      {
        cont=temp[i];
        return TRUE;
      }
    }
  }
  cont=0;
  return FALSE;
}

void RX485_Prepare_receive(void)
{
  ReceiveFlag=0;  
  uart_flag=0;    
  Uart_EnableFunc(UARTCH1,UartRx);         //为下次接收数据做准备。
}

void Public_Multi_interlocking(uint8_t *buff)
{
  uint8_t i, temp[4]={0x00,0x02,0x04,0x06};
  uint8_t temp2[4]={0x01,0x03,0x05,0x07};
  uint32_t temp3;
  for(i=0;i<4;i++)
  {
    if((Broadcast_data_packing (buff)==key_config[i]))    //收到的多控互锁广播，与自己完全相等（属于多控功能）。
    {
      switch(buff[4])                                     //比对开关属性。
      {
      case LED_ON:
        Linkage_open_close(i+1,1);
        break;
      case LED_OFF:
        Linkage_open_close(i+1,0);
        break;
      default:
        break;
      }
    }
    else if((rs485InitStruct.key_function[temp[i]]==buff[2])&&(rs485InitStruct.key_partition[i]==buff[1]))  //判断按键是否是09功能属性。
    {
      if((rs485InitStruct.key_function[temp2[i]]&0xf0)==(buff[3]&0xf0))  //判断类是否相等（相等就需要互锁）
      {
        switch(buff[4])                                                 //比对开关属性。
        {
        case LED_ON:
          Linkage_open_close(i+1,0);
          break;
        case LED_OFF:
          break;
        default:
          break;
        }
      }
    }
    else if(Second_control(i,buff))                             //判断第二属性。
    {
      temp3=(uint32_t)cont<<16|(key_config[i]&0x00ffff);        //生成第二属性键值。
      if(Broadcast_data_packing(buff)==temp3)                   //对比按键第二属性是否完全相等。
      {
        led_closestate0(buff[4],i);  
        cont=0;
      }
      else if((cont==buff[1])&&(rs485InitStruct.key_function[temp[i]]==buff[2]))
      {
        if((rs485InitStruct.key_function[temp2[i]]&0xf0)==(buff[3]&0xf0))//判断类是否相等（相等就需要互锁）
        {
          switch(buff[4])                                                //比对开关属性。
          {
          case LED_ON:
            Linkage_open_close(i+1,0);
            break;
          case LED_OFF:
            break;
          default:
            break;
          }
        } 
      }
    }
    else
    {
    }
  }
}

void clear_Disturb(uint8_t mode,uint8_t key)
{
  switch((uint8_t)(key_config[key]&0x0F))
  {       
  case 0x01:                        //清理功能。
    Linkage_open_close(key+1,0);  
    break;
  case 0x02:                        //勿扰功能。
    led_openstate0(mode,key); 
    break;
  default:
    break;          
  } 
}

uint8_t Exclusion_function (uint8_t mode,uint8_t key)
{
  uint8_t temp;
  switch((uint8_t)(key_config[key]>>8&0xff))
  {
  case NIGHT_LAMP_CMD:                                  //夜灯    OK
    led_openstate0(mode,key);
    temp=TRUE;
    break;
  case POINT_PRESS_CMD:                                 //点动开关  OK
    temp=TRUE;
    break;
  case CLEAR_DISTURB_CMD:                               //清理勿扰。OK
    clear_Disturb(mode,key);
    temp=TRUE;
    break;
  case GENERAL_OPEN_CMD:                                //总开 OK
    temp=TRUE;
    break;
  case GENERAL_CLOSE_COM:                               //总关  OK
    temp=TRUE;
    break;
  case SPECIAL_SERVICE_CMD:                             //独立开关  OK
    temp=TRUE;
    break;
  case INTER_LOCK_CMD:                                  //互锁开关  OK
    temp=TRUE;
    break;
    
    //  case WINDOW_CURTAINS_CMD:     //窗帘。
    //    led_closestate0(mode,key);
    //    temp=TRUE;
  default:
    temp=FALSE;
    break; 
  }
  return temp;
}

void led_openstate0(uint8_t state,uint8_t key)
{
  if(state==0)                                                 //跟传入状态确定是否打开LED。
  {
    Linkage_open_close(key+1,1);            
  }
  else
  {
    Linkage_open_close(key+1,0);                                //反之关闭夜灯。
  }
}

void led_closestate0(uint8_t state,uint8_t key)
{
  if(state==0)                                                 //跟传入状态确定是否打开LED。
  {
    Linkage_open_close(key+1,0);            
  }
  else
  {
    Linkage_open_close(key+1,1);                               
  }
}

void eeprom_erase_all(void)
{
    __disable_irq();
    Flash_SectorErase(AGREEMENT_ADDR);
    Flash_SectorErase(DEFSTATE_ADDR);
    Flash_SectorErase(LED_SATTE_ADDR);
    Flash_SectorErase(KEY_PARTITION_ADDR );
    __enable_irq();
}
