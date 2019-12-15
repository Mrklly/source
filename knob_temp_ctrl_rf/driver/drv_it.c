#include "drv_it.h"
#include "cmt2300a.h"
#include "spi.h"
#include "global_variables.h"
#include "drv_knob.h"


void Gpio_IRQHandler(uint8_t u8Param)
{
    uint8_t data;
    volatile uint8_t knob_temp_a, knob_temp_b;

    if(SPI1_INT_PORT == u8Param) {              // GPIO_PORT select

        if(M0P_GPIO->PA_STAT_f.PA01) {          // Gpio_GetIrqStatus(SPI1_INT_PORT, SPI1_INT_PIN)

            M0P_GPIO->PA_ICLR_f.PA01 = 0;       // Gpio_ClearIrq(SPI1_INT_PORT, SPI1_INT_PIN);

            if(rf433_status == RF433_TX) {
                data = bIntSrcFlagClr();                              // read out interrupt flag and clear
                bGoStandby();
                vIntSrcCfg(INT_FIFO_TH_RX, INT_FIFO_TH_RX);           // set INT2 active with RX fifo reaching threshold
                bGoRx();
                rf433_status = RF433_RX;
            } else if(rf433_status == RF433_RX) {
                uint8_t temp_rec;
                vEnableRdFifo();
                temp_rec = FIFO_ReadByte();
                for(uint8_t i=0; i<temp_rec; i++) {
                    mm[i] = FIFO_ReadByte();
                }
                bIntSrcFlagClr();
                vClearFIFO();
                rf433_flag = 1;
            }

        } else if(M0P_GPIO->PA_STAT_f.PA04) {    // Gpio_GetIrqStatus(PORT_KNOB_CHA, PIN_KNOB_CHA))

            knob_temp_a = M0P_GPIO->PAIN_f.PA04;
            knob_temp_b = M0P_GPIO->PAIN_f.PA05;

            if(g_knob_direction == DIRECTION_NONE) {
                if(knob_temp_a != knob_temp_b) {
                    g_knob_direction = DIRECTION_CCW_JIT;
                } else {
                    g_knob_direction = DIRECTION_NONE;
                }
            } else if(g_knob_direction == DIRECTION_CW_JIT) {
                if(knob_temp_a == knob_temp_b) {
                    g_knob_direction = DIRECTION_CW;
                } else {
                    g_knob_direction = DIRECTION_NONE;
                }
            }
            M0P_GPIO->PA_ICLR_f.PA04 = 0;         // Gpio_ClearIrq

        } else if(M0P_GPIO->PA_STAT_f.PA05) {     // Gpio_GetIrqStatus(PORT_KNOB_CHB, PIN_KNOB_CHB))

            knob_temp_a = M0P_GPIO->PAIN_f.PA04;
            knob_temp_b = M0P_GPIO->PAIN_f.PA05;

            if(g_knob_direction == DIRECTION_NONE) {
                if(knob_temp_a != knob_temp_b) {
                    g_knob_direction = DIRECTION_CW_JIT;
                } else {
                    g_knob_direction = DIRECTION_NONE;
                }
            } else if(g_knob_direction == DIRECTION_CCW_JIT) {
                if(knob_temp_a == knob_temp_b) {
                    g_knob_direction = DIRECTION_CCW;
                } else {
                    g_knob_direction = DIRECTION_NONE;
                }
            }

            M0P_GPIO->PA_ICLR_f.PA05 = 0;   // Gpio_ClearIrq

        }
    }
}

//                    printf("line: %d, func: %s\n", __LINE__, __FUNCTION__);

