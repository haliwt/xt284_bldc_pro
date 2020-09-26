#ifndef __PWM_H__
#define __PWM_H__
#include "cms8s6990.h"
//PWMOE  = 0x01;//WH
  //      PWMOE = 0X02;//VH
       // PWMOE = 0X04 ; //UH


#define UH   {PWMOE = 0x04;}
#define VH   {PWMOE = 0x02;}
#define WH   {PWMOE = 0x01;}

#define UL   P16 //P16 =0; //UL
#define VL   P14  //=0;//VL
#define WL   P00  //=0;//WL

#define VH   P15
#define VL   P14

#define WH   P13
#define WL   P00


//void EPWM_Config(void);
void EPWM_InDepend_Config(void);

void EPWM_InDepend_Init(void);


#endif 