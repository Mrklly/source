#include "drv_seglcd.h"
#include "drv_led.h"

void seglcd_io_init(void)
{
    Gpio_SetAnalogMode(GpioPortA, GpioPin9); //COM0
    Gpio_SetAnalogMode(GpioPortA, GpioPin10);//COM1
    Gpio_SetAnalogMode(GpioPortA, GpioPin11);//COM2
    Gpio_SetAnalogMode(GpioPortA, GpioPin12);//COM3

    Gpio_SetAnalogMode(GpioPortA, GpioPin8);//SEG0
    Gpio_SetAnalogMode(GpioPortC, GpioPin9);//SEG1
    Gpio_SetAnalogMode(GpioPortC, GpioPin8);//SEG2
    Gpio_SetAnalogMode(GpioPortC, GpioPin7);//SEG3
    Gpio_SetAnalogMode(GpioPortC, GpioPin6);//SEG4

    Gpio_SetAnalogMode(GpioPortB, GpioPin11);//SEG9
    Gpio_SetAnalogMode(GpioPortB, GpioPin10);//SEG10
    Gpio_SetAnalogMode(GpioPortB, GpioPin2); //SEG11

    Gpio_SetAnalogMode(GpioPortB, GpioPin0);//SEG13
    Gpio_SetAnalogMode(GpioPortC, GpioPin5);//SEG14
    Gpio_SetAnalogMode(GpioPortC, GpioPin4);//SEG15
    Gpio_SetAnalogMode(GpioPortA, GpioPin7);//SEG16
#if TEMP_CTRL_TOUCH
    Gpio_SetAnalogMode(GpioPortA, GpioPin6);//SEG17
    Gpio_SetAnalogMode(GpioPortA, GpioPin5);//SEG18
    Gpio_SetAnalogMode(GpioPortA, GpioPin4);//SEG19
    Gpio_SetAnalogMode(GpioPortA, GpioPin3);//SEG20
#endif

    stc_gpio_config_t stcLEDPortCfg;
    DDL_ZERO_STRUCT(stcLEDPortCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stcLEDPortCfg.enDir  = GpioDirIn;
    stcLEDPortCfg.enDrv  = GpioDrvH;
    stcLEDPortCfg.enOD   = GpioOdDisable;
    stcLEDPortCfg.enPuPd = GpioNoPuPd;
    stcLEDPortCfg.enCtrlMode = GpioFastIO;
    
    Gpio_SetAfMode(GpioPortB, GpioPin3, GpioAf0);
    Gpio_SetAfMode(GpioPortB, GpioPin4, GpioAf0);
    Gpio_SetAfMode(GpioPortB, GpioPin5, GpioAf0);
    Gpio_SetAfMode(GpioPortB, GpioPin6, GpioAf0);
    Gpio_Init(GpioPortB, GpioPin3, &stcLEDPortCfg);
    Gpio_Init(GpioPortB, GpioPin4, &stcLEDPortCfg);
    Gpio_Init(GpioPortB, GpioPin5, &stcLEDPortCfg);
    Gpio_Init(GpioPortB, GpioPin6, &stcLEDPortCfg);

// following used for ExtCap
//    Gpio_SetAnalogMode(GpioPortB, GpioPin3);//VLCDH
//    Gpio_SetAnalogMode(GpioPortB, GpioPin4);//VLCD3
//    Gpio_SetAnalogMode(GpioPortB, GpioPin5);//VLCD2
//    Gpio_SetAnalogMode(GpioPortB, GpioPin6);//VLCD1
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

    seglcd_io_init();//和LCD相关端口都要配置为模拟端口

    stcSegComPara.enBiasSrc = LcdInRes_High;
    stcSegComPara.enDuty    = LcdDuty4;
#if TEMP_CTRL_KNOB
    stcSegComPara.u8MaxSeg  = 18;
#elif TEMP_CTRL_TOUCH
    stcSegComPara.u8MaxSeg  = 21;
#endif
    LCD_GetSegCom(&stcSegComPara,&stcLcdSegCom);//根据DUTY及SRC获取COM端口配置

    for(uint8_t i=0; i<stcSegComPara.u8MaxSeg; i++) {
        if((i == 5) || (i == 6) || (i == 7) || (i == 8) || (i == 12)) {
            continue ;
        }
        stcLcdSegCom.u32Seg0_31 |= 1<<i;
    }
    stcLcdSegCom.u32Seg0_31 = ~stcLcdSegCom.u32Seg0_31; //超过32seg的由用户去设置stcLcdSegCom结构体中的seg32_39

    LCD_SetSegCom(&stcLcdSegCom);//comseg口配置

    stcLcdCfg.enDispMode = LcdMode0;
    stcLcdCfg.enBias = LcdBias3;
    stcLcdCfg.enBiasSrc = LcdInRes_High;            // LcdInRes_High LcdExtCap
    stcLcdCfg.enClk = LcdRCL;
    stcLcdCfg.enDuty = LcdDuty4;
    stcLcdCfg.enCpClk = LcdClk8k;
    stcLcdCfg.enScanClk = LcdClk512hz;
    LCD_Init(&stcLcdCfg);

//    M0P_LCD->POEN1_f.MUX = 1;

    LCD_EnFunc(LcdEn, TRUE);
    LCD_ClearDisp();
}

