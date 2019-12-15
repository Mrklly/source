#include "drv_uart.h"
#include "drv_led.h"
#include "mcu_api.h"


volatile uint8_t g_rx_index = 0;
uint8_t g_rx_buf[RX_BUF_LEN];
uint8_t g_tx_buf[TX_BUF_LEN];

uint8_t u8RxFlg=0;
uint8_t CheckFlg=0;


void RxIntCallback(void)
{
    uart_receive_input(M0P_UART1->SBUF);
}

void ErrIntCallback(void)
{

}

void TxIntCallback(void)
{

}

void hal_uart_init(void)
{
    uint16_t timer=0;
    uint32_t pclk=0;

    stc_uart_config_t  stcConfig;
    stc_uart_irq_cb_t stcUartIrqCb;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_config_t stcBaud;
    stc_bt_config_t stcBtConfig;

    DDL_ZERO_STRUCT(stcUartIrqCb);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);
    DDL_ZERO_STRUCT(stcBtConfig);
    
    Clk_SetPeripheralGate(ClkPeripheralBt, TRUE);
    Clk_SetPeripheralGate(ClkPeripheralUart1, TRUE);

    Gpio_InitIOExt(PORT_UART_TX, PIN_UART_TX, GpioDirOut, TRUE,FALSE,FALSE,FALSE);
    Gpio_InitIOExt(PORT_UART_RX, PIN_UART_RX, GpioDirIn,  TRUE,FALSE,FALSE,FALSE);

    Gpio_SetFunc_UART1TX_P35();
    Gpio_SetFunc_UART1RX_P36();

    stcConfig.enRunMode = UartMode1;

        stcMulti.enMulti_mode = UartNormal;
        stcMulti.u8SlaveAddr  = 0;
        stcMulti.u8SaddEn     = 0;
    stcConfig.pstcMultiMode = &stcMulti;

        stcUartIrqCb.pfnRxIrqCb = RxIntCallback;
        stcUartIrqCb.pfnTxIrqCb = TxIntCallback;
        stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback;
    stcConfig.pstcIrqCb = &stcUartIrqCb;

    stcConfig.bTouchNvic = TRUE;

//    Uart_SetMMDOrCk(UARTCH1, Data);

    stcBaud.u8Mode = UartMode1;
    stcBaud.bDbaud  = 0;
    stcBaud.u32Baud = 9600;

    pclk = Clk_GetPClkFreq();
    timer=Uart_SetBaudRate(UARTCH1, pclk, &stcBaud);

    stcBtConfig.enMD = BtMode2;
    stcBtConfig.enCT = BtTimer;
    Bt_Init(TIM1, &stcBtConfig);
    Bt_ARRSet(TIM1,timer);
    Bt_Cnt16Set(TIM1,timer);
    Bt_Run(TIM1);

//    M0P_UART1->SCON_f.REN = 1;

    Uart_Init(UARTCH1, &stcConfig);
    Uart_ClrStatus(UARTCH1,UartRxFull);
    Uart_EnableFunc(UARTCH1,UartRx);
    Uart_EnableIrq(UARTCH1,UartRxIrq);
}

void uart_send_buf(uint8_t *buf, uint8_t length)
{
    uint8_t i=0;
    for(; i<length; i++) {

        M0P_UART1->ICR_f.TICLR = 0;

        M0P_UART1->SBUF = buf[i];

        while (TRUE != M0P_UART1->ISR_f.TI);

        M0P_UART1->ICR_f.TICLR = 0;

//        Uart_SendData(UARTCH1, buf[i]);
    }
}
