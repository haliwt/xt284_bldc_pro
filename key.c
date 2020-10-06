#include<include.h>
#include "my_type.h"

#define POWER_PRES 	      1



#define		_KEY_ALL_OFF				0X1F

//普通按键按下值
#define		_KEY_TRG_1_POWER     			0x01  //电源普通按键按下---按键值
#define		_KEY_TRG_2_WIND     			0x02  //风速按键按下--按键值
#define		_KEY_TRG_3_TIMER     			0x04   //定时按键
#define		_KEY_TRG_4_FILTER     			0x08
#define		_KEY_TRG_5_WINDTI   			0x09

//长按按键检查 按键值或者组合按键值
#define		_KEY_CONT_1_POWER     	     0x81    //电源按键长按--按键值
#define		_KEY_CONT_2_WIND     		 0x82    //风速按键长按--按键值
#define		_KEY_CONT_3_TIMER     	     0x84    //定时按键长按 长按按键。
#define		_KEY_CONT_4_FILTER     	     0x88    //长按键 --设置滤网键
#define		_KEY_CONT_5_WINDTI     		 0x89


#define		_KEY_CONT_1     	    0x81    //按键计数值
#define		_KEY_CONT_2     		0x82
#define		_KEY_CONT_3     		0x84
#define		_KEY_CONT_4     	    0x88
#define		_KEY_CONT_5     		0x90

typedef  struct  _state_
{
 unsigned char         read;
 unsigned char         buffer;
 unsigned char         value;
 unsigned short int    on_time;
 unsigned short int    off_time;
 enum{
  start  = 0,
  first  = 1,
  second = 2,
  cont   = 3,
  end    = 4,
  finish = 5,
 }state;
}key_types;

 key_types key;


#define  POWER_KEY      P2_4




#if 0
void	key_value_init(void)
{
	key.read = 0;
	key.buffer = 0;
	key.value = 0;
	key.inc_delay = 0;
	key.on_time = 0;
	key.off_time = 0;
	key.state = 0;
}

/******************************************************************************
 **
 ** Function Name:	void KEY_FUNCTION(void)
 ** Function : receive key input message 
 ** Input Ref:NO
 ** Return Ref:NO
 **   
 ******************************************************************************/
uint8_t KEY_Scan(void)
{
	uint8_t  reval = 0;

	key.read = _KEY_ALL_OFF; //0x1F 


//	if(POWER_KEY == 1)
	{
//		key.read &= ~0x01; // 0x1E
	}
	if(WIND_KEY == 1)
	{
		key.read &= ~0x02;   //0x1C
	}
	if(TIMER_KEY == 1)
	{
		key.read &= ~0x04;  //0x1B
	}
	if(FILTER_KEY == 1)
	{
		key.read &= ~0x08;  //0x17
	}

	
	switch(key.state )
	{
		case start:
		{
			if(key.read != _KEY_ALL_OFF)
			{
				key.buffer   = key.read; //例如：key.buffer = 0x1E  POWER KEY 
				key.state    = first;
				key.on_time  = 0;
				key.off_time = 0;
			}
			break;
		}
		case first:
		{
			if(key.read == key.buffer) //继续按下
			{
				if(++key.on_time> 10) //消抖  0.5us
				{
					key.value = key.buffer^_KEY_ALL_OFF; // key.value = 0x1E ^ 0x1f = 0x01
					key.on_time = 0;
					key.state   = second;
				}
			}
			else
			{
				key.state   = start;
			}
			break;
		}
		case second:
		{
			if(key.read == key.buffer) //再次确认按键是否按下
			{
				if(++key.on_time>300)//长按按键
				{
					
					key.value = key.value|0x80; //key.value = 0x01 | 0x80  =0x81  
					key.on_time = 0;
					
					key.state   = finish;
				}
			}
			else if(key.read == _KEY_ALL_OFF)  //按键松开
				{
					if(++key.off_time>5) //松开按键消抖
					{
						key.value = key.buffer^_KEY_ALL_OFF; // key.value = 0x1E ^ 0x1f = 0x01
						
						key.state   = finish; //一般按键按下状态
					}
				}
		   
			break;
		}
		case finish:
		{
			
			 reval = key.value; //分两种情况： 1.普通按键的按下返回值 TIMER_KEY = 0x04  2.长按键返回值：TIMER_KEY = 0X84
			key.state   = end;
			break;
		}
		case end:
		{
			if(key.read == _KEY_ALL_OFF)
			{
				if(++key.off_time>10)
				{
					key.state   = start;
				}
			}
			break;
		}
		default:
		{
			key.state   = start;
			break;
		}
	}
	return  reval;
}




