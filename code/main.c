
#include "cms8s6990.h"
#include "led.h"
#include "key.h"
#include "bemf.h"
#include "pwm.h"

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
	LED_Init();
	KEY_Init();
	ACMP1_Config();
	EPWM_Config();

	while(1)
	{	
		if(HDKEY == 0)LED1 = 1;
		else LED1 = 0;
	}		
}






















