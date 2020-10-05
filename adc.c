#include<include.h>
#include "my_type.h"

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

uint16_t	get_current_now(void)
{
	static  CHANGE_TYPES  temp4;
	ADCON0 |= 0x80;
	ADCON1 = 0x3F|0x80;
	ADCLDO = 0x00;//5V
	delay_us();
	_start_ADC;
	while(ADCON0&0x02);
	temp4.change.count[1] = ADRESL;
	temp4.change.count[0] = ADRESH&0x0f;
	
	return temp4.change.math;
}


void	check_current_offset(void)
{
	static  CHANGE_TYPES  temp4;
	MOS_OFF;
	
	EA = 0;
	
	MOS_OFF;
	ADCON0 |= 0x80;
	ADCON1 = OP_current_channal|0x80;
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

void	read_current(void)
{
	while(ADCON0&0x02);
	ADCON0 |= 0x80;
	ADCON1 = OP_current_channal|0x80;
	ADCLDO = 0x00;//5V
	delay_us();
	_start_ADC;
	while(ADCON0&0x02);
	temp3.change.count[1] = ADRESL;
	temp3.change.count[0] = ADRESH&0x0f;
	
	ADC.current_sum -= ADC.current_read[ADC.current_count];
	ADC.current_read[ADC.current_count] = temp3.change.math;
	ADC.current_sum += temp3.change.math;
	ADC.current_ram = ADC.current_sum>>4;
	if(ADC.current_ram<=ADC.current_offset)
	{
		ADC.current = 0;
	}
	else
	{
		ADC.current = ADC.current_ram - ADC.current_offset;
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
    *
    *
    *
**************************************************************/
void	read_voltage(void)
{
	while(ADCON0&0x02);
	ADCON0 &= ~0x80; //ADCHS4
//	ADCON1 = voltage_channal|0x80;  //voltage_channale = 0x5C  AN12
    ADCON1 = 0x01|0x80;  //WT.EDIT。2020.09.29 AN1 
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
		ADC.voltage = ADC.voltage_sum>>4;
		ADC.voltage_sum = 0;
	}
}
/*****************************************************************/
void	read_pwm_adc(void)
{
	while(ADCON0&0x02);
	ADCON0 &= ~0x80;
	ADCON1 = pwm_adc_channal|0x80;
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
	ADCON0 &= ~0x80;
	ADCON1 = change_voltage_channal|0x80;//选择通道  //AN8
	ADCLDO = 0x00;//3V
	delay_us();
	_start_ADC;
	while(ADCON0&0x02);
	temp3.change.count[1] = ADRESL;
	temp3.change.count[0] = ADRESH&0x0f;
	ADC.change_voltage_sum +=temp3.change.math;
	if(++ADC.change_voltage_count>=16)//16次取平均值
	{
		ADC.change_voltage_count = 0;
		ADC.change_voltage = ADC.change_voltage_sum>>6;
//		if(ADC.change_voltage>=590)//2.8V时采集回来的值是590
//		{
//			ADC.change_voltage = 590;
//		}
		if(ADC.change_voltage<=233)
		{
			change_voltage = (ADC.change_voltage<<1)+(ADC.change_voltage>>3);
		}
		else
		{
			chagnge_voltage_1 = ADC.change_voltage -230;//取值范围是0~358
			change_voltage = 483 +(chagnge_voltage_1>>1)+(chagnge_voltage_1>>2);
		}
		
		if(change_voltage>=800)
		{
			change_voltage=750;
		}
		ADC.change_voltage_sum = 0;
	}
}


void	scan_adc_channal(void)
{
	//read_current();   //cancel ,works
	read_voltage();    //cancel ,don;t works.
	//read_pwm_adc(); //取消，works
}

