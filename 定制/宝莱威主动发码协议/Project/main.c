#include "stm8s.h"
#include "bsp_gpio.h"
#include "bsp_key.h"
#include "stm8s_conf.h"
#include "stm8s_itc.h"
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

void nvic_priority_set(void)
{
    ITC_SetSoftwarePriority(ITC_IRQ_TIM4_OVF, ITC_PRIORITYLEVEL_3);
    ITC_SetSoftwarePriority(ITC_IRQ_UART1_RX, ITC_PRIORITYLEVEL_2);
    ITC_SetSoftwarePriority(ITC_IRQ_TIM2_OVF, ITC_PRIORITYLEVEL_1);
}

int main(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);        //系统时钟配置16M。
    flash_Init();
    LED_Init();                                           //LED灯初始化，上电默认为灭。
    pwm_init();                                           // tim1
    Relay_Init();
    KEY_Init();                                           //按键初始化。
    CFG->GCR |= CFG_GCR_SWD;                                // disable SWIM, using IO
    RS485_Init();
    WWDG_Configuration();                                 //看门狗初始化。每49.152ms需要喂狗，否者复位。
    WWDGReset();                                          //看门狗复位。
    timer4_init();                                        // 1 ms irq
    timer2_init();                                      // 2000ms irq
    nvic_priority_set();
    dev_param_get();
    asm("rim");

    while(1) {
        protocol_buf_send();
        Refresh_WWDG_Window();
//        protocol_master_check();
//        time4_delay_ms(10);
    }
}
