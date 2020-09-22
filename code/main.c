
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
    LED_Init();
	KEY_Init();
	ACMP1_Config();
	EPWM_Config();

	while(1)
	{	
        
		powerkey =HDKey_Scan(0);
		if(powerkey == 1){
            
           pwflg = pwflg ^ 0x01;
            if(pwflg ==1){
                    LED1 = 1;
                    MOS_U_V	;
                   // MOS_U_W	;
                  // MOS_V_W	;
                //MOS_V_U	;
                //	MOS_W_U;
                  //  MOS_W_V	;
                  // MOS_OFF;	
            }
           else {
               LED1 = 0;
               MOS_OFF;	
           }
	}		
   }
}






















