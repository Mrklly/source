#ifndef _BSP_KYE_H
#define _BSP_KEY_H
#include "bsp_it.h"
/*****************************************************************************************************/
/*�������Ŷ���*/
#define KEY1_PIN        GpioPin15     //PA15_����1
#define KEY2_PIN        GpioPin3      //PB3_����2
#define KEY3_PIN        GpioPin4      //PB4_����3
#define KEY4_PIN        GpioPin5      //PB5_����4
#define KEY5_PIN        GpioPin14     //PC14_����5
#define KEY6_PIN        GpioPin15     //PC15_����6
/*�������ܹ�����*/
#define STEP            25             //�ܹ����ٲ�����Ȼ25���ȼ���
/*�����˿ڶ���*/
#define KEY1_PORT       GpioPortA
#define KEY2_PORT       GpioPortB
#define KEY3_PORT       GpioPortB
#define KEY4_PORT       GpioPortB
#define KEY5_PORT       GpioPortC
#define KEY6_PORT       GpioPortC
/*����ģʽ*/
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
//���尴�����뿪��
#define KEY         1                   //KEY�ܿ���
#define KEY_ON      1                   //�а������¡�
#define KEY_OFF     0                   //�ް������¡�
/**********************************************��������***********************************************/
void KEY_Init(void);
uint8_t Key_Scan(void);
void KEY_LED_Control(void);
void KEY_value_Loading_Data(uint8_t KeyVelue);
void KEY_SEND_Code(uint8_t keyfun,uint8_t keynumber);
void Uploading_data(void);
//void KEY_Modify_LED_state(uint8_t key);
void Local_control(void);
void Interlocking_control(uint8_t *buff,uint8_t key);
void Point_movement(void);
void Dismantling_function(uint8_t function,uint8_t key_number,uint8_t state);
uint8_t Key_value_detection (void);
void KEY_Modify_LED_state(uint8_t key,uint8_t state);
void Ligth_PowerOn(uint8_t number);
void Ligth_PowerOff(uint8_t number);
void Modify_Ligth_state(uint8_t number);
void LigthLED_ON(uint8_t key);
void LigthLED_OFF(uint8_t key);

#endif
