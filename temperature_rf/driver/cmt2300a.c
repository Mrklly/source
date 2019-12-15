#include "cmt2300a.h"
#include "spi_soft.h"
#include "spi.h"
#include "led.h"
#include "global_variables.h"

volatile _rf433_status rf433_status;

void Gpio_IRQHandler(uint8_t u8Param)
{
    uint8_t data;
    if(SPI1_INT_PORT == u8Param) {    // GPIO_PORT select
        if(TRUE == Gpio_GetIrqStatus(SPI1_INT_PORT, SPI1_INT_PIN)) {

            Gpio_ClearIrq(SPI1_INT_PORT, SPI1_INT_PIN);

            if(rf433_status == RF433_TX) {
                data = bIntSrcFlagClr();                              // read out interrupt flag and clear
                bGoStandby();
                vIntSrcCfg(INT_FIFO_TH_RX, INT_FIFO_TH_RX);           // set INT2 active with RX fifo reaching threshold
                bGoRx();
                rf433_status = RF433_RX;
            } else if(rf433_status == RF433_RX) {
                uint8_t temp_rec;
                vEnableRdFifo();
                temp_rec = FIFO_ReadByte();
                for(uint8_t i=0; i<temp_rec; i++) {
                    mm[i] = FIFO_ReadByte();
                }
                bIntSrcFlagClr();
                vClearFIFO();
                rf433_flag = 1;
            }
        }
    }
}

void rf433_set_channel(uint8_t channel)
{
    bGoStandby();
    Reg_WriteByte(CMT23_FREQ_CHNL, channel);
    bGoRx();
}

const uint8_t CMTBank[96] = { // // 软件自动配置生成的数据,用于初始化模块。模块上电进入睡眠模式。
    0x00,0x66,0xEC,0x1D,0x70,0x80,0x14,0x08,0x91,0x02,0x02,0xD0,0xAE,0xE0,0x75,0x00,
    0x00,0xF4,0x10,0xE2,0x42,0x20,0x20,0x81,0x42,0x71,0xCE,0x1C,0x42,0x5B,0x1C,0x1C,
    0x79,0x2E,0x11,0x10,0xDA,0x13,0x0C,0x0A,0x9F,0x4B,0x29,0x29,0xC0,0x62,0x03,0x53,
    0x10,0x00,0xB4,0x00,0x00,0x01,0x00,0x00,0x12,0x10,0x00,0x55,0x04,0x00,0x00,0x00,
    0x00,0x00,0xD4,0x2D,0xAF,0x01,0x1F,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x60,
    0xFF,0x00,0x00,0x20,0x10,0x50,0x9A,0x0C,0x00,0x04,0x50,0x00,0x8A,0x18,0x3F,0x7F
};

void cmt2300_init(void)
{
    uint8_t i;
    volatile uint8_t temp=0;
    Reg_WriteByte(0x7f, 0xff);            //0x7f,地址写0xff,软件复位
    delay_ms(30);                        //延时20MS
    Reg_WriteByte(0x60, 0x02);            //发送go_stby 进入STBY状态
    for(i=0; i<100; i++) {
        delay_us(500);                      //延时300us
        temp = (0x0f & Reg_ReadByte(0x61));     //从0x61读出状态，0x61是芯片当前状态。
        if(temp == 0x02) {
            break;                //进入到STBY状态，提前结束。
        }
    }
    /*以上确定进入配置寄存器状态*/
    temp = Reg_ReadByte(0x61);              //读状态
    temp|= EEP_CPY_DIS;
    temp &= (~RSTN_IN_EN);			//Disable RstPin
    Reg_WriteByte(0x61,temp);             //0x61 写进tmp
    bIntSrcFlagClr();                     //读出中断标志，清除中断
    for(i=0; i<96; i++) {
        Reg_WriteByte(i, CMTBank[i]);
    }

//    while(1) {
//
//        FIFO_WriteByte(0x5a);
//        temp = FIFO_ReadByte();
//        printf("temp = %02x\n", temp);
//
//        delay_ms(50);
//
//        FIFO_WriteByte(0xa5);
//        temp = FIFO_ReadByte();
//        printf("temp = %02x\n", temp);
//
//        delay_ms(50);
//
//    }

    Reg_WriteByte(CMT23_FREQ_OFS, CHANNEL_WIDTH);                          //设置跳频宽度。
    Reg_WriteByte(CMT23_FREQ_CHNL, g_dev_mac.channel);                  //设置跳频频道编号。
    vEnableAntSwitch(0x01);                                       //设置中断极性和增益，GPIO1,2的性置
    vSetTxPayloadLength(RF433_DEFAULT_TX_SIZE);
//    CMT2300A_SetFifoThreshold(RF433_DEFAULT_RX_SIZE);
    Reg_WriteByte(CMT23_IO_SEL,(GPIO1_DCLK+GPIO2_DCLK+GPIO3_INT2+GPIO4_DOUT));
    vIntSrcCfg(INT_FIFO_WBYTE_RX, INT_CRC_PASS);                  //配置IO口中断
    Reg_WriteByte(CMT23_INT_EN, (PKT_DONE_EN+CRC_PASS_EN+PREAMBLE_PASS_EN+SYNC_PASS_EN+TX_DONE_EN));
    vClearFIFO();                                         //清空FIFO
    bGoSleep();                                           //进入睡眠状态，并且保存以前的配置
    bGoRx();                                              //进入接收状态
    delay_ms(1);
    rf433_status = RF433_RX;
}

