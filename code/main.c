
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
	static uint8_t pwflg =0,powerkey,poweron=0,state=0;
	static uint16_t startnum=0;
	volatile uint8_t hall=0 ;
    LED_Init();
	BEMF_IOInit();
	

    KEY_Init();
	ACMP1_Config();
	EPWM_InDepend_Config();
	CMP_InputSignal();
    EPWM_InDepend_Init();

	while(1)
	{	
        powerkey =HDKey_Scan(0);
		if(powerkey == 1){
            
           pwflg = pwflg ^ 0x01;
            if(pwflg ==1){
                   
                    LED0 =1;
			        
                if(poweron==0){
                    poweron ++ ;
					startnum++;
                   for(startnum=0;startnum<3000;startnum++){
                      
					  //  PowerOn_MotorRun();
                       
                        OPEN3();
                        OPEN2();
                        
                         LED1=1;
                         
					    
                      }
             
                 // state =  NoHall_PhaseValue() ;
                  //NO_HallSensor_DectorPhase(state);
                   // InputValue_DectorPhase(state);
                   NoSense_InterruptPhase();
                 LED1=0;
                }
                  
              }
               else {
                       LED0 =0;
                       LED1=0;
                       poweron =0;
                    EPWM_ConfigChannelSymDuty(EPWM0, 0);
                    EPWM_ConfigChannelSymDuty(EPWM1, 0);
                    EPWM_ConfigChannelSymDuty(EPWM2, 0);
                     MOS_A_L =0;
                      MOS_B_L=0;
                      MOS_C_L= 0;
			  
                 }
	        
            
      }

}
}



















