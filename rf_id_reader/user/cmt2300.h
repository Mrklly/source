#ifndef _CMT2300_H_
#define _CMT2300_H_

#include "stm8s.h"

/*********CMT2300引脚驱动定义**********/
#define FCSB_H          GPIOC->ODR|=1<<4            //FIFO片选。
#define FCSB_L          GPIOC->ODR&=~(1<<4)

#define CSB_H           GPIOC->ODR|=1<<6            //寄存器片选。
#define CSB_L           GPIOC->ODR&=~(1<<6)

#define SCK_H           GPIOC->ODR|=1<<5            //时钟SCK
#define SCK_L           GPIOC->ODR&=~(1<<5)

#define SDIO_H          GPIOC->ODR|=1<<7            //SPI输入输出数据
#define SDIO_L          GPIOC->ODR&=~(1<<7)

#define SDIO_V          (GPIOC->IDR&(1<<7))         //读取SPI数值
#define GPIO3           (GPIOA->IDR&(1<<2))         //读取GPIO3的值

#define GPIO_LED_L      GPIOD->ODR&=~(1<<7)    
#define GPIO_LED_H      GPIOD->ODR|=1<<7  

/*******************************************函数声明*******************************************/
#define CMT23_DUTY_CTL          0x0D
#define	DUTY_MASK	        0xE0

#define	TIMER_RX_EN	        (1<<4)
#define	TIMER_SLEEP_EN		(1<<3)
#define TX_DUTY_CYC_EN		(1<<2)
#define	RX_DUTY_CYC_EN		(1<<1)
#define	DUTY_CYC_PAUSE		(1<<0)		

#define CMT23_EXIT_STATE	0x0E

#define	EXIT_MASK		0xF0

#define EXIT_TO_SLEEP		0
#define	EXIT_TO_STBY		1
#define EXIT_TO_FS		2

#define TX_EXIT_TO_SLEEP	(0<<2)
#define TX_EXIT_TO_STBY 	(1<<2)
#define TX_EXIT_TO_FS 		(2<<2)


#define CMT23_SL_TIMER_CTL1	0x0F
#define CMT23_SL_TIMER_CTL2 	0x10
#define CMT23_RX_TIMER_CTL1	0x11
#define CMT23_RX_TIMER_CTL2 	0x12
#define CMT23_EXT_TIMER_CTL1 	0x13
#define CMT23_EXT_TIMER_CTL2 	0x14	

#define CMT23_EXT_METHOD	0x15

#define	CMT23_CDR_CTL1		0x2B


#define CDR_AVG_MASK		0x8F
#define CDR_AVG_SHIFT		4

#define CDR_MODE_MASK		0xFC
#define CDR_MODE_SHIFT		0

#define CDR_RANGE_MASK		0xF3
#define	CDR_RANGE_SHIFT		2

#define CMT23_CDR_CTL2		0x2C

#define	CDR_3RD_EN		(1<<6)
#define	CDR_4TH_EN		(1<<5)

#define CMT23_PKT1		0x38		//data mode & Preamble unit & Preamble rx size
#define	PREAM_UNIT_SET		0x04		//0:nibble;  1:byte

#define	CMT23_TX_PREAM_L 	0x39
#define	CMT23_TX_PREAM_H	0x3A
#define CMT23_PREAM_VALUE	0x3B		//

#define	CMT23_SYNC_CTL		0x3C		//SyncTol[3] & SyncSize[3] & SyncDcFree 	
#define	SYNC_MAN_CODE		0x01

#define	CMT23_SYNC_ADDR		0x44		//0x3D~0x44 8Bytes SyncValue


#define	CMT23_PKT_CTRL1		0x45		//
#define	PKT_FORMAT		0x01
#define	PKT_LSB_PRIO		0x02		//Payload Bigend or Smallend
#define	NODE_POSITION		0x04		//1:after length;   0:before length
#define	AUTO_ACK		0x08

#define	CMT23_PKT_LEN		0x46		//Low Payload length 

