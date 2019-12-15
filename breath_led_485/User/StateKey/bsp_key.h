#ifndef _BSP_KYE_H_
#define _BSP_KEY_H_


#include "stm8s.h"
#include "bsp_gpio.h"


#define KEY1_PIN        GPIO_PIN_5      //PE5_����1
#define KEY2_PIN        GPIO_PIN_5      //PC5_����2
#define KEY3_PIN        GPIO_PIN_6      //PB6_����3
#define KEY4_PIN        GPIO_PIN_7      //PB7_����4

#define KEY1_PORT       GPIOE
#define KEY2_PORT       GPIOC
#define KEY3_PORT       GPIOB
#define KEY4_PORT       GPIOB

#define KEY_COUNT       4

#define NO_KEY                  0x0
#define KEY1_VALUE              0x01
#define KEY2_VALUE              0x02
#define KEY3_VALUE              0x04
#define KEY4_VALUE              0x08

#define KEY_VALUE(i)    (1 << i)

#define STATE_KEY_UP            0               //��ʼ״̬,δ����
#define STATE_KEY_DOWM          1               //��������
#define STATE_KEY_SHORT         2               //������ʱ̬
#define STATE_KEY_LONG          3               //����
#define STATE_KEY_LONG_UP       4               //����̧��

#define KEY_SHORT               0x10            //�̰�״̬��
#define KEY_SHORT_UP            0x20            //�̰�̧��
#define KEY_LONG                0x40            //����״̬��
#define KEY_LONG_UP             0x80            //����̧��


typedef struct _key_io_t_ {
    GPIO_TypeDef *key_port;
    uint8_t      key_pin;
} key_io_t;


#define KEY_CHECK(KEY_PORT, KEY_PIN) ((KEY_PORT->IDR) & KEY_PIN)


void KEY_Init(void);
uint8_t Key_Scan(void);
void KEY_value_Loading_Data(uint8_t KeyVelue,uint8_t state);
uint8_t Key_value_detection (void);


#endif

