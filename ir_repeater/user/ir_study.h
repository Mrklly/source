#ifndef _IR_STUDY_H_
#define _IR_STUDY_H_



#define NUM_POWER      2    // On off
#define NUM_MODE       3    // Cold Warm Fan
#define NUM_FAN        3    // Low Mid High
#define NUM_TEMP       15   // 16 to 30

#define NUM_IR_ARRAY   (NUM_POWER * NUM_MODE * NUM_FAN * NUM_TEMP)

#define ADDR_IR_OFS_POW  (g_dev.sta.power * NUM_MODE * NUM_FAN * NUM_TEMP)
#define ADDR_IR_OFS_MOD  (g_dev.sta.mode   * NUM_FAN * NUM_TEMP)
#define ADDR_IR_OFS_WID  ((g_dev.sta.wind - 1)  * NUM_TEMP) // default wind is Min = 1, not close
#define ADDR_IR_OFS_TMP  (g_dev.set_temp - 16)               // default set_set = 16

#define ADDR_IR_DATA  ((ADDR_IR_OFS_POW + ADDR_IR_OFS_MOD + ADDR_IR_OFS_WID + ADDR_IR_OFS_TMP) << 10)


void ir_study_clear_data(void);
void ir_study_read_part(void);
void ir_study_write_part(void);

void ir_repeater_send(void);
void ir_repeater_run(void);



#endif

