#include "bsp_system.h"
#include "bsp_it.h"


extern uint8_t ReceiveRs485Agreement[RS485_AGREEMENT_LEN];
extern uint8_t ReceiveFlag;
extern uint8_t uart_flag;


int main()
{
    system_init();
    TIMER2_Init();
    TIMER0_Init();
    RS485_Init();
    uart_flag = 0;
    Flash_Init(flash_interrupt, 12, TRUE);
    LED_Init();
    Relay_Init();
    KEY_Init();
    PWM_timer1();
    PWM_timer3();
    PWM_timer4();
    flash_read_buf(AGREEMENT_ADDR, ReceiveRs485Agreement, 24);
    //UART_Multi_byte(ReceiveRs485Agreement,24);
    EEPROM_data_packing();
    knob_io_init();

    while(1) {
        knob_handler();
        KEY_LED_Control();
        RS485_Agreement_Config();
    }
}

