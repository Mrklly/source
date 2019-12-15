#include "ht1621.h"




void HT1621_WR_Data(u8 Data,u8 cnt)
{
    u8  i;
    for(i=0; i<cnt; i++) {
        CLS_HT_WR;
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        if((Data & 0x80)==0x80) {
            SET_HT_DAT;
        } else {
            CLS_HT_DAT;
        }
        SET_HT_WR;
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        Data<<=1;
    }
}

void HT1621_WR_OneByte(u8 Addr,u8 Data)
{
    CLS_HT_CS;
    HT1621_WR_Data(0xa0,3);
    HT1621_WR_Data(Addr<<2,6);
    HT1621_WR_Data(Data,8);
    SET_HT_CS;
}

void HT1621_WR_TwoByte(u8 Addr,u16 Data)
{
    CLS_HT_CS;
    HT1621_WR_Data(0xa0,3);
    HT1621_WR_Data(Addr<<2,6);
    HT1621_WR_Data((Data >> 4),8);
    HT1621_WR_Data((u8)Data,8);
    SET_HT_CS;
}
void HT1621_CLR_LCD(void)
{
    u8 i;
    CLS_HT_CS;
    HT1621_WR_Data(0xa0,3);
    HT1621_WR_Data(0x00<<2,6);
    for (i=0; i<16; i++) {
        HT1621_WR_Data(0x00,8);
    }
    SET_HT_CS;
}

void HT1621_WR_Cmd(u8 Cmd)
{
    CLS_HT_CS;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    HT1621_WR_Data(0x80,4);
    HT1621_WR_Data(Cmd,8);
    SET_HT_CS;
}


void Ht1621_Init()
{
    GPIO_Init(HT1621_CS_PORT,HT1621_CS_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );
    GPIO_Init(HT1621_WR_PORT,HT1621_WR_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );
    GPIO_Init(HT1621_DAT_PORT,HT1621_DAT_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );
    GPIO_Init(HT1621_DEV_PORT,HT1621_DEV_PIN,GPIO_MODE_OUT_PP_HIGH_FAST );
    GPIO_WriteHigh(HT1621_DEV_PORT,HT1621_DEV_PIN);
    SET_HT_CS;
    SET_HT_WR;
    SET_HT_DAT;
    delay_ms(5);
    HT1621_WR_Cmd(BIAS);
    HT1621_WR_Cmd(RC256);
    HT1621_WR_Cmd(SYSDIS);
    HT1621_WR_Cmd(WDTDIS);
    HT1621_WR_Cmd(SYSEN);
    HT1621_WR_Cmd(LCDON);
    delay_ms(10);
    HT1621_CLR_LCD();
}

