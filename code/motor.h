#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "cms8s6990.h"


#define		MOS_U_V			{PWMMASKE = 0x39;C1CON0 = 0x82;C1CON2 = 0x00;} //V+ U - //C1COCN2 = Bit5 0正常输出
#define		MOS_U_W			{PWMMASKE = 0x2D;C1CON0 = 0x81;C1CON2 = 0x20;} //W+,U -//C1COCN2 -Bit5 =1 inversion 
#define		MOS_V_W			{PWMMASKE = 0x27;C1CON0 = 0x80;C1CON2 = 0x00;} //W+,V-
#define		MOS_V_U			{PWMMASKE = 0x36;C1CON0 = 0x82;C1CON2 = 0x20;} //U+,V-
#define		MOS_W_U			{PWMMASKE = 0x1E;C1CON0 = 0x81;C1CON2 = 0x00;} //U+ W-
#define		MOS_W_V			{PWMMASKE = 0x1B;C1CON0 = 0x80;C1CON2 = 0x20;} //W+ V- // V- 是上半周导通，W+ 下半周导通
#define		MOS_BREAK		{PWMMASKE = 0x3F;}//PWMFBKC = 0x10;}
#define		MOS_OFF			{PWMMASKE = 0x3f;}//PWMFBKC = 0x00;}



void HALL_Dector(void);



#endif 
