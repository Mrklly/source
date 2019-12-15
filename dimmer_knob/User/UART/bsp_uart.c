#include "bsp_uart.h"
#include "lpuart.h"

uint8_t uart_flag;                                       //���ռ�����
uint8_t ReceiveFlag;                                     //�յ����ݱ�ǡ�
extern uint8_t ReceiveRs485Agreement[24];                //����RS485Э����ջ��档

void Uart_PortInit(void)
{
  stc_gpio_config_t stcGpioCfg;
  DDL_ZERO_STRUCT(stcGpioCfg);
  
  stcGpioCfg.enDir = GpioDirOut;
  Gpio_Init(GpioPortA,GpioPin1,&stcGpioCfg);
  Gpio_SetIO(GpioPortA, GpioPin1);   
  
  Gpio_Init(GpioPortA,GpioPin2,&stcGpioCfg);
  Gpio_SetAfMode(GpioPortA,GpioPin2,GpioAf1);//TX
  
  stcGpioCfg.enDir = GpioDirIn;
  Gpio_Init(GpioPortA,GpioPin3,&stcGpioCfg);
  Gpio_SetAfMode(GpioPortA,GpioPin3,GpioAf1);//RX 
}

void RS485_Init(void)
{  
  uint16_t u16Scnt = 0;
  stc_uart_config_t  stcConfig;
  stc_uart_irq_cb_t stcUartIrqCb;
  stc_uart_multimode_t stcMulti;
  stc_uart_baud_t stcBaud;
  
// en_uart_mmdorck_t enTb8;
  
  DDL_ZERO_STRUCT(stcConfig);
  DDL_ZERO_STRUCT(stcUartIrqCb);
  DDL_ZERO_STRUCT(stcMulti);
  DDL_ZERO_STRUCT(stcBaud);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
//  Sysctrl_SetPeripheralGate(SysctrlPeripheralDma,TRUE);
  Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE);
  Uart_PortInit();
  
  stcUartIrqCb.pfnRxIrqCb   = RxIntCallback;
  stcUartIrqCb.pfnTxIrqCb   = TxIntCallback;
  stcUartIrqCb.pfnRxFEIrqCb = ErrIntCallback;
  stcUartIrqCb.pfnPEIrqCb   = PErrIntCallBack;
  stcUartIrqCb.pfnCtsIrqCb  = CtsIntCallBack;
  
  stcConfig.pstcIrqCb = &stcUartIrqCb;
  stcConfig.bTouchNvic = TRUE;
  
  
  stcConfig.enRunMode = UartMode1;              //ģʽ3
  stcConfig.enStopBit = Uart1bit;  
  
  //stcMulti.enMulti_mode = UartNormal;           //��������ģʽ
  //enTb8 = UartEven;                             //żУ��
  //Uart_SetMMDOrCk(UARTCH1,enTb8);
  stcConfig.pstcMultiMode = &stcMulti;
  
  Uart_Init(UARTCH1, &stcConfig);
  
  Uart_SetClkDiv(UARTCH1,Uart8Or16Div);
  stcBaud.u32Pclk = Sysctrl_GetPClkFreq();
  stcBaud.enRunMode = UartMode1;
  stcBaud.u32Baud = 9600;
  u16Scnt = Uart_CalScnt(UARTCH1,&stcBaud);
  Uart_SetBaud(UARTCH1,u16Scnt);
  
  Uart_EnableIrq(UARTCH1,UartRxIrq);
  Uart_ClrStatus(UARTCH1,UartRC);
  Uart_ClrStatus(UARTCH1,UartTC);
  Uart_EnableFunc(UARTCH1,UartRx);
  RS485_Recv();
}

void TxIntCallback(void)
{

}

void UART_Multi_byte(uint8_t *Buffer,uint8_t Length)
{
    RS485_Send();
    uint8_t len=0;
    for(len=0; len < Length; len++) 
	{
       Uart_SendData(UARTCH1,Buffer[len]);
    }
    RS485_Recv();
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
/*******************************************END-UART.C***********************************************/


