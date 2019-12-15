#include "drv_key.h"


void hal_key_init(void)
{
    Gpio_InitIOExt(PORT_KEY_1, PIN_KEY_1, GpioDirIn, FALSE, TRUE, FALSE, FALSE);
    Gpio_InitIOExt(PORT_KEY_2, PIN_KEY_2, GpioDirIn, FALSE, TRUE, FALSE, FALSE);
    Gpio_InitIOExt(PORT_KEY_3, PIN_KEY_3, GpioDirIn, FALSE, TRUE, FALSE, FALSE);
    Gpio_InitIOExt(PORT_KEY_4, PIN_KEY_4, GpioDirIn, FALSE, TRUE, FALSE, FALSE);
}

uint8_t Key_Scan(void)
{
    uint8_t KeyTemp = 0x00;

    if(KEY_STA_0) {
        KeyTemp = KEY1_VALUE;
        return KeyTemp;
    } else if(KEY_STA_1) {
        KeyTemp = KEY2_VALUE;
        return KeyTemp;
    } else if(KEY_STA_2) {
        KeyTemp = KEY3_VALUE;
        return KeyTemp;
    } else if(KEY_STA_3) {
        KeyTemp = KEY4_VALUE;
        return KeyTemp;
    } else {
        KeyTemp = NO_KEY;
    }

    return KeyTemp;
}

uint8_t Key_value_detection (void)
{
    static uint8_t state = STATE_KEY_UP;                             //������ʼ������״̬����־��
    static uint8_t key_last = 0;                          //��һ�ΰ�����¼��
    static uint8_t key_now = 0;                           //���ڰ�����¼��
    static uint8_t time_keep_cont = 0;                    //�̰�ά�ּ�ʱ��
    static uint8_t time_cont = 0;                         //������ʱ��
    static uint8_t temp = 0;                              //ά�ְ�����ʱ��
    uint8_t key_value = 0;                                //��ֵ��

    key_now = Key_Scan();                                 //��ȡ����ֵ��

    switch(state) {
    case STATE_KEY_UP: {                                  //��ʼ������״̬��
        if(key_now != key_last) {                         //�жϱ��ΰ����Ƿ�����һ�ΰ�����ȡ�
            state = STATE_KEY_DOWM;                       //ת��������ȥ��״̬��
        }
    }
    break;

    case STATE_KEY_DOWM: {                                //����ȥ����
        if(key_now == key_last) {
            state = STATE_KEY_SHORT;                      //ת���������̰�״̬��
        } else {
            state = STATE_KEY_UP;                         //�Ƕ�����
        }
    }
    break;

    case STATE_KEY_SHORT: {                               //�����̰�״̬��

        if(key_now == key_last) {                         //�а������¡�
            state = STATE_KEY_KEEP;                                      //ת���������̰�ά��״̬��
            key_value = key_last | KEY_SHORT;               //��ֵ�̰���
        } else {                                          //�̰�������
            state = STATE_KEY_UP;
            //key_value = key_last|KEY_SHORT;                 //��ֵ�̰���
        }
    }
    break;

    case STATE_KEY_KEEP:                                 //�̰�����ά�֡�
        if(key_now == key_last) {
            if(time_keep_cont++ <= 200) {                    //ά�ִ�Լ4s����,��4ms��Χ�������ֵΪ�̰���
                temp++;

                if(temp >= 8) {                                //ά�ְ�����ʱ��ÿ160ms�Ƴ����¶̰�ά�ּ�ֵ��
                    temp = 0;
                    key_value = key_last | KEY_SHORT_KEEP;       //��ֵ�̰�ά�����25�Σ��л�������״̬��
                }
            } else {                                        //�л�Ϊ����״̬��
                time_keep_cont = 0;
                temp = 0;
                state = STATE_KEY_LONG;                                    //ת��Ϊ����״̬��
            }
        } else {                                          //ά��״̬�°����ͷš�
            if(key_now == NO_KEY) {                         //��������̧��
                state = STATE_KEY_UP;
                temp = 0;
                time_keep_cont = 0;
                //key_value = key_last|KEY_SHORT;               //��ֵ�̰���
                key_value = key_last | KEY_SHORT_UP;             //��ֵ�̰�̧��
            } else {                                        //��İ����������̧��
                state = 1;
                temp = 0;
                time_keep_cont = 0;
                //key_value = key_last|KEY_SHORT;               //��ֵ�̰���
                key_value = key_last | KEY_SHORT_UP;      //��ֵ�̰�̧��
            }
        }

        break;

    case STATE_KEY_LONG:                               //����״̬��
        if(key_now == key_last) {                      //������ʼ��ʱ��
            if(time_cont++ > 254) {                      //6������Ϊ������
                time_cont = 0;
                state = STATE_KEY_LONG_UP;                                 //����ת��Ϊ�ȴ������ͷ�״̬��
                key_value = key_last | KEY_LONG;           //��ֵ�ǳ�����
            }
        } else {
            if(key_now == NO_KEY) {                      //��������̧��
                state = 0;
                time_cont = 0;
                //key_value = key_last|KEY_SHORT;            //��ֵ�޸�Ϊ�̰���
                key_value = key_last | KEY_SHORT_UP;          //��ֵ�޸�Ϊ�̰���
            } else {                                     //��İ����������̧��
                state = 1;
                time_cont = 0;
                //key_value = key_last|KEY_SHORT;            //��ֵ�޸�Ϊ�̰���
                key_value = key_last | KEY_SHORT_UP;          //��ֵ�޸�Ϊ�̰���
            }
        }
        break;

    case STATE_KEY_LONG_UP:                            //�������ּ��
        if(key_now != key_last) {                      //�����ͷ�
            key_value = key_last | KEY_LONG_UP;          //��ֵ�޸�Ϊ����̧��

            if(key_now == NO_KEY) {                      //��������̧��
                state = 0;
            } else {                                     //��İ����������̧��
                state = 1;
            }
        } else {
            //key_value = key_last|KEY_LONG;
        }
        break;

    default:
        key_value = 0;
        key_last = 0;
        key_now = 0;
        break;
    }

    key_last = key_now;                                //���¼�¼��һ�ΰ�����
    return key_value;                                  //���ؼ�ֵ��
}


