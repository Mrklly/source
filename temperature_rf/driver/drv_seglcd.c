#include "drv_seglcd.h"
#include "led.h"

void seglcd_port_clear(void)
{
    Gpio_SetAnalogMode(GpioPortA, GpioPin9);//COM0
    Gpio_SetAnalogMode(GpioPortA, GpioPin10);//COM1
    Gpio_SetAnalogMode(GpioPortA, GpioPin11);//COM2
    Gpio_SetAnalogMode(GpioPortA, GpioPin12);//COM3

    Gpio_SetAnalogMode(GpioPortA, GpioPin8);//SEG0
    Gpio_SetAnalogMode(GpioPortC, GpioPin9);//SEG1
    Gpio_SetAnalogMode(GpioPortC, GpioPin8);//SEG2
    Gpio_SetAnalogMode(GpioPortC, GpioPin7);//SEG3
    Gpio_SetAnalogMode(GpioPortC, GpioPin6);//SEG4

    Gpio_SetAnalogMode(GpioPortB, GpioPin11);//SEG5
    Gpio_SetAnalogMode(GpioPortB, GpioPin10);//SEG6
    Gpio_SetAnalogMode(GpioPortB, GpioPin2);//SEG7
    Gpio_SetAnalogMode(GpioPortB, GpioPin0);//SEG8
    Gpio_SetAnalogMode(GpioPortC, GpioPin5);//SEG9
    Gpio_SetAnalogMode(GpioPortC, GpioPin4);//SEG10
    Gpio_SetAnalogMode(GpioPortA, GpioPin7);//SEG11
    Gpio_SetAnalogMode(GpioPortA, GpioPin6);//SEG12
    Gpio_SetAnalogMode(GpioPortA, GpioPin5);//SEG13
    Gpio_SetAnalogMode(GpioPortA, GpioPin4);//SEG14
    Gpio_SetAnalogMode(GpioPortA, GpioPin3);//SEG15

    Gpio_SetAnalogMode(GpioPortB, GpioPin3);//VLCDH
    Gpio_SetAnalogMode(GpioPortB, GpioPin4);//VLCD3
    Gpio_SetAnalogMode(GpioPortB, GpioPin5);//VLCD2
    Gpio_SetAnalogMode(GpioPortB, GpioPin6);//VLCD1
}

void seglcd_init(void)
{
    Sysctrl_ClkSourceEnable(SysctrlClkRCL,TRUE);
    Sysctrl_SetRCLTrim(SysctrlRclFreq32768);

    LCD_BACKLIGHT_ON;

    stc_lcd_config_t stcLcdCfg;
    stc_lcd_segcompara_t stcSegComPara;
    stc_lcd_segcom_t stcLcdSegCom;

    DDL_ZERO_STRUCT(stcLcdCfg);
    DDL_ZERO_STRUCT(stcSegComPara);
    DDL_ZERO_STRUCT(stcLcdSegCom);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralLcd,TRUE);

    seglcd_port_clear();//和LCD相关端口都要配置为模拟端口

    stcSegComPara.enBiasSrc = LcdExtCap;
    stcSegComPara.enDuty = LcdDuty4;
    stcSegComPara.u8MaxSeg = 21;
    LCD_GetSegCom(&stcSegComPara,&stcLcdSegCom);//根据DUTY及SRC获取COM端口配置

    for(uint8_t i=0;i<stcSegComPara.u8MaxSeg;i++) {
        if((i == 5) || (i == 6) || (i == 12)) {
            continue ;
        }
        stcLcdSegCom.u32Seg0_31 |= 1<<i;
    }
    stcLcdSegCom.u32Seg0_31 = ~stcLcdSegCom.u32Seg0_31; //超过32seg的由用户去设置stcLcdSegCom结构体中的seg32_39

    LCD_SetSegCom(&stcLcdSegCom);//comseg口配置

    stcLcdCfg.enDispMode = LcdMode0;
    stcLcdCfg.enBias = LcdBias3;
    stcLcdCfg.enBiasSrc = LcdExtCap;
    stcLcdCfg.enClk = LcdRCL;
    stcLcdCfg.enDuty = LcdDuty4;
    stcLcdCfg.enCpClk = LcdClk8k;
    stcLcdCfg.enScanClk = LcdClk256hz;
    LCD_Init(&stcLcdCfg);

    LCD_EnFunc(LcdEn,TRUE);
    LCD_ClearDisp();

//    uint32_t data = 1u;
//    uint8_t ram = 0, i = 0;
//
//    while(1) {
//        data = 1u;
//        data <<= i;
//        LCD_WriteRam0_7Int32(ram, data);
//        delay_ms(500);
//        i++;
//        LCD_ClearDisp();
//    }

//    LCD_FullDisp();
}
