#include "stm8s.h"
#include "bsp_gpio.h"
#include "bsp_key.h"
#include "stm8s_conf.h"
#include "bsp_uart.h"
#include "bsp_e2p.h"
#include "bsp_timer.h"
#include "checksum.h"
#include "bsp_wwdg.h"
#include "global_variables.h"
#include "modbus_rtu.h"



void assert_failed(uint8_t* file, uint32_t line)
{
  while(1)
  {
  
  }
}


uint8_t buf_cmd_query[6];
uint8_t buf_cmd_ctrl[8];


int main(void)
{
    uint8_t i;
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);        //ϵͳʱ�����á�
    TIMER1_Init();                                        //��ʱ1����ʼ�����ϵ�Ϊ�ء�
    RS485_Init();                                         //���ڳ�ʼ����
    enableInterrupts();                                   //ʹ��ȫ���жϡ�
    RS485_Recv();                                         //�ص�����ģʽ��
    g_uart_idx=0;
    LED_Init();                                           //LED�Ƴ�ʼ�����ϵ�Ĭ��Ϊ��
    KEY_Init();                                           //������ʼ����
    UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);              //�������ڽ���ʹ�ܡ�

    buf_cmd_query[0] = 0x74;
    buf_cmd_query[1] = 0xAA;
    buf_cmd_query[2] = get_crc(buf_cmd_query, 2);

    volatile uint16_t systick_temp = 0;

    volatile uint8_t crc = 0;

    while(1) {

        modbus_recv_hander();

        if(systick_counter == systick_temp) {
            continue ;
        }

        if((systick_counter & 1)) {

//            PORT_TEST->ODR ^= PIN_TEST;

            if((KEY1_PORT->IDR & KEY1_PIN) == 0) {

                while((KEY1_PORT->IDR & KEY1_PIN) == 0);

                if(g_protocol_type != protocol_pinglian) {

                    memset(g_dev_status, 0, 0xff);

                    if(led_all_status == 0) {
                        led_all_status = 0xff;
                    } else if(led_all_status == 0xff) {
                        led_all_status = 0;
                    }

                    memset(&buf_send[1], led_all_status, 6);

                    for(i=0xd8; i<0xff; i++) {
                        buf_send[0] = i;

                        buf_send[7] = get_crc(buf_send, 7);

//                        while(g_uart_send);     // halt in uart sending in irq handler
                        g_uart_send = 1;
                        uart_send_buf(buf_send, 8);
                        g_uart_send = 0;
                    }
                } else {

                    g_panel_backlight2 ^= 0x04;

                    uint8_t buf_tmp[10];

                    buf_tmp[0] = 0xFF;
                    buf_tmp[1] = 0xAA;
                    buf_tmp[2] = 0x07;
                    buf_tmp[3] = 0xFE;
                    buf_tmp[4] = g_current_panel-1;
                    buf_tmp[5] = RCU_CTRL_ONE_PANEL;
                    buf_tmp[6] = g_panel_key;
                    buf_tmp[7] = g_panel_backlight1;
                    buf_tmp[8] = g_panel_backlight2;
                    buf_tmp[9] = get_crc_sum(&buf_tmp[3], buf_tmp[2] - 1);

                    g_uart_send = 1;
                    uart_send_buf(buf_tmp, 10);
                    g_uart_send = 0;

                }

                systick_counter = 0;

            } else if((KEY4_PORT->IDR & KEY4_PIN) == 0) {

                while((KEY4_PORT->IDR & KEY4_PIN) == 0);
                
                systick_counter = 0;
                g_current_panel = PANEL_NUMBER_BASE;

                if(g_protocol_type == protocol_poll) {

                    g_protocol_type = protocol_call;
                    buf_cmd_query[0] = 0x75;

//                    LED1_PORT->ODR &= ~LED1_PIN;
                    LED1_PORT->ODR |= LED1_PIN;

                } else if(g_protocol_type == protocol_call) {

                    g_protocol_type = protocol_pinglian;
                    buf_cmd_query[0] = 0xFF;
                    buf_cmd_query[1] = 0xAA;
                    buf_cmd_query[2] = 0x03;
                    buf_cmd_query[3] = 0xFE;
                    buf_cmd_query[4] = 0xFE;
                    buf_cmd_query[5] = 0xFC;

                } else if(g_protocol_type == protocol_pinglian) {

                    g_protocol_type = protocol_poll;
                    buf_cmd_query[0] = 0x74;
                    buf_cmd_query[1] = 0xAA;
                    buf_cmd_query[2] = get_crc(buf_cmd_query, 2);

                }

            }

        }

        if(g_protocol_type == protocol_poll) {

            if(systick_counter >= 10) {
                systick_counter = 0;

                g_uart_send = 1;
                uart_send_buf(buf_cmd_query, 3);
                g_uart_send = 0;
                LED1_PORT->ODR ^= LED1_PIN;
            }

        } else if(g_protocol_type == protocol_call) {

            buf_cmd_query[1] = g_current_panel++;

            if(g_current_panel == PANEL_NUMBER_MAX + PANEL_NUMBER_BASE) {
                g_current_panel = PANEL_NUMBER_BASE;
            }

            buf_cmd_query[2] = get_crc(buf_cmd_query, 2);

            g_uart_send = 1;
            uart_send_buf(buf_cmd_query, 3);
            g_uart_send = 0;

        } else if(g_protocol_type == protocol_pinglian) {

            if(systick_counter >= 10) {

                LED1_PORT->ODR |= LED1_PIN;
                systick_counter = 0;

                g_uart_send = 1;
                uart_send_buf(buf_cmd_query, 6);
                g_uart_send = 0;

            }

            LED1_PORT->ODR &= ~LED1_PIN;
        }

        systick_temp = systick_counter;
    }
}
