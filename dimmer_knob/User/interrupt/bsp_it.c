
#include "bsp_it.h"
#include "global_variables.h"


extern uint8_t ReceiveFlag;                                     //收到数据标记。
extern uint8_t uart_flag;                                       //接收计数。
extern uint8_t ReceiveRs485Agreement[RS485_AGREEMENT_LEN];      //定义RS485协议接收缓存。
extern uint8_t Key_Value_Temp;
extern uint8_t pwm_flag[6];
extern uint8_t key_led_state[4];                                //LED灯临时状态。
extern uint8_t Point_flag[4];
extern rs458_InitTypedef rs485InitStruct;                       //外部结构体变量。
extern uint8_t Exclude_flag[4];                                 //排除标记
uint8_t pwm_share_cont = 0;                                     //过零点记时。
uint8_t adjust1_flag = 1;
uint8_t adjust2_flag = 1;
uint8_t adjust1_cont = 0;
uint8_t adjust2_cont = 0;
uint8_t Chopping1_channel_cont = 13;
uint8_t Chopping2_channel_cont = 13;
extern uint8_t power1_flag;
extern uint8_t power2_flag;
extern uint8_t key_Result;
static uint8_t timer_cont = 0;

void Tim2Int(void)
{
    if(TRUE == Bt_GetIntFlag(TIM2, BtUevIrq)) {
        timer_cont++;

        if(timer_cont == 20) {
            key_Result = Key_value_detection ();
            timer_cont = 0;
        }

        if(pwm_flag[1] == 0x01) {
            PWM_Contorl_LED(2, key_led_state[0]);
        }

        if(pwm_flag[4] == 0x01) {
            PWM_Contorl_LED(5, key_led_state[1]);
        }

        TimingDelay_Decrement();
        Bt_ClearIntFlag(TIM2, BtUevIrq);
    }
}

void Tim0Int(void)
{
    if(TRUE == Bt_GetIntFlag(TIM0, BtUevIrq)) {
        pwm_share_cont++;

        if(adjust1_flag == 1) {
            adjust1_cont++;

            if(adjust1_cont == Chopping1_channel_cont) {
                Gpio_SetIO(CH3_PORT, CH3_PIN);
            }
        }

        if(adjust2_flag == 1) {
            adjust2_cont++;

            if(adjust2_cont == Chopping2_channel_cont) {
                Gpio_SetIO(CH4_PORT, CH4_PIN);
            }
        }

        if(pwm_share_cont >= STEP) {
            Bt_M0_Stop(TIM0);
            pwm_share_cont = 0;
            adjust1_flag = 0;
            adjust1_cont = 0;
            adjust2_flag = 0;
            adjust2_cont = 0;
            Gpio_ClrIO(CH3_PORT, CH3_PIN);
            Gpio_ClrIO(CH4_PORT, CH4_PIN);
        }

        Bt_ClearIntFlag(TIM0, BtUevIrq);
    }
}

void Gpio_IRQHandler(uint8_t u8Param)
{
    volatile uint8_t knob_temp_a, knob_temp_b;

    if(1 == u8Param) {
        if(TRUE == Gpio_GetIrqStatus(GpioPortB, GpioPin7)) {
            if(Gpio_GetInputIO(GpioPortB, GpioPin7) != 0) {
                Gpio_ClrIO(CH3_PORT, CH3_PIN);
                Gpio_ClrIO(CH4_PORT, CH4_PIN);
                Gpio_ClearIrq(GpioPortB, GpioPin7);
                return;
            }

            if(Gpio_GetInputIO(GpioPortB, GpioPin7) == 0) {
                if(power1_flag == 0) {
                    Gpio_ClrIO(CH3_PORT, CH3_PIN);
                    adjust1_flag = 0;
                } else if(Chopping1_channel_cont == 0) {
                    Gpio_SetIO(CH3_PORT, CH3_PIN);
                    adjust1_flag = 1;
                } else {
                    adjust1_flag = 1;
                }

                if(power2_flag == 0) {
                    Gpio_ClrIO(CH4_PORT, CH4_PIN);
                    adjust2_flag = 0;
                } else if(Chopping2_channel_cont == 0) {
                    Gpio_SetIO(CH4_PORT, CH4_PIN);
                    adjust2_flag = 1;
                } else {
                    adjust2_flag = 1;
                }

                adjust1_cont = 0;
                adjust2_cont = 0;
                pwm_share_cont = 0;
                Bt_M0_Run(TIM0);
                Gpio_ClearIrq(GpioPortB, GpioPin7);
            }
        }
    } else if(M0P_GPIO->PA_STAT_f.PA04) {    // Gpio_GetIrqStatus(PORT_KNOB_CHA, PIN_KNOB_CHA))

        knob_temp_a = M0P_GPIO->PAIN_f.PA04;
        knob_temp_b = M0P_GPIO->PAIN_f.PA05;

        if(g_knob_direction == DIRECTION_NONE) {
            if(knob_temp_a != knob_temp_b) {
                g_knob_direction = DIRECTION_CCW_JIT;
            } else {
                g_knob_direction = DIRECTION_NONE;
            }
        } else if(g_knob_direction == DIRECTION_CW_JIT) {
            if(knob_temp_a == knob_temp_b) {
                g_knob_direction = DIRECTION_CW;
            } else {
                g_knob_direction = DIRECTION_NONE;
            }
        }

        M0P_GPIO->PA_ICLR_f.PA04 = 0;         // Gpio_ClearIrq

    } else if(M0P_GPIO->PA_STAT_f.PA05) {     // Gpio_GetIrqStatus(PORT_KNOB_CHB, PIN_KNOB_CHB))

        knob_temp_a = M0P_GPIO->PAIN_f.PA04;
        knob_temp_b = M0P_GPIO->PAIN_f.PA05;

        if(g_knob_direction == DIRECTION_NONE) {
            if(knob_temp_a != knob_temp_b) {
                g_knob_direction = DIRECTION_CW_JIT;
            } else {
                g_knob_direction = DIRECTION_NONE;
            }
        } else if(g_knob_direction == DIRECTION_CCW_JIT) {
            if(knob_temp_a == knob_temp_b) {
                g_knob_direction = DIRECTION_CCW;
            } else {
                g_knob_direction = DIRECTION_NONE;
            }
        }

        M0P_GPIO->PA_ICLR_f.PA05 = 0;   // Gpio_ClearIrq

    }
}

void RxIntCallback(void)
{
    ReceiveRs485Agreement[uart_flag] = Uart_ReceiveData(UARTCH1);
    uart_flag++;

    if(uart_flag > 23) {
        uart_flag = 0;
    }

    ReceiveFlag = 1;
}
