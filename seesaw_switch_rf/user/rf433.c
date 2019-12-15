#include "rf433.h"
#include "global_variables.h"


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

    while(SPI_SOFT_INT_IN_DATA) {
        delay_ms(1);
    }
	bIntSrcFlagClr();
	bGoStandby();
	vIntSrcCfg(INT_FIFO_WBYTE_RX, INT_CRC_PASS);
	bGoRx();
    delay_ms(1);
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

void rf433_handler(void)
{
    uint8_t rf_buf[31];
    uint16_t crc;

    memcpy(rf_buf, g_rf_buf, 31);

    crc = getCRCByMsg(rf_buf, 29);

    if((uint8_t)(crc>>8) != rf_buf[29] || (uint8_t)crc != rf_buf[30]) {
        return ;                // crc error
    }

// rf_buf[0] = 0xAC, rf_buf[1..6] = ID[1..6], rf_buf[7] = status
    multi_control(&rf_buf[14], 7);

    if(rf_buf[12] == 0xAC) {
        
    }
}

