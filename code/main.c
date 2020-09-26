
#include "cms8s6990.h"
#include "led.h"
#include "key.h"
#include "bemf.h"
#include "pwm.h"
#include "motor.h"

uint32_t Systemclock = 24000000;


/*****************************************************************************
 ** \brief	 main
 **
 ** \param [in]  none   
 **
 ** \return 0
 *****************************************************************************/

int main(void)
{		
	static uint8_t pwflg =0,powerkey,poweron=0;
	static uint16_t startnum=0;
	volatile uint8_t hall=0 ;
    LED_Init();
	BEMF_IOInit();
	

    KEY_Init();
	ACMP1_Config();
	EPWM_Config();
	CMP_InputSignal();

	while(1)
	{	
       // MOS_OFF;


		
		powerkey =HDKey_Scan(0);
		if(powerkey == 1){
            
           pwflg = pwflg ^ 0x01;
            if(pwflg ==1){
                   
                    LED0 =1;
			
			        if(poweron==0){
                        poweron++;
			           
					  
                         ON_BLDC_INTE;
                         
                        for(startnum=0;startnum <5000;startnum++){
                            Start_MotorRun();
                           // PowerOn_MotorRun();
					        LED1 = 1;
			           }
                      
					  // NO_HallSensor_DectorPhase();
                        No_HallSensor_Input();
					// MOS_A_L;
                         LED1 = 0;
					    
                          
			          }
					 
                  
            }
           else {
               LED1 = 0;
               LED0= 0;
               MOS_OFF;	
               poweron=0;
              // MOS_A_H;
			  
           }
	   }

	   
	   
   }
}






















