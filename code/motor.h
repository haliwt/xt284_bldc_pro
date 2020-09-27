#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "cms8s6990.h"
#include "key.h"

#if 0
#define		MOS_U_V			{PWMMASKE = 0x39;C1CON0 = 0x82;C1CON2 = 0x00;} //V+ U -  // A+  B- // 6 C1COCN2 = Bit5 0正常输出
#define		MOS_U_W			{PWMMASKE = 0x2D;C1CON0 = 0x81;C1CON2 = 0x20;} //W+,U -  // A+  C- // 2  C1COCN2 -Bit5 =1 inversion 
#define		MOS_V_W			{PWMMASKE = 0x27;C1CON0 = 0x80;C1CON2 = 0x00;} //W+,V-   // B+  C- // 3  // 5-1-3-2-6-4
#define		MOS_V_U			{PWMMASKE = 0x36;C1CON0 = 0x82;C1CON2 = 0x20;} //U+,V-   // B+  A- // 1
#define		MOS_W_U			{PWMMASKE = 0x1E;C1CON0 = 0x81;C1CON2 = 0x00;} //U+ W-   // C+  A- // 5
#define		MOS_W_V			{PWMMASKE = 0x1B;C1CON0 = 0x80;C1CON2 = 0x20;} //V+ W-    //C+  B- //4  //V- 是上半周导通，W+ 下半周导通
#define		MOS_BREAK		{PWMMASKE = 0x3F;}//PWMFBKC = 0x10;}
#define		MOS_OFF			{PWMMASKE = 0x3f;}//PWMFBKC = 0x00;}
#endif 
#define		MOS_U_V			{PWMMASKE = 0x39;C1CON0 = 0x82;C1CON2 = 0x00;} //U- V +  // A+  B- // 6 C1COCN2 = Bit5 0正常输出
#define		MOS_U_W			{PWMMASKE = 0x2D;C1CON0 = 0x81;C1CON2 = 0x20;} //U-,W +  // A+  C- // 2  C1COCN2 -Bit5 =1 inversion 
#define		MOS_V_W			{PWMMASKE = 0x27;C1CON0 = 0x80;C1CON2 = 0x00;} //V-,W+   // B+  C- // 3  // 5-1-3-2-6-4
#define		MOS_V_U			{PWMMASKE = 0x36;C1CON0 = 0x82;C1CON2 = 0x20;} //V- U+   // B+  A- // 1
#define		MOS_W_U			{PWMMASKE = 0x1E;C1CON0 = 0x81;C1CON2 = 0x00;} //W- U+   // C+  A- // 5
#define		MOS_W_V			{PWMMASKE = 0x1B;C1CON0 = 0x80;C1CON2 = 0x20;} //W- V+    //C+  B- //4  //V- 是上半周导通，W+ 下半周导通
#define		MOS_BREAK		{PWMMASKE = 0x3F;}//PWMFBKC = 0x10;}
#define		MOS_OFF			{PWMMASKE = 0x3f;}//PWMFBKC = 0x00;}



#define     MOS_ON          {PWMMASKE = 0x00;}


#define    MOS_A_H    {PWMOE = 0x04;}// {PWMMASKE = 0x3E;C1CON0 = 0x80;C1CON2 = 0x00;} //PG0 UH
#define    MOS_A_L     P16 //= //{PWMMASKE = 0x3D;C1CON0 = 0x81;C1CON2 = 0x02;} //PG1 UL

#define    MOS_B_H    {PWMOE = 0x02;} //{PWMMASKE = 0x3B;C1CON2 = 0x00;} //PG 
#define    MOS_B_L     P14//{PWMMASKE = 0x37;C1CON2 = 0x00;} 

#define    MOS_C_H      {PWMOE = 0x01;} //{PWMMASKE = 0x2F;C1CON2 = 0x00;}
#define    MOS_C_L      P00//{PWMMASKE = 0x1F;C1CON2 = 0x00;}


#define UL   P16 //P16 =0; //UL
#define VL   P14  //=0;//VL
#define WL   P00  //=0;//WL

#define     SENSE     P01



#define UH   {PWMOE = 0x04;}
#define VH   {PWMOE = 0x02;}
#define WH   {PWMOE = 0x01;}

#define   ON_BLDC_INTE     {PWMPIF = 0x00;PWMZIF = 0x00;PWMUIF = 0x00;PWMDIF = 0x00;EIE2 |= 0x08;} // enable PWM interrupter
#define   OFF_BLDC_INTE    {PWMPIF = 0x00;PWMZIF = 0x00;PWMUIF = 0x00;PWMDIF = 0x00;EIE2 &=~0x08;} //disable PWM interrupter

#define   ON_PWM_IN_INTE     {T2IF = 0x00;ET2 = 1;} //ET2 =timer2 总中断允许位
#define   OFF_PWM_IN_INTE    {T2IF = 0x00;ET2 = 0;}


extern uint8_t gPhase;

extern uint8_t intBEMF;
uint8_t NO_HallSensor_GetPinState(void);


void PowerOn_MotorRun(void);
void Start_MotorRun(void);
void	com_charge(void);
uint8_t No_HallSensor_Input(void);
void NoSensor_Phase(uint8_t state);
void BEMF_IOInit(void);
void NO_HallSensor_DectorPhase(uint8_t state);


void OPEN(void);
void OPEN2(void);
uint8_t NoHall_PhaseValue(void) ;

void InputValue_DectorPhase(uint8_t state);
void OPEN3(void);
void NoSense_InterruptPhase(void);


#endif 
