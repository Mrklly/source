#ifndef __HT1621_H
#define	__HT1621_H


#include "global_variables.h"


#define BIAS   0x52            //0b1000 0101 0010  1/3duty 4com
#define SYSDIS 0X00           //0b1000 0000 0000  关振系统荡器和LCD偏压发生器
#define SYSEN  0X02           //0b1000 0000 0010 打开系统 振荡器
#define LCDOFF 0X04           //0b1000 0000 0100  关LCD偏压
#define LCDON  0X06           //0b1000 0000 0110  打开LCD偏 压
#define XTAL   0x28           //0b1000 0010 1000 外部接时钟
#define RC256  0X30           //0b1000 0011 0000  内部时钟RC256K
#define TONEON  0X12           //0b1000 0001 0010  打开声音 输出
#define TONEOFF 0X10           //0b1000 0001 0000 关闭声音输出
#define WDTDIS  0X0A        //0b1000 0000 1010  禁止看门狗
#define TONE_4K 0x80        //4k声音输出频率
#define TONE_2K 0xC0

/* HT1621 */


#define HT1621_DEV_PORT		GPIOC
#define HT1621_DEV_PIN		GPIO_PIN_4

#define HT1621_CS_PORT		GPIOC
#define HT1621_CS_PIN		GPIO_PIN_3

#define HT1621_WR_PORT		GPIOC
#define HT1621_WR_PIN		GPIO_PIN_2

#define HT1621_DAT_PORT		GPIOC
#define HT1621_DAT_PIN		GPIO_PIN_1


extern void HT1621_WR_OneByte(u8 Addr,u8 Data);
extern void Ht1621_Init(void); 						// - - 初始化 HT1621
extern void Ht1621WrAllData(u8 Addr,u8 *p,u8 cnt); // - - HT1621 连续写入数据函数
extern void HT1621_WR_Cmd(u8 Cmd);
extern void HT1621_WR_TwoByte(u8 Addr,u16 Data) ;

extern void Ht1621_delay_10us(u16 n); 				// - - 10微秒的延时子程序
extern void Ht1621_delayms(u16 xms); 				// - - 1毫秒延时程序


#define 	HT1621_CS_H			HT1621_CS_PORT->ODR |= (uint8_t)HT1621_CS_PIN;  // GPIO_WriteHigh(HT1621_CS_PORT,HT1621_CS_PIN);
#define		HT1621_WR_H			HT1621_WR_PORT->ODR |= (uint8_t)HT1621_WR_PIN;  // GPIO_WriteHigh(HT1621_WR_PORT,HT1621_WR_PIN);
#define		HT1621_DAT_H		HT1621_DAT_PORT->ODR |= (uint8_t)HT1621_DAT_PIN;  // GPIO_WriteHigh(HT1621_DAT_PORT,HT1621_DAT_PIN);

#define 	HT1621_CS_L			HT1621_CS_PORT->ODR &= (uint8_t)(~HT1621_CS_PIN); // GPIO_WriteLow(HT1621_CS_PORT,HT1621_CS_PIN);
#define		HT1621_WR_L			HT1621_WR_PORT->ODR &= (uint8_t)(~HT1621_WR_PIN); // GPIO_WriteLow(HT1621_WR_PORT,HT1621_WR_PIN);
#define		HT1621_DAT_L		HT1621_DAT_PORT->ODR &= (uint8_t)(~HT1621_DAT_PIN); // GPIO_WriteLow(HT1621_DAT_PORT,HT1621_DAT_PIN);

#define 	SET_HT_DAT			HT1621_DAT_H
#define 	SET_HT_WR			HT1621_WR_H
#define 	SET_HT_CS			HT1621_CS_H
#define 	CLS_HT_DAT			HT1621_DAT_L
#define 	CLS_HT_WR			HT1621_WR_L
#define 	CLS_HT_CS			HT1621_CS_L


//#define 	HI1621_LCD_OFF() 	HT1621_WR_Cmd(LCDOFF);

#endif /* __HT1621_H */


