#include "drv_ir.h"
#include "global_variables.h"


#define USE_TIMER_MODE1 1

volatile uint8_t ir_sta = 0;


void ir_init(void)
{
    ir_io_init();
    ir_timer0_init();
}

void ir_io_init(void)
{
    stc_gpio_config_t stcLEDPortCfg;
    DDL_ZERO_STRUCT(stcLEDPortCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stcLEDPortCfg.enDir  = GpioDirIn;
    stcLEDPortCfg.enPuPd = GpioNoPuPd;

    Gpio_Init(IR_PORT, IR_PIN, &stcLEDPortCfg);
}


#if USE_TIMER_MODE1
void ir_timer0_init(void)
{   // period = (0xffff + 1 - ARR)/(PCLK / DIV)
    uint16_t                   u16CntValue;
    stc_bt_mode1_config_t      stcBtBaseCfg;
    stc_bt_pwc_input_config_t  stcBtPwcInCfg;
    stc_gpio_config_t          stcTIM0APort;
    stc_gpio_config_t          stcTIM0BPort;

    DDL_ZERO_STRUCT(stcBtBaseCfg);
    DDL_ZERO_STRUCT(stcTIM0APort);
    DDL_ZERO_STRUCT(stcTIM0BPort);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE); //Base Timer外设时钟使能

    Gpio_SetAfMode(IR_PORT, IR_PIN, GpioAf5);

    stcBtBaseCfg.enWorkMode = BtWorkMode1;                  //定时器模式
    stcBtBaseCfg.enCT       = BtTimer;                      //定时器功能，计数时钟为内部PCLK
    stcBtBaseCfg.enPRS      = BtPCLKDiv64;                  //PCLK / 32
    stcBtBaseCfg.enOneShot  = BtPwcCycleDetect;             //PWC循环检测

    stcBtBaseCfg.pfnTim0Cb  = timer0_interrupt;                      //中断函数入口

    Bt_Mode1_Init(TIM0, &stcBtBaseCfg);                     //TIM0 的模式0功能初始化

    stcBtPwcInCfg.enTsSel  = BtTs6IAFP;                     //PWC输入选择 CHA
    stcBtPwcInCfg.enIA0Sel = BtIA0Input;                    //CHA选择IA0
    stcBtPwcInCfg.enFltIA0 = BtFltPCLKDiv64Cnt3;            //PCLK/16 3个连续有效
    //stcBtPwcInCfg.enIB0Sel = BtIB0Input;                  //CHB选择IB0
    //stcBtPwcInCfg.enFltIB0 = BtFltPCLKDiv16Cnt3;          //PCLK/16 3个连续有效

    Bt_M1_Input_Config(TIM0, &stcBtPwcInCfg);               //PWC输入设置

    Bt_M1_PWC_Edge_Sel(TIM0, BtPwcRiseToFall);

    u16CntValue = 0;
    Bt_M1_Cnt16Set(TIM0, u16CntValue);                      //设置计数初值

    Bt_ClearIntFlag(TIM0,BtUevIrq);                         //清Uev中断标志
    Bt_ClearIntFlag(TIM0,BtCA0Irq);                         //清捕捉中断标志

    Bt_Mode1_EnableIrq(TIM0, BtUevIrq);                     //使能TIM0溢出中断
    Bt_Mode1_EnableIrq(TIM0, BtCA0Irq);                     //使能TIM0捕获中断

    EnableNvic(TIM0_IRQn, IrqLevel3, TRUE);                 //TIM0中断使能

    Bt_M1_Run(TIM0);                                        //TIM0 运行。
}

uint32_t u32PwcCapValue;
uint16_t u16TIM0_CntValue;

//static uint16_t u16TIM0_OverFlowCnt;

