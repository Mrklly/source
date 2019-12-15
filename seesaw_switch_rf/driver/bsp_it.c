#include "bsp_it.h"
#include "global_variables.h"


void Tim2Int(void)
{
    static uint8_t timer_cont = 0;

    if(TRUE != Bt_GetIntFlag(TIM2, BtUevIrq)) {
        return ;
    }
    
    if(timer_cont == 20) {
        Key_value_detection();
        timer_cont = 0;
    }
    timer_cont++;

    if(g_led_flag && g_uart_delay) {
        g_uart_delay--;
    }

    for(uint8_t i=0; i<4; i++) {
        if(pwm_flag[i] == 0x01) {   //第一路调光。按键有变化。
            PWM_Contorl_LED(i, key_led_state[i]);
        }
    }

    TimingDelay_Decrement();

    Bt_ClearIntFlag(TIM2,BtUevIrq);
}

void Gpio_IRQHandler(uint8_t u8Param)
{
#if 0
    if(GpioPortA == u8Param) {
        if(TRUE == Gpio_GetIrqStatus(GpioPortA, GpioPin15))
        {    
            Delay(0xff);               
            if(Gpio_GetInputIO(GpioPortA, GpioPin15)==0)
            {
                Key_Value_Temp=KEY1_VALUE; 
                if(Point_flag[0]==1)
                {
                    Key_Value_Temp=NO_KEY;                                         
                    SetTIM1_PWM_DutyCycle(0,1);  
                    back_LED_open_Close_Config(1,0);
                    Relay_open_Close_Config(RELAY_PORT_CH1,RELAY_PIN_CH1,1);                    
                }
                Dismantling_function(rs485InitStruct.key_function[0],1,0);
            }
            while(Gpio_GetInputIO(GpioPortA, GpioPin15)==0);
            if(Point_flag[0]==1)
            {
                SetTIM1_PWM_DutyCycle(1800,1);     
                back_LED_open_Close_Config(1,1);
                Relay_open_Close_Config(RELAY_PORT_CH1,RELAY_PIN_CH1,0); 
            }
            Dismantling_function(rs485InitStruct.key_function[0],1,1);
            Gpio_ClearIrq(GpioPortA, GpioPin15);
        }
    } else if(GpioPortB == u8Param) {
        
        if(TRUE == Gpio_GetIrqStatus(GpioPortB, GpioPin3)) { 
            Delay(0xff);
            if(Gpio_GetInputIO(GpioPortB, GpioPin3)==0) { 
                Key_Value_Temp=KEY2_VALUE; 
                if(Point_flag[1]==1) {
                    Key_Value_Temp=NO_KEY;                 
                    SetTIM1_PWM_DutyCycle(0,2);
                    back_LED_open_Close_Config(2,0);
                    Relay_open_Close_Config(RELAY_PORT_CH2,RELAY_PIN_CH2,1); 
                }
                Dismantling_function(rs485InitStruct.key_function[2],2,0);
            }
            while(Gpio_GetInputIO(GpioPortB, GpioPin3)==0);
            if(Point_flag[1]==1) {                
                SetTIM1_PWM_DutyCycle(1800,2);
                back_LED_open_Close_Config(2,1);
                Relay_open_Close_Config(RELAY_PORT_CH2,RELAY_PIN_CH2,0); 
            }
            Dismantling_function(rs485InitStruct.key_function[2],2,1);
            Gpio_ClearIrq(GpioPortB, GpioPin3);
        } 
        
        if(TRUE == Gpio_GetIrqStatus(GpioPortB, GpioPin4)) { 
            Delay(0xff);
            if(Gpio_GetInputIO(GpioPortB, GpioPin4)==0);
            {
                Key_Value_Temp=KEY3_VALUE; 
                if(Point_flag[2]==1)
                {
                    Key_Value_Temp=NO_KEY;                 
                    SetTIM1_PWM_DutyCycle(0,3);
                    back_LED_open_Close_Config(3,0);
                    Relay_open_Close_Config(RELAY_PORT_CH3,RELAY_PIN_CH3,1); 
                }
                Dismantling_function(rs485InitStruct.key_function[4],3,0);
            }
            while(Gpio_GetInputIO(GpioPortB, GpioPin4)==0);
            if(Point_flag[2]==1) {
                SetTIM1_PWM_DutyCycle(1800,3);
                back_LED_open_Close_Config(3,1);
                Relay_open_Close_Config(RELAY_PORT_CH3,RELAY_PIN_CH3,0); 
            }
            Dismantling_function(rs485InitStruct.key_function[4],3,1);
            Gpio_ClearIrq(GpioPortB, GpioPin4);
        }
        
        if(TRUE == Gpio_GetIrqStatus(GpioPortB, GpioPin5)) { 
            Delay(0xff);
            if(Gpio_GetInputIO(GpioPortB, GpioPin5)==0)
            {
                Key_Value_Temp=KEY4_VALUE; 
                if(Point_flag[3]==1)
                {
                    Key_Value_Temp=NO_KEY;                
                    SetTIM1_PWM_DutyCycle(0,4);   
                    back_LED_open_Close_Config(4,0);
                    Relay_open_Close_Config(RELAY_PORT_CH4,RELAY_PIN_CH4,1); 
                }
                Dismantling_function(rs485InitStruct.key_function[6],4,0);
            }
            while(Gpio_GetInputIO(GpioPortB, GpioPin5)==0);
            if(Point_flag[3]==1)
            {             
                SetTIM1_PWM_DutyCycle(1800,4);
                back_LED_open_Close_Config(4,1);
                Relay_open_Close_Config(RELAY_PORT_CH4,RELAY_PIN_CH4,0); 
            }
            Dismantling_function(rs485InitStruct.key_function[6],4,1);
            Gpio_ClearIrq(GpioPortB, GpioPin5);
        } 
    } else {
        
    }
#endif
}

void RxIntCallback(void)
{
    if(g_rx_idx > UART_BUF_LEN) {
        g_rx_idx = 0;
        return ;
    }

    g_rx_buf[g_rx_idx++] = Uart_ReceiveData(UARTCH1);

    if(g_rx_buf[0] != 0x55) {
        g_rx_idx=0;
    }

    if(g_rx_idx >=8) {
        ReceiveFlag = 1;
    } else {
        ReceiveFlag = 0;
    }
}

