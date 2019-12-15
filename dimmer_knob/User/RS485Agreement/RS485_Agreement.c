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
��������RS485_Agreement_Config
���ܣ�RS485Э�����ú�����
�������޲�����
����ֵ���޷���ֵ��
*****************************************************************************************************/
void RS485_Agreement_Config(void)
{
  if(ReceiveFlag==1)                                                            //�д������ݡ�
  {
    time2_xms(15);                                                              //��ʱ15MS�ȴ����ڽ���������ɡ�
    Uart_DisableFunc(UARTCH1,UartRx);                                           //�رմ��ڽ���
    if((ReceiveRs485Agreement[0]==0xfe)&&(uart_flag>=ReceiveRs485Agreement[2])) //����ģʽ
    {  
      if((RS485_Parse_check(ReceiveRs485Agreement[2],ReceiveRs485Agreement)==ReceiveRs485Agreement[ReceiveRs485Agreement[2]+1])&&(ReceiveRs485Agreement[1]==0x55))
      {
        switch(ReceiveRs485Agreement[3])                                   //�ж��������
        {
        case PANEL_ADDRESS:                                                //�޸�����ַ��
          RS485_config_writeEeprom(PANEL_ADDR,4,6);                        //д������ַ��
          break;
        case PANEL_LED_STATE:                                              //�������LED�Ƴ�ʼ״̬         
          RS485_config_writeEeprom(LED_SATTE_ADDR,4,8);                    //��LEDд��ʼ״̬��
          break;
        case KEY_AREA_PROPERTIES:                                          //���ð����������ԡ�
          RS485_config_writeEeprom(KEY_PARTITION_ADDR,4,12);               //д���������ԡ�
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
	   flash_read_buf(AGREEMENT_ADDR, ReceiveRs485Agreement,24);                        //���������������á�ʵ�ʽ�������20�������ֽ�����
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
��������RS485_Parse_check
���ܣ��������У���롣
����������1:���ݳ��ȡ�����2�������������ݡ�
����ֵ������У���롣
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
��������RS485_config_writeEeprom
���ܣ���������Ϣд�뵽eeprom
����������1��EEPROM��ַ������2��д�����ʼ�㣬д�볤�ȡ�
����ֵ��2�ֽ�Э��ͷ+4�ֽ�LED��״̬+8�ֽڰ�������+4�ֽڰ�����������+4�ֽڿ������ԡ�
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
��������Broadcast_data_packing 
���ܣ����㲥���յ����ݴ������ϳ�һ��32λ���ݡ��������ڴ��������Ƚϡ�
����������1������Ҫ��������ݡ�
����ֵ���޷���ֵ
*****************************************************************************************************/
uint32_t Broadcast_data_packing (uint8_t *buff)
{
  uint32_t temp=0;
  temp=(uint32_t)buff[1]<<16|(uint32_t)buff[2]<<8|(uint32_t)buff[3];
  return temp;
}
/*****************************************************************************************************
��������EEPROM_data_packing 
���ܣ���EEPROM�ڲ��������ݴ����4���������á�
�������޲�����
����ֵ���޷���ֵ
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
��������Broadcast_control_LED
���ܣ����ݹ㲥���ݿ���LED�ơ�
�������޲�����
����ֵ���޷���ֵ
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
��������Always_open_control
���ܣ��ܿ�ģʽ����LED�ơ�
����������1���򿪻��߹رա�
����ֵ���޷���ֵ
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
��������Common_pattern_control
���ܣ���ͨģʽ���ݴ������յ������밴���������Աȣ��Աȳɹ����ٶԱȿ������ԣ�����ǿ��ʹ򿪵ƣ���֮�رյơ�
����������1������Ա����ݡ� 
����ֵ���޷���ֵ
*****************************************************************************************************/
void Common_pattern_control(uint8_t *buff)
{
  uint8_t i;
  uint32_t temp=0;
  for(i=0;i<4;i++)
  {
    if((Broadcast_data_packing (buff)==key_config[i]))          //�������롣
    {
      switch(buff[4])                                           //�ȶԿ������ԡ�
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
    else if(Second_control(i,buff))                             //�жϵڶ����ԡ�
    {
      temp=(uint32_t)cont<<16|(key_config[i]&0x00ffff);         //���ɵڶ����Լ�ֵ��
      if(Broadcast_data_packing(buff)==temp)                    //�ԱȰ����ڶ������Ƿ���ȫ��ȡ�
      {
        led_closestate0(buff[4],i);  
        cont=0;
      }     
    }
  }
}
/*****************************************************************************************************
��������Regional_model_control
���ܣ�����ģʽ���ݴ������յ������밴���������Աȣ��Աȳɹ����ٶԱȿ������ԣ�����ǿ��ʹ򿪵ƣ���֮�رյơ�
�������޲�����
����ֵ���޷���ֵ
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
  case GENERAL_OPEN_CMD:                                //���յ��ܿ������
    Regional_Close_opening(1,ReceiveRs485Agreement);
    return;
    break;
  case GENERAL_CLOSE_COM:                               //���յ��ܹ������
    Regional_Close_opening(0,ReceiveRs485Agreement);
    return;
    break;
  case MULTI_INTERLOCKING_CMD:                  //�໥�����ܹ�16�顣ÿ��16��������   OK
    Public_Multi_interlocking(ReceiveRs485Agreement); 
    return;
    break;
  case DOUBLE_INTERLOCKING_CMD:                 //˫�ػ�������ܹ�16�顣ÿ��16��������  ok 
    Public_Multi_interlocking(ReceiveRs485Agreement); 
    return;
    break;
  case CLEAR_DISTURB_CMD:                       //������������ܹ�16�顣ÿ��2��������  ok
    Public_Multi_interlocking(ReceiveRs485Agreement);
    return;
    break;
  default:
    break;
  }
  if(ReceiveRs485Agreement[1]>0)                //������ͨģʽ��
  {
    Common_pattern_control(ReceiveRs485Agreement);
  }
}
/*****************************************************************************************************
��������Regional_Close_opening
���ܣ�����ģʽ�µ��ܿ�����������ڸ�ģʽ�ĵƸ���ģʽ����״̬����رգ���ģʽ��Ҳ����ģʽ����
	          ����رգ��������κ�ģʽ�İ�����Ӱ�죬�����ڸ�ģʽ�Ҿ߱�ģʽ���Եĵ�ȫ���رա�
����������1:����״̬�� ����2������buff.
����ֵ���޷���ֵ
*****************************************************************************************************/
void Regional_Close_opening(uint8_t state,uint8_t *buff)
{
  uint8_t i;
  for(i=0;i<4;i++)
  {
    if((rs485InitStruct.key_partition[i]>0)||(rs485InitStruct.Transboundary_zoning[i]>0))//������������յ��������밴���޹ء�
    {
      if(buff[1]==rs485InitStruct.key_partition[i])     //��һģʽ�����Ƿ���ȡ�
      {  
        while(!Exclusion_function (state,i))            //�ų�ҹ�ƣ��㶯���������š�
        {
          led_closestate0(state,i);                     //����LED�ơ�
          break;                                       //����while,ִ����һ��FOR.
        } 
      }
      else if(Second_control(i,buff))                  //�жϵڶ����ԡ�
      {
        while(!Exclusion_function (state,i))           //�ų�ҹ�ƣ��㶯���������š�
        {
          led_closestate0(state,i);                    //����LED�ơ�
          break;                                       //����while,ִ����һ��FOR.
        } 
      }
      else if(buff[2]==GENERAL_CLOSE_COM)             //�����Ƿ��ܹء�      
      {
        
      }
      else if((buff[2]==TOTAL_SW_CMD)&&(state==0))
      {
        
      }
      else                                            //ģʽ���ԺͿ�ģʽ���Զ�����ȣ��Ҿ߱�ģʽ���ԡ���ֱ�ӹرյơ�
      {
        //        Linkage_open_close(i+1,0);
      }
    }
  }
}
/*****************************************************************************************************
��������Power_State_Config
���ܣ��ϵ��LED��״̬���ã�ȷ���Ƿ������ΰ����ǵĻ�����ֱ�����ƣ���֮����EEPROMȷ����״̬��
�������޲�����
����ֵ���޷���ֵ
*****************************************************************************************************/
void Power_State_Config(void)
{
  uint8_t i, temp[4]={0x00,0x02,0x04,0x06};
  for(i=0;i<4;i++)
  {
    if((rs485InitStruct.key_function[temp[i]]>=0x51)&&(rs485InitStruct.key_function[temp[i]]<=0x71))//�ڱ�������ֱ���ϵ����ơ�
    {
      pwm_flag[i]=0x01;
      Linkage_open_close(i+1,1);        //��LED�ơ�
    }
    else if(key_led_state[i]==0)        //������������У��͸���������Ϣȷ���ϵ�״̬��
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
��������Linkage_open_close
���ܣ��������ص�
����������1��ָ��·����������2������״̬��
����ֵ���޷���ֵ
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
��������Second_control
���ܣ�����ڶ����Դ�����������ֵΪ��ʱ����㲥��ڶ�������ȣ���֪�෴��
����������1:���밴������1-3������2������buff��
����ֵ������ֵ������١�
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
��������RX485_Prepare_receive
���ܣ�rx485��������׼����
�������޲���
����ֵ���޷���ֵ
*****************************************************************************************************/
void RX485_Prepare_receive(void)
{
  ReceiveFlag=0;  
  uart_flag=0;    
  Uart_EnableFunc(UARTCH1,UartRx);         //Ϊ�´ν���������׼����
}
/*****************************************************************************************************
��������Public_Multi_interlocking
���ܣ����ö໥����������
����������1������buff��
����ֵ���޷���ֵ
*****************************************************************************************************/
void Public_Multi_interlocking(uint8_t *buff)
{
  uint8_t i, temp[4]={0x00,0x02,0x04,0x06};
  uint8_t temp2[4]={0x01,0x03,0x05,0x07};
  uint32_t temp3;
  for(i=0;i<4;i++)
  {
    if((Broadcast_data_packing (buff)==key_config[i]))    //�յ��Ķ�ػ����㲥�����Լ���ȫ��ȣ����ڶ�ع��ܣ���
    {
      switch(buff[4])                                     //�ȶԿ������ԡ�
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
    else if((rs485InitStruct.key_function[temp[i]]==buff[2])&&(rs485InitStruct.key_partition[i]==buff[1]))  //�жϰ����Ƿ���09�������ԡ�
    {
      if((rs485InitStruct.key_function[temp2[i]]&0xf0)==(buff[3]&0xf0))  //�ж����Ƿ���ȣ���Ⱦ���Ҫ������
      {
        switch(buff[4])                                                 //�ȶԿ������ԡ�
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
    else if(Second_control(i,buff))                             //�жϵڶ����ԡ�
    {
      temp3=(uint32_t)cont<<16|(key_config[i]&0x00ffff);        //���ɵڶ����Լ�ֵ��
      if(Broadcast_data_packing(buff)==temp3)                   //�ԱȰ����ڶ������Ƿ���ȫ��ȡ�
      {
        led_closestate0(buff[4],i);  
        cont=0;
      }
      else if((cont==buff[1])&&(rs485InitStruct.key_function[temp[i]]==buff[2]))
      {
        if((rs485InitStruct.key_function[temp2[i]]&0xf0)==(buff[3]&0xf0))//�ж����Ƿ���ȣ���Ⱦ���Ҫ������
        {
          switch(buff[4])                                                //�ȶԿ������ԡ�
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
��������clear_Disturb
���ܣ��������Ŵ���
����������1������״̬������2������ֵ��
����ֵ���޷���ֵ
*****************************************************************************************************/
void clear_Disturb(uint8_t mode,uint8_t key)
{
  switch((uint8_t)(key_config[key]&0x0F))
  {       
  case 0x01:                        //�����ܡ�
    Linkage_open_close(key+1,0);  
    break;
  case 0x02:                        //���Ź��ܡ�
    led_openstate0(mode,key); 
    break;
  default:
    break;          
  } 
}
/*****************************************************************************************************
��������Exclusion_function
���ܣ��ų������ܹؿ��ƵĹ��ܡ�
����������1������ģʽ����أ�����2������ֵ��
����ֵ���������������Ҫ�ų���ģʽ�����ؼ�������Ҫ�ų���ģʽ��
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
  //    //  case WINDOW_CURTAINS_CMD:     //������
  //    //    led_closestate0(mode,key);
  //    //    temp=TRUE;
  //  default:
  temp=FALSE;
  //    break; 
  //  }
  return temp;
}
/*****************************************************************************************************
��������led_openstate0
���ܣ�״̬Ϊ0��LED�ƿ���
����������1������״̬������2������ֵ��
����ֵ���޷���ֵ
*****************************************************************************************************/
void led_openstate0(uint8_t state,uint8_t key)
{
  if(state==0)                                                 //������״̬ȷ���Ƿ��LED��
  {
    Linkage_open_close(key+1,1);            
  }
  else
  {
    Linkage_open_close(key+1,0);                                //��֮�ر�ҹ�ơ�
  }
}
/*****************************************************************************************************
��������led_closestate0
���ܣ�״̬Ϊ0��LED�ƹء�
����������1������״̬������2������ֵ��
����ֵ���޷���ֵ
*****************************************************************************************************/
void led_closestate0(uint8_t state,uint8_t key)
{
  if(state==0)                                                 //������״̬ȷ���Ƿ��LED��
  {
    Linkage_open_close(key+1,0);            
  }
  else
  {
    Linkage_open_close(key+1,1);                               
  }
}
/*****************************************************************************************************
��������led_closestate0
���ܣ�״̬Ϊ0��LED�ƹء�
����������1������״̬������2������ֵ��
����ֵ���޷���ֵ
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