void timer0_interrupt(void)
{

    static uint16_t u16TIM0_CapValue;

    if(TRUE == Bt_GetIntFlag(TIM0, BtUevIrq)) {

        if(ir_sta & IR_STA_DATA) {
            ir_sta |= IR_STA_END;
        } else if(ir_sta & IR_STA_END) {
            ir_sta = 0;
            ir_index = 0;
        }

//        u16TIM0_OverFlowCnt++;

//        if((ir_sta & IR_STA_SPACE) && (ir_sta & IR_STA_DATA)) {
//            Bt_M1_PWC_Edge_Sel(TIM0, BtPwcFallToRise);
//            ir_sta = 0;
//        } else {
//            ir_index = 0;
//        }

        Bt_ClearIntFlag(TIM0,BtUevIrq);

        return ;
    }

    if(TRUE == Bt_GetIntFlag(TIM0, BtCA0Irq)) {
        u16TIM0_CntValue = Bt_M1_Cnt16Get(TIM0);
        u16TIM0_CapValue = Bt_M1_PWC_CapValueGet(TIM0);

        if(       (PULSE_START - PULSE_START_ZONE) < u16TIM0_CapValue && u16TIM0_CapValue < (PULSE_START + PULSE_START_ZONE)) {

            ir_sta |= IR_STA_START;
            ir_index = 0;

        } else if((PULSE_SPACE - PULSE_SPACE_ZONE) < u16TIM0_CapValue && u16TIM0_CapValue < (PULSE_SPACE + PULSE_SPACE_ZONE)) {

            if(ir_sta & IR_STA_START) {

            }
            ir_sta |= IR_STA_SPACE;
            ir_data[ir_index++] = u16TIM0_CapValue;

        } else if(((PULSE_L - PULSE_L_ZONE)         < u16TIM0_CapValue && u16TIM0_CapValue < (PULSE_L + PULSE_L_ZONE)) || 
                  ((PULSE_H - PULSE_H_ZONE)         < u16TIM0_CapValue && u16TIM0_CapValue < (PULSE_H + PULSE_H_ZONE)) || 
                  ((PULSE_CNT_LG - PULSE_CNT_LG_ZONE) < u16TIM0_CapValue && u16TIM0_CapValue < (PULSE_CNT_LG + PULSE_CNT_LG_ZONE)) || 
                  ((PULSE_CNT_ST - PULSE_CNT_ST_ZONE) < u16TIM0_CapValue && u16TIM0_CapValue < (PULSE_CNT_ST + PULSE_CNT_ST_ZONE))) {

            if(ir_sta & IR_STA_SPACE) {
                ir_sta |= IR_STA_DATA;
                ir_data[ir_index++] = u16TIM0_CapValue;
            } else {
                ir_sta = 0;
                ir_index = 0;
            }

        } else {

            ir_sta = 0;
            ir_index = 0;

        }

        Bt_M1_Cnt16Set(TIM0, 0);
        
        Bt_ClearIntFlag(TIM0, BtCA0Irq);

    }
}

#else

uint32_t  Bt_CapTemp_Value1,Bt_CapTemp_Value2;
int32_t   Bt_Capture_Value;
uint16_t  Bt_Uev_Cnt;

