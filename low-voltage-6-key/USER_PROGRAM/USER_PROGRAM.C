
#include    "USER_PROGRAM.H"

#pragma vector  Interrupt_Extemal        @ 0x04
void Interrupt_Extemal()
{
    _nop();
}

#define SWITCH_S1       _pc7
#define SWITCH_S2       _pc6
#define SWITCH_S3       _pc5
#define GLIMMER         _pc4

void USER_PROGRAM_INITIAL()
{
    //设为输入
    _pbc2=1;
    _pbc3=1;
    _pbc4=1;
    _pbc5=1;
    _pbc6=1;
    _pbc7=1;
    //设为上拉
    _pbpu2=1;
    _pbpu3=1;
    _pbpu4=1;
    _pbpu5=1;
    _pbpu6=1;
    _pbpu7=1;

    /***********************设置输出**********************/
    //输出控制设为输出
    _pbc0=0;
    _pbc1=0;
    _pcc0=0;
    _pcc1=0;
    _pcc2=0;
    _pcc3=0;

    //背光控制设为输出
    _pac1=0;
    _pac4=0;
    _pac3=0;
    _pac0=0;
    _pac2=0;
    _pac7=0;

    //背光微亮控制设为输出
    _pcc4=0;

    /**********************设置输出为低********************/
    //继电器输出关闭
    _pc3=0;
    _pc2=0;
    _pc1=0;
    _pc0=0;
    _pb1=0;
    _pb0=0;

    //背光灯关闭
    _pa1=0;
    _pa4=0;
    _pa3=0;
    _pa0=0;
    _pa2=0;
    _pa7=0;

    //背光微亮关闭
    _pc4=0;

    /**********************程序配置管脚*********************/

    //PC7设为上拉输入，如果是低电平，就用没有微光     0开启微亮背光，1关闭微亮背光
    _pcc7=1;
    _pcpu7=1;

//PC6设为上拉输入，如果是低电平，触摸按键没有用。 0为单片机点动亮背光，1为开关量背光
    _pcc6=1;
    _pcpu6=1;

    //按下就亮松开就灭   0为点动，1为非点动
    _pcc5=1;
    _pcpu5=1;

}

//==============================================
//**********************************************
//==============================================

void USER_PROGRAM()
{
    //GET_KEY_BITMAP();  // invoke touch function

    static unsigned char flag_key[6] = {0,0,0,0,0,0};

    unsigned char  key[6];
	unsigned char  Out[6];
	unsigned char  BL[6];
	unsigned char  K[6];

    int i = 0;

    if(SWITCH_S1 == 0) {
        GLIMMER = 1;
    } else {
        GLIMMER = 0;
    }

    K[0]=_pb7;
    K[1]=_pb6;
    K[2]=_pb5;
    K[3]=_pb4;
    K[4]=_pb3;
    K[5]=_pb2;

    if(SWITCH_S2 == 0 && SWITCH_S3 == 0) {  // 点动背光， 自锁输出

        for(i=0; i<6; i++) {
            if(K[i]==0)	{
                BL[i]=1;
            } else {
                BL[i]=0;
            }

            if(K[i] == flag_key[i]) {
                continue ;
            }

            if(K[i]==0 && key[i]==0) {
                Out[i]=1;
                key[i]=1;
            } else if(K[i]==0 && key[i]==1 ) {
                Out[i]=0;
                key[i]=0;
            }
            flag_key[i] = K[i];
        }

    } else if(SWITCH_S2 == 0 && SWITCH_S3 == 1) {   // 点动背光， 点动输出

        for(i=0; i<6; i++) {
            if(K[i]==0)	{
                Out[i]=1;
                BL[i]=1;
            } else {
                Out[i]=0;
                BL[i]=0;
            }
        }

    } else if(SWITCH_S2 == 1 && SWITCH_S3 == 0) {   // 自锁背光， 自锁输出

        for(i=0; i<6; i++) {

            if(K[i] == flag_key[i]) {
                continue ;
            }

            if(K[i]==0 && key[i]==0) {
                Out[i]=1;
                BL[i]=1;
                key[i]=1;
            } else if(K[i]==0 && key[i]==1 ) {
                Out[i]=0;
                BL[i]=0;
                key[i]=0;
            }
            flag_key[i] = K[i];
        }


    } else if(SWITCH_S2 == 1 && SWITCH_S3 == 1) {   // 自锁背光，点动输出

        for(i=0; i<6; i++) {
            if(K[i]==0)	{
                Out[i]=1;
            } else {
                Out[i]=0;
            }

            if(K[i] == flag_key[i]) {
                continue ;
            }

            if(K[i]==0 && key[i]==0) {
                BL[i]=1;
                key[i]=1;
            } else if(K[i]==0 && key[i]==1 ) {
                BL[i]=0;
                key[i]=0;
            }
            flag_key[i] = K[i];
        }
    }
// out
    if(Out[3]) {
        _pc0=1;
    } else {
        _pc0=0;
    }

    if(Out[2]) {
        _pc1=1;
    } else {
        _pc1=0;
    }

    if(Out[1]) {
        _pc2=1;
    } else {
        _pc2=0;
    }

    if(Out[0]) {
        _pc3=1;
    } else {
        _pc3=0;
    }

    if(Out[5]) {
        _pb0=1;
    } else {
        _pb0=0;
    }

    if(Out[4]) {
        _pb1=1;
    } else {
        _pb1=0;
    }
// BL
    if(BL[0]) {
        _pa7=1;
    } else {
        _pa7=0;
    }

    if(BL[1]) {
        _pa2=1;
    } else {
        _pa2=0;
    }

    if(BL[2]) {
        _pa0=1;
    } else {
        _pa0=0;
    }

    if(BL[3]) {
        _pa3=1;
    } else {
        _pa3=0;
    }

    if(BL[4]) {
        _pa4=1;
    } else {
        _pa4=0;
    }

    if(BL[5]) {
        _pa1=1;
    } else {
        _pa1=0;
    }

    for(i=0; i<10; i++) {
        GCC_DELAY(20000);
        GCC_CLRWDT();
        GCC_CLRWDT1();
        GCC_CLRWDT2();
    }
}
