
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
	static uint8_t pwflg =0,powerkey;
	uint16_t startnum=0;
    LED_Init();
	KEY_Init();
	ACMP1_Config();
	EPWM_Config();
	CMP_InputSignal();

	while(1)
	{	
        
		powerkey =HDKey_Scan(0);
		if(powerkey == 1){
            
           pwflg = pwflg ^ 0x01;
            if(pwflg ==1){
                    LED1 = 1;
                    LED0 =1;
					startnum++;
			        if(startnum < 1000)
					   PowerOn_MotorRun();
					 else{

						NO_HallSensor_DectorPhase();

					 }
                  
            }
           else {
               LED1 = 0;
               LED0= 0;
               MOS_OFF;	
           }
	   }

	   
	   
   }
}






















