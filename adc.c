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
	ADCLDO = 0x00;//5V
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
	ADCLDO = 0x00;//5V
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
	ADC.current_offset = ADC.current_sum>>4;
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
	ADCLDO = 0x00;//5V
	delay_us();
	_start_ADC;
	while(ADCON0&0x02);
	temp3.change.count[1] = ADRESL;
	temp3.change.count[0] = ADRESH&0x0f; //右对齐
	
	ADC.current_sum -= ADC.current_read[ADC.current_count]; //这一步 赋值
	ADC.current_read[ADC.current_count] = temp3.change.math;
	ADC.current_sum += temp3.change.math;
	ADC.current_ram = ADC.current_sum>>4; //ADC 右对齐 8bit
	if(ADC.current_ram<=ADC.current_offset)
	{
		ADC.current = 0;
	}
	else
	{
		ADC.current = ADC.current_ram - ADC.current_offset; //
	}
	
	if(++ADC.current_count>=16)
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
   // ADCON1 = 0x01|0x80;  //WT.EDIT。2020.09.29 AN1 =P01
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
void	read_pwm_adc(void)
{
	while(ADCON0&0x02);
	ADCON0 &= ~0x80;
	ADCON1 = pwm_adc_channal|0x80; //0X5B =0B0101 1011 = 01011 
	
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

/********************************************************************************
    *
    *Function Name:void read_change_voltage(void)
    *Function :电压调速
    *
    *
*********************************************************************************/
void read_change_voltage(void)
{
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
	if(++ADC.change_voltage_count>=8)//8次取平均值
	{
		ADC.change_voltage_count = 0;
        ADC.change_voltage = ADC.change_voltage_sum>>3; //WT.EDIT
        
		ADC.change_voltage = ADC.change_voltage >>2;
       
      
        if(ADC.change_voltage >10 ){
        
           change_voltage=ADC.change_voltage;
		    if(change_voltage>=800)
            {
                change_voltage= 800;
                
            }
			#if 0
				else if(change_voltage >720 && change_voltage <800){  //95%
			   	      change_voltage=760;

			   }
	          
			   else if( change_voltage <=720 && change_voltage >680){  //85%
					
			        change_voltage=720;
					

			   }
			   else if( change_voltage < 680 && change_voltage >=640){  // 80%
					
			        change_voltage=640;

			   }
			   else if( change_voltage <640 && change_voltage >=600){  // 75%
					
			        change_voltage=600;

			   }
           
		  #endif 
          
		 // BLDC.pwm_set = change_voltage;
        }
		else {
			change_voltage=0;
           // BLDC.pwm_set = change_voltage;
		}

		
		ADC.change_voltage_sum = 0;
	}
}


void	scan_adc_channal(void)
{
	read_current();   //cancel ,works
	read_voltage();    //cancel ,don;t works.
	read_pwm_adc(); //取消，works
}

