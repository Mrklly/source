#include <string.h>
#include "bsp_key.h"
#include "bsp_timer.h"
#include "bsp_wwdg.h"
#include "global_variable.h"


const key_io_t key_define[4] = {
    {KEY1_PORT, KEY1_PIN},
    {KEY2_PORT, KEY2_PIN},
    {KEY3_PORT, KEY3_PIN},
    {KEY4_PORT, KEY4_PIN}
};



/*****************************************************************************************************
*��������KEY_Init
***���ܣ�����4��������������������������ģʽ��
***�������޲�����
*����ֵ���޷���ֵ��
***���ߣ�������
***ʱ�䣺2018��4��21�š�
*****************************************************************************************************/
void KEY_Init(void)
{
  GPIO_Init(KEY1_PORT,KEY1_PIN, GPIO_MODE_IN_PU_NO_IT);//�⺯��������Ϊ���ж��������롣
  GPIO_Init(KEY2_PORT,KEY2_PIN, GPIO_MODE_IN_PU_NO_IT);//�⺯��������Ϊ���ж��������롣
  GPIO_Init(KEY3_PORT,KEY3_PIN, GPIO_MODE_IN_PU_NO_IT);//�⺯��������Ϊ���ж��������롣
  GPIO_Init(KEY4_PORT,KEY4_PIN, GPIO_MODE_IN_PU_NO_IT);//�⺯��������Ϊ���ж��������롣
}
/*****************************************************************************************************
*��������scan_key
***���ܣ�IO��ɨ�躯����ɨ��4������IO��״̬������һ��IO״̬��
***�������޲�����
*����ֵ�����ؼ�ֵ��
***���ߣ�������
***ʱ�䣺2018��8��21�š�
*****************************************************************************************************/
uint8_t Key_Scan(void)
{
  volatile uint8_t KeyTemp=0x00;                               //��������ʱ����ֵ��
  if(GPIO_ReadInputPin(KEY1_PORT,KEY1_PIN)==0)        //Key����1
  { 
    KeyTemp=KEY1_VALUE;

  } else if(GPIO_ReadInputPin(KEY2_PORT,KEY2_PIN)==0)   //KEY����2
  {
    KeyTemp=KEY2_VALUE;                            
  } else if(GPIO_ReadInputPin(KEY3_PORT,KEY3_PIN)==0)  //KEY����3
  {
    KeyTemp=KEY3_VALUE;
  } else if(GPIO_ReadInputPin(KEY4_PORT,KEY4_PIN)==0)  //KEY����4
  {
    KeyTemp=KEY4_VALUE;
  } else {
    KeyTemp=NO_KEY;
  }
  return KeyTemp;
}

uint8_t Key_value_detection (void)
{// key_Result[4]
    static uint8_t state[4] = {0, 0, 0, 0};                       //������ʼ������״̬����־��                           
    static uint8_t key_last[4] = {0, 0, 0, 0};                    //��һ�ΰ�����¼��                        
    static uint8_t key_now[4] = {0, 0, 0, 0};                     //���ڰ�����¼�� 
    static uint8_t time_cont[4] = {0, 0, 0, 0};                      //������ʱ��
    uint8_t key_value[4] = {0, 0, 0, 0};                          //��ֵ��

//    key_now = Key_Scan();                           //��ȡ����ֵ��

    uint8_t i = 0;

    for(; i<4; i++) {

        if(KEY_CHECK(key_define[i].key_port, key_define[i].key_pin) == 0) {
            key_now[i] = 1 << i;
        } else {
            key_now[i] = 0;
        }

        switch(state[i])  {

        case STATE_KEY_UP:                            //��ʼ������״̬��
            if(key_now[i] != key_last[i]) {                     //�жϱ��ΰ����Ƿ�����һ�ΰ�����ȡ�
                state[i] = 1;                                //ת��������ȥ��״̬��
            }
            key_value[i] = 0;
            time_cont[i] = 0;
            break;
        case STATE_KEY_DOWM:                          //����ȥ����
            if(key_now[i] == key_last[i]) {
            
                state[i] = 2;                                //ת���������̰�״̬��
                key_value[i] = key_last[i]|KEY_SHORT;

            } else {
                state[i] = 0;                                //�Ƕ�����
                key_value[i] = 0;
            }
            time_cont[i] = 0;
            break;
        case STATE_KEY_SHORT:                         //�����̰�״̬��
            if(key_now[i] == key_last[i]) {                     //�а������¡�
                state[i] = 3;                                //ת������������״̬��
            } else {                                      //�̰�������
                state[i] = 0;
    //           key_value[i] = key_last[i]|KEY_SHORT;         //��ֵ�̰���
            }
            key_value[i] = 0;
            time_cont[i] = 0;
            break;
        case STATE_KEY_LONG:                         //����״̬��
            if(key_now[i] == key_last[i]) {                    //��ʼ��ʱ��
                if(time_cont[i]++ > 75) {                   //1.5������Ϊ������
                    time_cont[i] = 0;
                    state[i] = 4;                             //����ת��Ϊ�ȴ������ͷ�״̬��
                    key_value[i] = key_last[i] | KEY_LONG;       //��ֵ�ǳ�����
                }
            } else {
                state[i] = 0;
    //           key_value[i] = key_last[i]|KEY_SHORT;        //��ֵ�޸�Ϊ�̰���
            }
            break;
        case STATE_KEY_LONG_UP:                      //�������ּ��
            if(key_now[i] != key_last[i]) {                 //�����ͷ�
                key_value[i] = key_last[i] | KEY_LONG_UP;      //��ֵ�޸�Ϊ����̧��
                if(key_now[i] == NO_KEY) {                   //��������̧��
                    state[i] = 0;
                } else {                                  //��İ����������̧��
                    state[i] = 1;
                }
            }
            time_cont[i] = 0;
            break;
            default:
            key_value[i] = 0;
            key_last[i] = 0;
            key_now[i] = 0;
            break;
        }

        key_last[i] = key_now[i];                          //���¼�¼��һ�ΰ�����

    }

//    return key_value;                           //���ؼ�ֵ��

    memcpy(key_Result, key_value, 4);
    
    return 0;
}

