#include "spi.h"

volatile uint8_t u8IrqData, u8SendData, Buff[20];
volatile uint8_t bSendIrq, bRecvIrq;


void spi_io_init(void)
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stc_gpio_config_t stcGpioCfg;
    DDL_ZERO_STRUCT(stcGpioCfg);

    stcGpioCfg.enDir = GpioDirOut;

    Gpio_Init(     SPI1_CSB_PORT, SPI1_CSB_PIN, &stcGpioCfg);    // CS
    Gpio_SetAfMode(SPI1_CSB_PORT, SPI1_CSB_PIN, GpioAf0);       // GPIO

    Gpio_Init(     SPI1_FCSB_PORT, SPI1_FCSB_PIN, &stcGpioCfg);    // FCS
    Gpio_SetAfMode(SPI1_FCSB_PORT, SPI1_FCSB_PIN, GpioAf0);       // GPIO

    Gpio_Init(     SPI1_MOSI_PORT, SPI1_MOSI_PIN, &stcGpioCfg);    // MOSI
    Gpio_SetAfMode(SPI1_MOSI_PORT, SPI1_MOSI_PIN, GpioAf1);

    Gpio_Init(     SPI1_SCLK_PORT, SPI1_SCLK_PIN, &stcGpioCfg);     // SCLK
    Gpio_SetAfMode(SPI1_SCLK_PORT, SPI1_SCLK_PIN, GpioAf1);

    stcGpioCfg.enDir  = GpioDirIn;
    stcGpioCfg.enPuPd = GpioNoPuPd;
    Gpio_Init(     SPI1_MISO_PORT, SPI1_MISO_PIN, &stcGpioCfg);    // MISO
    Gpio_SetAfMode(SPI1_MISO_PORT, SPI1_MISO_PIN, GpioAf1);

    stcGpioCfg.enDir = GpioDirIn;
    stcGpioCfg.enDrv = GpioDrvH;
    stcGpioCfg.enPuPd = GpioPd;
    stcGpioCfg.enOD = GpioOdDisable;
    stcGpioCfg.enCtrlMode = GpioAHB;
//    Gpio_SetAfMode(SPI1_INT_PORT, SPI1_INT_PIN, GpioAf0);       // GPIO
    Gpio_Init(     SPI1_INT_PORT, SPI1_INT_PIN, &stcGpioCfg);    // INT
    Gpio_ClearIrq( SPI1_INT_PORT, SPI1_INT_PIN);                // using IRQ
    Gpio_EnableIrq(SPI1_INT_PORT, SPI1_INT_PIN, GpioIrqRising);
    EnableNvic(    SPI1_INT_PORT, IrqLevel3, TRUE);
}

void spi_function_init(void)
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralSpi1, TRUE); //using SPI1

    stc_spi_config_t SPIConfig;
    DDL_ZERO_STRUCT( SPIConfig);

    Spi_SetCS(Spi1, TRUE);          // idle CS is high

    SPIConfig.bCPHA = Spicphafirst; // first SCLk edge to get data
    SPIConfig.bCPOL = Spicpollow;   // idle SCLK = 0
    SPIConfig.bMasterMode = SpiMaster;  
    SPIConfig.u8BaudRate = SpiClkDiv128;

#if SPI1_IRQ
    SPIConfig.bIrqEn = TRUE;        // enable interrupt
    SPIConfig.pfnSpi1IrqCb = Spi1IntCallBack;
#else
    SPIConfig.bIrqEn = FALSE;
    SPIConfig.pfnSpi1IrqCb = NULL;
#endif

    Spi_Init(Spi1,&SPIConfig);
}

#if SPI1_IRQ
void Spi1IntCallBack(void)
{
    if(Spi_GetState(Spi1) & 0x38) {
        Spi_ClearStatus(Spi1);
        return;
    }

    if(Spi_GetStatus(Spi1, SpiRxne)) {
      bRecvIrq = 1;
      Spi_FuncEn(Spi1, SpiRxNeIe, FALSE);
      u8IrqData = M0P_SPI1->DATA;
      Spi_ClearStatus(Spi1);
    } else if(Spi_GetStatus(Spi1, SpiTxe)) {
      bSendIrq = 1;
      Spi_FuncEn(Spi1, SpiTxEIe, FALSE);
      M0P_SPI1->DATA = u8SendData;
      Spi_ClearStatus(Spi1);
    }
}
#endif



void spi_init(void)
{
    spi_io_init();
    spi_function_init();
}

void spi_write_byte(uint8_t data)
{
    while(0 == M0P_SPI1->STAT_f.TXE){;}

    M0P_SPI1->DATA = data;

    while(0 == M0P_SPI1->STAT_f.RXNE){;}

    data = M0P_SPI1->DATA;
}

uint8_t spi_read_byte(void)
{
    uint8_t data;

    while(0 == M0P_SPI1->STAT_f.TXE){;}

    M0P_SPI1->DATA = 0x00;

    SPI1_MOSI_IO();
    SPI1_MOSI_IN();

    while(0 == M0P_SPI1->STAT_f.RXNE){;}

    data = M0P_SPI1->DATA;

    delay_us(5);

    SPI1_MOSI_SPI();
    SPI1_MOSI_OUT();

    return data;
}


uint8_t spi_rw_data(uint8_t data)
{
    uint8_t retry=0;

    M0P_SPI1->SSN = 0;
    
    while (M0P_SPI1->STAT_f.TXE == 0) { //检查发送缓存空标志位设置与否                
        retry++;
        if(retry > 200) {
            return 0;
        }
    }

    M0P_SPI1->DATA = data;

    retry=0;    
    while (M0P_SPI1->STAT_f.RXNE == 0) {//检查接受缓存非空标志位设置与否        
        retry++;      
        if(retry > 200) {
            return 0;
        }
    }

    M0P_SPI1->SSN = 1;

    return M0P_SPI1->DATA; //返回通过SPI1最近接收的数据

}

void spi_write_buff(uint8_t *buff, uint8_t length)
{
    uint8_t i;
    bSendIrq = 0;
	bRecvIrq = 0;
    for(i=0; i<length; i++)
    {
        u8SendData = *(buff + i);
        Spi_FuncEn(Spi1, SpiRxNeIe,TRUE);
        Spi_FuncEn(Spi1, SpiTxEIe,TRUE);
		while(0 == bSendIrq);
		bSendIrq = 0;
		while(0 == bRecvIrq);
		bRecvIrq = 0;
    }
}

void spi_read_buff(uint8_t *buff, uint8_t length)
{
    uint8_t i;
    uint16_t u32TimeOut;

    bRecvIrq = 0;

    for(i=0; i<length; i++) {
        Spi_FuncEn(Spi1, SpiRxNeIe, TRUE);
        u32TimeOut = 1000;   
        M0P_SPI1->DATA = 0; //接收数据时，用于主机产生时钟   
        while(u32TimeOut--) {
            if(1 == bRecvIrq) {
                *(buff + i) = u8IrqData;
                break;
            }
        }
        bRecvIrq = 0;
    }
}

