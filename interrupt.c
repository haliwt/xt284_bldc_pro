/*******************************************************************************
* Copyright (C) 2019 China Micro Semiconductor Limited Company. All Rights Reserved.
*
* This software is owned and published by:
* CMS LLC, No 2609-10, Taurus Plaza, TaoyuanRoad, NanshanDistrict, Shenzhen, China.
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with CMS
* components. This software is licensed by CMS to be adapted only
* for use in systems utilizing CMS components. CMS shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. CMS is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/

/****************************************************************************/
/** \file isr.c
**
** 
**
**	History:
**		
*****************************************************************************/
/****************************************************************************/
/*	include files
*****************************************************************************/
#include<include.h>
#include "my_type.h"
/****************************************************************************/
/*	Local pre-processor symbols('#define')
****************************************************************************/

/****************************************************************************/
/*	Global variable definitions(declared in header file with 'extern')
****************************************************************************/

/****************************************************************************/
/*	Local type definitions('typedef')
****************************************************************************/

/****************************************************************************/
/*	Local variable  definitions('static')
****************************************************************************/


/****************************************************************************/
/*	Local function prototypes('static')
****************************************************************************/


/****************************************************************************/
/*	Function implementation - global ('extern') and local('static')
****************************************************************************/


/******************************************************************************
 ** \brief	 INT0 interrupt service function
 **			
 ** \param [in]  none   
 **
 ** \return none
 ******************************************************************************/
void INT0_IRQHandler(void)  interrupt 0
{

}
/******************************************************************************
 ** \brief	 Timer 0 interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
******************************************************************************/
void Timer0_IRQHandler(void)  interrupt 1 
{

}
/******************************************************************************
 ** \brief	 INT0 interrupt service function
 **			
 ** \param [in]  none   
 **
 ** \return none
 ******************************************************************************/
void INT1_IRQHandler(void)  interrupt 2
{
	;
}
/******************************************************************************
 ** \brief	 Timer 1 interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
******************************************************************************/
void Timer1_IRQHandler(void)  interrupt 3 
{

}
/******************************************************************************
 ** \brief	 UART 0 interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
******************************************************************************/
void UART0_IRQHandler(void)  interrupt 4 
{

}
/******************************************************************************
 ** \brief	 UART 1 interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
******************************************************************************/
void UART1_IRQHandler(void)  interrupt 6 
{
	;
}
/******************************************************************************
 ** \brief	 GPIO 0 interrupt service function
 **	
 ** \param [in]  none   
 **
 ** \return none
 ******************************************************************************/
void P0EI_IRQHandler(void)  interrupt 7 
{
	
}
/******************************************************************************
 ** \brief	 GPIO 1 interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
 ******************************************************************************/
void P1EI_IRQHandler(void)  interrupt 8 
{
	;
}
/******************************************************************************
 ** \brief	 GPIO 2 interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
 ******************************************************************************/
void P2EI_IRQHandler(void)  interrupt 9 
{

}
/******************************************************************************
 ** \brief	 GPIO 3 interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
 ******************************************************************************/
void P3EI_IRQHandler(void)  interrupt 10 
{
	
}
/******************************************************************************
 ** \brief	 LVD interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
 ******************************************************************************/
void LVD_IRQHandler(void)  interrupt 12 
{

}
/******************************************************************************
 ** \brief	 LSE interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
 ******************************************************************************/
void LSE_IRQHandler(void)  interrupt 13 
{

}
/********************************************************************************
 ** \brief	 ACMP interrupt service function
 **			
 ** \param [in]  none   
 **

 ** \return none
 ******************************************************************************/
void ACMP_IRQHandler(void)  interrupt 14 
{
	
}
/******************************************************************************
 ** \brief	 Timer 3 interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
******************************************************************************/
void Timer3_IRQHandler(void)  interrupt 15 
{

}
/******************************************************************************
 ** \brief	 Timer 4 interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
******************************************************************************/
void Timer4_IRQHandler(void)  interrupt 16 
{

}
/******************************************************************************
 ** \brief	 EPWM interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
 ******************************************************************************/

/******************************************************************************
 ** \brief	 ADC interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
 ******************************************************************************/

/******************************************************************************
 ** \brief	 WDT interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
 ******************************************************************************/
void WDT_IRQHandler(void)  interrupt 20 
{

}
/******************************************************************************
 ** \brief	I2C interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
 ******************************************************************************/
void I2C_IRQHandler(void)  interrupt 21 
{
	;
}
/******************************************************************************
 ** \brief	SPI interrupt service function
 **
 ** \param [in]  none   
 **
 ** \return none
 ******************************************************************************/
void SPI_IRQHandler(void)  interrupt 22 
{
	;
}

















