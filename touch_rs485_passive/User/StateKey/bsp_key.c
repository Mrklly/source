#include "bsp_key.h"
#include "bsp_timer.h"
#include "bsp_wwdg.h"
#include "global_variable.h"


const key_io_t key_define[4] = {
    {KEY1_PORT, KEY1_PIN},
    {KEY2_PORT, KEY2_PIN},
    {KEY3_PORT, KEY3_PIN},
    {KEY4_PORT, KEY4_PIN}
};


void KEY_Init(void)
{
  GPIO_Init(KEY1_PORT,KEY1_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(KEY2_PORT,KEY2_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(KEY3_PORT,KEY3_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(KEY4_PORT,KEY4_PIN, GPIO_MODE_IN_FL_NO_IT);
}

uint8_t Key_Scan(void)
{
  uint8_t KeyTemp=0x00;                               //用来做临时按键值。
  if(GPIO_ReadInputPin(KEY1_PORT,KEY1_PIN)!=0)        //Key按键1
  { 
    KeyTemp=KEY1_VALUE;
    return KeyTemp;                                   //返回按键键值。
  }
  else if(GPIO_ReadInputPin(KEY2_PORT,KEY2_PIN)!=0)   //KEY按键2
  {
    KeyTemp=KEY2_VALUE;                            
    return KeyTemp;
  }
  else if(GPIO_ReadInputPin(KEY3_PORT,KEY3_PIN)!=0)  //KEY按键3
  {
    KeyTemp=KEY3_VALUE;
    return KeyTemp;
  }
  else if(GPIO_ReadInputPin(KEY4_PORT,KEY4_PIN)!=0)  //KEY按键4
  {
    KeyTemp=KEY4_VALUE;
    return KeyTemp;
  }
  else
  {
    KeyTemp=NO_KEY;
  }
  return KeyTemp;
}


uint8_t Key_value_detection (void)
{
    static uint8_t key_last[4] = {0, 0, 0, 0};
    uint8_t key_now[4] = {0, 0, 0, 0};

    volatile uint8_t temp_key_res = 0;
    volatile uint8_t temp_key_changed = 0;
    uint8_t i = 0;

    for(; i<4; i++) {

        if(KEY_CHECK(key_define[i].key_port, key_define[i].key_pin)) {
            temp_key_res |= 1 << i;
            key_now[i] = 1 << i;
        } else {
            temp_key_res &= ~(1 << i);
            key_now[i] = 0;
        }

        if(key_now[i] != key_last[i]) {
            temp_key_changed = 1;
        }

        key_last[i] = key_now[i];
    }

    if(temp_key_changed == 0) { // no key pressed
        return 0;
    }

    key_Result = temp_key_res;
    key_change_flag = temp_key_changed;

    if(key_Result) {
        key_down_value= key_Result;
    }

    key_send_flag = 0;
    
    return 0;
}


#if 0
uint8_t Key_value_detection (void)
{
    uint8_t i = 0;

    static uint8_t key_last = 0x00;
    volatile uint8_t key_temp = 0x00;

//    if(!master_flag) {
//        return ;
//    }

    for(; i<4; i++) {

        if(KEY_CHECK(key_define[i].key_port, key_define[i].key_pin)) {
            key_temp |= 1 << i;
        } else {
            key_temp &= ~(1 << i);
        }

        if((key_last & (1 << i)) == (key_temp & (1 << i))) {
            key_Result &= ~(1 << i) ;
        } else {
            key_Result |= 1 << i;
        }

        if(key_temp & (1 << i)) {
            key_last |= (1 << i);
        } else {
            key_last &= ~(1 << i);
        }
    }
    
//    if(key_last == key_temp) {
//        key_Result = 0;
//    } else {
//        key_Result = key_temp;
//    }

//    key_last = key_temp;

    return 0;
}
#endif


