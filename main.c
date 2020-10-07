#include<include.h>
#include "my_type.h"


SYS_TYPES sys;
CHANGE_TYPES  temp1;
extern  BLDC_TYPES    BLDC;
extern unsigned int change_voltage;

void	main(void)
{
	hardware_init();
	//TM1650_init();
	bldc_value_init();
	adc_value_init();
	pwm_value_init();
	//led_value_init();
	//key_value_init();
	check_current_offset();
	MOS_OFF;
	BLDC.status = 1;
	BLDC.error = _pwm_limit_error; 
	BLDC.pwm_set = 1000;
	while(BLDC.pwm_set)
	{
		if(EIF2&0x01)//Timer3¼ÆÊýÆ÷Òç³ö
		{
			EIF2 &= ~0x01;
			temp1.change.math = ~4000;
			TL3   = temp1.change.count[1];
			TH3   = temp1.change.count[0];
			BLDC.pwm_set --;
		}
	}
	BLDC.pwm_set = 0;
	
	while(1)
	{
		POWER_LED =1;
		BLDC.pwm_set = change_voltage;
		//BLDC.pwm_set = 720;//800;//720//760//640//800;
   
		if(EIF2&0x01) //
		{
			EIF2 &= ~0x01;
			temp1.change.math = ~4000; //1ms 
			TL3   = temp1.change.count[1];
			TH3   = temp1.change.count[0];
			scan_adc_channal();
			read_change_voltage();
            BLDC_main();
		
            
			//key_handing();
			//display();
		}
		
	}
	  
}


