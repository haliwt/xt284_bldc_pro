#include "bemf.h"

/*********************************************
	*
	*Function Name:void ACMP1_Config(void)
	*Function:
	*Input Ref: NO
	*Output Ref:NO
	*
**********************************************/
void ACMP1_Config(void)
{
#define NULL  0
	uint8_t AdjValue;
	/*
	(1) get up ACMP model adjust value
	*/
	AdjValue = ACMP_GetOffsetAdjValue(ACMP1,ACMP_ADJ_GND,NULL,NULL);
	/*
	(2)set up ACMP positive
	*/
	ACMP_ConfigPositive(ACMP1, ACMP_POSSEL_P0);
	/*
	(3)set up ACMP negative input
	*/
	ACMP_ConfigNegative(ACMP1, ACMP_NEGSEL_CN,0);
	/*
	(4)set up output phase
	*/
	ACMP_DisableReverseOutput(ACMP1);   //disable reverter		
	/*
	(5)sef up filter
	*/
	ACMP_EnableFilter(ACMP1,ACMP_NFCLK_3_TSYS);	
	/*
	(6)set up output hysteresis
	*/	
	ACMP_EnableHYS(ACMP1,ACMP_HYS_SEL_P,ACMP_HYS_10);
	/*
	(7) set up Adjust
	*/			
	ACMP_ConfigOffsetAdj(ACMP1,ACMP_OFFSET_ACMPDJ,AdjValue);
	
	/*
	(8)set up Interrupter
	*/		
	ACMP_EnableInt(ACMP1);
	IRQ_SET_PRIORITY(IRQ_ACMP, IRQ_PRIORITY_LOW);		//ижии???D??ио??ии??
	IRQ_ALL_ENABLE();									//?a??б┴и╣?D??	
	/*
	(11)set up ACMP IO
	*/		
	GPIO_SET_MUX_MODE(P04CFG, GPIO_P04_MUX_C1P0);  //BEMF W   
	GPIO_SET_MUX_MODE(P03CFG, GPIO_P03_MUX_C1P1);  //BEMF V
	GPIO_SET_MUX_MODE(P02CFG, GPIO_P02_MUX_C1P2);  //BEMF U


	/*
	(10) start On ACMP1
	*/		
	ACMP_Start(ACMP1);
}

