#include<include.h>
#include "my_type.h"

/******************************************************************************
 **
 ** Function Name:	void delay_10us(uint16_t n) 
 ** Function : 延时10*n微秒 
 ** Input Ref:NO
 ** Return Ref:NO
 **   
 ******************************************************************************/
void Delay_ms(uint16_t n)   
{  
    uint16_t i=0,j;  
    for(i=0;i<n;i++) {

	     for(j=0;j<100;j++){
		  _nop_(); 
		  _nop_(); 
		  _nop_(); 
		  _nop_(); 
		  _nop_(); 
		  _nop_(); 
		  _nop_(); 
		  _nop_(); 
		  _nop_(); 
		  _nop_(); 
	     }
		 
		  
     } 
}  

/**********************************************************************
	*
	*
	*
	*
	*
	*
***********************************************************************/
void KEY_Init(void)
{
	/*
	(1)设置P23 IO功能
	*/
	GPIO_SET_MUX_MODE(P24CFG, GPIO_MUX_GPIO);		//设置P24为GPIO模式
	GPIO_ENABLE_INPUT(P2TRIS, GPIO_PIN_4);			//设置为输入模式
	GPIO_ENABLE_RD(P2RD, GPIO_PIN_4);	//开启下拉


	
	

	#if 0
	/*
	(2)设置中断方式
	*/
	GPIO_SET_INT_MODE(P23EICFG, GPIO_INT_FALLING);	//设置为下降沿中断模式
	GPIO_EnableInt(GPIO2, GPIO_PIN_3_MSK);			//开启P23中断
	/*
	(3)设置中断优先级
	*/
	IRQ_SET_PRIORITY(IRQ_P2, IRQ_PRIORITY_LOW);
	/*
	(4)开启总中断
	*/	
	IRQ_ALL_ENABLE();
	#endif 

	
}
/****************************************************
	*
	*Function Name: uint8_t HDKey_Scan(uint8_t mode)
	*Function :
	*Inpute Ref: 0 ---涓嶆敮鎸佽繛缁寜閿?
	*Return Ref: 0 --娌℃湁鎸夐敭鎸変笅锛?1---鏈夋寜閿寜涓?
	*
*****************************************************/
uint8_t HDKey_Scan(uint8_t mode)
{
	
		static uint8_t key_up=1;	 //掳麓录眉脣脡驴陋卤锚脰戮
		if(mode==1)key_up=1;	// 鏀寔杩炵画鎸夐敭
		if(key_up&&(POWER_KEY== 1))
		{
		    key_up =0 ;
			Delay_ms(20);
			if(POWER_KEY== 1 ) 	return POWER_PRES;
		
		}else if(POWER_KEY==0)key_up=1;
		return 0;	//娌℃湁鎸夐敭鎸変笅
}


