#include "spi_soft.h"
#include "global_variables.h"

void rf433_io_init(void)
{
    RF433_CSB_PORT->DDR |= RF433_CSB_PIN;             //CSB
    RF433_CSB_PORT->CR1 |= RF433_CSB_PIN;
    RF433_CSB_PORT->ODR |= RF433_CSB_PIN;
    
    RF433_FCSB_PORT->DDR |= RF433_FCSB_PIN;           //FCSB
    RF433_FCSB_PORT->CR1 |= RF433_FCSB_PIN;
    RF433_FCSB_PORT->ODR |= RF433_FCSB_PIN;
    
    RF433_SCLK_PORT->DDR |= RF433_SCLK_PIN;           //SCK
    RF433_SCLK_PORT->CR1 |= RF433_SCLK_PIN;
    RF433_SCLK_PORT->ODR &=~RF433_SCLK_PIN;           // output low
    
    RF433_SDIO_PORT->DDR |= RF433_SDIO_PIN;           //SDIO out
    RF433_SDIO_PORT->CR1 |= RF433_SDIO_PIN;
    RF433_SDIO_PORT->ODR &=~RF433_SDIO_PIN;
    
    RF433_INT_PORT->DDR &= ~RF433_INT_PIN;            //GPIO3 interrupt
    RF433_INT_PORT->CR1 |=  RF433_INT_PIN;
    RF433_INT_PORT->CR2 &= ~RF433_INT_PIN;
}

void Reg_WriteByte(uint8_t addre, uint8_t data)
 {
	uint8_t i;
	addre = addre & 0x7f;

	RF433_FCSB_H;
    RF433_CSB_H;
	RF433_SCLK_L;
	RF433_SDIO_OUT;
	RF433_SDIO_H;
	RF433_CSB_L;

	delay_us(2);

	for(i=0; i<8; i++) {
		if(addre & 0x80) {
			RF433_SDIO_H;
		} else {
			RF433_SDIO_L;
		}

        RF433_SCLK_H;
		addre <<= 1;
		delay_us(2);
		RF433_SCLK_L;
	}
    
    delay_us(2);
		
	for(i=0; i<8; i++) {
		if(data & 0x80) {
			RF433_SDIO_H;
		} else {
			RF433_SDIO_L;
		}
        
		RF433_SCLK_H;
		data <<= 1;
		delay_us(2);
		RF433_SCLK_L;
	}
	delay_us(2);
	RF433_FCSB_H;
    RF433_CSB_H;
 }

uint8_t Reg_ReadByte(uint8_t addre)
{
    uint8_t i, data=0;

    addre = addre | 0x80;

    RF433_FCSB_H;
    RF433_CSB_H;
    RF433_SCLK_L;
    RF433_SDIO_OUT;
    RF433_SDIO_H;
    RF433_CSB_L;

    delay_us(2);
    for(i=0; i<8; i++) {
        if(addre & 0x80) {
            RF433_SDIO_H;
        } else {
            RF433_SDIO_L;
        }

        RF433_SCLK_H;
        addre <<= 1;
        delay_us(2);
        RF433_SCLK_L;
    }

    RF433_SDIO_IN;

    for(i=0; i<8; i++) {
        data <<= 1;
        RF433_SCLK_H;
        delay_us(2);
        if(RF433_SDIO_IN_DATA) {
            data |= 0x01;
        }
        RF433_SCLK_L;
        delay_us(1);
    }

    RF433_SCLK_L;
    RF433_SDIO_OUT;
    RF433_SDIO_H;
    RF433_FCSB_H;
    RF433_CSB_H;

    return data;
}

void FIFO_WriteByte(uint8_t data)
{
    uint8_t i; 

    RF433_FCSB_H;
    RF433_CSB_H;
    RF433_SDIO_OUT;
    RF433_SDIO_H;
    RF433_SCLK_L;
    RF433_FCSB_L;

    for(i=8; i!=0; i--) {
        if(data & 0x80) {
            RF433_SDIO_H;
        } else {
            RF433_SDIO_L;
        }
        
        delay_us(2);

        RF433_SCLK_H;
        data <<= 1;
        delay_us(1);
        RF433_SCLK_L;
    }

    delay_us(3);

    RF433_FCSB_H;
    RF433_CSB_H;
    RF433_SDIO_H;
    delay_us(5);
}

uint8_t FIFO_ReadByte(void)
{
    uint8_t i, data=0;

    RF433_SCLK_L;
    RF433_FCSB_H;
    RF433_CSB_H;
    RF433_SDIO_IN;
    RF433_FCSB_L;

    delay_us(1);

    for(i=0; i<8; i++) {
        data <<= 1;
        delay_us(2);
        RF433_SCLK_H;
        delay_us(2);
        if(RF433_SDIO_IN_DATA) {
            data |= 0x01;
        }
        RF433_SCLK_L;
    }

    delay_us(3);

    RF433_FCSB_H;
    RF433_CSB_H;
    RF433_SDIO_OUT;
    delay_us(5);

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
