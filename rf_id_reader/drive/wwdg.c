#include "wwdg.h"

#define CounterInit 0x7f
#define window      0x7f


void WWDG_Configuration(void)
{
    /* WWDG Configuration */
    /* Watchdog Window= 0x7F step to 0x3F step
    = (0x7F-0x3F) * 1 step
    = 64 * 1 step
    = 64 * (12288/2Mhz)
    = 393,216ms
    */
    /* Allowed Window = (0x7F-window) * 1 step
    = (0x7F-0x77) * 1 step
    = 7 * 1 step
    = 7 * (12288/2Mhz)
    = 43.008ms
    */
    /* So the non allowed window starts from 0.0ms to 43.008ms
    and the alowed window starts from 43.008ms to 393,216ms */
//    WWDG_Init(CounterInit,window);

    WWDG->WR = WWDG_WR_RESET_VALUE;
    WWDG->CR = (uint8_t)((uint8_t)(WWDG_CR_WDGA | WWDG_CR_T6) | (uint8_t)CounterInit);
    WWDG->WR = (uint8_t)((uint8_t)(~WWDG_CR_WDGA) & (uint8_t)(WWDG_CR_T6 | window));
}

void WWDGReset(void)
{
    FlagStatus WwdgFlag;

    /*Get WWDG Reset Status */
//  WwdgFlag = RST_GetFlagStatus(RST_FLAG_WWDGF);
    WwdgFlag = ((FlagStatus)((uint8_t)RST->SR & (uint8_t)RST_FLAG_WWDGF));
    /* Test if a WWDG Reset has occured */
    if (WwdgFlag) {
        /* Clear IWDGF Flag */
//    RST_ClearFlag(RST_FLAG_WWDGF);
        RST->SR = (uint8_t)RST_FLAG_WWDGF;
    }
}


void Refresh_WWDG_Window(void)//Î¹¹·
{
//    u8 CounterValue;
//    CounterValue = (u8)(WWDG_GetCounter() & 0x7F);
//
//    if(CounterValue < window) {
//        WWDG_SetCounter(CounterInit);
//        //  LCD_ShowString(100,22,"Refresh WDOG:");
//    }
    WWDG->CR = (uint8_t)(CounterInit & 0x7F);
}
