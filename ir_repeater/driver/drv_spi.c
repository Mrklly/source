#include "drv_spi.h"

volatile uint8_t u8IrqData, u8SendData, Buff[20];
volatile uint8_t bSendIrq, bRecvIrq;


void spi_io_init(void)
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stc_gpio_config_t stcGpioCfg;
    DDL_ZERO_STRUCT(stcGpioCfg);

    stcGpioCfg.enDir = GpioDirOut;

    Gpio_Init(     PORT_SPI0_CS, PIN_SPI0_CS, &stcGpioCfg);    // CS
    Gpio_SetAfMode(PORT_SPI0_CS, PIN_SPI0_CS, GpioAf1);        // GPIO

    Gpio_Init(     PORT_SPI0_MOSI, PIN_SPI0_MOSI, &stcGpioCfg);// MOSI
    Gpio_SetAfMode(PORT_SPI0_MOSI, PIN_SPI0_MOSI, GpioAf1);

    Gpio_Init(     PORT_SPI0_CLK, PIN_SPI0_CLK, &stcGpioCfg);  // CLK
    Gpio_SetAfMode(PORT_SPI0_CLK, PIN_SPI0_CLK, GpioAf1);

    Gpio_Init(     PORT_SPI0_WP,   PIN_SPI0_WP,   &stcGpioCfg);  // WP
    Gpio_Init(     PORT_SPI0_HOLD, PIN_SPI0_HOLD, &stcGpioCfg);  // HOLD

    Gpio_SetIO(PORT_SPI0_WP, PIN_SPI0_WP);
    Gpio_SetIO(PORT_SPI0_HOLD, PIN_SPI0_HOLD);

    stcGpioCfg.enDir  = GpioDirIn;
    stcGpioCfg.enPuPd = GpioNoPuPd;
    Gpio_Init(     PORT_SPI0_MISO, PIN_SPI0_MISO, &stcGpioCfg);// MISO
    Gpio_SetAfMode(PORT_SPI0_MISO, PIN_SPI0_MISO, GpioAf1);
}

void spi_function_init(void)
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralSpi0, TRUE);

    stc_spi_config_t SPIConfig;
    DDL_ZERO_STRUCT(SPIConfig);

    Spi_SetCS(Spi0, TRUE);          // idle CS is high

    SPIConfig.bCPHA = Spicphafirst; // first SCLk edge to get data
    SPIConfig.bCPOL = Spicpollow;   // idle SCLK = 0
    SPIConfig.bMasterMode = SpiMaster;
    SPIConfig.u8BaudRate = SpiClkDiv2;
    SPIConfig.bIrqEn = FALSE;
    SPIConfig.pfnSpi1IrqCb = NULL;

    Spi_Init(Spi0, &SPIConfig);
}

void spi_init(void)
{
    spi_io_init();
    spi_function_init();
}

void spi_write_byte(uint8_t data)
{
    while(0 == M0P_SPI0->STAT_f.TXE);

    M0P_SPI0->DATA = data;

    while(0 == M0P_SPI0->STAT_f.RXNE);

    data = M0P_SPI0->DATA;
}

uint8_t spi_read_byte(void)
{
    uint8_t data;

    while(0 == M0P_SPI0->STAT_f.TXE);

    M0P_SPI0->DATA = 0x00;

    while(0 == M0P_SPI0->STAT_f.RXNE);

    data = M0P_SPI0->DATA;

    return data;
}


uint8_t spi_rw_data(uint8_t data)
{
    uint8_t retry = 0;

//    M0P_SPI0->SSN = 0;

    while (M0P_SPI0->STAT_f.TXE == 0) { //检查发送缓存空标志位设置与否
        retry++;

        if(retry > 200) {
            return 0;
        }
    }

    M0P_SPI0->DATA = data;

    retry = 0;

    while (M0P_SPI0->STAT_f.RXNE == 0) {//检查接受缓存非空标志位设置与否
        retry++;

        if(retry > 200) {
            return 0;
        }
    }

//    M0P_SPI0->SSN = 1;

    return M0P_SPI0->DATA; //返回通过SPI1最近接收的数据
}

void spi_write_buff(uint8_t *buff, uint8_t length)
{
    uint8_t i;
    bSendIrq = 0;
    bRecvIrq = 0;

    for(i = 0; i < length; i++) {
        u8SendData = *(buff + i);
        Spi_FuncEn(Spi1, SpiRxNeIe, TRUE);
        Spi_FuncEn(Spi1, SpiTxEIe, TRUE);

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

    for(i = 0; i < length; i++) {
        Spi_FuncEn(Spi1, SpiRxNeIe, TRUE);
        u32TimeOut = 1000;
        M0P_SPI0->DATA = 0; //接收数据时，用于主机产生时钟

        while(u32TimeOut--) {
            if(1 == bRecvIrq) {
                *(buff + i) = u8IrqData;
                break;
            }
        }

        bRecvIrq = 0;
    }
}

