#ifndef _DRV_KEY_H_
#define _DRV_KEY_H_


#include "hc32_gpio.h"


#define PORT_KEY_1  3
#define PIN_KEY_1   4

#define PORT_KEY_2  1
#define PIN_KEY_2   4

#define PORT_KEY_3  1
#define PIN_KEY_3   5

#define PORT_KEY_4  0
#define PIN_KEY_4   3


#define KEY_STA_0   M0P_GPIO->P3IN_f.P34
#define KEY_STA_1   M0P_GPIO->P1IN_f.P14
#define KEY_STA_2   M0P_GPIO->P1IN_f.P15
#define KEY_STA_3   M0P_GPIO->P0IN_f.P03


#define SHORT_PRESS     5                            //�̰���20ms*5=100ms
#define LONG_PRESS      100                          //������:20ms*100=2000ms
#define REPEAT_PRESS    50                           //������20ms*50=1000ms
/*�����ֵ*/
#define NO_KEY          0x0
#define KEY1_VALUE      0x01
#define KEY2_VALUE      0x02
#define KEY3_VALUE      0x03
#define KEY4_VALUE      0x04
#define KEY5_VALUE      0x05
#define KEY6_VALUE      0x06
/*����״̬��5��״̬*/
#define STATE_KEY_UP            0               //��ʼ״̬,δ����
#define STATE_KEY_DOWM          1               //��������
#define STATE_KEY_SHORT         2               //������ʱ̬
#define STATE_KEY_KEEP          3               //�̰�ά�֡�
#define STATE_KEY_LONG          4               //����
#define STATE_KEY_LONG_UP       5               //����̧��

/*���尴��״̬*/
#define KEY_SHORT               0x10            //�̰�״̬��
#define KEY_SHORT_KEEP          0x20            //�̰�ά�֡�
#define KEY_SHORT_UP            0x30            //�̰�̧��
#define KEY_LONG                0x40            //����״̬��
#define KEY_LONG_UP             0x80            //����̧��


uint8_t Key_Scan(void);
uint8_t Key_value_detection (void);
void hal_key_init(void);


#endif