void Reg_WriteByte(uint8_t addre, uint8_t data)
{
    addre &= 0x7f;

    SPI_SOFT_FCSB_H;
    SPI_SOFT_CSB_H;
    SPI_SOFT_SCLK_L;
    SPI_SOFT_CSB_L;
    Spi_SetCS(Spi1, FALSE);

    delay_us(2);

#if SPI_SOFT

    spi_soft_write_byte(addre);
    spi_soft_write_byte(data);

#else

    spi_write_byte(addre);
    spi_write_byte(data);

#endif

    delay_us(2);
    Spi_SetCS(Spi1, TRUE);
    SPI_SOFT_FCSB_H;
    SPI_SOFT_CSB_H;
}

uint8_t Reg_ReadByte(uint8_t addre)
{
    uint8_t data=0;

    addre |= 0x80;

    SPI_SOFT_FCSB_H;
    SPI_SOFT_CSB_H;
    SPI_SOFT_SCLK_L;
    SPI_SOFT_CSB_L;
    Spi_SetCS(Spi1, FALSE);

    delay_us(2);

#if SPI_SOFT

    spi_soft_write_byte(addre);

    data = spi_soft_read_byte();

#else

    spi_write_byte(addre);

    data = spi_read_byte();


#endif

    Spi_SetCS(Spi1, TRUE);
    SPI_SOFT_FCSB_H;
    SPI_SOFT_CSB_H;

    return data;
}

void FIFO_WriteByte(uint8_t data)
{
    SPI_SOFT_FCSB_H;
    SPI_SOFT_CSB_H;
    SPI_SOFT_FCSB_L;
    Spi_SetCS(Spi1, FALSE);

    delay_us(2);

#if SPI_SOFT

    spi_soft_write_byte(data);

#else

    spi_write_byte(data);

#endif

    delay_us(2);

    Spi_SetCS(Spi1, TRUE);
    SPI_SOFT_FCSB_H;
    SPI_SOFT_CSB_H;
    delay_us(5);
}

uint8_t FIFO_ReadByte(void)
{
    uint8_t data=0;

    SPI_SOFT_SCLK_L;
    SPI_SOFT_FCSB_H;
    SPI_SOFT_CSB_H;
    SPI_SOFT_FCSB_L;
    Spi_SetCS(Spi1, FALSE);

    delay_us(2);

#if SPI_SOFT

    data = spi_soft_read_byte();

#else

    data = spi_read_byte();

#endif

    delay_us(4);

    Spi_SetCS(Spi1, TRUE);
    SPI_SOFT_FCSB_H;
    SPI_SOFT_CSB_H;
    delay_us(2);

    return data;
}

void FIFO_Write_x(uint8_t *data, uint8_t length)
{
	uint8_t i;
	if(length != 0) {
        for(i=0; i<length; i++) {
            FIFO_WriteByte(data[i]);
        }
	}
}

void FIFO_Read_x(uint8_t *data, uint8_t length)
{
	uint8_t i;
	if(length != 0) {
		for(i=0; i<length; i++) {
            data[i] = FIFO_ReadByte();
        }
	}
}

