/*******************************************************************************
* Copyright (C) 2017, Huada Semiconductor Co.,Ltd All rights reserved.
*
* This software is owned and published by:
* Huada Semiconductor Co.,Ltd ("HDSC").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with HDSC
* components. This software is licensed by HDSC to be adapted only
* for use in systems utilizing HDSC components. HDSC shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. HDSC is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* Disclaimer:
* HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
* WARRANTY OF NONINFRINGEMENT.
* HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
* SAVINGS OR PROFITS,
* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
* FROM, THE SOFTWARE.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/
/******************************************************************************/
/** \file clk.h
 **
 ** Headerfile for CLK functions
 ** @link CLK Group Some description @endlink 
 **
 ** History:
 **   - 2017-05-02   Ronnie     First Version
 **
 ******************************************************************************/

#ifndef __CLK_H__
#define __CLK_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ddl.h"
#include "hc32_it.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup ClkGroup Clock (CLK)
  **
 ******************************************************************************/
//@{

/**
 *******************************************************************************
 ** function prototypes.
 ******************************************************************************/

/*******************************************************************************
 * Global definitions
 ******************************************************************************/
#define CLK_RCH_VAL             (24*1000*1000)       ///< �ڲ�����ʱ��Ƶ��ֵ
#define CLK_RCL_VAL             (38400)             ///< �ڲ�����ʱ��Ƶ��ֵ
#ifndef CLK_XTH_VAL
#define CLK_XTH_VAL             (32*1000*1000)      ///< �ⲿ����ʱ��Ƶ��ֵĬ��ֵ
#endif
/**
 *******************************************************************************
 ** \brief ʱ������Դ����
 ** \note
 ******************************************************************************/
typedef enum en_clk_source
{
    ClkRCH  = 0u,               ///< �ڲ�����ʱ��
    ClkXTH  = 1u,               ///< �ⲿ����ʱ��
    ClkRCL  = 2u,               ///< �ڲ�����ʱ��
}en_clk_source_t;

/**
 *******************************************************************************
 ** \brief ʱ�ӷ�Ƶϵ��
 ******************************************************************************/
typedef enum en_clk_div
{
    ClkDiv1 = 1,                ///< ����Ƶ
    ClkDiv2 = 2,                ///< 2 ��Ƶ
    ClkDiv4 = 4,                ///< 4 ��Ƶ
    ClkDiv8 = 8,                ///< 8 ��Ƶ
    ClkDiv16 = 16,              ///< 16 ��Ƶ
    ClkDiv32 = 32,              ///< 32 ��Ƶ
    ClkDiv64 = 64,              ///< 64 ��Ƶ
    ClkDiv128 = 128,            ///< 128 ��Ƶ
}en_clk_div_t;

/**
 *******************************************************************************
 ** \brief ʱ��������
 ******************************************************************************/
typedef enum en_clk_cycle
{
    ClkCycle4,                  ///< 4 ��������
    ClkCycle16,                 ///< 16 ��������
    ClkCycle64,                 ///< 64 ��������
    ClkCycle256,                ///< 256 ��������
    ClkCycle1024,               ///< 1024 ��������
    ClkCycle4096,               ///< 4096 ��������
    ClkCycle16384,              ///< 16384 ��������
}en_clk_cycle_t;

/**
 *******************************************************************************
 ** \brief ʱ��Ƶ��ֵö��
 ******************************************************************************/
typedef enum en_clk_freq
{
    ClkFreq32768,               ///< 32.8KHz
    ClkFreq38_4K,               ///< 38.4KHz
    ClkFreq4Mhz,                ///< 4MHz
    ClkFreq8Mhz,                ///< 8MHz
    ClkFreq16Mhz,               ///< 16MHz
    ClkFreq22_12Mhz,            ///< 22.12MHz
    ClkFreq24Mhz,               ///< 24MHz
}en_clk_freq_t;

/**
 *******************************************************************************
 ** \brief ʱ��ģ�鹦�ܹ�������ѡ��
 ******************************************************************************/
typedef enum en_clk_func
{
    ClkFuncWkupRCH,                     ///< ����ʱʹ��RCHʱ��
    ClkFuncXTHEn,                       ///< ʹ���ⲿ����ʱ����������
    ClkFuncFaultEn,                     ///< ʹ��ʱ��ʧЧ���
    ClkFuncLockUpEn,                    ///< ֻ�ܴ���ʧЧ����
    ClkFuncRstPinIOEn,                  ///< reset����ΪIO
    ClkFuncSwdPinIOEn,                  ///< swd����ΪIO
}en_clk_func_t;

/**
 *******************************************************************************
 ** \brief ����ʱ���ſؿ�������ö��
 ******************************************************************************/
