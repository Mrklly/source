#include <string.h>
#include "stm8s.h"
#include "stm8s_conf.h"
#include "bsp_uart.h"
#include "bsp_e2p.h"
#include "bsp_timer.h"
#include "Third_agreement.h"
#include "bsp_wwdg.h"
#include "bsp_pwm.h"
#include "key_handler.h"


void assert_failed(uint8_t* file, uint32_t line)
{
  while(1)
  {
  
  }
}


int main(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);        //ϵͳʱ������16M��

    WWDG_Configuration();                                 //���Ź���ʼ����ÿ49.152ms��Ҫι�������߸�λ��
    WWDGReset();                                          //���Ź���λ��

    TIMER2_Init();                                        //��ʱ1����ʼ�����ϵ�Ϊ�ء�
    RS485_Init();                                         //���ڳ�ʼ����
    asm("rim");                                           //ʹ��ȫ���жϡ�
    flash_Init();

    protocol_param_get();

    time2_xms(1);
    Relay_Init();                                         //�̵�����ʼ����
    //  LED_Init();                                           //LED�Ƴ�ʼ�����ϵ�Ĭ��Ϊ��
    KEY_Init();                                           //������ʼ����
    PWM_Init();
    UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);              //�������ڽ���ʹ�ܡ�


    uint8_t buf_key_value[KEY_COUNT], buf_key_status[KEY_COUNT];
    memset(buf_key_value,  0, KEY_COUNT);
    memset(buf_key_status, 0, KEY_COUNT);


    while(1) { 
        Refresh_WWDG_Window();
        key_value_pack(buf_key_value, buf_key_status);
        protocol_send_key_status(buf_key_value, buf_key_status);
        protocol_run();
    }
}

