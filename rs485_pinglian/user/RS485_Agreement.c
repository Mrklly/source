#include "RS485_Agreement.h"
uint8_t ReceiveRs485Agreement[RS485_AGREEMENT_LEN];              //����RS485Э����ջ��档
uint8_t SendRs485Agreement   [RS485_AGREEMENT_LEN];              //����RS485Э�鷢�ͻ��档
extern uint8_t ReceiveFlag;                                     //���ձ�־��1�����д�������,0���������ݡ�
extern uint8_t uart_flag;                                       //���ռ�����
extern uint8_t pwm_flag[4];
uint32_t key_config[4];                                         //�������á�
uint8_t key_led_state[4]={0,0,0,0};                             //LED��ʱ״̬��
rs458_InitTypedef rs485InitStruct;                              //��ʼ���ṹ�塣
uint8_t cont=0;

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
        case SET_KEY_FUNCTION:                                             //���ð������ܡ�
          RS485_config_writeEeprom(KEY_FUNCTION_ADDR,4,12);                //������д�빦�ܡ�
          break;
        case READ_PANEL_STATE:                                             //��λ�������״̬��
          break;
        case RESTORE_FACTORY_SETTING:                                      //�ָ��������á�         
          eeprom_erase_all();
          break;
        default:
          break;   
        }
        RX485_Prepare_receive();                                           //Ϊ�´ν���������׼����                         
      }
      else
      {
        RX485_Prepare_receive();                           
      }
    }
    else if((ReceiveRs485Agreement[0]==0xfa)&&(uart_flag==6)&&(ReceiveRs485Agreement[1]==0))        //��ͨ�㲥��
    {
      if(RS485_Parse_check(4,ReceiveRs485Agreement)==ReceiveRs485Agreement[5])                      //ʶ��У���롣
      {
        Broadcast_control_LED();           //���ݹ㲥����ִ�й��ܡ�
      }
      RX485_Prepare_receive();                               
    }
    else if((ReceiveRs485Agreement[0]==0xfc)&&(uart_flag==6)&&(ReceiveRs485Agreement[1]>0))        //����㲥��
    {
      Regional_model_control();           //����ģʽ���ݴ���
      RX485_Prepare_receive();            //Ϊ�´ν���������׼����                      
    }
    else                                 //���϶����������������ݽ��մ���
    {
      RX485_Prepare_receive();           //Ϊ�´ν���������׼����
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
  flash_read_buf(AGREEMENT_ADDR,config_data,24);                    //���������������á�
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
  flash_read_buf(AGREEMENT_ADDR,ReceiveRs485Agreement,24);                    //���������������á�                      
  rs485InitStruct.SOH=ReceiveRs485Agreement[0];                               //Э��ͷ�������ֽڡ�
  rs485InitStruct.def_sta=ReceiveRs485Agreement[1];                           //�����ֽڡ�
  for(i=2;i<6;i++)
  {
    rs485InitStruct.led_state[i-2]=ReceiveRs485Agreement[i];                  //��LED״̬��ֵ���ṹ�������4���ֽڡ�
  }     
  for(i=6;i<14;i++)
  {
    rs485InitStruct.key_function[i-6]=ReceiveRs485Agreement[i];               //���������ܸ�ֵ���ṹ�塣
  }
  for(i=14;i<18;i++)
  {
    rs485InitStruct.key_partition[i-14]=ReceiveRs485Agreement[i];             //�������Ը�ֵ�ṹ�塣
  }
  for(i=18;i<23;i++)
  {
    rs485InitStruct.Transboundary_zoning[i-18]=ReceiveRs485Agreement[i];      //���������Ը�ֵ��
  }
  rs485InitStruct.panel_add=ReceiveRs485Agreement[23];                        //����ַ
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
  case TOTAL_SW_CMD:                            //���յ��ܿ������� 0K
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
  case GENERAL_OPEN_CMD:                        //���յ��ܿ������ OK
    Always_LED_control(1);
    return;
    break;
  case GENERAL_CLOSE_COM:                       //���յ��ܹ������ OK
    Always_LED_control(0);
    return;
    break;
  case MULTI_INTERLOCKING_CMD:                  //�໥�����ܹ�16�顣ÿ��16��������   ok  
    Public_Multi_interlocking(ReceiveRs485Agreement); 
    return;
    break;
  case DOUBLE_INTERLOCKING_CMD:                 //˫�ػ�������ܹ�16�顣ÿ��16�������� ok   
    Public_Multi_interlocking(ReceiveRs485Agreement); 
    return;
    break;
  case CLEAR_DISTURB_CMD:                       //������������ܹ�16�顣ÿ��2��������ok 
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
    while(!Exclusion_function (mode,i))     //�ų�ҹ�ƣ��㶯���������ţ��������أ��������ء�
    {
      led_closestate0(mode,i);              
      break;                                //����while,ִ����һ��FOR��
    } 
  }  
}

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

