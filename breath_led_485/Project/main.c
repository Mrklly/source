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
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);        //系统时钟配置16M。

    WWDG_Configuration();                                 //看门狗初始化。每49.152ms需要喂狗，否者复位。
    WWDGReset();                                          //看门狗复位。

    TIMER2_Init();                                        //定时1器初始化，上电为关。
    RS485_Init();                                         //串口初始化。
    asm("rim");                                           //使能全局中断。
    flash_Init();

    protocol_param_get();

    time2_xms(1);
    Relay_Init();                                         //继电器初始化。
    //  LED_Init();                                           //LED灯初始化，上电默认为灭。
    KEY_Init();                                           //按键初始化。
    PWM_Init();
    UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);              //开启串口接收使能。


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