#define	CMT23_NODE_CTL		0x47
#define	NODE_DISABLE		0x00
#define	NODE_ONLY		0x01		//only
#define NODE_AND0		0x02		//with broadcast 0
#define	NODE_ALL		0x03		//with broadcast 0 and 1

#define	CMT23_NODE_ADDR		0x4B

#define CMT23_CRC_CTL		0x4C
#define	CRC_ENABLE		0x01

#define CRC_TYPE_CCITT		(0<<1)
#define CRC_TYPE_IBM		(1<<1)
#define CRC_TYPE_ITU		(2<<1)
#define	CRC_DATA_ONLY		(1<<3)
#define	CRC_BIT_INV		(1<<4)		//resutlt inverse
#define	CRC_SWAP		(1<<5)
#define	FEC_ENABLE		(1<<6)
#define FEC_TYPE		(1<<7)


#define	CMT23_CRC_SEEDL		0x4D
#define	CMT23_CRC_SEEDH		0x4E

#define	CMT23_PKT_CTRL2		0x4F
#define	MAN_ENABLE		(1<<0)
#define MAN_TYPE		(1<<1)
#define	WHT_ENABLE		(1<<2)

#define	WHT_TYPE_CCITT		(0<<3)
#define	WHT_TYPE_IBM		(1<<3)
#define WHT_TYPE_PN7		(2<<3)

#define WHT_PN7_SEED		(1<<5)

#define CMT23_WHT_SEED		0x50
#define	CMT23_PREFIX		0x51
#define	CMT23_TX_PKT_NUM	0x52
#define	CMT23_TX_PKT_GAP    	0x53

#define	CMT23_FIFO_TH		0x54
#define FIFO_AUTO_RESTORE_EN (1<<7)

#define	CMT23_MODE_CTL		0x60
#define	MODE_GO_EEPROM		(1<<0)			//0x01
#define	MODE_GO_STBY		(1<<1)			//0x02
#define	MODE_GO_RFS		(1<<2)			//0x04
#define	MODE_GO_RX		(1<<3)			//0x08
#define	MODE_GO_SLEEP		(1<<4)			//0x10
#define	MODE_GO_TFS		(1<<5)			//0x20
#define	MODE_GO_TX		(1<<6)			//0x40
#define	MODE_GO_SWITCH		(1<<7)			//0x80		//only use for "Tx to Rx" or "Rx to Tx"


#define	CMT23_MODE_STA		0x61	

#define RSTN_IN_EN		(1<<5)			//0x20
#define	EEP_CPY_DIS		(1<<4)			//0x10		//import

#define	MODE_MASK_STA		0x0F

#define	MODE_STA_IDLE		0x00
#define	MODE_STA_SLEEP		0x01
#define	MODE_STA_STBY		0x02
#define	MODE_STA_RFS		0x03
#define MODE_STA_TFS		0x04
#define	MODE_STA_RX		0x05
#define	MODE_STA_TX		0x06
#define	MODE_STA_EEPROM		0x07
#define	MODE_STA_TUNE		0x08


#define	CMT23_EN_CTL		0x62
#define	LD_STOP_EN		(1<<5)			//0x20
#define	LBD_STOP_EN		(1<<4)			//0x10
#define	EEP_STOP		(1<<3)			//0x08
#define	EEP_START		(1<<2)			//0x04
#define	EEP_PWRON		(1<<1)			//0x02
#define	EEP_NPROTECT		(1<<0)			//0x01

#define	CMT23_FREQ_CHNL		0x63			//Channel Number	
#define	CMT23_FREQ_OFS		0x64			//Channel OFFSET
#define CHANNEL_WIDTH           0x6E                    //2.5K

#define	CMT23_IO_SEL		0x65
#define	GPIO1_DOUT		(0<<0)			//0x00
#define GPIO1_INT1		(1<<0)			//0x01
#define	GPIO1_INT2		(2<<0)			//0x02
#define GPIO1_DCLK		(3<<0)			//0x03

#define	GPIO2_INT1		(0<<2)			//0x00
#define	GPIO2_INT2		(1<<2)			//0x04
#define	GPIO2_Dout		(2<<2)			//0x08
#define	GPIO2_DCLK		(3<<2)			//0x0C

