#include "drv_uart.h"
#include "drv_led.h"

volatile uint8_t g_rx_index = 0;
uint8_t g_rx_buf[RX_BUF_LEN];
uint8_t g_tx_buf[TX_BUF_LEN];

uint8_t u8RxFlg=0;
uint8_t CheckFlg=0;


void RxIntCallback(void)
{
    if(g_rx_index >= RX_BUF_LEN) {
        g_rx_index = 0;
        return ;
    }

    g_rx_buf[g_rx_index++] = M0P_UART1->SBUF;
//    M0P_UART1->ICR_f.RICLR = 0;
}

void ErrIntCallback(void)
{
    LED_YEL_ON;
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

    Clk_SetPeripheralGate(ClkPeripheralUart1,TRUE);
    Clk_SetPeripheralGate(ClkPeripheralBt, TRUE);
//    Clk_SetPeripheralGate(ClkPeripheralAdt,TRUE);
//    Clk_SetPeripheralGate(ClkPeripheralTick,TRUE);
//    Clk_SetPeripheralGate(ClkPeripheralFlash,TRUE);

    Gpio_InitIOExt(3,5,GpioDirOut,TRUE,FALSE,FALSE,FALSE);
    Gpio_InitIOExt(3,6,GpioDirIn, TRUE,FALSE,FALSE,FALSE);

    Gpio_SetFunc_UART1TX_P35();
    Gpio_SetFunc_UART1RX_P36();


    stcUartIrqCb.pfnRxIrqCb = RxIntCallback;
    stcUartIrqCb.pfnTxIrqCb = NULL;
    stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback;

    stcConfig.pstcIrqCb = &stcUartIrqCb;
    stcConfig.bTouchNvic = TRUE;
    stcConfig.enRunMode = UartMode1;
    stcMulti.enMulti_mode = UartNormal;
//    Uart_SetMMDOrCk(UARTCH1, Data);
    stcConfig.pstcMultiMode = &stcMulti;

    stcBaud.bDbaud  = 0;//双倍波特率功能
    stcBaud.u32Baud = 19200u;//更新波特率位置
    stcBaud.u8Mode = UartMode1; //计算波特率需要模式参数
    pclk = Clk_GetPClkFreq();
    timer=Uart_SetBaudRate(UARTCH1, pclk, &stcBaud);

    stcBtConfig.enMD = BtMode2;
    stcBtConfig.enCT = BtTimer;
    Bt_Init(TIM1, &stcBtConfig);//调用basetimer1设置函数产生波特率
    Bt_ARRSet(TIM1,timer);
    Bt_Cnt16Set(TIM1,timer);
    Bt_Run(TIM1);

    M0P_UART1->SCON_f.REN = 1;

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