uint8_t bIntSrcFlagClr()
{
    uint8_t tmp, int_clr2 = 0, int_clr1 = 0, flg = 0;

    tmp = Reg_ReadByte(CMT23_INT_FLG);            //读出中断标志
    if(tmp&LBD_STATUS_FLAG) {	        //LBD_FLG_Active
        int_clr2 |= LBD_CLR;
    }

    if(tmp&PREAMBLE_PASS_FLAG) {	        //Preamble Active
        int_clr2 |= PREAMBLE_PASS_CLR;
        flg |= PREAMBLE_PASS_EN;
    }
    if(tmp&SYNC_PASS_FLAG) {		//Sync Active
        int_clr2 |= SYNC_PASS_CLR;
        flg |= SYNC_PASS_EN;
    }
    if(tmp&NODE_PASS_FLAG) {		//Node Addr Active
        int_clr2 |= NODE_PASS_CLR;
        flg |= NODE_PASS_EN;
    }
    if(tmp&CRC_PASS_FLAG) {		        //Crc Pass Active
        int_clr2 |= CRC_PASS_CLR;
        flg |= CRC_PASS_EN;
    }
    if(tmp&RX_DONE_FLAG) {		        //Rx Done Active
        int_clr2 |= RX_DONE_CLR;
        flg |= PKT_DONE_EN;
    }
    if(tmp&COLLISION_ERR_FLAG) {	        //这两个都必须通过RX_DONE清除
        int_clr2 |= RX_DONE_CLR;
    }
    if(tmp&DC_ERR_FLAG) {
        int_clr2 |= RX_DONE_CLR;
    }
    Reg_WriteByte(0x6b,int_clr2);	                //Clear flag
    tmp = Reg_ReadByte(CMT23_INT_CLR1);           //0X6A
    if(tmp&TX_DONE_FLAG) {
        int_clr1 |= TX_DONE_CLR;
        flg |= TX_DONE_EN;
    }
    if(tmp&SLEEP_TIMEOUT_FLAG) {
        int_clr1 |= SLEEP_TIMEOUT_CLR;
        flg |= SLEEP_TMO_EN;
    }
    if(tmp&RX_TIMEOUT_FLAG) {
        int_clr1 |= RX_TIMEOUT_CLR;
        flg |= RX_TMO_EN;
    }
    Reg_WriteByte(0x6a,int_clr1);	                //Clear flag

    //	Reg_WriteByte(0x6b,0xff); //清除所有标志
    //	Reg_WriteByte(0x6a,0x07); //清除所有标志
    return(flg);
}

void Wireless_reception_control (void)
{
    uint8_t i,temp;
    if(SPI_SOFT_INT_IN_DATA != 0) {
        vEnableRdFifo();                            //允许读
        temp = FIFO_ReadByte();                       //读一个字节
        for(i=0; i<temp; i++) {
            mm[i] = FIFO_ReadByte();     //读出多个字节
        }
        bIntSrcFlagClr();
        vClearFIFO();                               //清除
        Reg_ReadByte(CMT23_MODE_STA);               //读状态
        rf433_flag = 1;
    }
}

void vEnableRdFifo(void)
{
    uint8_t tmp;
    tmp = Reg_ReadByte(CMT23_FIFO_CTL);
    tmp &= (~(SPI_FIFO_RD_WR_SEL|FIFO_RX_TX_SEL));
    Reg_WriteByte(CMT23_FIFO_CTL,tmp);
}


uint8_t vClearFIFO(void)
{
    uint8_t tmp;
    tmp = Reg_ReadByte(CMT23_FIFO_FLG); //0x6e
    Reg_WriteByte(CMT23_FIFO_CLR,(FIFO_CLR_RX+FIFO_CLR_TX));
    return(tmp);
}

void rf433_tx(uint8_t *buf, uint8_t len)
{
    bGoStandby();
    rf433_status = RF433_STBY;

    vIntSrcCfg(INT_FIFO_WBYTE_RX, INT_FIFO_NMTY_TX);

    bIntSrcFlagClr();

    vSetTxPayloadLength(len);

    vEnableWrFifo();

    for(uint8_t i=0; i<len; i++) {
        FIFO_WriteByte(buf[i]);
    }

    rf433_status = RF433_TX;
    bGoTx();

#if SPI_SOFT
    while(SPI_SOFT_INT_IN_DATA) {
        delay_ms(1);
    }
	bIntSrcFlagClr();
	bGoStandby();
	vIntSrcCfg(INT_FIFO_WBYTE_RX, INT_CRC_PASS);
	bGoRx();
    delay_ms(1);
#endif
}

