#include<include.h>
#include "my_type.h"


SYS_TYPES sys;
CHANGE_TYPES  temp1;
extern  BLDC_TYPES    BLDC;
uint16_t count ;

void	main(void)
{


	hardware_init();
	bldc_value_init();
	adc_value_init();
	pwm_value_init();
	key_value_init();
	check_current_offset();
	MOS_OFF;
	BLDC.status = 1;
	BLDC.error = _pwm_limit_error; 
	BLDC.pwm_set = 1000;

	
	while(1)
	{
         
		    BLDC_start();
                
                count++ ;
				if(count < 5000)
				     OPEN();
				else{
				   count =6000;
                  LOOP();
                  CHECK();
                 com_charge();
					}
     
			
	
	}
}


