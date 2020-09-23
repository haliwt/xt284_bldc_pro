#ifndef _BLDC_H_
#define _BLDC_H_
#include "my_type.h"


void	bldc_value_init(void);
void	BLDC_main(void);
void  blcd_start(void);
void BLDC_start(void);

void  blcd_stop(void);
void  out_pwm(unsigned int  in);
unsigned int  soft_pwm(unsigned int in,unsigned int out);	
void  check_FB(void);
void  OPEN(void);
void	confirm_phase(void);
void  CHECK(void);
void	LOOP(void);
void com_charge(void);




#endif
