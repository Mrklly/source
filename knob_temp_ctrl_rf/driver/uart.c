#include "uart.h"
#include "global_variables.h"
#include "mcu_api.h"

uint8_t u8TxData[2] = {0x00,0x55};
uint8_t u8RxData;
uint8_t u8TxCnt=0,u8RxCnt=0;

void TxIntCallback(void)
{

}

void uart_send_buf(uint8_t *buf, uint16_t len)
{
    RS485_SEND;
    uint16_t i=0;
    for(; i < len; i++) {
        LPUart_SendData(LPUART0, buf[i]);
    }
    RS485_RECV;
}

void RxIntCallback(void)
{

    if(g_idx > (RX_BUFF_LEN)) {
        g_idx = 0;
        return ;
    }

    g_rx_buff[g_idx++] = LPUart_ReceiveData(LPUART0);

    LPUart_ClrStatus(LPUART0, LPUartRC);
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

void uart_init_gpio(void)
{

    stc_gpio_config_t stcGpioCfg;
    DDL_ZERO_STRUCT(stcGpioCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stcGpioCfg.enDir =  GpioDirOut;
    stcGpioCfg.enPuPd = GpioPu;
    Gpio_Init(GpioPortC, GpioPin12, &stcGpioCfg);//TX

    stcGpioCfg.enDir =  GpioDirIn;
    Gpio_Init(GpioPortC, GpioPin11, &stcGpioCfg);//RX

    Gpio_SetAfMode(GpioPortC,GpioPin11,GpioAf2);
    Gpio_SetAfMode(GpioPortC,GpioPin12,GpioAf1);

    rs485_control_pin_init();
}


void uart_init(dev_baud_t baud)
{
    uint16_t u16Scnt = 0;

    stc_lpuart_baud_t      stcBaud;
    stc_lpuart_config_t    stcConfig;
    stc_lpuart_irq_cb_t    stcLPUartIrqCb;
    stc_lpuart_multimode_t stcMulti;
    stc_lpuart_sclk_sel_t  stcSclk;

    DDL_ZERO_STRUCT(stcBaud);
    DDL_ZERO_STRUCT(stcConfig);
    DDL_ZERO_STRUCT(stcLPUartIrqCb);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcSclk);

    uart_init_gpio();

    Sysctrl_SetPeripheralGate(SysctrlPeripheralLpUart0, TRUE);

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

    LPUart_SetMMDOrCk(LPUART0, LPUartDataOrAddr);
    LPUart_Init(LPUART0, &stcConfig);

    LPUart_SetClkDiv(LPUART0,LPUart4Or8Div);

    stcBaud.u32Sclk = Sysctrl_GetPClkFreq();
    stcBaud.enRunMode = LPUartMode1;

    if       (baud == BAUD9600) {
        stcBaud.u32Baud = 9600u;
    } else if(baud == BAUD19200) {
        stcBaud.u32Baud = 19200u;
    }

    u16Scnt = LPUart_CalScnt(LPUART0,&stcBaud);
    LPUart_SetBaud(LPUART0,u16Scnt);

    LPUart_EnableIrq(LPUART0,LPUartRxIrq);
    LPUart_ClrStatus(LPUART0,LPUartRC);
    LPUart_ClrStatus(LPUART0,LPUartTC);
    LPUart_EnableFunc(LPUART0,LPUartRx);
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

void uart_zigbee_gpio_init()
{
    stc_gpio_config_t stcGpioCfg;
    DDL_ZERO_STRUCT(stcGpioCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stcGpioCfg.enDir =  GpioDirOut;
    stcGpioCfg.enPuPd = GpioPu;
    Gpio_Init(GpioPortA, GpioPin0, &stcGpioCfg);    // TX

    stcGpioCfg.enDir =  GpioDirIn;
    Gpio_Init(GpioPortA, GpioPin1, &stcGpioCfg);    // RX

    Gpio_SetAfMode(GpioPortA, GpioPin0, GpioAf2);
    Gpio_SetAfMode(GpioPortA, GpioPin1, GpioAf2);
}

void uart_zigbee_init(dev_baud_t baud)
{
    uint16_t u16Scnt = 0;

    stc_lpuart_baud_t      stcBaud;
    stc_lpuart_config_t    stcConfig;
    stc_lpuart_irq_cb_t    stcLPUartIrqCb;
    stc_lpuart_multimode_t stcMulti;
    stc_lpuart_sclk_sel_t  stcSclk;

    DDL_ZERO_STRUCT(stcBaud);
    DDL_ZERO_STRUCT(stcConfig);
    DDL_ZERO_STRUCT(stcLPUartIrqCb);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcSclk);

    uart_zigbee_gpio_init();

    Sysctrl_SetPeripheralGate(SysctrlPeripheralLpUart1, TRUE);

    stcLPUartIrqCb.pfnRxIrqCb   = callback_uart_zigbee;
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

    LPUart_SetMMDOrCk(LPUART1, LPUartDataOrAddr);
    LPUart_Init(LPUART1, &stcConfig);

    LPUart_SetClkDiv(LPUART1,LPUart4Or8Div);

    stcBaud.u32Sclk = Sysctrl_GetPClkFreq();
    stcBaud.enRunMode = LPUartMode1;

    if       (baud == BAUD9600) {
        stcBaud.u32Baud = 9600u;
    } else if(baud == BAUD19200) {
        stcBaud.u32Baud = 19200u;
    }

    u16Scnt = LPUart_CalScnt(LPUART1,&stcBaud);
    LPUart_SetBaud(LPUART1,u16Scnt);

    EnableNvic(LPUART1_IRQn, IrqLevel1, TRUE);  // high priority

    LPUart_ClrStatus(LPUART1,LPUartRC);
    LPUart_ClrStatus(LPUART1,LPUartTC);
    LPUart_EnableFunc(LPUART1,LPUartRx);
    LPUart_EnableIrq(LPUART1,LPUartRxIrq);
}

void callback_uart_zigbee(void)
{
    uart_receive_input(LPUart_ReceiveData(LPUART1));
//    LPUart_ClrStatus(LPUART1, LPUartRC);
}

