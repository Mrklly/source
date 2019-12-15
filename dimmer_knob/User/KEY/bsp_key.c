#include "bsp_key.h"
extern uint8_t SendRs485Agreement[RS485_AGREEMENT_LEN];         //定义第三方协议发送缓存。
extern rs458_InitTypedef rs485InitStruct;                       //外部结构体变量。
extern uint32_t key_config[4];                                  //按键配置，用于对码。
extern uint8_t key_led_state[4];                                //LED灯临时状态。
uint8_t Key_Value_Temp = NO_KEY;
uint8_t Point_flag[4]   = {0, 0, 0, 0};                         //点动标记。
uint8_t Exclude_flag[4] = {0, 0, 0, 0};                         //排除标记。
extern uint8_t adjust1_flag;
extern uint8_t adjust2_flag;
extern uint8_t Chopping1_channel_cont;
extern uint8_t Chopping2_channel_cont;
uint8_t power1_flag = 0;                                        //第一路上电标志。
uint8_t power2_flag = 0;                                        //第一路上电标志。
/*pwm_flag标志有3种状态，
0x00数据不能修改，按键变光没有执行完成。
0x01代表按键有变化。
0x02代表变光完成，也代表按键无变化，代表数据可以修改。
*/
uint8_t pwm_flag[6] = {0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
uint8_t key_Result = NO_KEY;            //按键结果。
/*****************************************************************************************************
程序名：KEY_Init
功能：配置4个独立按键，工作在上拉输入模式。
参数：无参数。
返回值：无返回值。
*****************************************************************************************************/
void KEY_Init(void)
{
    stc_gpio_config_t     stcKEYPortCfg;

    DDL_ZERO_STRUCT(stcKEYPortCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    stcKEYPortCfg.enDir   = GpioDirIn;                            //输入。
    stcKEYPortCfg.enDrv   = GpioDrvL;                             //高驱动能力。
    stcKEYPortCfg.enPuPd  = GpioPu;                               //无上拉。
    stcKEYPortCfg.enOD    = GpioOdDisable;                        //开漏关闭。

    Gpio_Init(GpioPortB, GpioPin7,  &stcKEYPortCfg);
    EnableNvic(PORTB_IRQn, IrqLevel3, TRUE);
    Gpio_EnableIrq(GpioPortB, GpioPin7, GpioIrqRising);
    Gpio_EnableIrq(GpioPortB, GpioPin7, GpioIrqFalling);

    Gpio_Init(GpioPortA, GpioPin15, &stcKEYPortCfg);              //1
    Gpio_Init(GpioPortB, GpioPin3,  &stcKEYPortCfg);              //2
    Gpio_Init(GpioPortB, GpioPin4,  &stcKEYPortCfg);              //3
    Gpio_Init(GpioPortB, GpioPin5,  &stcKEYPortCfg);              //4
    Gpio_Init(GpioPortC, GpioPin14,  &stcKEYPortCfg);             //5
    Gpio_Init(GpioPortC, GpioPin15,  &stcKEYPortCfg);             //6

    //  EnableNvic(PORTA_IRQn, IrqLevel3, TRUE);


    //  Gpio_EnableIrq(GpioPortA, GpioPin15, GpioIrqFalling);
    //  Gpio_EnableIrq(GpioPortB, GpioPin3,  GpioIrqFalling);
    //  Gpio_EnableIrq(GpioPortB, GpioPin4,  GpioIrqFalling);
    //  Gpio_EnableIrq(GpioPortB, GpioPin5,  GpioIrqFalling);


    //***************************************************************************************************
    //    stcKEYPortCfg.enDir   = GpioDirOut;
    //    stcKEYPortCfg.enPuPd  = GpioPd;
    //    Gpio_Init(GpioPortA, GpioPin6, &stcKEYPortCfg);
    //    Gpio_Init(GpioPortA, GpioPin7, &stcKEYPortCfg);
    //    Gpio_Init(GpioPortB, GpioPin0, &stcKEYPortCfg);
    //    Gpio_Init(GpioPortB, GpioPin1, &stcKEYPortCfg);
    //    Gpio_ClrIO(GpioPortB, GpioPin1);
    //    Gpio_SetIO(GpioPortB, GpioPin1);
}

uint8_t Key_Scan(void)
{
    uint8_t KeyTemp = 0x00;                             //用来做临时按键值。

    if(Gpio_GetInputIO(KEY1_PORT, KEY1_PIN) == 0) {   //Key按键1
        KeyTemp = KEY1_VALUE;
        return KeyTemp;                                   //返回按键键值。
    } else if(Gpio_GetInputIO(KEY2_PORT, KEY2_PIN) == 0) { //KEY按键2
        KeyTemp = KEY2_VALUE;
        return KeyTemp;
    } else if(Gpio_GetInputIO(KEY3_PORT, KEY3_PIN) == 0) { //KEY按键3
        KeyTemp = KEY3_VALUE;
        return KeyTemp;
    } else if(Gpio_GetInputIO(KEY4_PORT, KEY4_PIN) == 0) { //KEY按键4
        KeyTemp = KEY4_VALUE;
        return KeyTemp;
    } else if(Gpio_GetInputIO(KEY5_PORT, KEY5_PIN) == 0) { //KEY按键5
        KeyTemp = KEY5_VALUE;
        return KeyTemp;
    } else if(Gpio_GetInputIO(KEY6_PORT, KEY6_PIN) == 0) { //KEY按键6
        KeyTemp = KEY6_VALUE;
        return KeyTemp;
    } else {
        KeyTemp = NO_KEY;
    }

    return KeyTemp;
}

uint8_t Key_value_detection (void)
{
    static uint8_t state = 0;                             //按键初始化按键状态机标志。
    static uint8_t key_last = 0;                          //上一次按键记录。
    static uint8_t key_now = 0;                           //现在按键记录。
    static uint8_t time_keep_cont = 0;                         //短按维持计时。
    static uint8_t time_cont = 0;                              //长按计时。
    static uint8_t temp = 0;                              //维持按键计时。
    uint8_t key_value = 0;                                //键值。


    key_now = Key_Scan();                                 //读取按键值。

    switch(state) {
    case STATE_KEY_UP: {                                  //初始化按键状态。
        if(key_now != key_last) {                         //判断本次按键是否与上一次按键相等。
            state = 1;                                      //转换到按键去抖状态。
        }
    }
    break;

    case STATE_KEY_DOWM: {                                //按键去抖动
        if(key_now == key_last) {
            state = 2;                                      //转换到按键短按状态。
        } else {
            state = 0;                                      //是抖动。
        }
    }
    break;

    case STATE_KEY_SHORT: {                               //按键短按状态。

        if(key_now == key_last) {                         //有按键按下。
            state = 3;                                      //转换到按键短按维持状态。
            key_value = key_last | KEY_SHORT;               //键值短按。
        } else {                                          //短按按键。
            state = 0;
            //key_value = key_last|KEY_SHORT;                 //键值短按。
        }
    }
    break;

    case STATE_KEY_KEEP:                                 //短按按键维持。
        if(key_now == key_last) {
            if(time_keep_cont++ <= 200) {                    //维持大约4s以上,在4ms范围内输出键值为短按。
                temp++;

                if(temp >= 8) {                                //维持按键计时。每160ms推出以下短按维持键值。
                    temp = 0;
                    key_value = key_last | KEY_SHORT_KEEP;       //键值短按维持输出25次，切换到长按状态。
                }
            } else {                                        //切换为长按状态。
                time_keep_cont = 0;
                temp = 0;
                state = 4;                                    //转换为长按状态。
            }
        } else {                                          //维持状态下按键释放。
            if(key_now == NO_KEY) {                         //按键正常抬起。
                state = 0;
                temp = 0;
                time_keep_cont = 0;
                //key_value = key_last|KEY_SHORT;               //键值短按。
                key_value = key_last | KEY_SHORT_UP;             //键值短按抬起
            } else {                                        //别的按键按下造成抬起。
                state = 1;
                temp = 0;
                time_keep_cont = 0;
                //key_value = key_last|KEY_SHORT;               //键值短按。
                key_value = key_last | KEY_SHORT_UP;             //键值短按抬起
            }
        }

        break;

    case STATE_KEY_LONG: {                             //长按状态。
        if(key_now == key_last) {                      //长按开始计时。
            if(time_cont++ > 100) {                      //6秒以上为长按。
                time_cont = 0;
                state = 5;                                 //按键转换为等待长按释放状态。
                key_value = key_last | KEY_LONG;           //键值是长按。
            }
        } else {
            if(key_now == NO_KEY) {                      //按键正常抬起。
                state = 0;
                time_cont = 0;
                //key_value = key_last|KEY_SHORT;            //键值修改为短按。
                key_value = key_last | KEY_SHORT_UP;          //键值修改为短按。
            } else {                                     //别的按键按下造成抬起。
                state = 1;
                time_cont = 0;
                //key_value = key_last|KEY_SHORT;            //键值修改为短按。
                key_value = key_last | KEY_SHORT_UP;          //键值修改为短按。
            }
        }
    }
    break;

    case STATE_KEY_LONG_UP: {                          //长按松手检测
        if(key_now != key_last) {                      //按键释放
            key_value = key_last | KEY_LONG_UP;          //键值修改为长按抬起。

            if(key_now == NO_KEY) {                      //按键正常抬起。
                state = 0;
            } else {                                     //别的按键按下造成抬起。
                state = 1;
            }
        } else {
            //key_value = key_last|KEY_LONG;
        }
    }
    break;

    default:
        key_value = 0;
        key_last = 0;
        key_now = 0;
        break;
    }

    key_last = key_now;                                //更新记录上一次按键。
    return key_value;                                  //返回键值。
}


void KEY_LED_Control(void)
{
    uint8_t key;

    if(key_Result == 0) {
        return;
    }

    key = key_Result;
    key_Result = 0x0;

    switch(key) {
    //第一路开关
    case KEY2_VALUE|KEY_SHORT:
        Modify_Ligth_state(1);
        break;

    //第二路开关
    case KEY5_VALUE|KEY_SHORT:
        Modify_Ligth_state(2);
        break;

    //第一路调亮
    case KEY3_VALUE|KEY_SHORT:
        if(key_led_state[0] == 0) {
            Ligth_PowerOn(1);
            LigthLED_ON(3);//按键3
            break;
        }

        if(Chopping1_channel_cont <= 0) {
            Chopping1_channel_cont = 0;
            Gpio_ClrIO(CH3_PORT, CH3_PIN);
            break;
        }

        Chopping1_channel_cont--;
        LigthLED_ON(3);//按键3
        Gpio_ClrIO(CH3_PORT, CH3_PIN);
        break;

    case KEY3_VALUE|KEY_SHORT_KEEP :
        if(Chopping1_channel_cont <= 0) {
            Chopping1_channel_cont = 0;
            break;
        }

        Chopping1_channel_cont--;
        LigthLED_ON(3);//按键3
        break;

    case KEY3_VALUE|KEY_SHORT_UP:
        LigthLED_OFF(3);//按键3
        break;

    case KEY3_VALUE|KEY_LONG_UP:
        LigthLED_OFF(3);//按键3
        break;


    //第一路调暗
    case KEY1_VALUE|KEY_SHORT:
        if(power1_flag == 1) {
            Chopping1_channel_cont++;

            if(Chopping1_channel_cont >= STEP) {
                Ligth_PowerOff(1);
                break;
            }

            LigthLED_ON(1);//按键1
        }

        Gpio_ClrIO(CH3_PORT, CH3_PIN);
        break;

    case KEY1_VALUE|KEY_SHORT_KEEP:
        if(power1_flag == 1) {
            Chopping1_channel_cont++;

            if(Chopping1_channel_cont >= STEP) {
                Ligth_PowerOff(1);
                break;
            }

            LigthLED_ON(1);//按键1
        }

        break;

    case KEY1_VALUE|KEY_SHORT_UP:
        LigthLED_OFF(1);//按键1
        break;

    case KEY1_VALUE|KEY_LONG_UP:
        LigthLED_OFF(1);//按键1
        break;

    //第二路调亮
    case KEY4_VALUE|KEY_SHORT:
        if(key_led_state[1] == 0) {
            Ligth_PowerOn(2);
            LigthLED_ON(4);//按键4
            break;
        }

        if(Chopping2_channel_cont <= 0) {
            Chopping2_channel_cont = 0;
            Gpio_ClrIO(CH4_PORT, CH4_PIN);
            break;
        }

        Chopping2_channel_cont--;
        LigthLED_ON(4);//按键4
        Gpio_ClrIO(CH4_PORT, CH4_PIN);
        break;

    case KEY4_VALUE|KEY_SHORT_KEEP:
        if(Chopping2_channel_cont <= 0) {
            Chopping2_channel_cont = 0;
            break;
        }

        Chopping2_channel_cont--;
        LigthLED_ON(4);//按键4
        break;

    case KEY4_VALUE|KEY_SHORT_UP:
        LigthLED_OFF(4);//按键4
        break;

    case KEY4_VALUE|KEY_LONG_UP:
        LigthLED_OFF(4);//按键4
        break;

    //第二路调暗
    case KEY6_VALUE|KEY_SHORT:
        if(power2_flag == 1) {
            Chopping2_channel_cont++;

            if(Chopping2_channel_cont >= STEP) {
                Ligth_PowerOff(2);
                break;
            }

            LigthLED_ON(6);//按键6
        }

        Gpio_ClrIO(CH4_PORT, CH4_PIN);
        break;

    case KEY6_VALUE|KEY_SHORT_KEEP:
        if(power2_flag == 1) {
            Chopping2_channel_cont++;

            if(Chopping2_channel_cont >= STEP) {
                Ligth_PowerOff(2);
            }

            LigthLED_ON(6);//按键6
        }

        break;

    case KEY6_VALUE|KEY_SHORT_UP:
        LigthLED_OFF(6);//按键6
        break;

    case KEY6_VALUE|KEY_LONG_UP:
        LigthLED_OFF(6);//按键6
        break;

    default:
        key = NO_KEY;
        break;
    }
}

void KEY_value_Loading_Data(uint8_t KeyVelue)
{
    if(rs485InitStruct.key_partition[KeyVelue] == 0) {                            //判断是否具有分区属性。
        SendRs485Agreement[0] = 0xFA;                                               //装载普通模式数据头。
        SendRs485Agreement[1] = 00;                                                 //装载普通数据。
        SendRs485Agreement[4] = key_led_state[KeyVelue];                            //装载开关状态。
        SendRs485Agreement[2] = rs485InitStruct.key_function[KeyVelue += KeyVelue]; //装载按键配置功能。
        SendRs485Agreement[3] = rs485InitStruct.key_function[KeyVelue + 1];         //装载按键配置功能。
        SendRs485Agreement[5] = RS485_Parse_check(4, SendRs485Agreement);           //装载校验码。
    } else {                                                                     //有分区属性装载FC打头数据。
        SendRs485Agreement[0] = 0xFC;                                              //转载情景模式数据头。
        SendRs485Agreement[1] = rs485InitStruct.key_partition[KeyVelue];           //装载情景模式区域属性。
        SendRs485Agreement[4] = key_led_state[KeyVelue];                           //开关状态。
        SendRs485Agreement[2] = rs485InitStruct.key_function[KeyVelue += KeyVelue]; //装载按键配置功能
        SendRs485Agreement[3] = rs485InitStruct.key_function[KeyVelue + 1];        //装载按键配置功能。
        SendRs485Agreement[5] = RS485_Parse_check(4, SendRs485Agreement);          //装载校验码。
    }
}
/*****************************************************************************************************
** 程序名：KEY_SEND_Code
** 功能：根据键值，发送广播,并且处理本机函数。
** 参数1：按键功能，
** 参数2：按键值。0 1 2 3
** 返回值：无返回值。
*****************************************************************************************************/
void KEY_SEND_Code(uint8_t keyfun, uint8_t keynumber)
{
    switch(keyfun) {
    case ORDINARY_BUTTON_CMD:                     //普通按键直接退出。0x01
        break;

    case TOTAL_SW_CMD:                            //总开关OK。0x02
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case GENERAL_OPEN_CMD:                        //总开。OK。0x03
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case GENERAL_CLOSE_COM:                       //总关。OK  0x04
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case MULTI_CONTROL_CMD:                       //多控开关。OK 0x06
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case DOUBLE_CONTROL_CMD:                      //双控开关。OK 0x07
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case MULTI_INTERLOCKING_CMD:                  //多互锁。OK 0x09
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case DOUBLE_INTERLOCKING_CMD:                 //双控互锁。OK 0x08
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case POINT_LOCK_CMD:                          //点动自锁。跟普通开关相同。OK 0x0b
        break;

    case NIGHT_LAMP_CMD:                          //夜灯OK 0x0c
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case SPECIAL_SERVICE_CMD:                     //独立开关，不受总开关控制。0x0d
        break;

    case INTER_LOCK_CMD:                          //互锁。OK 0x0e
        KEY_value_Loading_Data(keynumber);
        SendRs485Agreement[4] = 0x00;               //按键按下后永远发出关灯命令
        SendRs485Agreement[5] = RS485_Parse_check(4, SendRs485Agreement);
        Uploading_data();
        Interlocking_control(SendRs485Agreement, keynumber);
        break;

    case CLEAR_DISTURB_CMD:                       //清理勿扰。0x0f
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case PORCH_LAMP_CMMD:                         //廊灯。   0x51
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case ELECTRIC_LAMP_CMMD:                      //上电亮灯，同名不双控。0x52
        break;

    default:
        Common_pattern_control(SendRs485Agreement); //进入普通命令处理。
        break;
    }
}
/*****************************************************************************************************
程序名：Uploading_data
功能：上传数据函数。会把发送缓存的所有数据发给上位机。
参数：无参数。
返回值：无返回值。
*****************************************************************************************************/
void Uploading_data(void)
{
    UART_Multi_byte(SendRs485Agreement, 6);
}
/*****************************************************************************************************
程序名：KEY_Modify_LED_state
功能：按键按下修改LED状态。
参数：参数1：传入按键号。0,1,2,3。
返回值：无返回值。
*****************************************************************************************************/
#if 0
void KEY_Modify_LED_state(uint8_t key)
{
    key_led_state[key - 1] = !key_led_state[key - 1]; //此函数被调用代表有按键按下，按键按下灯状态变量就需要取反。

    if((key == 1) || (key == 2)) {
        //    if(key_led_state[key-1]==0)
        //    {
        //      Linkage_open_close(1,0);
        //    }
        //    else
        //    {
        //      Linkage_open_close(1,1);
        //    }
    } else {
        //    if(key_led_state[key-1]==0)                      //根据灯变量状态确定灯的状态。
        //    {
        //      Linkage_open_close(key,0);
        //    }
        //    else
        //    {
        //      Linkage_open_close(key,1);
        //    }
    }
}
#endif
void KEY_Modify_LED_state(uint8_t key, uint8_t state)
{
    key_led_state[key - 1] = state;
}

/*****************************************************************************************************
程序名：Modify_Ligth_state
功能：      修改灯开与关的状态
参数：      第几路
返回值：无
*****************************************************************************************************/
void Modify_Ligth_state(uint8_t number)
{
    if(key_led_state[number - 1] == 0) {
        Ligth_PowerOn(number);
    } else {
        Ligth_PowerOff(number);
    }
}

/*****************************************************************************************************
程序名：Ligth_PowerOn
功能：  开灯
参数1：第几路1,2;
返回值：无
*****************************************************************************************************/
void Ligth_PowerOn(uint8_t number)
{
    if(number == 1) {
        KEY_Modify_LED_state(1, 1);
        pwm_flag[1] = 0x01;
        Chopping1_channel_cont = 13;
        power1_flag = 1;
        Gpio_ClrIO(CH3_PORT, CH3_PIN);
    }

    if(number == 2) {
        KEY_Modify_LED_state(2, 1);
        pwm_flag[4] = 0x01;
        Chopping2_channel_cont = 13;
        power2_flag = 1;
        Gpio_ClrIO(CH4_PORT, CH4_PIN);
    }

}


/*****************************************************************************************************
程序名：Ligth_PowerOff
功能：  关灯
参数1：第几路1,2;
返回值：无
*****************************************************************************************************/
void Ligth_PowerOff(uint8_t number)
{
    if(number == 1) {
        KEY_Modify_LED_state(1, 0);
        pwm_flag[1] = 0x01;
        Chopping1_channel_cont = STEP;
        power1_flag = 0;
        Gpio_ClrIO(CH3_PORT, CH3_PIN);
    }

    if(number == 2) {
        KEY_Modify_LED_state(2, 0);
        pwm_flag[4] = 0x01;
        Chopping2_channel_cont = STEP;
        power2_flag = 0;
        Gpio_ClrIO(CH4_PORT, CH4_PIN);
    }
}


/*****************************************************************************************************
程序名：Local_control
功能：本机控制函数，按键按控制本机。
参数：无参数。
返回值：无返回值。
*****************************************************************************************************/
void Local_control(void)
{
    switch(SendRs485Agreement[2]) {
    case TOTAL_SW_CMD:                                    //接收到总开关命令
        if((SendRs485Agreement[1] != 0) && (SendRs485Agreement[0] == 0xFC)) {
            switch(SendRs485Agreement[4]) {
            case LED_ON:
                Regional_Close_opening(1, SendRs485Agreement);
                break;

            case LED_OFF:
                Regional_Close_opening(0, SendRs485Agreement);
                break;

            default:
                break;
            }
        } else {
            switch(SendRs485Agreement[4]) {
            case LED_ON:
                Always_LED_control(1);
                break;

            case LED_OFF:
                Always_LED_control(0);
                break;

            default:
                break;
            }
        }

        break;

    case GENERAL_OPEN_CMD:                                //接收到总开命令处理。
        if((SendRs485Agreement[1] != 0) && (SendRs485Agreement[0] == 0xFC)) {
            Regional_Close_opening(1, SendRs485Agreement);
        } else {
            Always_LED_control(1);
        }

        break;

    case GENERAL_CLOSE_COM:                               //接收到总关命令处理。
        if((SendRs485Agreement[1] != 0) && (SendRs485Agreement[0] == 0xFC)) {
            Regional_Close_opening(0, SendRs485Agreement);
        } else {
            Always_LED_control(0);
        }

        break;

    case MULTI_INTERLOCKING_CMD:                          //本机多互锁命令处理。
        Public_Multi_interlocking(SendRs485Agreement);
        break;

    case DOUBLE_INTERLOCKING_CMD:                         //本机双控互锁命令处理。
        Public_Multi_interlocking(SendRs485Agreement);
        break;

    case CLEAR_DISTURB_CMD:                                       //本机清理勿扰命令处理。
        Public_Multi_interlocking(SendRs485Agreement);
        break;

    default:
        Common_pattern_control(SendRs485Agreement);                 //进入普通命令处理。
        break;
    }
}
/*****************************************************************************************************
程序名：Interlocking_control
功能：本机互锁处理
参数：无参数。
返回值：参数1：传入发送缓存，参数2：传入按键值。
*****************************************************************************************************/
void Interlocking_control(uint8_t *buff, uint8_t key)
{
    uint8_t i;

    for(i = 0; i < 4; i++) {
        if(i == key) {

        } else if((Broadcast_data_packing (buff) == key_config[i])) { //按键对码。
            switch(buff[4]) {                                        //比对开关属性。
            case LED_ON:
                Linkage_open_close(i + 1, 1);
                break;

            case LED_OFF:
                Linkage_open_close(i + 1, 0);
                break;

            default:
                break;
            }
        }
    }
}
/*****************************************************************************************************
程序名：Point_movement
功能：点动按键标记处理
参数：无参数。
返回值：无返回值
*****************************************************************************************************/
void Point_movement(void)
{
    uint8_t temp;

    for(temp = 0; temp < 4; temp++) {
        if(rs485InitStruct.key_function[temp * 2] == POINT_PRESS_CMD) {
            Point_flag[temp] = 1;
        } else {
            Point_flag[temp] = 0;
        }
    }
}

/*****************************************************************************************************
程序名：LigthLED_ON
功能：按键光圈开启
参数：按键值
返回值：无返回值
*****************************************************************************************************/
void LigthLED_ON(uint8_t key)
{
    SetTIM1_PWM_DutyCycle(0, key);
    back_LED_open_Close_Config(key, 0);
}


/*****************************************************************************************************
程序名：LigthLED_OFF
功能：按键光圈关闭
参数：按键值
返回值：无返回值
*****************************************************************************************************/
void LigthLED_OFF(uint8_t key)
{
    SetTIM1_PWM_DutyCycle(1800, key);
    back_LED_open_Close_Config(key, 1);
}


/*****************************************************************************************************
程序名：Dismantling_function
功能：根据功能与按键编号，处理相应功能。
参数：参数1：按键功能，参数2：按键编号。参数3：代表按键释放与按下。
返回值：无返回值
*****************************************************************************************************/
void Dismantling_function(uint8_t function, uint8_t key_number, uint8_t state)
{
    switch(function) {
    case GENERAL_OPEN_CMD:                                 //总开排除。
        if(state) {
            SetTIM1_PWM_DutyCycle(1800, key_number);            //关闭灯。
            back_LED_open_Close_Config(key_number, 0);
        } else {
            SetTIM1_PWM_DutyCycle(0, key_number);
            back_LED_open_Close_Config(key_number, 1);
        }

        Exclude_flag[key_number - 1] = 1;
        key_led_state[key_number - 1] = 1;                   //此函数被调用代表有按键按下，按键按下灯状态变量就需要取反。
        break;

    case GENERAL_CLOSE_COM:                                //总关排除。
        if(state) {
            SetTIM1_PWM_DutyCycle(1800, key_number);           //关闭灯。
            back_LED_open_Close_Config(key_number, 1);
        } else {
            SetTIM1_PWM_DutyCycle(0, key_number);
            back_LED_open_Close_Config(key_number, 0);
        }

        Exclude_flag[key_number - 1] = 1;
        break;

    default:
        Exclude_flag[key_number - 1] = 0;
        break;
    }
}

