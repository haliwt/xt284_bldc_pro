#include<include.h>
#include "my_type.h"


SYS_TYPES sys;
CHANGE_TYPES  temp1;
extern  BLDC_TYPES    BLDC;
uint16_t count ;

void	main(void)
{

    static uint8_t poweronflg =0,pflg;
	hardware_init();
	KEY_Init();
    LED_Init();
	bldc_value_init();
	adc_value_init();
	pwm_value_init();
	check_current_offset();
	MOS_OFF;
	BLDC.status = 1;
	BLDC.error = _pwm_limit_error; 
	BLDC.pwm_set = 1000;

	
	while(1)
	{
         
		    BLDC_start();
        
           pflg =  HDKey_Scan(0);
           if(pflg==1){
               poweronflg = poweronflg ^0x01;
               if(poweronflg ==1){
                 LED0=1;
                   LED1=1;
                   
                  count++ ;
				if(count < 2000){
				     StartMotorRun();
                     StartTest();//sound a little 
                }
				else{
                    MotorRun();
                    count =4000;
				}
     
			 
               }
               else{
                LED0=0;
                LED1=0;
              
                MotorStop();
                   hardware_init();
                    KEY_Init();
                    LED_Init();
                    bldc_value_init();
                    adc_value_init();
                    pwm_value_init();
                    check_current_offset();
                    MOS_OFF;
                    BLDC.status = 1;
                    BLDC.error = _pwm_limit_error; 
                    BLDC.pwm_set = 1000;
               
               }
           }
              
	
	}
}


