#ifndef _IR_STUDY_H_
#define _IR_STUDY_H_


#include "drv_ir.h"
#include "ir_parse.h"
#include "eeprom.h"
#include "wireless.h"


#define NUM_POWER      2    // On off
#define NUM_MODE       3    // Cold Warm Fan
#define NUM_FAN        3    // Low Mid High
#define NUM_TEMP       15   // 16 to 30

#define NUM_IR_ARRAY   (NUM_POWER * NUM_MODE * NUM_FAN * NUM_TEMP)


// flash address 0xEC00 to 0xFBFF = 4k bytes
#define EEP_IR_SIZE    (NUM_IR_ARRAY * sizeof(gree_data))   // 3240 bytes

#define EEP_IR_OFS_POW  (g_dev.sta.power * NUM_MODE * NUM_FAN * NUM_TEMP)
#define EEP_IR_OFS_MOD  (g_dev.sta.mode   * NUM_FAN * NUM_TEMP)
#define EEP_IR_OFS_WID  ((g_dev.sta.wind - 1)  * NUM_TEMP) // default wind is Min = 1, not close
#define EEP_IR_OFS_TMP  (g_dev.set_temp - 16)               // default set_set = 16

#define EEP_IR_OFS_ALL  ((EEP_IR_OFS_POW + EEP_IR_OFS_MOD + EEP_IR_OFS_WID + EEP_IR_OFS_TMP)*sizeof(gree_data))


#define IR_RF_NONE  0x00
#define IR_RF_STRT  0x01
#define IR_RF_STOP  0x02
#define IR_RF_WAIT  0x04
#define IR_RF_END   0x10


void ir_study_run(void);


void btn_callback(uint8_t key);
void ir_eep_send(void);
void ir_eep_recv(void);



#endif

