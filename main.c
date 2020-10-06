#include<include.h>
#include "my_type.h"


SYS_TYPES sys;
CHANGE_TYPES  temp1;
extern  BLDC_TYPES    BLDC;
extern unsigned int change_voltage;
uint8_t MOTOR_RUN_FLG;

void	main(void)
{
    static uint8_t preflg=0,motor=0,poweron=0;
    
    uint8_t event =0;
    uint16_t t=0;
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
     LED_POWER_RED =0;
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
		//BLDC.pwm_set = change_voltage;
		BLDC.pwm_set = 200;//800;//720//760//640//800;
        event =0;
	//	KEY_Handing();
		#if 1
         MOTOR_RUN_FLG = HDKey_Scan(0);
			if(MOTOR_RUN_FLG ==1){
                 preflg = preflg ^ 0x01;
                if(preflg ==1 && event ==0){
                    event =1;
				    LED_POWER_RED =1;
                    motor =1;
                }
                else if(event !=1) {
                     event =1;
                    LED_POWER_RED =0;
                    motor =0;
                }

			}
		#endif 
       if(motor==1){
          
            if(EIF2&0x01 && poweron==0) 
            {
                poweron++;
                EIF2 &= ~0x01;
                temp1.change.math = ~4000; //1ms 
                TL3   = temp1.change.count[1];
                TH3   = temp1.change.count[0];
                BLDC.pwm_set = 100;
                scan_adc_channal();
                read_change_voltage();
                BLDC_main();
                
                for(t=0;t<1000;t++){
                    if( t<500){
                       BLDC.pwm_set = 200;
                          scan_adc_channal();
                read_change_voltage();
                       BLDC_main();
                        
                    }
                    if(t>=500){
                        BLDC.pwm_set = 300;
                        scan_adc_channal();
                       read_change_voltage();
                       BLDC_main();
                    }
                }
             }
             if(poweron==1){
                
                scan_adc_channal();
                read_change_voltage();
                BLDC_main();
             
             }
       }
       else{
          BLDC.pwm_set=0;
		  BLDC_stop();
          poweron=0;
       }
		
	}
}


