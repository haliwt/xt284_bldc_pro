#ifndef _BLDC_H_
#define _BLDC_H_
#include "my_type.h"


void	bldc_value_init(void);
void	BLDC_main(void);
void  blcd_start(void);
void BLDC_start(void);

void  MotorStop(void);
void  out_pwm(unsigned int  in);
unsigned int  soft_pwm(unsigned int in,unsigned int out);	
void  check_FB(void);
void  StartMotorRun(void);

void	confirm_phase(void);
void  StartTest(void);


void  MotorRun(void);
void com_charge(void);

void NormalMotorRun(void);

void  DutyRef(void);


#endif