void Regional_model_control(void)
{
  switch(ReceiveRs485Agreement[2])
  {
  case TOTAL_SW_CMD:                               //�ܿ������0x02��OK
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

void Linkage_open_close(uint8_t key,uint8_t state) 
{
  switch(key)
  {
  case 1:
    LED_open_Close_Config(0,state);  //LED1���0
    Relay_open_Close_Config(RELAY_CH1_PORT,RELAY_CH1_PIN,state);  //�̵���ͨ��1���õ͵�ƽ��
    break;
  case 2:
    LED_open_Close_Config(1,state);  //LED2���0
    Relay_open_Close_Config(RELAY_CH2_PORT,RELAY_CH2_PIN,state);  //�̵���ͨ��2���õ͵�ƽ��
    break;
  case 3:
    LED_open_Close_Config(2,state);  //LED3���0
    Relay_open_Close_Config(RELAY_CH3_PORT,RELAY_CH3_PIN,state);  //�̵���ͨ��3���õ͵�ƽ��
    break;
  case 4:
    LED_open_Close_Config(3,state);  //LED4���0
    Relay_open_Close_Config(RELAY_CH4_PORT,RELAY_CH4_PIN,state);  //�̵���ͨ��4���õ͵�ƽ��
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
  Uart_EnableFunc(UARTCH1,UartRx);         //Ϊ�´ν���������׼����
}

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

uint8_t Exclusion_function (uint8_t mode,uint8_t key)
{
  uint8_t temp;
  switch((uint8_t)(key_config[key]>>8&0xff))
  {
  case NIGHT_LAMP_CMD:                                  //ҹ��    OK
    led_openstate0(mode,key);
    temp=TRUE;
    break;
  case POINT_PRESS_CMD:                                 //�㶯����  OK
    temp=TRUE;
    break;
  case CLEAR_DISTURB_CMD:                               //�������š�OK
    clear_Disturb(mode,key);
    temp=TRUE;
    break;
  case GENERAL_OPEN_CMD:                                //�ܿ� OK
    temp=TRUE;
    break;
  case GENERAL_CLOSE_COM:                               //�ܹ�  OK
    temp=TRUE;
    break;
  case SPECIAL_SERVICE_CMD:                             //��������  OK
    temp=TRUE;
    break;
  case INTER_LOCK_CMD:                                  //��������  OK
    temp=TRUE;
    break;
    
    //  case WINDOW_CURTAINS_CMD:     //������
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
  if(state==0)                                                 //������״̬ȷ���Ƿ��LED��
  {
    Linkage_open_close(key+1,1);            
  }
  else
  {
    Linkage_open_close(key+1,0);                                //��֮�ر�ҹ�ơ�
  }
}

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

void eeprom_erase_all(void)
{
    __disable_irq();
    Flash_SectorErase(AGREEMENT_ADDR);
    Flash_SectorErase(DEFSTATE_ADDR);
    Flash_SectorErase(LED_SATTE_ADDR);
    Flash_SectorErase(KEY_PARTITION_ADDR );
    __enable_irq();
}
