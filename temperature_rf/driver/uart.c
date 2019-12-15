#include "uart.h"
#include "global_variables.h"

uint8_t u8TxData[2] = {0x00,0x55};
uint8_t u8RxData;
uint8_t u8TxCnt=0,u8RxCnt=0;

void TxIntCallback(void)
{

}

void UART_Send(uint8_t *Buffer, uint16_t Length)
{
    RS485_SEND;
    uint16_t len=0;
    for(; len < Length; len++) {
        LPUart_SendData(LPUART1, Buffer[len]);
    }
    RS485_RECV;
}

void RxIntCallback(void)
{

    if(g_idx > (RX_BUFF_LEN)) {
        g_idx = 0;
        return ;
    }

    g_rx_buff[g_idx++] = LPUart_ReceiveData(LPUART1);

    LPUart_ClrStatus(LPUART1, LPUartRC);
}

void ErrIntCallback(void)
{

}
void PErrIntCallBack(void)
{
}
void CtsIntCallBack(void)
{

}

void uart_init()
{
    uint16_t u16Scnt = 0;

    stc_gpio_config_t stcGpioCfg;
    stc_lpuart_sclk_sel_t stcSclk;
    stc_lpuart_config_t  stcConfig;
    stc_lpuart_irq_cb_t stcLPUartIrqCb;
    stc_lpuart_multimode_t stcMulti;
    stc_lpuart_baud_t stcBaud;

    DDL_ZERO_STRUCT(stcConfig);
    DDL_ZERO_STRUCT(stcLPUartIrqCb);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);
    DDL_ZERO_STRUCT(stcGpioCfg);
    DDL_ZERO_STRUCT(stcSclk);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralLpUart1, TRUE);

    stcGpioCfg.enDir =  GpioDirOut;
    stcGpioCfg.enPuPd = GpioPu;
    Gpio_Init(GpioPortC, GpioPin12, &stcGpioCfg);//TX

    stcGpioCfg.enDir =  GpioDirIn;
    Gpio_Init(GpioPortC, GpioPin11, &stcGpioCfg);//RX

    Gpio_SetAfMode(GpioPortC,GpioPin11,GpioAf1);
    Gpio_SetAfMode(GpioPortC,GpioPin12,GpioAf2);

    rs485_control_pin_init();

    stcLPUartIrqCb.pfnRxIrqCb   = RxIntCallback;
    stcLPUartIrqCb.pfnTxIrqCb   = NULL;
    stcLPUartIrqCb.pfnRxFEIrqCb = ErrIntCallback;
    stcLPUartIrqCb.pfnPEIrqCb   = PErrIntCallBack;
    stcLPUartIrqCb.pfnCtsIrqCb  = CtsIntCallBack;
    stcConfig.pstcIrqCb = &stcLPUartIrqCb;
    stcConfig.bTouchNvic = TRUE;

    stcConfig.enStopBit = LPUart1bit;
    stcConfig.enRunMode = LPUartMode1;
    stcSclk.enSclk_Prs  = LPUart4Or8Div;
    stcSclk.enSclk_sel  = LPUart_Pclk;
    stcConfig.pstcLpuart_clk = &stcSclk;

    stcMulti.enMulti_mode = LPUartNormal;
    stcConfig.pstcMultiMode = &stcMulti;

    LPUart_SetMMDOrCk(LPUART1,LPUartDataOrAddr);
    LPUart_Init(LPUART1, &stcConfig);

    LPUart_SetClkDiv(LPUART1,LPUart4Or8Div);

    stcBaud.u32Sclk = Sysctrl_GetPClkFreq();
    stcBaud.enRunMode = LPUartMode1;
    stcBaud.u32Baud = 19200u;
    u16Scnt = LPUart_CalScnt(LPUART1,&stcBaud);
    LPUart_SetBaud(LPUART1,u16Scnt);

    LPUart_EnableIrq(LPUART1,LPUartRxIrq);
    LPUart_ClrStatus(LPUART1,LPUartRC);
    LPUart_ClrStatus(LPUART1,LPUartTC);
    LPUart_EnableFunc(LPUART1,LPUartRx);
}

void rs485_control_pin_init(void)
{
    stc_gpio_config_t stcGpioCfg;

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    DDL_ZERO_STRUCT(stcGpioCfg);

    stcGpioCfg.enDir = GpioDirOut;
//    stcGpioCfg.enDrv = GpioDrvH;
//    stcGpioCfg.enPuPd = GpioPd;
//    stcGpioCfg.enOD = GpioOdDisable;
//    stcGpioCfg.enCtrlMode = GpioFastIO;
    Gpio_Init(RS485_EN_PORT, RS485_EN_PIN, &stcGpioCfg);

    RS485_RECV;
}
