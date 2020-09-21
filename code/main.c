
#include "cms8s6990.h"
#include "led.h"
#include "key.h"

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

	while(1)
	{	
		if(HDKEY == 0)LED1 = 1;
		else LED1 = 0;
	}		
}






