/******************************************************************************
 **
 ** Function Name:	void KEY_FUNCTION(void)
 ** Function : receive key input message 
 ** Input Ref:NO
 ** Return Ref:NO
 **   
 ******************************************************************************/
 void KEY_Handing(void)
{
     
	  uint8_t keyflg =0;
	  if(Telecom.power_state ==1){

		  
			if(Telecom.timer_state == 1&& Telecom.keyEvent ==0){
	                 Telecom.timer_state=0;
					 Telecom.wind_state =0;
			         Telecom.net_state =0;
					 Telecom.TimerOn =0;
			         Telecom.keyEvent =1;
					 Telecom.TimerEvent = 0; //计时，时间计时开始时间
                     timer0_ten_num=0; //清空PM 检测值
			
			        
					Telecom.TimeBaseUint ++ ;
					if(Telecom.TimeHour == 8){
					    Telecom.TimeBaseUint=0;
					}
					else if(Telecom.TimeBaseUint == 10){
						Telecom.TimeBaseUint=0;
						Telecom.TimeMinute++;
						if(Telecom.TimeMinute==6){ 
							Telecom.TimeMinute =0;
							Telecom.TimeHour ++;
							{
							   if(Telecom.TimeHour == 8){
									
										Telecom.TimeBaseUint=0;
										Telecom.TimeMinute=0;
										
								}
							    if(Telecom.TimeHour >8){
									Telecom.TimeBaseUint=0;
									Telecom.TimeMinute=0;
										
									Telecom.TimeHour=0;

								}
							   
							 
							}
						}	
					}
					 Telecom.keyEvent =0;
					Telecom.TimerEvent = 0;
			      LEDDisplay_TimerTim(Telecom.TimeHour,Telecom.TimeMinute,Telecom.TimeBaseUint);
				}
				
			if(Telecom.wind_state ==1 && keyflg ==0){
			       //  NetKeyNum =0;
					 keyflg =1;
					Telecom.wind_state =0;
					 Telecom.timer_state=0;
					   Telecom.net_state =0;
			          timer0_ten_num=0; //清空PM检查值
			           delay_20us(1000);
			        

				 if(Telecom.WindLevelData ==5)Telecom.WindLevelData =0;
					 Telecom.WindLevelData ++ ;
                     if(Telecom.WindLevelData ==5)Telecom.WindLevelData =1;
					 	
					switch (Telecom.WindLevelData ){

					    case  wind_sleep :
						  LEDDisplay_SleepLamp();
						  Telecom.WindSelectLevel =wind_sleep;
						  OutputData(0x01);
						break;
						
						case wind_middle:
						
							OutputData(0x02);
							Telecom.WindSelectLevel =wind_middle;
						break;
							
						case wind_high:
							OutputData(0x03);
							Telecom.WindSelectLevel =wind_high;
					   break ;

					   case wind_auto:
						
							 Telecom.WindSelectLevel =wind_auto;
						break;
					}
			}
			 
		   if(Telecom.net_state ==1 && keyflg==0 ){
		   	    Telecom.net_state =0;
				 
				keyflg =0;
				     FLASH_Init();
                    BUZZER_Config();
                     delay_20us(5000)  ; 
                   BUZ_DisableBuzzer();
				   
                    
				   Flash_DisplayNumber();
			 }
		  
	 }
	 

}


#endif 
static void Delay_ms(uint16_t n)   
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


/****************************************************
	*
	*Function Name: uint8_t HDKey_Scan(uint8_t mode)
	*Function :
	*Inpute Ref: 0 ---不支持连续按键
	*Return Ref: 0 --没有按键按下， 1---有按键按下
	*
*****************************************************/
uint8_t HDKey_Scan(uint8_t mode)
{
	
		static uint8_t key_up=1;	 //°´¼üËÉ¿ª±êÖ¾
		if(mode==1)key_up=1;	// 支持连续按键
		if(key_up&&(POWER_KEY== 1))
		{
		    key_up =0 ;
			Delay_ms(40);
			if(POWER_KEY== 1 ) 	return POWER_PRES;
		
		}
		else if(POWER_KEY==0)key_up=1;
		return 0;	//没有按键按下
}


