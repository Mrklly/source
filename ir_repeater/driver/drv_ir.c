#include "drv_ir.h"
#include "global_variables.h"


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

    stcBtBaseCfg.enWorkMode    = BtWorkMode2;              //��ݲ�ģʽ
    stcBtBaseCfg.enCT          = BtTimer;                  //��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK
    stcBtBaseCfg.enPRS         = BtPCLKDiv64;              //PCLK/64
    stcBtBaseCfg.enCntDir      = BtCntUp;                  //���ϼ����������ǲ�ģʽʱֻ��

    stcBtBaseCfg.pfnTim0Cb  = timer0_interrupt;            //�жϺ������

    Bt_Mode23_Init(TIM0, &stcBtBaseCfg);                   //TIM0 ��ģʽ0���ܳ�ʼ��

    stcBtPortCapCfg.enCh0ACmpCap   = BtCHxCapMode;         //CHBͨ������Ϊ����ģʽ
    stcBtPortCapCfg.enCH0ACapSel   = BtCHxCapFall;         //CHBͨ���������½��ز���ʹ��
    stcBtPortCapCfg.enCH0AInFlt    = BtFltPCLKDiv64Cnt3;   //PCLK/16 3��������Ч
    stcBtPortCapCfg.enCH0APolarity = BtPortPositive;       //�����������

    Bt_M23_PortInput_Config(TIM0, &stcBtPortCapCfg);       //�˿������ʼ������

    uint16_t u16ArrValue = 0xFFFF;
    uint16_t u16CntValue = 0;
    Bt_M23_ARRSet(TIM0, u16ArrValue, TRUE);                //��������ֵ,��ʹ�ܻ���

    Bt_M23_Cnt16Set(TIM0, u16CntValue);                    //���ü�����ֵ

    Bt_ClearAllIntFlag(TIM0);                              //���жϱ�־
    Bt_Mode23_EnableIrq(TIM0,BtUevIrq);                    //ʹ��TIM0 UEV�����ж�
    Bt_Mode23_EnableIrq(TIM0,BtCA0Irq);                    //ʹ��TIM0 CB0�Ƚ�/�����ж�
    EnableNvic(TIM0_IRQn, IrqLevel0, TRUE);                //TIM0�ж�ʹ��

    Bt_M23_Run(TIM0);                                      //TIM0 ����
}

void timer0_interrupt(void)
{
    volatile uint16_t data_captured;
    static uint16_t idx = 0;

    if(M0P_TIM0_MODE23->IFR_f.UIF) {

        if(g_ir_study_buf[2] != 0) {
            g_ir_flag = 1;
            g_ir_idx = idx;
        }

        idx = 0;

        M0P_TIM0_MODE23->ICLR_f.UIF = 0;
    }

    if(M0P_TIM0_MODE23->IFR_f.CA0F) {

        if(IR_PIN_DATA) {   // rising edge captured

//            M0P_TIM0_MODE23->M23CR_f.CTEN = 0;
            M0P_TIM0_MODE23->CRCH0_f.CSA           = 1;
            M0P_TIM0_MODE23->CRCH0_f.CFA_CRA_BKSA  = BtCHxCapFall;
//            M0P_TIM0_MODE23->M23CR_f.CTEN = 1;

        } else {            // falling edge captured

            M0P_TIM0_MODE23->CRCH0_f.CSA           = 1;
            M0P_TIM0_MODE23->CRCH0_f.CFA_CRA_BKSA  = BtCHxCapRise;

        }

        data_captured = M0P_TIM0_MODE23->CCR0A_f.CCR0A;

        M0P_TIM0_MODE23->CNT_f.CNT = 0;

        if(idx) {           // throw first capture

            g_ir_study_buf[idx++] = (uint8_t)(data_captured >> 8);
            g_ir_study_buf[idx++] = (uint8_t)(data_captured);

        } else {
            idx += 2;
        }

        M0P_TIM0_MODE23->ICLR_f.CA0F = 0;
    }
}

