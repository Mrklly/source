#include "bsp_key.h"
extern uint8_t SendRs485Agreement[RS485_AGREEMENT_LEN];         //���������Э�鷢�ͻ��档
extern rs458_InitTypedef rs485InitStruct;                       //�ⲿ�ṹ�������
extern uint32_t key_config[4];                                  //�������ã����ڶ��롣
extern uint8_t key_led_state[4];                                //LED����ʱ״̬��
uint8_t Key_Value_Temp = NO_KEY;
uint8_t Point_flag[4]   = {0, 0, 0, 0};                         //�㶯��ǡ�
uint8_t Exclude_flag[4] = {0, 0, 0, 0};                         //�ų���ǡ�
extern uint8_t adjust1_flag;
extern uint8_t adjust2_flag;
extern uint8_t Chopping1_channel_cont;
extern uint8_t Chopping2_channel_cont;
uint8_t power1_flag = 0;                                        //��һ·�ϵ��־��
uint8_t power2_flag = 0;                                        //��һ·�ϵ��־��
/*pwm_flag��־��3��״̬��
0x00���ݲ����޸ģ��������û��ִ����ɡ�
0x01�������б仯��
0x02��������ɣ�Ҳ�������ޱ仯���������ݿ����޸ġ�
*/
uint8_t pwm_flag[6] = {0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
uint8_t key_Result = NO_KEY;            //���������
/*****************************************************************************************************
��������KEY_Init
���ܣ�����4��������������������������ģʽ��
�������޲�����
����ֵ���޷���ֵ��
*****************************************************************************************************/
void KEY_Init(void)
{
    stc_gpio_config_t     stcKEYPortCfg;

    DDL_ZERO_STRUCT(stcKEYPortCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    stcKEYPortCfg.enDir   = GpioDirIn;                            //���롣
    stcKEYPortCfg.enDrv   = GpioDrvL;                             //������������
    stcKEYPortCfg.enPuPd  = GpioPu;                               //��������
    stcKEYPortCfg.enOD    = GpioOdDisable;                        //��©�رա�

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
    uint8_t KeyTemp = 0x00;                             //��������ʱ����ֵ��

    if(Gpio_GetInputIO(KEY1_PORT, KEY1_PIN) == 0) {   //Key����1
        KeyTemp = KEY1_VALUE;
        return KeyTemp;                                   //���ذ�����ֵ��
    } else if(Gpio_GetInputIO(KEY2_PORT, KEY2_PIN) == 0) { //KEY����2
        KeyTemp = KEY2_VALUE;
        return KeyTemp;
    } else if(Gpio_GetInputIO(KEY3_PORT, KEY3_PIN) == 0) { //KEY����3
        KeyTemp = KEY3_VALUE;
        return KeyTemp;
    } else if(Gpio_GetInputIO(KEY4_PORT, KEY4_PIN) == 0) { //KEY����4
        KeyTemp = KEY4_VALUE;
        return KeyTemp;
    } else if(Gpio_GetInputIO(KEY5_PORT, KEY5_PIN) == 0) { //KEY����5
        KeyTemp = KEY5_VALUE;
        return KeyTemp;
    } else if(Gpio_GetInputIO(KEY6_PORT, KEY6_PIN) == 0) { //KEY����6
        KeyTemp = KEY6_VALUE;
        return KeyTemp;
    } else {
        KeyTemp = NO_KEY;
    }

    return KeyTemp;
}

uint8_t Key_value_detection (void)
{
    static uint8_t state = 0;                             //������ʼ������״̬����־��
    static uint8_t key_last = 0;                          //��һ�ΰ�����¼��
    static uint8_t key_now = 0;                           //���ڰ�����¼��
    static uint8_t time_keep_cont = 0;                         //�̰�ά�ּ�ʱ��
    static uint8_t time_cont = 0;                              //������ʱ��
    static uint8_t temp = 0;                              //ά�ְ�����ʱ��
    uint8_t key_value = 0;                                //��ֵ��


    key_now = Key_Scan();                                 //��ȡ����ֵ��

    switch(state) {
    case STATE_KEY_UP: {                                  //��ʼ������״̬��
        if(key_now != key_last) {                         //�жϱ��ΰ����Ƿ�����һ�ΰ�����ȡ�
            state = 1;                                      //ת��������ȥ��״̬��
        }
    }
    break;

    case STATE_KEY_DOWM: {                                //����ȥ����
        if(key_now == key_last) {
            state = 2;                                      //ת���������̰�״̬��
        } else {
            state = 0;                                      //�Ƕ�����
        }
    }
    break;

    case STATE_KEY_SHORT: {                               //�����̰�״̬��

        if(key_now == key_last) {                         //�а������¡�
            state = 3;                                      //ת���������̰�ά��״̬��
            key_value = key_last | KEY_SHORT;               //��ֵ�̰���
        } else {                                          //�̰�������
            state = 0;
            //key_value = key_last|KEY_SHORT;                 //��ֵ�̰���
        }
    }
    break;

    case STATE_KEY_KEEP:                                 //�̰�����ά�֡�
        if(key_now == key_last) {
            if(time_keep_cont++ <= 200) {                    //ά�ִ�Լ4s����,��4ms��Χ�������ֵΪ�̰���
                temp++;

                if(temp >= 8) {                                //ά�ְ�����ʱ��ÿ160ms�Ƴ����¶̰�ά�ּ�ֵ��
                    temp = 0;
                    key_value = key_last | KEY_SHORT_KEEP;       //��ֵ�̰�ά�����25�Σ��л�������״̬��
                }
            } else {                                        //�л�Ϊ����״̬��
                time_keep_cont = 0;
                temp = 0;
                state = 4;                                    //ת��Ϊ����״̬��
            }
        } else {                                          //ά��״̬�°����ͷš�
            if(key_now == NO_KEY) {                         //��������̧��
                state = 0;
                temp = 0;
                time_keep_cont = 0;
                //key_value = key_last|KEY_SHORT;               //��ֵ�̰���
                key_value = key_last | KEY_SHORT_UP;             //��ֵ�̰�̧��
            } else {                                        //��İ����������̧��
                state = 1;
                temp = 0;
                time_keep_cont = 0;
                //key_value = key_last|KEY_SHORT;               //��ֵ�̰���
                key_value = key_last | KEY_SHORT_UP;             //��ֵ�̰�̧��
            }
        }

        break;

    case STATE_KEY_LONG: {                             //����״̬��
        if(key_now == key_last) {                      //������ʼ��ʱ��
            if(time_cont++ > 100) {                      //6������Ϊ������
                time_cont = 0;
                state = 5;                                 //����ת��Ϊ�ȴ������ͷ�״̬��
                key_value = key_last | KEY_LONG;           //��ֵ�ǳ�����
            }
        } else {
            if(key_now == NO_KEY) {                      //��������̧��
                state = 0;
                time_cont = 0;
                //key_value = key_last|KEY_SHORT;            //��ֵ�޸�Ϊ�̰���
                key_value = key_last | KEY_SHORT_UP;          //��ֵ�޸�Ϊ�̰���
            } else {                                     //��İ����������̧��
                state = 1;
                time_cont = 0;
                //key_value = key_last|KEY_SHORT;            //��ֵ�޸�Ϊ�̰���
                key_value = key_last | KEY_SHORT_UP;          //��ֵ�޸�Ϊ�̰���
            }
        }
    }
    break;

    case STATE_KEY_LONG_UP: {                          //�������ּ��
        if(key_now != key_last) {                      //�����ͷ�
            key_value = key_last | KEY_LONG_UP;          //��ֵ�޸�Ϊ����̧��

            if(key_now == NO_KEY) {                      //��������̧��
                state = 0;
            } else {                                     //��İ����������̧��
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

    key_last = key_now;                                //���¼�¼��һ�ΰ�����
    return key_value;                                  //���ؼ�ֵ��
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
    //��һ·����
    case KEY2_VALUE|KEY_SHORT:
        Modify_Ligth_state(1);
        break;

    //�ڶ�·����
    case KEY5_VALUE|KEY_SHORT:
        Modify_Ligth_state(2);
        break;

    //��һ·����
    case KEY3_VALUE|KEY_SHORT:
        if(key_led_state[0] == 0) {
            Ligth_PowerOn(1);
            LigthLED_ON(3);//����3
            break;
        }

        if(Chopping1_channel_cont <= 0) {
            Chopping1_channel_cont = 0;
            Gpio_ClrIO(CH3_PORT, CH3_PIN);
            break;
        }

        Chopping1_channel_cont--;
        LigthLED_ON(3);//����3
        Gpio_ClrIO(CH3_PORT, CH3_PIN);
        break;

    case KEY3_VALUE|KEY_SHORT_KEEP :
        if(Chopping1_channel_cont <= 0) {
            Chopping1_channel_cont = 0;
            break;
        }

        Chopping1_channel_cont--;
        LigthLED_ON(3);//����3
        break;

    case KEY3_VALUE|KEY_SHORT_UP:
        LigthLED_OFF(3);//����3
        break;

    case KEY3_VALUE|KEY_LONG_UP:
        LigthLED_OFF(3);//����3
        break;


    //��һ·����
    case KEY1_VALUE|KEY_SHORT:
        if(power1_flag == 1) {
            Chopping1_channel_cont++;

            if(Chopping1_channel_cont >= STEP) {
                Ligth_PowerOff(1);
                break;
            }

            LigthLED_ON(1);//����1
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

            LigthLED_ON(1);//����1
        }

        break;

    case KEY1_VALUE|KEY_SHORT_UP:
        LigthLED_OFF(1);//����1
        break;

    case KEY1_VALUE|KEY_LONG_UP:
        LigthLED_OFF(1);//����1
        break;

    //�ڶ�·����
    case KEY4_VALUE|KEY_SHORT:
        if(key_led_state[1] == 0) {
            Ligth_PowerOn(2);
            LigthLED_ON(4);//����4
            break;
        }

        if(Chopping2_channel_cont <= 0) {
            Chopping2_channel_cont = 0;
            Gpio_ClrIO(CH4_PORT, CH4_PIN);
            break;
        }

        Chopping2_channel_cont--;
        LigthLED_ON(4);//����4
        Gpio_ClrIO(CH4_PORT, CH4_PIN);
        break;

    case KEY4_VALUE|KEY_SHORT_KEEP:
        if(Chopping2_channel_cont <= 0) {
            Chopping2_channel_cont = 0;
            break;
        }

        Chopping2_channel_cont--;
        LigthLED_ON(4);//����4
        break;

    case KEY4_VALUE|KEY_SHORT_UP:
        LigthLED_OFF(4);//����4
        break;

    case KEY4_VALUE|KEY_LONG_UP:
        LigthLED_OFF(4);//����4
        break;

    //�ڶ�·����
    case KEY6_VALUE|KEY_SHORT:
        if(power2_flag == 1) {
            Chopping2_channel_cont++;

            if(Chopping2_channel_cont >= STEP) {
                Ligth_PowerOff(2);
                break;
            }

            LigthLED_ON(6);//����6
        }

        Gpio_ClrIO(CH4_PORT, CH4_PIN);
        break;

    case KEY6_VALUE|KEY_SHORT_KEEP:
        if(power2_flag == 1) {
            Chopping2_channel_cont++;

            if(Chopping2_channel_cont >= STEP) {
                Ligth_PowerOff(2);
            }

            LigthLED_ON(6);//����6
        }

        break;

    case KEY6_VALUE|KEY_SHORT_UP:
        LigthLED_OFF(6);//����6
        break;

    case KEY6_VALUE|KEY_LONG_UP:
        LigthLED_OFF(6);//����6
        break;

    default:
        key = NO_KEY;
        break;
    }
}

void KEY_value_Loading_Data(uint8_t KeyVelue)
{
    if(rs485InitStruct.key_partition[KeyVelue] == 0) {                            //�ж��Ƿ���з������ԡ�
        SendRs485Agreement[0] = 0xFA;                                               //װ����ͨģʽ����ͷ��
        SendRs485Agreement[1] = 00;                                                 //װ����ͨ���ݡ�
        SendRs485Agreement[4] = key_led_state[KeyVelue];                            //װ�ؿ���״̬��
        SendRs485Agreement[2] = rs485InitStruct.key_function[KeyVelue += KeyVelue]; //װ�ذ������ù��ܡ�
        SendRs485Agreement[3] = rs485InitStruct.key_function[KeyVelue + 1];         //װ�ذ������ù��ܡ�
        SendRs485Agreement[5] = RS485_Parse_check(4, SendRs485Agreement);           //װ��У���롣
    } else {                                                                     //�з�������װ��FC��ͷ���ݡ�
        SendRs485Agreement[0] = 0xFC;                                              //ת���龰ģʽ����ͷ��
        SendRs485Agreement[1] = rs485InitStruct.key_partition[KeyVelue];           //װ���龰ģʽ�������ԡ�
        SendRs485Agreement[4] = key_led_state[KeyVelue];                           //����״̬��
        SendRs485Agreement[2] = rs485InitStruct.key_function[KeyVelue += KeyVelue]; //װ�ذ������ù���
        SendRs485Agreement[3] = rs485InitStruct.key_function[KeyVelue + 1];        //װ�ذ������ù��ܡ�
        SendRs485Agreement[5] = RS485_Parse_check(4, SendRs485Agreement);          //װ��У���롣
    }
}
/*****************************************************************************************************
** ��������KEY_SEND_Code
** ���ܣ����ݼ�ֵ�����͹㲥,���Ҵ�����������
** ����1���������ܣ�
** ����2������ֵ��0 1 2 3
** ����ֵ���޷���ֵ��
*****************************************************************************************************/
void KEY_SEND_Code(uint8_t keyfun, uint8_t keynumber)
{
    switch(keyfun) {
    case ORDINARY_BUTTON_CMD:                     //��ͨ����ֱ���˳���0x01
        break;

    case TOTAL_SW_CMD:                            //�ܿ���OK��0x02
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case GENERAL_OPEN_CMD:                        //�ܿ���OK��0x03
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case GENERAL_CLOSE_COM:                       //�ܹء�OK  0x04
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case MULTI_CONTROL_CMD:                       //��ؿ��ء�OK 0x06
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case DOUBLE_CONTROL_CMD:                      //˫�ؿ��ء�OK 0x07
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case MULTI_INTERLOCKING_CMD:                  //�໥����OK 0x09
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case DOUBLE_INTERLOCKING_CMD:                 //˫�ػ�����OK 0x08
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case POINT_LOCK_CMD:                          //�㶯����������ͨ������ͬ��OK 0x0b
        break;

    case NIGHT_LAMP_CMD:                          //ҹ��OK 0x0c
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case SPECIAL_SERVICE_CMD:                     //�������أ������ܿ��ؿ��ơ�0x0d
        break;

    case INTER_LOCK_CMD:                          //������OK 0x0e
        KEY_value_Loading_Data(keynumber);
        SendRs485Agreement[4] = 0x00;               //�������º���Զ�����ص�����
        SendRs485Agreement[5] = RS485_Parse_check(4, SendRs485Agreement);
        Uploading_data();
        Interlocking_control(SendRs485Agreement, keynumber);
        break;

    case CLEAR_DISTURB_CMD:                       //�������š�0x0f
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case PORCH_LAMP_CMMD:                         //�ȵơ�   0x51
        KEY_value_Loading_Data(keynumber);
        Uploading_data();
        Local_control();
        break;

    case ELECTRIC_LAMP_CMMD:                      //�ϵ����ƣ�ͬ����˫�ء�0x52
        break;

    default:
        Common_pattern_control(SendRs485Agreement); //������ͨ�����
        break;
    }
}
/*****************************************************************************************************
��������Uploading_data
���ܣ��ϴ����ݺ�������ѷ��ͻ�����������ݷ�����λ����
�������޲�����
����ֵ���޷���ֵ��
*****************************************************************************************************/
void Uploading_data(void)
{
    UART_Multi_byte(SendRs485Agreement, 6);
}
/*****************************************************************************************************
��������KEY_Modify_LED_state
���ܣ����������޸�LED״̬��
����������1�����밴���š�0,1,2,3��
����ֵ���޷���ֵ��
*****************************************************************************************************/
#if 0
void KEY_Modify_LED_state(uint8_t key)
{
    key_led_state[key - 1] = !key_led_state[key - 1]; //�˺��������ô����а������£��������µ�״̬��������Ҫȡ����

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
        //    if(key_led_state[key-1]==0)                      //���ݵƱ���״̬ȷ���Ƶ�״̬��
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
��������Modify_Ligth_state
���ܣ�      �޸ĵƿ���ص�״̬
������      �ڼ�·
����ֵ����
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
��������Ligth_PowerOn
���ܣ�  ����
����1���ڼ�·1,2;
����ֵ����
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
��������Ligth_PowerOff
���ܣ�  �ص�
����1���ڼ�·1,2;
����ֵ����
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
��������Local_control
���ܣ��������ƺ��������������Ʊ�����
�������޲�����
����ֵ���޷���ֵ��
*****************************************************************************************************/
void Local_control(void)
{
    switch(SendRs485Agreement[2]) {
    case TOTAL_SW_CMD:                                    //���յ��ܿ�������
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

    case GENERAL_OPEN_CMD:                                //���յ��ܿ������
        if((SendRs485Agreement[1] != 0) && (SendRs485Agreement[0] == 0xFC)) {
            Regional_Close_opening(1, SendRs485Agreement);
        } else {
            Always_LED_control(1);
        }

        break;

    case GENERAL_CLOSE_COM:                               //���յ��ܹ������
        if((SendRs485Agreement[1] != 0) && (SendRs485Agreement[0] == 0xFC)) {
            Regional_Close_opening(0, SendRs485Agreement);
        } else {
            Always_LED_control(0);
        }

        break;

    case MULTI_INTERLOCKING_CMD:                          //�����໥�������
        Public_Multi_interlocking(SendRs485Agreement);
        break;

    case DOUBLE_INTERLOCKING_CMD:                         //����˫�ػ��������
        Public_Multi_interlocking(SendRs485Agreement);
        break;

    case CLEAR_DISTURB_CMD:                                       //�����������������
        Public_Multi_interlocking(SendRs485Agreement);
        break;

    default:
        Common_pattern_control(SendRs485Agreement);                 //������ͨ�����
        break;
    }
}
/*****************************************************************************************************
��������Interlocking_control
���ܣ�������������
�������޲�����
����ֵ������1�����뷢�ͻ��棬����2�����밴��ֵ��
*****************************************************************************************************/
void Interlocking_control(uint8_t *buff, uint8_t key)
{
    uint8_t i;

    for(i = 0; i < 4; i++) {
        if(i == key) {

        } else if((Broadcast_data_packing (buff) == key_config[i])) { //�������롣
            switch(buff[4]) {                                        //�ȶԿ������ԡ�
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
��������Point_movement
���ܣ��㶯������Ǵ���
�������޲�����
����ֵ���޷���ֵ
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
��������LigthLED_ON
���ܣ�������Ȧ����
����������ֵ
����ֵ���޷���ֵ
*****************************************************************************************************/
void LigthLED_ON(uint8_t key)
{
    SetTIM1_PWM_DutyCycle(0, key);
    back_LED_open_Close_Config(key, 0);
}


/*****************************************************************************************************
��������LigthLED_OFF
���ܣ�������Ȧ�ر�
����������ֵ
����ֵ���޷���ֵ
*****************************************************************************************************/
void LigthLED_OFF(uint8_t key)
{
    SetTIM1_PWM_DutyCycle(1800, key);
    back_LED_open_Close_Config(key, 1);
}


/*****************************************************************************************************
��������Dismantling_function
���ܣ����ݹ����밴����ţ�������Ӧ���ܡ�
����������1���������ܣ�����2��������š�����3���������ͷ��밴�¡�
����ֵ���޷���ֵ
*****************************************************************************************************/
void Dismantling_function(uint8_t function, uint8_t key_number, uint8_t state)
{
    switch(function) {
    case GENERAL_OPEN_CMD:                                 //�ܿ��ų���
        if(state) {
            SetTIM1_PWM_DutyCycle(1800, key_number);            //�رյơ�
            back_LED_open_Close_Config(key_number, 0);
        } else {
            SetTIM1_PWM_DutyCycle(0, key_number);
            back_LED_open_Close_Config(key_number, 1);
        }

        Exclude_flag[key_number - 1] = 1;
        key_led_state[key_number - 1] = 1;                   //�˺��������ô����а������£��������µ�״̬��������Ҫȡ����
        break;

    case GENERAL_CLOSE_COM:                                //�ܹ��ų���
        if(state) {
            SetTIM1_PWM_DutyCycle(1800, key_number);           //�رյơ�
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