typedef enum en_clk_peripheral_gate
{
    ClkPeripheralUart0      = 0u,       ///< ����0
    ClkPeripheralUart1      = 1u,       ///< ����1
    ClkPeripheralI2c        = 4u,       ///< I2C
    ClkPeripheralSpi        = 6u,       ///< SPI
    ClkPeripheralBt         = 8u,       ///< ����ʱ��
    ClkPeripheralAdt        = 10u,      ///< �߼�ʱ��
    ClkPeripheralPca        = 14u,      ///< �ɱ�̼�������
    ClkPeripheralWdt        = 15u,      ///< ���Ź�
    ClkPeripheralAdcBgr     = 16u,      ///< ADC
    ClkPeripheralVcLvd      = 17u,      ///< ��ѹ�ȽϺ͵͵�ѹ���
    ClkPeripheralClkTrim    = 21u,      ///< ʱ��У׼
    ClkPeripheralTick       = 24u,      ///< ϵͳ��ʱ��
    ClkPeripheralCrc        = 26u,      ///< CRC
    ClkPeripheralGpio       = 28u,      ///< GPIO
    ClkPeripheralFlash      = 31u,      ///< Flash
}en_clk_peripheral_gate_t;

/**
 *******************************************************************************
 ** \brief ʱ��������Կ���
 ** \note �����󣬵������ж�ʱ������ʱ����Ȼ����
 ******************************************************************************/
typedef enum
{
    ClkDBGTim0,             ///<  ʱ��0
    ClkDBGTim1,             ///<  ʱ��1
    ClkDBGTim2,             ///<  ʱ��2
    ClkDBGLpTim,            ///<  �͹���ʱ��
    ClkDBGTim4,             ///<  ʱ��4
    ClkDBGTim5,             ///<  ʱ��5 
    ClkDBGTim6,             ///<  ʱ��6 
    ClkDBGTPca,             ///<  �ɱ�̼�������
    ClkDBGTWdt,             ///<  ���Ź�
    ClkDBGTRtc,             ///<  RTC
    ClkDBGTTick,            ///<  systick
}en_clk_debug_t;

/**
 *******************************************************************************
 ** \brief Systick����
 ** \note ���systick��ARM��systick����һ�£������systick���趨ָ��ʱ��Դ������
 ******************************************************************************/
typedef struct
{
    en_clk_source_t     enClk;      ///< ʱ��Դѡ��
                                    ///< RCL  -> 38.5k
                                    ///< RCH  -> hclk/8
                                    ///< XTH  -> XTH
    boolean_t           bNoRef;     ///< ʹ��ϵͳʱ��
                                    ///< TRUE  :  ʹ��ϵͳʱ��HCLK 
                                    ///< FALSE :  ʹ��enClk
    uint32_t            u32LoadVal; ///< ��������ֵ
}stc_clk_systickcfg_t;

/**
 *******************************************************************************
 ** \brief ʱ�ӳ�ʼ������
 ******************************************************************************/
typedef struct
{
    en_clk_source_t enClkSrc;       ///< ʱ��Դѡ��
    en_clk_div_t    enHClkDiv;      ///< ϵͳʱ�ӷ�Ƶϵ��
    en_clk_div_t    enPClkDiv;      ///< ����ʱ�ӷ�Ƶϵ��
}stc_clk_config_t;

/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
///< ʱ�ӳ�ʼ�� / ȥ��ʼ��
en_result_t Clk_Init(stc_clk_config_t *pstcCfg);
en_result_t Clk_DeInit(void);

///< ʱ�ӻ�����������
en_result_t Clk_Enable(en_clk_source_t enSource, boolean_t bFlag);
en_result_t Clk_SetSource(en_clk_source_t enSource);
boolean_t Clk_GetClkRdy(en_clk_source_t enSource);
en_result_t Clk_SetXTH_StableTime(en_clk_cycle_t enCycle);
en_result_t Clk_SetRCL_StableTime(en_clk_cycle_t enCycle);

///< ʱ��Դ�л�
en_result_t Clk_SwitchTo(en_clk_source_t enSource);

///< ʱ��Ƶ���趨
en_result_t Clk_SetRCHFreq(en_clk_freq_t enVal);
en_result_t Clk_SetRCLFreq(en_clk_freq_t enVal);

///< ʱ�ӷ�Ƶ����
en_result_t Clk_SetHClkDiv(uint8_t u8Div);
en_result_t Clk_SetPClkDiv(uint8_t u8Div);

///< ��ȡʱ��Ƶ��
uint32_t Clk_GetHClkFreq(void);
uint32_t Clk_GetPClkFreq(void);

///< ʱ�������ſؿ���
en_result_t Clk_SetPeripheralGate(en_clk_peripheral_gate_t enPeripheral,
                                  boolean_t bFlag);
boolean_t Clk_GetPeripheralGate(en_clk_peripheral_gate_t enPeripheral);


///< ʱ�ӹ��ܿ���
en_result_t Clk_SetFunc(en_clk_func_t enFunc, boolean_t bFlag);

///< systick ����
en_result_t Clk_SysTickConfig(stc_clk_systickcfg_t *pstcCfg);
en_result_t Clk_DebugClk(en_clk_debug_t enPeri, boolean_t bFlag);

//Systick SKEW ��ȡ
boolean_t Clk_SysTickSkewGet(void);
//Systick CALIB ��ȡ
uint32_t Clk_SysTickStcalibGet(void);
//@} // ClkGroup

#ifdef __cplusplus
#endif


#endif /* __CLK_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


