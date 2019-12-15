#include "stm8s.h"
#include "bsp_gpio.h"
#include "bsp_key.h"
#include "stm8s_conf.h"
#include "bsp_uart.h"
#include "bsp_timer.h"
#include "bsp_pwm.h"
#include "bsp_wwdg.h"

#include "global_variable.h"
#include "protocol_shanghui.h"
#include "led_dimmer.h"



void assert_failed(uint8_t* file, uint32_t line)
{
  while(1)
  {
  
  }
}

int main(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);        //ϵͳʱ������16M��
    flash_Init();
    dev_param_get();
    LED_Init();                                           //LED�Ƴ�ʼ�����ϵ�Ĭ��Ϊ��
    pwm_init();                                           // tim1
    Relay_Init();
    KEY_Init();                                           //������ʼ����
    CFG->GCR |= CFG_GCR_SWD;                                // disable SWIM, using IO
    RS485_Init();
    WWDG_Configuration();                                 //���Ź���ʼ����ÿ49.152ms��Ҫι�������߸�λ��
    WWDGReset();                                          //���Ź���λ��
    timer4_init();                                        // 1 ms irq
    timer2_init();                                      // 2000ms irq
    asm("rim");

    while(1) {
        Refresh_WWDG_Window();
        protocol_master_check();
//        time4_delay_ms(10);
    }
}
