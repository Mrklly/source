#ifndef _BSP_KYE_H
#define _BSP_KEY_H
#include "stm8s.h"
#include "stm8s_clk.h"
#include "bsp_gpio.h"
#include "checksum.h"
/*****************************************************************************************************/
/*�������Ŷ���*/
#define KEY1_PIN        GPIO_PIN_5
#define KEY2_PIN        GPIO_PIN_5
#define KEY3_PIN        GPIO_PIN_6
#define KEY4_PIN        GPIO_PIN_7
/*�����˿ڶ���*/
#define KEY1_PORT       GPIOE
#define KEY2_PORT       GPIOC
#define KEY3_PORT       GPIOB
#define KEY4_PORT       GPIOB
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
//���尴�����뿪��
#define KEY         1                   //KEY�ܿ���
#define KEY_ON      1                   //�а������¡�
#define KEY_OFF     0                   //�ް������¡�
typedef struct _com_io_t_{
	GPIO_TypeDef 		*key_port;//�����˿�
	GPIO_Pin_TypeDef	 key_pin;//�������š�
        uint8_t                  key_value;
}port_io;
/**********************************************��������***********************************************/
void KEY_Init(void);
uint8_t Key_Scan(void);
void KEY_LED_Control(void);
void KEY_value_Loading_Data(uint8_t KeyVelue);
#endif
