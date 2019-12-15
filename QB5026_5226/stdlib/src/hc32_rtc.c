/*************************************************************************************
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
/** \file rtc.c
 **
 ** RTC function driver API.
 ** @link SampleGroup Some description @endlink
 **
 **   - 2017-05-17  1.0  CJ First version for Device Driver Library of Module.
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "rtc.h"
/**
 ******************************************************************************
 ** \addtogroup RtcGroup
 ******************************************************************************/
//@{

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/
#define IS_VALID_CLK(x)             (RtcClk32768 == (x)||\
                                     RtcClk32768_1== (x)||\
                                     RtcClk32 == (x)||\
                                     RtcClk32_1 == (x)||\
                                     RtcClkHxt128 == (x)||\
                                     RtcClkHxt256 == (x)||\
                                     RtcClkHxt512 == (x)||\
                                     RtcClkHxt1024 == (x))

#define IS_VALID_CYCSEL(x)        (RtcPrads == (x)||\
                                 RtcPradx==(x))

#define IS_VALID_PRDS(x)     (Rtc_None == (x)||\
                              Rtc_05S == (x)||\
                              Rtc_1S == (x)||\
                              Rtc_1Min == (x)||\
                              Rtc_1H == (x)||\
                              Rtc_1Day == (x)||\
                              Rtc_1Mon == (x)||\
                              Rtc_1Mon_1 == (x))  

#define     IS_VALID_IRQ_SEL(x)      (RtcPrdf == (x) ||\
                                      RtcAlmf == (x))

#define     IS_VALID_FUNC(x)  ((RtcCount==(x))||\
                               (RtcAlarmEn==(x))||\
                               (Rtc_ComenEn==(x))||\
                               (Rtc1HzOutEn==(x)))
#define     CkDateTime    0x7F
#define     CkDate        0x78
#define     CkTime        0x07

//#define DecToBcd(x)           ((((x)/10)<<4) + ((x)%10))
//#define BcdToDec(x)           ((((x)>>4)*10) + ((x)&0x0F))

#define RTC_TIMEOUT             1000//test 1s

/******************************************************************************/
/* Local function prototypes ('const')                                       */
/******************************************************************************/
const uint8_t Leap_Month_Base[]     =    {3,6,0,3,5,1,3,6,2,4,0,2};
const uint8_t NonLeap_Month_Base[]  =    {4,0,0,3,5,1,3,6,2,4,0,2};
const uint8_t Cnst_Month_Tbl[12]={0x31,0x28,0x31,0x30,0x31,0x30,0x31,0x31,0x30,0x31,0x30,0x31};
/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
static stc_rtc_intern_cb_t* RtcGetInternDataCb(void);
/******************************************************************************/
/* Local variable prototypes ('static')                                       */
/******************************************************************************/
static stc_rtc_intern_cb_t stcRtcIrqCb = {NULL, NULL};
/**
 ******************************************************************************
 ** \brief  RTC����ʱ��ѡ��
 **
 ** \param [in] enClkʱ��Դ
 **
 ** \retval Ok
 **
 ******************************************************************************/
