
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
    _pbc0=0;    // set ralay pin output
    _pbc1=0;
    _pcc0=0;
    _pcc1=0;
    _pcc2=0;
    _pcc3=0;

    _pac1=0;    // set backlight pin output
    _pac4=0;
    _pac3=0;
    _pac0=0;
    _pac2=0;
    _pac7=0;

    _pcc4=0;    // set dimmer pin output

    _pc3=0;     // relay output low
    _pc2=0;
    _pc1=0;
    _pc0=0;
    _pb1=0;
    _pb0=0;

    _pa1=0;     // backlight off
    _pa4=0;
    _pa3=0;
    _pa0=0;
    _pa2=0;
    _pa7=0;

    _pc4=0;     // dimmer off

    /**********************S1 S2 S3 Resistor input status********************
    S1 status:
    0: dimmer on
    1: dimmer off
    *********************S1 S2 S3 Resistor input status*********************/

    _pcc7=1;    // set pull-up input
    _pcpu7=1;

    _pcc6=1;
    _pcpu6=1;

    _pcc5=1;
    _pcpu5=1;
}

//==============================================
//**********************************************
//==============================================

void USER_PROGRAM()
{
    GET_KEY_BITMAP();  // invoke touch function

    unsigned char  key[6];
    unsigned char  Out[6];
    unsigned char  BL[6];
    unsigned char  K[6];
    unsigned char i;

    static unsigned char flag_key[6] = {0,0,0,0,0,0};

    if(SWITCH_S1 == 0) {
        GLIMMER = 1;
    } else {
        GLIMMER = 0;
    }

    if(DATA_BUF[0]) {
	    K[0] = (DATA_BUF[0] & 0X80);
	    K[1] = (DATA_BUF[0] & 0X40);
	    K[2] = (DATA_BUF[0] & 0X20);
	    K[3] = (DATA_BUF[0] & 0X10);
	    K[4] = (DATA_BUF[0] & 0X08);
	    K[5] = (DATA_BUF[0] & 0X04);
    } else {
		for(i=0; i<6; i++) {
		    K[i] = 0;
		}
    }

    if(SWITCH_S2 == 0 && SWITCH_S3 == 0) {  // 点动背光， 自锁输出

        for(i=0; i<6; i++) {

            if(K[i])	{
                BL[i]=1;
            } else {
                BL[i]=0;
            }

            if(K[i] == flag_key[i]) {
                continue ;
            }

            if(K[i] && key[i]==0) {
                Out[i]=1;
                key[i]=1;
            } else if(K[i] && key[i]==1 ) {
                Out[i]=0;
                key[i]=0;
            }
            flag_key[i] = K[i];
        }

    } else if(SWITCH_S2 == 0 && SWITCH_S3 == 1) {   // 点动背光， 点动输出

		for(i=0; i<6; i++) {

	        if(K[i]) {
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

            if(K[i] && key[i]==0) {
                Out[i]=1;
                BL[i]=1;
                key[i]=1;
            } else if(K[i] && key[i]==1 ) {
                Out[i]=0;
                BL[i]=0;
                key[i]=0;
            }
            flag_key[i] = K[i];
        }
    } else if(SWITCH_S2 == 1 && SWITCH_S3 == 1) {   // 自锁背光，点动输出

        for(i=0; i<6; i++) {

            if(K[i])	{
                Out[i]=1;
            } else {
                Out[i]=0;
            }

            if(K[i] == flag_key[i]) {
                continue ;
            }

            if(K[i] && key[i]==0) {
                BL[i]=1;
                key[i]=1;
            } else if(K[i] && key[i]==1 ) {
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

    GCC_DELAY(1000);
    GCC_CLRWDT();
    GCC_CLRWDT1();
    GCC_CLRWDT2();
}
