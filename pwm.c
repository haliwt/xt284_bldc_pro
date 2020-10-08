#include<include.h>
#include "my_type.h"


#define   START        0
#define   GET_T_F      1
#define   GET_H_L_B    2
#define   GET_H_L_F    3
#define   FINISH       4

PWM_TYPES  idata pwm;
CHANGE_TYPES  CH;
/******************************************************************************
 ** \brief	 Timer 2 interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
******************************************************************************/
#if 0
static  void  delay_us(void)
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}
#endif 

void Timer2_IRQHandler(void)  interrupt 5 
{
	//CCEN = 0x00;
	pwm.always_count = 0;
	
	CH.change.count[0]=CCH1;
	CH.change.count[1]=CCL1;
	pwm.H_time = CH.change.math;
	
	CH.change.count[0]=RLDH;
	CH.change.count[1]=RLDL;
	pwm.T_time = CH.change.math;
	
	
	CH.change.count[0]=CCH1;
	CH.change.count[1]=CCL1;
	pwm.H_time_ram = CH.change.math;
	
	if(pwm.H_time!=pwm.H_time_ram)
	{
		//delay_us();
		CH.change.count[0]=CCH1;
		CH.change.count[1]=CCL1;
		pwm.H_time = CH.change.math;
	}
	
	//CCEN = 0x05;
	
	if(pwm.flag == START)
	{
		pwm.T_last =  pwm.T_time;
		pwm.S_time  = pwm.T_last;
		pwm.flag = GET_H_L_B;
	}
	else 
	{
		pwm.always_count = 0;
		pwm.T_last =  pwm.T_time;
		
		pwm.H_time -= pwm.S_time;
	  pwm.T_time -= pwm.S_time;
		pwm.S_time  = pwm.T_last;
		
		if(pwm.H_time>=pwm.T_time)
		{
			if(P2_3)
			{
				pwm.H_time = pwm.T_time;
			}
			else
			{
				pwm.H_time -= pwm.T_time;
			}
		}
		
		
		pwm.H_time_sum += pwm.H_time;
		pwm.T_time_sum += pwm.T_time;
		
		pwm.flag = GET_H_L_B;
		if(++pwm.read_count >= 8)
		{
			pwm.read_count = 0;
			pwm.H_time_out = pwm.H_time_sum>>3;
			pwm.T_time_out = pwm.T_time_sum>>3;
			pwm.T_time_sum = 0;
			pwm.H_time_sum = 0;
			
		
			pwm.temp32  =  _pwm_max;
			pwm.temp32 *=  pwm.H_time_out;
			pwm.temp32 /=  pwm.T_time_out;  
			pwm.read = (unsigned int)pwm.temp32;
			pwm.flag = FINISH;
			
			OFF_PWM_IN_INTE;
		}
	}
	T2IF &= ~0x03;
}


void  pwm_value_init(void)
{
	pwm.always_count = 0;
	
	pwm.T_time = 0;    
	pwm.H_time = 0; 
	pwm.S_time = 0;
	
	pwm.T_last = 0;
	pwm.temp32 = 0;
	
	pwm.T_time_sum = 0;
	pwm.H_time_sum = 0;
	pwm.T_time_out = 0; 
	pwm.H_time_out = 0; 
	
	pwm.read_sum = 0;
	pwm.out = _pwm_max;    
	pwm.out_count = 0; 
	pwm.flag = 0;
}



#if 0
unsigned int  pwm_handing(void)
{
	if(++pwm.always_count>10)
	{
		pwm.always_count = 100;
		if(P2_3)
		{
			delay_us();
			if(P2_3)
			{
				delay_us();
				if(P2_3)
				{
					pwm.read = _pwm_max;
					pwm.flag = FINISH;
				}
			}
		}
		else
		{
			delay_us();
			if(!P2_3)
			{
				delay_us();
				if(!P2_3)
				{
					pwm.read = 0;
					pwm.flag = FINISH;
				}
			}
		}
	}
	
	if(pwm.flag == FINISH)
	{
		pwm.flag = START;
		ON_PWM_IN_INTE;
		pwm.read_sum +=  pwm.read;
		if(++pwm.out_count >= 1)
		{
			pwm.out_count = 0;
			pwm.out = pwm.read_sum>>0;
			pwm.read_sum = 0;
		}
	}
	
	return  pwm.out;
}

#endif 