void ir_timer0_init(void)
{
    stc_sysctrl_clk_config_t      stcClkConfig;
    stc_bt_mode23_config_t        stcBtBaseCfg;
    stc_bt_m23_input_config_t     stcBtPortCapCfg;
    stc_gpio_config_t             stcTIM0Port;

    DDL_ZERO_STRUCT(stcClkConfig);
    DDL_ZERO_STRUCT(stcBtBaseCfg);
    DDL_ZERO_STRUCT(stcTIM0Port);
    DDL_ZERO_STRUCT(stcBtPortCapCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE);
    Gpio_SetAfMode(IR_PORT, IR_PIN, GpioAf5);

    stcBtBaseCfg.enWorkMode    = BtWorkMode2;              //锯齿波模式
    stcBtBaseCfg.enCT          = BtTimer;                  //定时器功能，计数时钟为内部PCLK
    stcBtBaseCfg.enPRS         = BtPCLKDiv64;              //PCLK/64
    stcBtBaseCfg.enCntDir      = BtCntUp;                  //向上计数，在三角波模式时只读

    stcBtBaseCfg.pfnTim0Cb  = timer0_interrupt;            //中断函数入口

    Bt_Mode23_Init(TIM0, &stcBtBaseCfg);                   //TIM0 的模式0功能初始化

    stcBtPortCapCfg.enCh0ACmpCap   = BtCHxCapMode;         //CHB通道设置为捕获模式
    stcBtPortCapCfg.enCH0ACapSel   = BtCHxCapFall;         //CHB通道上升沿下降沿捕获都使能
    stcBtPortCapCfg.enCH0AInFlt    = BtFltPCLKDiv64Cnt3;   //PCLK/16 3个连续有效
    stcBtPortCapCfg.enCH0APolarity = BtPortPositive;       //正常输入输出

    Bt_M23_PortInput_Config(TIM0, &stcBtPortCapCfg);       //端口输入初始化配置

    uint16_t u16ArrValue = 0xFFFF;
    uint16_t u16CntValue = 0;
    Bt_M23_ARRSet(TIM0, u16ArrValue, TRUE);                //设置重载值,并使能缓存

    Bt_M23_Cnt16Set(TIM0, u16CntValue);                    //设置计数初值

    Bt_ClearAllIntFlag(TIM0);                              //清中断标志
    Bt_Mode23_EnableIrq(TIM0,BtUevIrq);                    //使能TIM0 UEV更新中断
    Bt_Mode23_EnableIrq(TIM0,BtCA0Irq);                    //使能TIM0 CB0比较/捕获中断
    EnableNvic(TIM0_IRQn, IrqLevel0, TRUE);                //TIM0中断使能

    Bt_M23_Run(TIM0);                                      //TIM0 运行
}

void timer0_interrupt(void)
{
    static uint8_t i;
    volatile uint16_t data_captured;

    if(TRUE == Bt_GetIntFlag(TIM0, BtUevIrq)) {
        Bt_Uev_Cnt++;
        Bt_ClearIntFlag(TIM0,BtUevIrq);
    }

    if(TRUE == Bt_GetIntFlag(TIM0, BtCA0Irq)) {

        if(IR_PIN_DATA) {   // rising edge captured

            data_captured = M0P_TIM0_MODE23->CCR0A_f.CCR0A;

            if((PULSE_START - PULSE_START_ZONE) < data_captured && data_captured < (PULSE_START + PULSE_START_ZONE)) {
                ir_data[0] = data_captured;
                ir_sta |= IR_STA_START;
            }
            
//            Bt_M23_Stop(TIM0);
            Bt_M23_CapEdge_Config(TIM0, BtCHxCapFall);
            Bt_M23_Cnt16Set(TIM0, 0);
//            Bt_M23_Run(TIM0);

        } else {            // falling edge captured

            data_captured = M0P_TIM0_MODE23->CCR0A_f.CCR0A;

            Bt_M23_CapEdge_Config(TIM0, BtCHxCapRise);
            Bt_M23_Cnt16Set(TIM0, 0);

            if (      ir_sta & IR_STA_SPACE) {
                ir_data[ir_index++] = data_captured;
            } else if(ir_sta & IR_STA_START) {
                ir_data[1] = data_captured;
                ir_sta |= IR_STA_SPACE;
            } else {        // waiting start pulse
                return ;
            }
        }
        
        Bt_ClearIntFlag(TIM0,BtCA0Irq);

    }
    
}

#endif

void ir_debug(void)
{
    if(ir_sta & IR_STA_END) {
        for(uint8_t i=0; i<ir_index; i++) {
            printf("ir[%d]: %d\n", i, (ir_data[i] << 2) / 3);
        }
    }
}