#define	GPIO3_CLKO		(0<<4)			//0x00
#define GPIO3_DOUT		(1<<4)			//0x10
#define	GPIO3_INT2		(2<<4)			//0x20
#define	GPIO3_DCLK		(3<<4)			//0x30

#define	GPIO4_NRST		(0<<6)			//0x00
#define	GPIO4_INT1		(1<<6)			//0x40
#define	GPIO4_DOUT		(2<<6)			//0x80
#define	GPIO4_DCLK		(3<<6)			//0xC0

#define	CMT23_INT1_CTL		0x66
#define	RF_SWT1_EN 		(1<<7)			//GPIO1 会输出 RX_ACTIVE，GPIO2 会输出 TX_ACTIVE
#define RF_SWT2_EN              (1<<6)			//GPIO1 会输出 RX_ACTIVE， GPIO2 会输出 RX_ACTIVE 取反，即完全差分

#define	INT_POLAR		(1<<5)
#define	INT_MASK		0xE0

#define	CMT23_INT2_CTL		0x67
#define	TX_DATA_INV		0x20

#define	INT_RX_ACTIVE		0x00	
#define	INT_TX_ACTIVE		0x01
#define	INT_RSSI_VALID		0x02
#define	INT_PREAM_PASS		0x03
#define	INT_SYNC_PASS		0x04
#define	INT_NODE_PASS		0x05
#define	INT_CRC_PASS		0x06
#define	INT_PKT_DONE		0x07
#define	INT_SLEEP_TMO		0x08
#define	INT_RX_TMO		0x09
#define	INT_TX_DONE		0x0A
#define	INT_FIFO_NMTY_RX	0x0B
#define INT_FIFO_TH_RX		0x0C
#define	INT_FIFO_FULL_RX	0x0D
#define	INT_FIFO_WBYTE_RX	0x0E
#define	INT_FIFO_OVF_RX		0x0F
#define	INT_FIFO_NMTY_TX	0x10
#define	INT_FIFO_TH_TX		0x11
#define	INT_FIFO_FULL_TX	0x12
#define	INT_STBY_STATE		0x13
#define	INT_FS_STATE		0x14
#define	INT_RX_STATE		0x15
#define	INT_TX_STATE		0x16
#define	INT_LBD_STATE		0x17
#define	INT_TRX_ACTIVE		0x18		
#define INT_PKT_ERR_COL		0x19

#define	CMT23_INT_EN		0x68
#define	SLEEP_TMO_EN		(1<<7)		//0x80
#define	RX_TMO_EN		(1<<6)		//0x40
#define	TX_DONE_EN		(1<<5)		//0x20
#define	PREAMBLE_PASS_EN	(1<<4)		//0x10
#define	SYNC_PASS_EN		(1<<3)		//0x08
#define	NODE_PASS_EN		(1<<2)		//0x04
#define	CRC_PASS_EN		(1<<1)		//0x02
#define	PKT_DONE_EN		(1<<0)		//0x01

#define	CMT23_FIFO_CTL		0x69
#define	TX_DIN_EN		(1<<7)


#define	RX_FIFO_CLR_DIS		(1<<4)		//0: Entry Rx, auto clear fifo
#define	TX_FIFO_RD_EN		(1<<3)		//0: Tx FIFO can not be read
#define	FIFO_RX_TX_SEL		(1<<2)		//when "FIFO_SHARE_EN=1" active, 0=use for Rx; 1=use for Tx
#define	FIFO_SHARE_EN		(1<<1)		//0: not share, both 32Byte for Tx/Rx FIFO; 1: total 64Byte
#define	SPI_FIFO_RD_WR_SEL	(1<<0)		//0: SPI use to read FIFO;  1: SPI use to write FIFO;

#define CMT23_INT_CLR1		0x6A
#define	SLEEP_TIMEOUT_FLAG	(1<<5)		//0x20
#define	RX_TIMEOUT_FLAG		(1<<4)		//0x10
#define	TX_DONE_FLAG		(1<<3)		//0x08
#define	TX_DONE_CLR		(1<<2)		//0x04
#define	SLEEP_TIMEOUT_CLR	(1<<1)		//0x02
#define	RX_TIMEOUT_CLR		(1<<0)		//0x01	

