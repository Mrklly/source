#include "RS485_Agreement.h"
uint8_t ReceiveRs485Agreement[RS485_AGREEMENT_LEN];              
uint8_t SendRs485Agreement   [RS485_AGREEMENT_LEN];              
extern uint8_t ReceiveFlag;                                     
extern uint8_t uart_flag;                                      
extern uint8_t pwm_flag[6];
uint32_t key_config[4];                                         
uint8_t key_led_state[4]={0,0,0,0};                             
rs458_InitTypedef rs485InitStruct;                              
uint8_t cont=0;
extern uint8_t power1_flag;                                     
extern uint8_t power2_flag;                                     
extern uint8_t Chopping1_channel_cont;                          
extern uint8_t Chopping2_channel_cont; 
/*****************************************************************************************************
程序名：RS485_Agreement_Config
功能：RS485协议配置函数。
参数：无参数。
返回值：无返回值。
*****************************************************************************************************/
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
        case SET_KEY_FUNCTION:                                             
          RS485_config_writeEeprom(KEY_FUNCTION_ADDR,4,12);                
          break;
        case READ_PANEL_STATE:                                             
          break;
        case RESTORE_FACTORY_SETTING:                                          
          eeprom_erase_all();
          break;
	 case  READ_FACTORY_SETTING:  
	   flash_read_buf(AGREEMENT_ADDR, ReceiveRs485Agreement,24);                        //读出按键区域配置。实际仅定义了20个有用字节数。
	   UART_Multi_byte(ReceiveRs485Agreement,24);
          break;
        default:
          break;   
        }
        RX485_Prepare_receive();                                                                 
      }
      else
      {
        RX485_Prepare_receive();                           
      }
    }
    else if((ReceiveRs485Agreement[0]==0xfa)&&(uart_flag==6)&&(ReceiveRs485Agreement[1]==0))       
    {
      if(RS485_Parse_check(4,ReceiveRs485Agreement)==ReceiveRs485Agreement[5])                     
      {
        Broadcast_control_LED();           
      }
      RX485_Prepare_receive();                               
    }
    else if((ReceiveRs485Agreement[0]==0xfc)&&(uart_flag==6)&&(ReceiveRs485Agreement[1]>0))        
    {
      Regional_model_control();           
      RX485_Prepare_receive();                              
    }
    else                                 
    {
      RX485_Prepare_receive();           
    }
  }
}
/*****************************************************************************************************
程序名：RS485_Parse_check
功能：生成异或校验码。
参数：参数1:数据长度。参数2：参与亦或的数据。
返回值：返回校验码。
*****************************************************************************************************/
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
/*****************************************************************************************************
程序名：RS485_config_writeEeprom
功能：将配置信息写入到eeprom
参数：参数1：EEPROM地址。参数2：写入的起始点，写入长度。
返回值：2字节协议头+4字节LED灯状态+8字节按键功能+4字节按键区域属性+4字节跨区属性。
*****************************************************************************************************/
void RS485_config_writeEeprom(uint32_t addr,uint8_t rise,uint8_t length)
{
  uint8_t i;
  uint8_t config_data[24];
  flash_read_buf(AGREEMENT_ADDR,config_data,24);                   
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
/*****************************************************************************************************
程序名：Broadcast_data_packing 
功能：将广播接收的数据打包，组合成一个32位数据。用来与内存数据做比较。
参数：参数1：传入要打包的数据。
返回值：无返回值
*****************************************************************************************************/
uint32_t Broadcast_data_packing (uint8_t *buff)
{
  uint32_t temp=0;
  temp=(uint32_t)buff[1]<<16|(uint32_t)buff[2]<<8|(uint32_t)buff[3];
  return temp;
}
/*****************************************************************************************************
程序名：EEPROM_data_packing 
功能：将EEPROM内部配置数据打包成4个按键配置。
参数：无参数。
返回值：无返回值
*****************************************************************************************************/
void EEPROM_data_packing(void)
{
  uint8_t i;
  flash_read_buf(AGREEMENT_ADDR,ReceiveRs485Agreement,24);                                         
  rs485InitStruct.SOH=ReceiveRs485Agreement[0];                              
  rs485InitStruct.def_sta=ReceiveRs485Agreement[1];                           
  for(i=2;i<6;i++)
  {
    rs485InitStruct.led_state[i-2]=ReceiveRs485Agreement[i];                 
  }     
  for(i=6;i<14;i++)
  {
    rs485InitStruct.key_function[i-6]=ReceiveRs485Agreement[i];               
  }
  for(i=14;i<18;i++)
  {
    rs485InitStruct.key_partition[i-14]=ReceiveRs485Agreement[i];             
  }
  for(i=18;i<23;i++)
  {
    rs485InitStruct.Transboundary_zoning[i-18]=ReceiveRs485Agreement[i];      
  }
  rs485InitStruct.panel_add=ReceiveRs485Agreement[23];                       
  key_config[0]=(uint32_t)rs485InitStruct.key_partition[0]<<16|(uint32_t)rs485InitStruct.key_function[0]<<8|(uint32_t)rs485InitStruct.key_function[1];
  key_config[1]=(uint32_t)rs485InitStruct.key_partition[1]<<16|(uint32_t)rs485InitStruct.key_function[2]<<8|(uint32_t)rs485InitStruct.key_function[3];
  key_config[2]=(uint32_t)rs485InitStruct.key_partition[2]<<16|(uint32_t)rs485InitStruct.key_function[4]<<8|(uint32_t)rs485InitStruct.key_function[5];
  key_config[3]=(uint32_t)rs485InitStruct.key_partition[3]<<16|(uint32_t)rs485InitStruct.key_function[6]<<8|(uint32_t)rs485InitStruct.key_function[7];
  if(rs485InitStruct.led_state[0]==0)
  {
    power1_flag=0;
    key_led_state[0]=0;
    pwm_flag[1]=0x02;
  }
  else if(rs485InitStruct.led_state[0]==1)
  {
    power1_flag=1;
    key_led_state[0]=1;
    pwm_flag[1]=0x01;
  }
  else
  {
    power1_flag=0;
    key_led_state[0]=0;
  }
  if(rs485InitStruct.led_state[1]==0)
  {
    power2_flag=0;
    key_led_state[1]=0;
    pwm_flag[4]=0x02;
  }
  else if(rs485InitStruct.led_state[1]==1)
  {
    power2_flag=1;
    key_led_state[1]=1;
    pwm_flag[4]=0x01;
  }
  else
  {
    power2_flag=0;
    key_led_state[1]=0;
  }
}
/*****************************************************************************************************
程序名：Broadcast_control_LED
功能：根据广播数据控制LED灯。
参数：无参数。
返回值：无返回值
*****************************************************************************************************/
void Broadcast_control_LED(void)
{   
  
  switch(ReceiveRs485Agreement[2])
  {
  case TOTAL_SW_CMD:                           
    switch(ReceiveRs485Agreement[4])
    {
    case LED_ON:          
      Always_LED_control(1);
      return;
      break;
    case LED_OFF:
      Always_LED_control(0);
      return;
      break;
    default:
      break;
    }
    break;
  case GENERAL_OPEN_CMD:                        
    Always_LED_control(1);
    break;
  case GENERAL_CLOSE_COM:                       
    Always_LED_control(0);
    break;
  default:
    break; 
  }
}
/*****************************************************************************************************
程序名：Always_open_control
功能：总开模式控制LED灯。
参数：参数1：打开或者关闭。
返回值：无返回值
*****************************************************************************************************/
void Always_LED_control(uint8_t mode)
{
  if(mode==1)                           
  {
    if(power1_flag==0)
    {
      power1_flag=1;
      key_led_state[0]=1;
      pwm_flag[1]=0x01;
      if(Chopping1_channel_cont==STEP)
      {
        Chopping1_channel_cont=13;
      }
      Gpio_SetIO(CH3_PORT,CH3_PIN);
    }
  }
  else if(mode==0)                       
  {
    power1_flag=0;
    Gpio_ClrIO(CH3_PORT,CH3_PIN);
    key_led_state[0]=0;
    pwm_flag[1]=0x01;
  }
  else
  {
    
  }
  /*************************************************************************************************/
  if(mode==1)                           
  {
    if(power2_flag==0)
    {
      power2_flag=1;
      key_led_state[1]=1;
      pwm_flag[4]=0x01;
      if(Chopping2_channel_cont==STEP)
      {
        Chopping2_channel_cont=13;
      }
      Gpio_SetIO(CH4_PORT,CH4_PIN);
    }
  }
  else if(mode==0)                       
  {
    power2_flag=0;
    Gpio_ClrIO(CH4_PORT,CH4_PIN);
    key_led_state[1]=0;
    pwm_flag[4]=0x01;
  }
  else
  {
    
  }
}
/*****************************************************************************************************
程序名：Common_pattern_control
功能：普通模式数据处理：接收的数据与按键配置做对比，对比成功后再对比开关属性，如果是开就打开灯，反之关闭灯。
参数：参数1：传入对比数据。 
返回值：无返回值
*****************************************************************************************************/
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
/*****************************************************************************************************
程序名：Regional_model_control
功能：区域模式数据处理：接收的数据与按键配置做对比，对比成功后再对比开关属性，如果是开就打开灯，反之关闭灯。
参数：无参数。
返回值：无返回值
*****************************************************************************************************/
void Regional_model_control(void)
{
  switch(ReceiveRs485Agreement[2])
  {
  case TOTAL_SW_CMD:                              
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
/*****************************************************************************************************
程序名：Regional_Close_opening
功能：区域模式下的总开关命令处理。属于该模式的灯根据模式按键状态打开与关闭，跨模式的也根据模式开关
	          打开与关闭，不属于任何模式的按键无影响，不属于该模式且具备模式属性的灯全部关闭。
参数：参数1:开关状态。 参数2：数据buff.
返回值：无返回值
*****************************************************************************************************/
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
        //        Linkage_open_close(i+1,0);
      }
    }
  }
}
/*****************************************************************************************************
程序名：Power_State_Config
功能：上电后LED灯状态配置，确定是否是区段按键是的话根据直接亮灯，反之根据EEPROM确定灯状态。
参数：无参数。
返回值：无返回值
*****************************************************************************************************/
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
/*****************************************************************************************************
程序名：Linkage_open_close
功能：联动开关灯
参数：参数1：指哪路按键。参数2：开关状态。
返回值：无返回值
*****************************************************************************************************/
void Linkage_open_close(uint8_t key,uint8_t state) 
{
  uint8_t mode;
  mode=state;
  if(key==1)
  {
    if(mode==1)                           
    {
      if(power1_flag==0)
      {
        power1_flag=1;
        key_led_state[0]=1;
        pwm_flag[1]=0x01;
        if(Chopping1_channel_cont==STEP)
        {
          Chopping1_channel_cont=13;
        }
        Gpio_SetIO(CH3_PORT,CH3_PIN);
      }
    }
    else if(mode==0)                       
    {
      power1_flag=0;
      Gpio_ClrIO(CH3_PORT,CH3_PIN);
      key_led_state[0]=0;
      pwm_flag[1]=0x01;
    }
    else
    {
      
    }
  }
  
  /*************************************************************************************************/
  if(key==2)
  {
    if(mode==1)                           
    {
      if(power2_flag==0)
      {
        power2_flag=1;
        key_led_state[1]=1;
        pwm_flag[4]=0x01;
        if(Chopping2_channel_cont==STEP)
        {
          Chopping2_channel_cont=13;
        }
        Gpio_SetIO(CH4_PORT,CH4_PIN);
      }
    }
    else if(mode==0)                       
    {
      power2_flag=0;
      Gpio_ClrIO(CH4_PORT,CH4_PIN);
      key_led_state[1]=0;
      pwm_flag[4]=0x01;
    }
    else
    {
      
    }
  }
}
/*****************************************************************************************************
程序名：Second_control
功能：处理第二属性处理函数。返回值为真时代表广播与第二属性相等，反知相反。
参数：参数1:传入按键代号1-3。参数2：数据buff。
返回值：返回值：真与假。
*****************************************************************************************************/
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
/*****************************************************************************************************
程序名：RX485_Prepare_receive
功能：rx485接收数据准备。
参数：无参数
返回值：无返回值
*****************************************************************************************************/
void RX485_Prepare_receive(void)
{
  ReceiveFlag=0;  
  uart_flag=0;    
  Uart_EnableFunc(UARTCH1,UartRx);         //为下次接收数据做准备。
}
/*****************************************************************************************************
程序名：Public_Multi_interlocking
功能：共用多互锁处理函数。
参数：参数1：数据buff。
返回值：无返回值
*****************************************************************************************************/
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
/*****************************************************************************************************
程序名：clear_Disturb
功能：清理勿扰处理
参数：参数1：工作状态，参数2：按键值。
返回值：无返回值
*****************************************************************************************************/
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
/*****************************************************************************************************
程序名：Exclusion_function
功能：排除不受总关控制的功能。
参数：参数1：工作模式开与关，参数2：按键值。
返回值：返回真代表是需要排除的模式，返回假则不是需要排除的模式。
*****************************************************************************************************/
uint8_t Exclusion_function (uint8_t mode,uint8_t key)
{
  uint8_t temp;
  //  switch((uint8_t)(key_config[key]>>8&0xff))
  //  {
  //  case NIGHT_LAMP_CMD:                                  
  //    led_openstate0(mode,key);
  //    temp=TRUE;
  //    break;
  //  case POINT_PRESS_CMD:                                
  //    break;
  //  case CLEAR_DISTURB_CMD:                             
  //    clear_Disturb(mode,key);
  //    temp=TRUE;
  //    break;
  //  case GENERAL_OPEN_CMD:                                
  //    temp=TRUE;
  //    break;
  //  case GENERAL_CLOSE_COM:                               
  //    temp=TRUE;
  //    break;
  //  case SPECIAL_SERVICE_CMD:                            
  //    temp=TRUE;
  //    break;
  //  case INTER_LOCK_CMD:                                  
  //    temp=TRUE;
  //    break;
  //    
  //    //  case WINDOW_CURTAINS_CMD:     //窗帘。
  //    //    led_closestate0(mode,key);
  //    //    temp=TRUE;
  //  default:
  temp=FALSE;
  //    break; 
  //  }
  return temp;
}
/*****************************************************************************************************
程序名：led_openstate0
功能：状态为0，LED灯开。
参数：参数1：开关状态，参数2：按键值。
返回值：无返回值
*****************************************************************************************************/
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
/*****************************************************************************************************
程序名：led_closestate0
功能：状态为0，LED灯关。
参数：参数1：开关状态，参数2：按键值。
返回值：无返回值
*****************************************************************************************************/
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
/*****************************************************************************************************
程序名：led_closestate0
功能：状态为0，LED灯关。
参数：参数1：开关状态，参数2：按键值。
返回值：无返回值
*****************************************************************************************************/
void eeprom_erase_all(void)
{
  __disable_irq();
  Flash_SectorErase(AGREEMENT_ADDR);
  Flash_SectorErase(DEFSTATE_ADDR);
  Flash_SectorErase(LED_SATTE_ADDR);
  Flash_SectorErase(KEY_PARTITION_ADDR );
  __enable_irq();
}
/*******************************************END-RS485_Agreement.C************************************************/