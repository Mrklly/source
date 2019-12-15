/*************************************************************************************
* Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
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
/** \file flash.h
 **
 ** FLASH ���ݽṹ��API����.
 **
 ** - 2017-05-02 LuX V1.0
 **
 ******************************************************************************/

#ifndef __FLASH_H__
#define __FLASH_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "hc32_ddl.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup FlashGroup Flash Controller (Flash)
 **
 **
 ******************************************************************************/
//@{

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/*******************************************************************************
* Local pre-processor symbols/macros ('#define')
*******************************************************************************/
#define FLASH_END_ADDR              (0x0000FFFFu)
#define FLASH_BYPASS()              M0P_FLASH->BYPASS_f.BYSEQ = 0x5A5A;\
                                    M0P_FLASH->BYPASS_f.BYSEQ = 0xA5A5;
#define FLASH_IE_TRUE               (0x03)
#define FLASH_IE_FALSE              (0x00)

/**
 ******************************************************************************
  ** \brief Flash�ж������ض���
 *****************************************************************************/
typedef enum en_flash_int_type
{
    FlashPCInt    = 1u,          ///<��дPC��ַ�����ж�
    FlashSlockInt = 0u,          ///<��д���������ж�
} en_flash_int_type_t;


/**
 ******************************************************************************
  ** \brief Flash���ȴ����������ض���
 *****************************************************************************/
typedef enum en_flash_waitcycle
{
    FlashWaitCycle0 = 0u,       ///< ���ȴ���������Ϊ0����HCLKС�ڵ���24MHzʱ��
    FlashWaitCycle1 = 1u,       ///< ���ȴ���������Ϊ1����HCLK����24MHzʱ��������Ϊ1��
    FlashWaitCycle2 = 2u,       ///< ���ȴ���������Ϊ2����HCK����48MHzʱ��������Ϊ2��
} en_flash_waitcycle_t;

/**
 ******************************************************************************
  ** \brief Flash��д������Χ�ض���
 *****************************************************************************/
typedef enum en_flash_sector_lock
{
    FlashSector0_3     = 0x00000001u,           ///<Sector0_3    
    FlashSector4_7     = 0x00000002u,           ///<Sector4_7    
    FlashSector8_11    = 0x00000004u,           ///<Sector8_11   
    FlashSector12_15   = 0x00000008u,           ///<Sector12_15  
    FlashSector16_19   = 0x00000010u,           ///<Sector16_19  
    FlashSector20_23   = 0x00000020u,           ///<Sector20_23  
    FlashSector24_27   = 0x00000040u,           ///<Sector24_27  
    FlashSector28_31   = 0x00000080u,           ///<Sector28_31  
    FlashSector32_35   = 0x00000100u,           ///<Sector32_35  
    FlashSector36_39   = 0x00000200u,           ///<Sector36_39  
    FlashSector40_43   = 0x00000400u,           ///<Sector40_43  
    FlashSector44_47   = 0x00000800u,           ///<Sector44_47  
    FlashSector48_51   = 0x00001000u,           ///<Sector48_51  
    FlashSector52_55   = 0x00002000u,           ///<Sector52_55  
    FlashSector56_59   = 0x00004000u,           ///<Sector56_59  
    FlashSector60_63   = 0x00008000u,           ///<Sector60_63  
    FlashSector64_67   = 0x00010000u,           ///<Sector64_67  
    FlashSector68_71   = 0x00020000u,           ///<Sector68_71  
    FlashSector72_75   = 0x00040000u,           ///<Sector72_75  
    FlashSector76_79   = 0x00080000u,           ///<Sector76_79  
    FlashSector80_83   = 0x00100000u,           ///<Sector80_83  
    FlashSector84_87   = 0x00200000u,           ///<Sector84_87  
    FlashSector88_91   = 0x00400000u,           ///<Sector88_91  
    FlashSector92_95   = 0x00800000u,           ///<Sector92_95  
    FlashSector96_99   = 0x01000000u,           ///<Sector96_99  
    FlashSector100_103 = 0x02000000u,           ///<Sector100_103
    FlashSector104_107 = 0x04000000u,           ///<Sector104_107
    FlashSector108_111 = 0x08000000u,           ///<Sector108_111
    FlashSector112_115 = 0x10000000u,           ///<Sector112_115
    FlashSector116_119 = 0x20000000u,           ///<Sector116_119
    FlashSector120_123 = 0x40000000u,           ///<Sector120_123
    FlashSector124_127 = (int)0x80000000u,           ///<Sector124_127
    FlashSectorAll     = (int)0xFFFFFFFFu,           ///<SectorAll    
} en_flash_sector_lock_t;