uint8_t rf433_rx(uint8_t *buf, uint8_t len)
{
    uint8_t i;
    if(!SPI_SOFT_INT_IN_DATA) {
        return 0;
    }

    memset(buf, 0, len);

    vEnableRdFifo();
	uint8_t temp = FIFO_ReadByte();

	for(i=0; i<len; i++) {
        buf[i] = FIFO_ReadByte();
    }

	bIntSrcFlagClr();
	vClearFIFO();

    return 1;
}

void Wireless_rx(uint8_t x, uint8_t y)
{
    uint8_t i, temp[10]= {0,0,0,0,0,0,0,0,0,0};        //6个字节本机地址+2个字节操作内容+2个字节校验码。
    uint16_t crc;
    for(i=0; i<6; i++) {
        temp[i] = i;    //装载本机地址6个字节
    }
    temp[6] = x;                                            //装载功能码。
    temp[7] = y;                                            //装载状态。

    crc=getCRCByMsg(temp,8);                              //得到CRC校验

    temp[8]=(crc>>8);                                     //装载校验码。
    temp[9]=crc&0xff;                                     //装载校验码。

    bGoStandby();                                         //进入配置模式
    rf433_status = RF433_STBY;
    vIntSrcCfg(INT_FIFO_WBYTE_RX, INT_TX_DONE);      //配置IO口为发送完中断

    bIntSrcFlagClr();                                     //读出中断标志并清中断

    vSetTxPayloadLength(10);                              //先择发送个数
    vEnableWrFifo();	                                //对fifo是 写操作
    for(i=0; i<10; i++) {
        FIFO_WriteByte(temp[i]);    //发送多个字节
    }
    bGoTx();                                              //进入发射模式
    rf433_status = RF433_TX;
//    Gpio_EnableIrq(SPI1_INT_PORT, SPI1_INT_PIN, GpioIrqFalling);

//    while(SPI_SOFT_INT_IN_DATA != 0) {
//        delay_ms(1);    //等待发送完成
//    }

//    bIntSrcFlagClr();                                     //读出中断标志并清中断
//    bGoStandby();                                         //进入配置模式
//    vIntSrcCfg(INT_FIFO_WBYTE_RX,INT_CRC_PASS);           //配置IO口为接收完成中断
//    bGoRx();                                              //进入接收状态
//    rf433_status = RF433_RX;

}

//根据CRC校验的基本原理,并行计算任意长度CRC校验值的算法
// CRC 高位字节值表
const uint8_t auchCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

//CRC 低位字节值表
const uint8_t auchCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

uint16_t getCRCByMsg(uint8_t *sendBArry, uint16_t len)
{
    uint8_t uchCRCHi = 0xFF, uchCRCLo = 0xFF, uIndex = 0;       // CRC 字节初始化
    uint16_t i;                 // CRC 循环中的索引
    for( i = 0; i<len; i++) {
        uIndex = uchCRCHi ^ sendBArry[i];    // 计算CRC
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
        uchCRCLo = auchCRCLo[uIndex] ;
    }
    return (uchCRCHi << 8 | uchCRCLo);
}

uint8_t bGoStandby(void)
{
    uint8_t tmp, i;
    // RssiTrig = 0x00;
    Reg_WriteByte(CMT23_MODE_CTL,MODE_GO_STBY);	//0x60写0x02
    for(i=0; i<50; i++) {
//        delay_us(200);
        tmp = (MODE_MASK_STA & Reg_ReadByte(CMT23_MODE_STA));
        if(tmp==MODE_STA_STBY) {
            break;
        }
    }
    if(i>=50) {
        return 0x00;
    } else {
        return 0x01;
    }
}

void vIntSrcCfg(uint8_t int_1, uint8_t int_2)
{
    uint8_t tmp;
    tmp = INT_MASK & Reg_ReadByte(CMT23_INT1_CTL); //读出0x66
    Reg_WriteByte(CMT23_INT1_CTL,(tmp|int_1));
    //0XEA 为INT_MASK
    tmp = INT_MASK & Reg_ReadByte(CMT23_INT2_CTL);//读出OX67
    Reg_WriteByte(CMT23_INT2_CTL,(tmp|int_2));
}

