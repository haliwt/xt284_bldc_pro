#include<include.h>
#include "my_type.h"
#include "bldc.h"

idata  ADC_TYPES  ADC;
CHANGE_TYPES  temp3;
unsigned int change_voltage=0;
unsigned int chagnge_voltage_1 = 0;//用于转速控制计算的中间变量




void ADC_IRQHandler(void)  interrupt 19 
{

}




void	adc_value_init(void)
{
	uint8_t i;
	for(i=0;i<16;i++)
	{
		ADC.current_read[i] = 0;  
	}
	
	
	ADC.voltage_sum = 0;    
	ADC.current_sum = 0;   
	ADC.pwm_adc_sum = 0; 
	
	
	ADC.voltage = 0;    
	ADC.current = 0;   
	ADC.pwm_adc = 0;
	
	ADC.voltage_count = 0;
	ADC.current_count = 0;
	ADC.pwm_adc_count = 0;
}


static  void  delay_us(void)
{
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
/**************************************************************
    *
    *Function Name:uint16_t get_current_now(void)
    *Function: 
    *Input Ref:NO
    *Return Ref: 采样电压值
    *
**************************************************************/
uint16_t get_current_now(void)
{
	static  CHANGE_TYPES  temp4;
	ADCON0 |= 0x80; //ADCON0 = 0X48 init
	ADCON1 = 0x3F|0x80; //0x3F =0B 0011 1111 =  OP0_O = P30 电流采样电压
	ADCLDO = 0xA0;//2V //WT.EDIT 
	delay_us();
	_start_ADC;
	while(ADCON0&0x02);
	temp4.change.count[1] = ADRESL;
	temp4.change.count[0] = ADRESH&0x0f;
	
	return temp4.change.math;
}

/**************************************************************
    *
    *Function Name:void check_current_offset(void)
    *Function: 
    *Input Ref:NO
    *Return Ref:NO
    *
**************************************************************/
void check_current_offset(void)
{
	static  CHANGE_TYPES  temp4;
	MOS_OFF;
	
	EA = 0;
	
	MOS_OFF;
	ADCON0 |= 0x80;
	ADCON1 = OP_current_channal|0x80;////0x3F =0B 0011 1111 =  OP0_O = P30 电流采样电压
	ADCLDO = 0xA0;//2V WT.EDIT 2020.10.07
	delay_us();
	ADC.current_sum = 0;
	for(ADC.current_count=0;ADC.current_count<16;ADC.current_count++)
	{
		
		_start_ADC;
		while(ADCON0&0x02);
		temp4.change.count[1] = ADRESL;
		temp4.change.count[0] = ADRESH&0x0f;
		
		ADC.current_sum += temp4.change.math;
		
	}
	ADC.current_offset = ADC.current_sum>>4; //平均值
	ADC.current_sum = 0;
	ADC.current_count = 0;
	
	EA = 1;
}
/**************************************************************
    *
    *Function Name:void read_current(void)
    *Function: 
    *Input Ref:NO
    *Return Ref:NO
    *
**************************************************************/

void read_current(void)
{
	while(ADCON0&0x02);
	ADCON0 |= 0x80; // ADCON0 =0x48 ;
	ADCON1 = OP_current_channal|0x80; //0X5F = 0B0101 1111 AN22 = P30 -4pin
	ADCLDO = 0xA0;//2V ,WT.EDIT
	delay_us();
	_start_ADC;
	while(ADCON0&0x02);
	temp3.change.count[1] = ADRESL;
	temp3.change.count[0] = ADRESH&0x0f; //右对齐
	
	ADC.current_sum -= ADC.current_read[ADC.current_count]; //16次取样
	ADC.current_read[ADC.current_count] = temp3.change.math;
	ADC.current_sum += temp3.change.math;
	ADC.current_ram = ADC.current_sum>>4; //取平均值 2^12 = 4096
	if(ADC.current_ram<=ADC.current_offset)
	{
		ADC.current = 0;
	}
	else
	{
		ADC.current = ADC.current_ram - ADC.current_offset; //
	}
	
	if(++ADC.current_count>=16) //
	{
		ADC.current_count = 0;
	}
	
}
/**************************************************************
    *
    *Function Name:read_voltage(void)
    *Function: 保护电压，高于1v，电机停止工作，1v以下电机工作
    *Input Ref:NO
    *Return Ref:NO
    *
**************************************************************/
void	read_voltage(void)
{
	while(ADCON0&0x02);
	ADCON0 &= ~0x80; //ADCHS4 =0
	ADCON1 = voltage_channal|0x80;  //voltage_channale = 0x51 0101 1100  AN1=IO //原 AN12 
	ADCLDO = 0xE0;//3V
	delay_us();
	_start_ADC;
	while(ADCON0&0x02);
	temp3.change.count[1] = ADRESL;
	temp3.change.count[0] = ADRESH&0x0f;
	ADC.voltage_sum +=temp3.change.math;  //鐢靛帇楂樹簬 1v淇濇姢锛屼綆浜?v锛岀數鏈哄伐浣?
	if(++ADC.voltage_count>=16)
	{
		ADC.voltage_count = 0;
		ADC.voltage = ADC.voltage_sum>>4; // 1/16 平均值
		ADC.voltage_sum = 0;
	}
}
/*****************************************************************/
#if 0
void read_pwm_adc(void)
{
	while(ADCON0&0x02);
	ADCON0 &= ~0x80;
	ADCON1 = pwm_adc_channal|0x80; //0X5B =0B0101 1011 = 01011 =AN11  //控制继电器 “A”
	
	ADCLDO = 0x00;//5V
	delay_us();
	_start_ADC;
	while(ADCON0&0x02);
	temp3.change.count[1] = ADRESL;
	temp3.change.count[0] = ADRESH&0x0f;
	ADC.pwm_adc_sum +=temp3.change.math;
	if(++ADC.pwm_adc_count>=16)
	{
		ADC.pwm_adc_count = 0;
		ADC.pwm_adc = ADC.pwm_adc_sum>>4;
		ADC.pwm_adc_sum = 0;
	}
}
#endif 
/********************************************************************************
    *
    *Function Name:void read_change_voltage(void)
    *Function :电压调速
    *
    *
*********************************************************************************/
void read_change_voltage(void)
{
	static uint8_t poweron =0;
    uint8_t i=0;
	while(ADCON0&0x02);
	ADCON0 &= ~0x80; // ADCON0 = 0x48 & (~0x80) = 0x48
	ADCON1 = change_voltage_channal|0x80;//选择通道 0X58  //AN8 =P22
	ADCLDO = 0xE0;//3V
	delay_us();
	_start_ADC;
	while(ADCON0&0x02);
	temp3.change.count[1] = ADRESL;
	temp3.change.count[0] = ADRESH&0x0f;
	ADC.change_voltage_sum +=temp3.change.math;
	if(++ADC.change_voltage_count>=16)//8次取平均值
	{
		ADC.change_voltage_count = 0;
        ADC.change_voltage = ADC.change_voltage_sum>>6; //WT.EDIT
        
		//ADC.change_voltage = ADC.change_voltage >>2;
       
      
        if(ADC.change_voltage >60 ){
        
		  change_voltage=ADC.change_voltage;
            #if 0
		  if(poweron ==0){
		  	 
                switch(poweron)
                {
                        case 0:{MOS_U_V;} //A+ B- "6"
                        case 1:{MOS_W_V;} //C+ B- "4"
                        case 2:{MOS_W_U;} //C+ A- "5"
                        case 3:{MOS_V_U;} //B+ A- "1"
                        case 4:{MOS_V_W;} //B+ C- "3"
                        case 5:{MOS_U_W;} //A+ C- "2"
                        
                        poweron++ ;
                }
            
            	 poweron++ ;
			  	
		  } 
          #endif 
		   if(change_voltage>=800)
            {
                change_voltage= 800;
                
				
            }
		   
			
          MOTOR_LED =1;
		 
        }
		else {
			change_voltage=0;
			
			 MOTOR_LED =0;
             poweron=0;
		}

		
		ADC.change_voltage_sum = 0;
	}
}


void	scan_adc_channal(void)
{
	read_current();   //cancel ,works
	read_voltage();    //cancel ,don;t works.
	//read_pwm_adc(); //取消，works,外部控制PWM信号电压 现在重复
}