/**
 ******************************************************************************
 ** \brief FLASH OP
 ** 
 ** Flash �����������������ض��� 
 ******************************************************************************/
typedef enum en_flash_op
{
    Read        = 0u,           ///<������ֵ
    Program     = 1u,           ///<�������ֵ
    SectorErase = 2u,           ///<������������ֵ
    ChipErase   = 3u,           ///<ȫƬ��������ֵ
} en_flash_op_t;

/**
 ******************************************************************************
 ** \brief FLASH LOCK
 ** 
 ** Flash �ӽ������������ض��� 
 ******************************************************************************/
typedef enum en_flash_lock
{
    LockAll   = 0x00000000u,      ///<ȫƬ����  
    UnlockAll = (int)0xFFFFFFFFu,      ///<ȫƬ����
} en_flash_lock_t;

/**
 ******************************************************************************
 ** \brief FLASH ���ʱ���������
 ** 
 ** FLASH���ʱ������������������ض��� (4MHz)
 ******************************************************************************/
typedef enum en_flash_prgtimer
{
    Tnvs      = 0x20u,
    Tpgs      = 0x17u,
    Tprog     = 0x1Bu,
    Tserase   = 0x4650u,
    Tmerase   = 0x222E0u,
    Tprcv     = 0x18u,
    Tsrcv     = 0xF0u,
    Tmrcv     = 0x3E8u,
} en_flash_prgtimer_t;

/**
 ******************************************************************************
 ** \brief Redefinition of FLASH register structure 
 ******************************************************************************/

/******************************************************************************
 * Global definitions
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions ('extern')                                     
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)                        
 ******************************************************************************/
///<Flash ��ʼ������(�жϺ��������ʱ�����������ģʽ���ã�
en_result_t Flash_Init(func_ptr_t pfnFlashCb, uint8_t u8FreqCfg, boolean_t bDpstbEn);

///<Flash ҳ/ȫƬ����
en_result_t Flash_SectorErase(uint32_t u32SectorAddr);
en_result_t Flash_ChipErase(void);

///<Flash �ֽ�/����/��д
en_result_t Flash_WriteByte(uint32_t u32Addr, uint8_t u8Data);
en_result_t Flash_WriteHalfWord(uint32_t u32Addr, uint16_t u16Data);
en_result_t Flash_WriteWord(uint32_t u32Addr, uint32_t u32Data);

///<Flash ��̱�������/����
en_result_t Flash_Lock(en_flash_sector_lock_t enFlashSector);
en_result_t Flash_Unlock(en_flash_sector_lock_t enFlashSector);

///<Flash ���ȴ������趨
en_result_t Flash_WaitCycle(en_flash_waitcycle_t enWaitCycle);

///<�ж���غ���
///<�ж�ʹ��/��ֹ
en_result_t Flash_EnableIrq(en_flash_int_type_t enFlashIntType);
en_result_t Flash_DisableIrq(en_flash_int_type_t enFlashIntType);
///<�жϱ�־��ȡ
boolean_t Flash_GetIntFlag(en_flash_int_type_t enFlashIntType);
///<�жϱ�־���
en_result_t Flash_ClearIntFlag(en_flash_int_type_t enFlashIntType);

//@} // FlashGroup

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/