#define	CMT23_INT_CLR2		0x6B
#define	LBD_CLR			(1<<5)		//0x20
#define	PREAMBLE_PASS_CLR	(1<<4)		//0x10
#define	SYNC_PASS_CLR		(1<<3)		//0x08
#define	NODE_PASS_CLR		(1<<2)		//0x04
#define	CRC_PASS_CLR		(1<<1)		//0x02
#define	RX_DONE_CLR		(1<<0)		//0x01

#define	CMT23_FIFO_CLR		0x6C
#define	FIFO_RESTORE		(1<<2)		//0x04
#define	FIFO_CLR_RX		(1<<1)		//0x02
#define	FIFO_CLR_TX		(1<<0)		//0x01

#define	CMT23_INT_FLG		0x6D
#define	LBD_STATUS_FLAG		(1<<7)		//0x80
#define	COLLISION_ERR_FLAG	(1<<6)		//0x40
#define	DC_ERR_FLAG		(1<<5)		//0x20
#define	PREAMBLE_PASS_FLAG	(1<<4)		//0x10
#define	SYNC_PASS_FLAG		(1<<3)		//0x08
#define	NODE_PASS_FLAG		(1<<2)		//0x04
#define	CRC_PASS_FLAG		(1<<1)		//0x02
#define	RX_DONE_FLAG		(1<<0)		//0x01

#define	CMT23_FIFO_FLG		0x6E
#define RX_FIFO_FULL_FLAG	(1<<6)		//0x40
#define	RX_FIFO_NMTY_FLAG	(1<<5)		//0x20
#define	RX_FIFO_THRESH_FLAG	(1<<4)		//0x10
#define	RX_FIFO_OV_FLAG		(1<<3)		//0x08	
#define	TX_FIFO_FULL_FLAG	(1<<2)		//0x04
#define	TX_FIFO_NMTY_FLAG	(1<<1)		//0x02
#define	TX_FIFO_THRESH_FLAG	(1<<0)		//0x01

#define	CMT23_RSSI_CODE		0x6F
#define	CMT23_RSSI_DBM		0x70
#define	CMT23_LBD_RESULT	0x71

#define	CMT23_EE_DAT_LOW	0x76
#define	CMT23_EE_DAT_HIG	0x77
#define	CMT23_EE_ADD_USER	0x78
#define	CMT23_EE_CTL		0x79
#define	CMT23_EE_ADD_CONF	0x7A
#define	CMT23_EE_EXT_CTL	0x7B
#define	CMT23_SOFTRST		0x7F
/*******************************************函数声明*******************************************/
void cmt2300_init(void);
void Reg_WriteByte(unsigned char addre, unsigned char data);
unsigned char Reg_ReadByte(unsigned char addre);
unsigned char  bIntSrcFlagClr();
void sdio_out();
void sdio_in();
void vEnableRdFifo(void);
unsigned char FIFO_ReadByte(void);
unsigned char vClearFIFO(void);
void Wireless_reception_control (void);
unsigned int getCRCByMsg( unsigned char * sendBArry, int len);
unsigned char bGoStandby(void);
void vIntSrcCfg(unsigned char int_1, unsigned char int_2);
void vSetTxPayloadLength(unsigned char length);
void vEnableWrFifo();
unsigned char bGoTx(void);
void FIFO_WriteByte(unsigned char data);
unsigned char bGoRx(void);
void Wireless_rx(unsigned char x,unsigned char y);
void vEnableAntSwitch(unsigned char mode) ;
unsigned char bGoSleep(void);
void app_lock_control(void);
void query_lock_ACK(void);
void query_battery_ACK(void);
void app_open_lock(void);
void awaken_lock(void);
void Generating_state(uint8_t channal,uint8_t state);
void Control_luminance (uint8_t brightness,uint8_t channal_number);
void rf_set_channel(uint8_t channel);
#endif