void vSetTxPayloadLength(uint8_t length)
{
    uint8_t tmp, len;
    bGoStandby(); //进入配置模式
    tmp = Reg_ReadByte(CMT23_PKT_CTRL1);
    tmp&= 0x8F;
    /*
    if(length!=0)
    {
    if(FixedPktLength)
    len = length-1;
    else
    len = length;
    }
    else
    len = 0;
    */
    len=length;
    tmp|= (((uint8_t)(len>>8)&0x07)<<4);
    Reg_WriteByte(CMT23_PKT_CTRL1,tmp);
    Reg_WriteByte(CMT23_PKT_LEN,len);	//Payload length
    //bGoSleep();
}

void vEnableWrFifo()
{
    uint8_t tmp;
    tmp = Reg_ReadByte(CMT23_FIFO_CTL);                   //0x69
    tmp |= (SPI_FIFO_RD_WR_SEL|FIFO_RX_TX_SEL);           //说明是写操作
    Reg_WriteByte(CMT23_FIFO_CTL,tmp);
}

uint8_t bGoTx(void)
{
    uint8_t i, tmp;

    Reg_WriteByte(CMT23_MODE_CTL,MODE_GO_TFS);	        //0x60地址写入0x04,进入配置状态
    for(i=0; i<50; i++) {
        delay_us(200);
        tmp = (MODE_MASK_STA & Reg_ReadByte(CMT23_MODE_STA));
        if(tmp==MODE_STA_TFS) {
            break;
        }
    }
    if(i>=50) {
        return 0x0;
    }

    Reg_WriteByte(CMT23_MODE_CTL,MODE_GO_TX);	        //进入发送状态
    for(i=0; i<50; i++) {
        delay_us(200);
        tmp = (MODE_MASK_STA & Reg_ReadByte(CMT23_MODE_STA));
        if(tmp==MODE_STA_TX) {
            break;
        }
    }
    if(i>=50) {
        return 0x0;
    } else {
        return 0x01;
    }
}


uint8_t bGoRx(void)
{
    uint8_t ii, tmp;
    //   RssiTrig = 0;
    Reg_WriteByte(CMT23_MODE_CTL,MODE_GO_RFS);	                //0x60地址写入0x04,进入配置状态
    for(ii=0; ii<50; ii++) {
//        delay_us(300);
        tmp = (MODE_MASK_STA & Reg_ReadByte(CMT23_MODE_STA));	//0x61读出状态
        if(tmp==MODE_STA_RFS) {
            break;
        }
    }
    if(ii>=50) {
        return 0x0;
    }

    Reg_WriteByte(CMT23_MODE_CTL,MODE_GO_RX);	                //进入接收状态
    for(ii=0; ii<50; ii++) {
        delay_us(300);
        tmp = (MODE_MASK_STA & Reg_ReadByte(CMT23_MODE_STA));	//读出是不是接收状态
        if(tmp==MODE_STA_RX) {
            break;
        }
    }
    if(ii>=50) {
        return 0x0;
    } else {
        return 0x01;
    }
}

void vEnableAntSwitch(uint8_t mode)
{
    uint8_t tmp;
    tmp = Reg_ReadByte(CMT23_INT1_CTL);
    tmp&= 0x3F;
    switch(mode) {
    case 1:
        tmp |= RF_SWT1_EN;
        break;		//GPO1=RxActive; GPO2=TxActive
    case 2:
        tmp |= RF_SWT2_EN;
        break;		//GPO1=RxActive; GPO2=!RxActive
    case 0:
    default:
        break;				//Disable
    }
    Reg_WriteByte(0x66,tmp);
}

uint8_t bGoSleep(void)
{
    uint8_t tmp;
    Reg_WriteByte(CMT23_MODE_CTL,MODE_GO_SLEEP);
    delay_us(100);
    tmp = (MODE_MASK_STA & Reg_ReadByte(CMT23_MODE_STA));
    if(tmp==MODE_GO_SLEEP) {
        return 1;
    } else {
        return 0x0;
    }
}

void CMT2300A_SetFifoThreshold(uint8_t nFifoThreshold)
{
    uint8_t tmp = Reg_ReadByte(CMT23_FIFO_TH);

    tmp &= ~0x7F;
    tmp |= nFifoThreshold & 0x7F;

    Reg_WriteByte(CMT23_FIFO_TH, tmp);
}