en_result_t Rtc_SelClk(en_rtc_clk_t enClk)
{
    en_result_t enRet = Error;
    ASSERT(IS_VALID_CLK(enClk));
    M0P_RTC->CR1_f.CKSEL = enClk;
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  RTC�����жϷ�ʽѡ��
 **
 ** \param [in] pstccCyc�����жϷ�ʽ�����ڼ��ѡ��
 **
 ** \retval Ok
 **
 ******************************************************************************/
en_result_t Rtc_SetCyc(stc_rtc_cyc_sel_t* pstcCyc)
{
    en_result_t enRet = Error;
    ASSERT(IS_VALID_CYCSEL(pstcCyc->enCyc_sel));
    ASSERT(IS_VALID_PRDS(pstcCyc->enPrds_sel));
    M0P_RTC->CR0_f.PRDSEL = pstcCyc->enCyc_sel;
    if(pstcCyc->enCyc_sel)
    {
        M0P_RTC->CR0_f.PRDX = pstcCyc->u8Prdx;
    }
    else
    {
        M0P_RTC->CR0_f.PRDS = pstcCyc->enPrds_sel;
    }
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  RTCʱ��ѡ��
 **
 ** \param [in] bmode��12ʱ��or24ʱ��
 **
 ** \retval Ok  ��������
 ** \retval ErrorInvalidParameter �����쳣
 ******************************************************************************/
en_result_t Rtc_SetAmPm(en_rtc_ampm_t enMode)
{
    en_result_t enRet = Error;
    switch(enMode)
    {
        case 0:
        case 1:
            M0P_RTC->CR0_f.AMPM = enMode;
            break;
        default:
            return ErrorInvalidParameter;
    }
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  RTCʱ�ƻ�ȡ
 **
 ** \param [in] ��
 **
 ** \retval ʱ��
 ******************************************************************************/
boolean_t Rtc_GetHourMode(void)
{
  return(M0P_RTC->CR0_f.AMPM);
}
/**
 ******************************************************************************
 ** \brief  RTC�����ж�����
 **
 ** \param [in] pstcAlarmTime����ʱ��ʱ���֡��� 
 **
 ** \retval Ok  ��������
 ** 
 ******************************************************************************/
en_result_t Rtc_SetAlarmTime(stc_rtc_alarmset_t* pstcAlarmTime)
{
    en_result_t enRet = Ok;
    ASSERT(NULL != pstcAlarmTime);
    if(Rtc12h == M0P_RTC->CR0_f.AMPM)
    {
        enRet = Check_BCD_Format(pstcAlarmTime->u8Hour,0x00,0x12);
    }
    else
    {
        enRet = Check_BCD_Format(pstcAlarmTime->u8Hour,0x00,0x24);
    }
    if(enRet != Ok)
    {
        return enRet;
    }
    enRet = Check_BCD_Format(pstcAlarmTime->u8Minute,0x00,0x59);
    if(enRet != Ok)
    {
        return enRet;
    }
   // enRet = Check_BCD_Format(pstcAlarmTime->u8Week,0x00,0x06);
    if(enRet != Ok)
    {
        return enRet; 
    }
    M0P_RTC->ALMHOUR = pstcAlarmTime->u8Hour;
    M0P_RTC->ALMMIN = pstcAlarmTime->u8Minute;
    M0P_RTC->ALMWEEK = pstcAlarmTime->u8Week;
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  RTC�����ж�ʱ���ȡ
 **
 ** \param [in] pstcAlarmTime����ʱ��ʱ���֡��� 
 **
 ** \retval Ok  ��������
 ** 
 ******************************************************************************/
en_result_t Rtc_GetAlarmTime(stc_rtc_alarmset_t* pstcAlarmTime)
{
    en_result_t enRet = Error;
    ASSERT(NULL != pstcAlarmTime); 
    pstcAlarmTime->u8Minute = M0P_RTC->ALMMIN;
    pstcAlarmTime->u8Hour = M0P_RTC->ALMHOUR;
    pstcAlarmTime->u8Week = M0P_RTC->ALMWEEK;
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  RTC 1hzģʽѡ��
 **
 ** \param [in] bmode �߾��Ⱥ���ͨ����
 **
 ** \retval Ok  ��������
 ** 
 ******************************************************************************/
en_result_t Rtc_Set1HzMode(boolean_t bMode)
{
    en_result_t enRet = Error;
    M0P_RTC->CR0_f.HZ1SEL = bMode;
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  RTC 1hz����ֵ����
 **
 ** \param [in] u16Cr ����ֵ
 **
 ** \retval Ok  ��������
 ** 
 ******************************************************************************/
en_result_t Rtc_SetCompCr(uint16_t u16Cr)
{
    en_result_t enRet = Error;
    M0P_RTC->COMPEN_f.CR = u16Cr;
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  RTC ����ʹ������
 **
 ** \param [in] enFunc ����ѡ��
 **
 ** \retval Ok  ��������
 ** \retval ErrorInvalidParameter �����쳣
 ******************************************************************************/
en_result_t Rtc_EnableFunc(en_rtc_func_t enFunc)
{
    ASSERT(IS_VALID_FUNC(enFunc));
    switch(enFunc)
    {
        case RtcCount:
            M0P_RTC->CR0_f.START = 1u;
            break;
        case RtcAlarmEn:
            M0P_RTC->CR1_f.ALMEN = 1u;
            break;
        case Rtc_ComenEn:
            M0P_RTC->COMPEN_f.EN = 1u;
            break;
        case Rtc1HzOutEn:
            M0P_RTC->CR0_f.HZ1OE = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  RTC ���ܽ�ֹ����
 **
 ** \param [in] enFunc ����ѡ��
 **
 ** \retval Ok  ��������
 ** \retval ErrorInvalidParameter �����쳣
 ******************************************************************************/
en_result_t Rtc_DisableFunc(en_rtc_func_t enFunc)
{
    ASSERT(IS_VALID_FUNC(enFunc));
    switch(enFunc)
    {
        case RtcCount:
            M0P_RTC->CR0_f.START = 0u;
            break;
        case RtcAlarmEn:
            M0P_RTC->CR1_f.ALMEN = 0u;
            break;
        case Rtc_ComenEn:
            M0P_RTC->COMPEN_f.EN = 0u;
            break;
        case Rtc1HzOutEn:
            M0P_RTC->CR0_f.HZ1OE = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    return Ok;
}
uint8_t Change_DateTimeFormat(uint8_t u8sr)
{
    uint8_t u8de=0;
    while(u8sr>=0x10)
    {
        u8de +=10;
        u8sr -=0x10;
    }
    u8de += u8sr;
    return(u8de);
}
/**
 ******************************************************************************
 ** \brief  RTC ƽ��������
 **
 ** \param [in] u8year ��ʮ���Ƶ���λ
 **
 ** \retval 1  ����
 ** \retval 0 ƽ��
 ******************************************************************************/
uint8_t Rtc_CheckLeapYear(uint8_t u8year)
{
    uint8_t u8year_shl,u8year_shr;
    u8year_shl = u8year>>2;
    u8year_shr = u8year_shl<<2;
    if(u8year== u8year_shr)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/**
 ******************************************************************************
 ** \brief  RTC�������ڼ�������
 **
 ** \param [in] pu8Date����
 **
 ** \retval week  ����
 ** 
 ******************************************************************************/
uint8_t Rtc_CalWeek(uint8_t* pu8Date)
{
    uint8_t u8week;
    if((Rtc_CheckLeapYear(Change_DateTimeFormat(*(pu8Date+2)))==1))
    {
        u8week = (Change_DateTimeFormat(*(pu8Date+2))+Change_DateTimeFormat(*(pu8Date+2))/4+Leap_Month_Base[Change_DateTimeFormat(*(pu8Date+1))-1]+Change_DateTimeFormat(*(pu8Date))+2)%7;
    }
    else
    {
        u8week = (Change_DateTimeFormat(*(pu8Date+2))+Change_DateTimeFormat(*(pu8Date+2))/4+NonLeap_Month_Base[Change_DateTimeFormat(*(pu8Date+1))-1]+Change_DateTimeFormat(*(pu8Date))+2)%7;
    }
    return u8week;
}
/**
 ******************************************************************************
 ** \brief  RTC�������»�ȡ����
 **
 ** \param [in] u8month�·ݣ�u8year���
 **
 ** \retval u8day����
 ** 
 ******************************************************************************/
uint8_t Get_Month_Max_Day(uint8_t u8month, uint8_t u8year)
{
    uint8_t u8day = 0;
    
    u8day = Cnst_Month_Tbl[u8month - 1];
    if((u8month == 2) && ((u8year % 4) == 0))
    {
        u8day++;
    }
    return(u8day);//day�ĸ�ʽ��bcd�룬���磻��Ϊ31�죬day=0x31
}
/**
 ******************************************************************************
 ** \brief  RTC�������ڼ�������
 **
 ** \param [in] pu8buf����ʱ�����ݣ�u8len������ݳ��ȣ�u8limit_min��Сֵ��u8limit_max���ֵ
 **
 ** \retval Error ����OkУ����ȷ
 ** 
 ******************************************************************************/
en_result_t Check_BCD_Format(uint8_t u8data,uint8_t u8limit_min, uint8_t u8limit_max)
{

    if (((u8data & 0x0F) > 0x09) || ((u8data & 0xF0) > 0x90)
    ||(u8data > u8limit_max) || (u8data < u8limit_min))
    {
        return Error;
    }
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  RTCʱ���ʽ���
 **
 ** \param [in] pu8TimeDate����ʱ�����ݣ�u8Mode���ģʽ
 **
 ** \retval enRetУ����
 ** 
 ******************************************************************************/
en_result_t Rtc_CheckDateTimeFormat(uint8_t* pu8TimeDate,uint8_t u8Mode)
{
    uint8_t u8i=0;
    uint8_t u8mon_max_day = 0x28;
    uint8_t u8date[3];
    uint8_t u8Hour = 0;
    en_result_t enRet=Error;
    while(u8i<7)
    {
        if(u8Mode&&(1<<u8i))
        {
            switch(u8i)
            {
                case 0:
                    enRet = Check_BCD_Format(*pu8TimeDate,0x00,0x59);//��
                    break;
                case 1:
                    enRet = Check_BCD_Format(*pu8TimeDate,0x00,0x59);//��
                    break;
                case 2:
                    if(Rtc12h == M0P_RTC->CR0_f.AMPM)
                    {
                        u8Hour = *pu8TimeDate&0x1f;
                        enRet = Check_BCD_Format(u8Hour,0x00,0x12);//ʱ
                    }
                    else
                    {
                        enRet = Check_BCD_Format(*pu8TimeDate,0x00,0x24);
                    }
                    break;
                case 3:
                    enRet = Check_BCD_Format(*pu8TimeDate,0x00,0x06);
                    break;
                case 4:
                    enRet = Check_BCD_Format(*pu8TimeDate,0x00,0x31);
                    u8date[0] = *pu8TimeDate;
                    break;
                case 5:
                    enRet = Check_BCD_Format(*pu8TimeDate,0x00,0x12);
                    u8date[1] = *pu8TimeDate;
                    break;
                case 6:
                    enRet = Check_BCD_Format(*pu8TimeDate,0x00,0x99);
                    u8date[2] = *pu8TimeDate;
                    break;
                default:
                  break;
            }
            pu8TimeDate++;
        }
        if(enRet!=Ok)
        {
            return enRet;
        }
        u8i++;
    }
       if((u8Mode&0x10)&&(u8Mode&0x20))
    {
        if(u8Mode&0x40)
        {
          u8mon_max_day = Get_Month_Max_Day(Change_DateTimeFormat(u8date[1]), Change_DateTimeFormat(u8date[2]));
        }
        else
        {
            u8mon_max_day = Get_Month_Max_Day(Change_DateTimeFormat(u8date[1]), 1);
        }
        if(u8date[0]>u8mon_max_day)
        {
            return Error;
        }
    }
    if((u8Mode&0x10)&&(!(u8Mode&0x20)))
    {
        if(u8date[0]>0x28)
        {
            return Error;
        }
    }
    enRet = Ok;
    return(enRet); 
}
/**
 ******************************************************************************
 ** \brief  RTC����ʱ�亯��
 **
 ** \param [in] pstcTimeDate����ʱ�����ݡ�bUpdateTime�Ƿ����ʱ�䡢bUpdateDate�Ƿ��������
 **
 ** \retval Ok  ��������
 ** \retval ErrorTimeout ʱ���������
 ******************************************************************************/
en_result_t Rtc_WriteDateTime(stc_rtc_time_t* pstcTimeDate,boolean_t bUpdateTime,
                            boolean_t bUpdateDate)
{
    int32_t u32TimeOut;
    uint8_t* pu8TimeDate;
    en_result_t enRet = Ok;
    u32TimeOut = RTC_TIMEOUT;
    pu8TimeDate = &pstcTimeDate->u8Second;
    ASSERT(NULL != pstcTimeDate);
    if(1 == M0P_RTC->CR0_f.START)
    {
        M0P_RTC->CR1_f.WAIT = 1;
        while(--u32TimeOut)
        {
            if(M0P_RTC->CR1_f.WAITF)
            {
                break;
            }
        }
        if(u32TimeOut==0)
        {
            return ErrorTimeout;
        }
    }
    if(TRUE == bUpdateTime)
    {
        enRet = Rtc_CheckDateTimeFormat(pu8TimeDate,CkTime);
        if(enRet != Ok)
        {
            return enRet;
        }
        M0P_RTC->SEC = pstcTimeDate->u8Second;
        M0P_RTC->MIN = pstcTimeDate->u8Minute;
        M0P_RTC->HOUR = pstcTimeDate->u8Hour;
    }
    if(TRUE == bUpdateDate)
    {
        enRet = Rtc_CheckDateTimeFormat(pu8TimeDate,CkDate);
        if(enRet != Ok)
        {
            return enRet;
        }
        M0P_RTC->DAY = pstcTimeDate->u8Day;
        M0P_RTC->MON = pstcTimeDate->u8Month;
        M0P_RTC->YEAR = pstcTimeDate->u8Year;
        M0P_RTC->WEEK = pstcTimeDate->u8DayOfWeek;
    }
    M0P_RTC->CR1_f.WAIT = 0;
    if(1 == M0P_RTC->CR0_f.START)
    {
        while(M0P_RTC->CR1_f.WAITF)
        {}
    }
    return enRet;
}
/**
 ******************************************************************************
** \brief  RTC 12Сʱ����������ȡ
 **
 ** \param [in] ��
 **
** \retval ���������
 ******************************************************************************/
boolean_t Rtc_RDAmPm(void)
{
	boolean_t bRet; 
		
    bRet = M0P_RTC->HOUR&0x20;
    bRet>>=5;
    return bRet;
}
/**
 ******************************************************************************
 ** \brief  RTC��ȡʱ�亯��
 **
 ** \param [in] pstcTimeDate����ʱ������
 **
 ** \retval Ok  ��ȡ����
 ** \retval ErrorTimeout ʱ���������
 ******************************************************************************/
en_result_t Rtc_ReadDateTime(stc_rtc_time_t* pstcTimeDate)
{
    uint32_t u32TimeOut;
    uint8_t  u8DayOfWeek, u8BcdSec, u8BcdMin, u8BcdHour, u8Day, u8Month, u8Year;

    ASSERT(NULL != pstcTimeDate);  
    u32TimeOut = RTC_TIMEOUT;
    if(1 == M0P_RTC->CR0_f.START)
    {
        M0P_RTC->CR1_f.WAIT = 1;
        while(u32TimeOut--)
        {
            if(M0P_RTC->CR1_f.WAITF)
            {
            break;
            }
        }
        if(u32TimeOut==0)
        {
            return ErrorTimeout;
        }
    }
    u8BcdSec  = M0P_RTC->SEC;
    u8BcdMin  = M0P_RTC->MIN;
    u8BcdHour = M0P_RTC->HOUR;
    u8Day     = M0P_RTC->DAY;
    u8Month   = M0P_RTC->MON;
    u8Year    = M0P_RTC->YEAR;
    u8DayOfWeek = M0P_RTC->WEEK;

	pstcTimeDate->u8Second = u8BcdSec;
	pstcTimeDate->u8Minute = u8BcdMin;
	if(1 == M0P_RTC->CR0_f.AMPM)
	{
		pstcTimeDate->u8Hour   = u8BcdHour;
	}
	else
	{
		pstcTimeDate->u8Hour   = u8BcdHour&0x1f;
	}
	pstcTimeDate->u8Day    = u8Day;
	pstcTimeDate->u8Month  = u8Month;
	pstcTimeDate->u8Year  = u8Year;
	pstcTimeDate->u8DayOfWeek = u8DayOfWeek;

    M0P_RTC->CR1_f.WAIT = 0;
    if(1 == M0P_RTC->CR0_f.START)
    {
        while(M0P_RTC->CR1_f.WAITF)
        {}
    }
    
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  RTC����or��д״̬��ȡ
 **
 ** \param [in] ��
 **
 ** \retval ����or��д״̬
 ** 
 ******************************************************************************/
boolean_t Rtc_RDStatus(void)
{
    boolean_t bRet; 
    bRet = M0P_RTC->CR1_f.WAITF;
    return bRet;
}
/**
 ******************************************************************************
 ** \brief  RTC�����ж�ʹ��
 **
 ** \param [in] enordis�ж�ʹ��or��ֹ
 **
 ** \retval Ok���óɹ�
 ** 
 ******************************************************************************/
en_result_t Rtc_EnAlarmIrq(en_rtc_alarmirq_t enIrqEn)
{
    en_result_t enRet = Error;
    M0P_RTC->CR1_f.ALMIE = enIrqEn;
    Rtc_ClrIrqStatus(RtcAlmf);//ʹ���жϺ�����ж�������
    enRet = Ok;
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  RTC�ж�����״̬��ȡ
 **
 ** \param [in] enIrqSel��ȡ�����ж�����
 **
 ** \retval �ж�����״̬
 ** 
 ******************************************************************************/
boolean_t Rtc_GetIrqStatus(en_rtc_status_irq_t enIrqSel)
{
    boolean_t bRet = FALSE;
    ASSERT(IS_VALID_IRQ_SEL(enIrqSel));
    switch(enIrqSel)
    {
        case RtcPrdf:
            (M0P_RTC->CR1_f.PRDF == 1)?(bRet = TRUE) : (bRet = FALSE);
            break;
        case RtcAlmf :
            (M0P_RTC->CR1_f.ALMF == 1)?(bRet = TRUE) : (bRet = FALSE);
            break;
        default:
            break;
    }
    return bRet;
}
/**
 ******************************************************************************
 ** \brief  RTC�ж��������
 **
 ** \param [in] enIrqSel��������ж�����
 **
 ** \retval Ok ����ɹ�
 ** \retval ErrorInvalidParameter ���ʧ��
 ******************************************************************************/
en_result_t Rtc_ClrIrqStatus(en_rtc_status_irq_t enIrqSel)
{
    ASSERT(IS_VALID_IRQ_SEL(enIrqSel));
    switch(enIrqSel)
    {
        case RtcPrdf:
            M0P_RTC->CR1_f.PRDF = 0;
            break;
        case  RtcAlmf:
            M0P_RTC->CR1_f.ALMF = 0;
            break;
        default:
            return ErrorInvalidParameter;
    }
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  RTC�жϴ������ӿڻ�ȡ
 **
 ** \param [in] ��
 **
 ** \retval �ӿں�����ַ
 ** 
 ******************************************************************************/
static stc_rtc_intern_cb_t* RtcGetInternDataCb(void) 
{
    return &stcRtcIrqCb;
}
/**
 ******************************************************************************
 ** \brief  RTC�����ʼ������
 **
 ** \param [in] pstcRtcConfig��ʼ���ṹ
 **
 ** \retval Ok��ʼ���ɹ�
 ** \retval ErrorInvalidParameter ��ʼ������
 ******************************************************************************/
en_result_t Rtc_Init(stc_rtc_config_t* pstcRtcConfig)
{
    en_result_t enRet = Error;
    stc_rtc_intern_cb_t* pstcRtcInternCb;
    if(NULL == pstcRtcConfig)
    {
        return Error;
    }
    pstcRtcInternCb =  RtcGetInternDataCb();
    enRet = Rtc_SelClk(pstcRtcConfig->enClkSel);
    enRet = Rtc_SetAmPm(pstcRtcConfig->enAmpmSel);
    if(enRet != Ok)
    {
      return enRet;
    }
    if(NULL != pstcRtcConfig->pstcCycSel)
    {
        if(Ok != Rtc_SetCyc(pstcRtcConfig->pstcCycSel))
        {
            return Error;   
        }
    }
    if(NULL != pstcRtcConfig->pstcTimeDate)
    {
        if(Ok != Rtc_WriteDateTime(pstcRtcConfig->pstcTimeDate,TRUE,TRUE))
        {
            return Error;
        }
    }
    if(NULL != pstcRtcConfig->pstcIrqCb)
    {
        pstcRtcInternCb->pfnAlarmIrqCb = pstcRtcConfig->pstcIrqCb->pfnAlarmIrqCb;
        pstcRtcInternCb->pfnTimerIrqCb = pstcRtcConfig->pstcIrqCb->pfnTimerIrqCb;
    }
    if(TRUE == pstcRtcConfig->bTouchNvic)
    {
        EnableNvic(RTC_IRQn,IrqLevel3,TRUE);
    }
    else
    {
        EnableNvic(RTC_IRQn,IrqLevel3,FALSE);
    }
    return enRet;
}
/**
 ******************************************************************************
 ** \brief  RTC������ֹ����
 **
 ** \param [in] ��
 **
 ** \retval Ok��ֹ���óɹ�
 ** 
 ******************************************************************************/
en_result_t Rtc_DeInit(void)
{
    EnableNvic(RTC_IRQn,IrqLevel3,FALSE);
    Rtc_DisableFunc(RtcCount);
    Rtc_DisableFunc(RtcAlarmEn);
    Rtc_DisableFunc(Rtc_ComenEn);
    Rtc_DisableFunc(Rtc1HzOutEn);
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  RTC�жϴ�����
 **
 ** \param [in] ��
 **
 ** \retval ��
 ** 
 ******************************************************************************/
void Rtc_IRQHandler(void)
{
    stc_rtc_intern_cb_t* pstcRtcInternCb;
    pstcRtcInternCb = RtcGetInternDataCb() ;
    if(TRUE == M0P_RTC->CR1_f.ALMF)
    {
        M0P_RTC->CR1_f.ALMF = 0u;
        if(NULL != pstcRtcInternCb->pfnAlarmIrqCb)
        {
            pstcRtcInternCb->pfnAlarmIrqCb();
        }
    }
    if(TRUE == M0P_RTC->CR1_f.PRDF)
    {
        M0P_RTC->CR1_f.PRDF = 0;
        if(NULL != pstcRtcInternCb->pfnTimerIrqCb)
        {
            pstcRtcInternCb->pfnTimerIrqCb();
        }
    }
}
//@} // RtcGroup
