#include "include.h"
#include "my_type.h"


#define  _STOP           0
#define  _CHECK          1
#define  _OPEN           2
#define  _LOOP           3
#define  _BREAK          4

#define  OPEN_CONF_COM       0
#define  OPEN_WAIT_CHECK     1
#define  OPEN_CHECK_B        2
#define  OPEN_CHECK_ZERO     3
#define  OPEN_COM_CHARGE     4
#define  OPEN_DEGAUSS       5
#define  OPEN_ERROR          6



#define  _WAIT_CHECK     0
#define  _CHECK_ZERO_B   1
#define  _CHECK_ZERO     2
#define  _WAIT_COM       3
#define  _COM_CHARGE     4
#define  _DEGAUSS        5
#define  _ERROR          6





// 用于反电势检测的择多滤波器配置表
/*
code unsigned char cBEMF_FILTER[64] = {  0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,
																					32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,
																					0,2,4,6,8,10,12,14,16,18,1,22,1,26,28,30,
																				  32,34,36,38,1,42,44,46,1,1,1,54,56,58,60,62};		
*/

CHANGE_TYPES  pwm_now;
BLDC_TYPES    BLDC;
extern  idata  ADC_TYPES  ADC;				
																					
static  void	delay_us(uint8_t  in)
{
	do
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
	while(--in);
}


void	bldc_value_init(void)
{
	BLDC.cw_ccw_in = 0;
	BLDC.cw_ccw_now = 0;
	BLDC.EMI_flag = 0;
	BLDC.EMI_count = 0;
	BLDC.motor_step = 0;
	
	BLDC.EMI_OK_count = 0;
	BLDC.EMI_NG_count = 0;
	BLDC.turn_OK_count = 0;
	
	BLDC.duzhuan_time = 0;
	
	BLDC.open_period = _open_max_time;
	BLDC.check_over_time = 0;
	
	BLDC.zero_now_time.all = 0xffff;
	BLDC.zero_period.all = 0xffff;
	
	BLDC.read_EMF = 0;    
	BLDC.EMF_now = 0;   
	BLDC.EMF_last = 0;
	
	BLDC.output_time.all = 0xffff;
	BLDC.zero_check_time.all = 0xffff;
	
	BLDC.pwm_out = _start_pwm_set;
	BLDC.reset_time = 0;
	
	BLDC.loop_status = 0;
	BLDC.status = 0; 
	//BLDC.error = 0; 
}


void	com_charge(void)
{
	switch(BLDC.motor_step)
	{
		case 0:{MOS_U_V;break;}
		case 1:{MOS_W_V;break;}
		case 2:{MOS_W_U;break;}
		case 3:{MOS_V_U;break;}
		case 4:{MOS_V_W;break;}
		case 5:{MOS_U_W;break;}
		default :{MOS_OFF;break;}
	}
}


unsigned int  soft_pwm(unsigned int in,unsigned int out)
{
	static unsigned char  i;
	if(++i>_soft_step_time)
	{
		i = 0;
		if(in>out)
		{
			out++;
		}
		else if(in<out)
		{
			out--;
		}
	}
	return  out;
}

void  out_pwm(unsigned int    in)
{
	if(in>_pwm_max){in = _pwm_max;}
	
	pwm_now.change.math = _pwm_max-in;
	
	if(in<350)
	{
		PWMPIE = 0x00;  //周期中断
		PWMZIE = 0x01;  //零点中断
		PWMUIE = 0x00;  //向上比较中断
		PWMDIE = 0x00;  //向下比较中断
		PWMPIF = 0x00;  //中断标志
		PWMZIF = 0x00;
		PWMUIF = 0x00;
		PWMDIF = 0x00;
	}
	else if(in>450)
	{
		PWMPIE = 0x01;  //周期中断
		PWMZIE = 0x00;  //零点中断
		PWMUIE = 0x00;  //向上比较中断
		PWMDIE = 0x00;  //向下比较中断
		PWMPIF = 0x00;  //中断标志
		PWMZIF = 0x00;
		PWMUIF = 0x00;
		PWMDIF = 0x00;
	}
	if(BLDC.loop_status != _DEGAUSS)
	{
		PWMD1L   = pwm_now.change.count[1];
		PWMD1H   = pwm_now.change.count[0];
		PWMD0L   = 0x00;
		PWMD0H   = 0x00;
		PWMLOADEN = 0x03;
	}
}

/********************************************************************
	*
	*Function Name:void MotoRun(void)
	*Function:
	*Inputr Ref:NO
	*Return Ref:NO
	*
*********************************************************************/
void MotorRun(void)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		switch(BLDC.loop_status)
		{
			case   _WAIT_CHECK:
				if(TF1)
				{
					TF1 = 0; //
					BLDC.loop_status = _CHECK_ZERO;
				}
				break;
			case   _CHECK_ZERO_B:
				if((BLDC.EMI_flag&0x80)==0x00)
				{
					BLDC.loop_status = _CHECK_ZERO;
				}
				/*if(TF1)
				{
					TF1 = 0;
					BLDC.error  |= _emf_Error;
				}*/
				break;
			case  _CHECK_ZERO:
				BLDC.zero_now_time.one.h  = TH0;
				BLDC.zero_now_time.one.l  = TL0;
				if(BLDC.EMI_flag&0x80)
				{
					TH0 = 0;
					TL0 = 0;
					TF0 = 0;
					BLDC.zero_period.all = BLDC.zero_period.all - (BLDC.zero_period.all >>2) + BLDC.zero_now_time.all;//240
					BLDC.output_time.all = BLDC.zero_period.all >>3;//30
					BLDC.step_read = BLDC.output_time.all;
					BLDC.output_time.all -= (BLDC.output_time.all>>3);
					BLDC.zero_check_time.all = BLDC.output_time.all >>1;
					BLDC.zero_check_time.all -= _DEGAUSS_TIME;
				  BLDC.zero_check_time.all -= (BLDC.zero_check_time.all>>3);
					BLDC.duzhuan_time = 0;
					TL1 = ~BLDC.output_time.one.l;					
					TH1 = ~BLDC.output_time.one.h; 
					TF1 = 0;
					
					BLDC.step_time_sum += BLDC.step_read;
					if(++BLDC.step_time_count>=64)
					{
						BLDC.step_time = BLDC.step_time_sum>>5;
						BLDC.step_time_count = 0;
						BLDC.step_time_sum = 0;
						BLDC.step_updata_flag = 1;
					}
					BLDC.loop_status = _WAIT_COM;
				}
				/*if(TF1)
				{
					TF1 = 0;
					BLDC.error  |= _emf_Error;
				}*/
				break;
			case  _WAIT_COM:
				if(TF1)
				{
					TF1 = 0;
					BLDC.loop_status = _COM_CHARGE;
				}
				break;
			case  _COM_CHARGE:
				BLDC.output_time.all = _DEGAUSS_TIME;
				TL1 = ~BLDC.output_time.one.l;					
				TH1 = ~BLDC.output_time.one.h; 
				TF1 = 0;
				if(++BLDC.motor_step>=6){BLDC.motor_step = 0;}
				switch(BLDC.motor_step)
				{
					case 0:
						{
						PWMD1L   = 0x90;
						PWMD1H   = 0x01;
						PWMD0L   = 0x00;
						PWMD0H   = 0x00;
						PWMLOADEN = 0x03;
						break;
						}
					case 1:
						{
						PWMD0L   = 0x90;
						PWMD0H   = 0x01;
						PWMD1L   = 0x00;
						PWMD1H   = 0x00;
						PWMLOADEN = 0x03;
						break;
						}
					case 2:
						{
						PWMD1L   = 0x90;
						PWMD1H   = 0x01;
						PWMD0L   = 0x00;
						PWMD0H   = 0x00;
						PWMLOADEN = 0x03;
						break;
						}
					case 3:
						{
						PWMD0L   = 0x90;
						PWMD0H   = 0x01;
						PWMD1L   = 0x00;
						PWMD1H   = 0x00;
						PWMLOADEN = 0x03;
						break;
						}
					case 4:
						{
						PWMD1L   = 0x90;
						PWMD1H   = 0x01;
						PWMD0L   = 0x00;
						PWMD0H   = 0x00;
						PWMLOADEN = 0x03;
						break;
						}
					case 5:
						{
						PWMD0L   = 0x90;
						PWMD0H   = 0x01;
						PWMD1L   = 0x00;
						PWMD1H   = 0x00;
						PWMLOADEN = 0x03;
						break;
						}
					default :{MOS_OFF;break;}
				}
				com_charge();
				BLDC.loop_status = _DEGAUSS;
				break;
			case  _DEGAUSS:
				if(TF1)//&&((BLDC.EMI_flag&0x80)==0x00))
				{
					
					PWMD1L   = pwm_now.change.count[1];
					PWMD1H   = pwm_now.change.count[0];
					PWMD0L   = 0x00;
					PWMD0H   = 0x00;
					PWMLOADEN = 0x03;
					TH1 = ~BLDC.zero_check_time.one.h;
					TL1 = ~BLDC.zero_check_time.one.l; 
					TF1 = 0;
					BLDC.loop_status = _WAIT_CHECK;
				}
				break;	
			case  _ERROR:
				break;
			default:BLDC.loop_status = _WAIT_CHECK;
				break;
		}
	}
}
/*************************************************************
	*
	*Function Name() : void StartTest(void)
	*Function : check reserves voltage BEMF
	*Input Ref: NO
	*Return: Ref : NO
	*
*************************************************************/
void  StartTest(void)
{
	if(++BLDC.check_over_time<5000)
	{
		BLDC.duzhuan_time = 0;
		BLDC.EMF_now = 0;
		C1CON2 = 0x00; //比较控制寄存器2 --
		C1CON0 = 0x80; //比较控制寄存器0 --enable compare
		delay_us(20);
		if(C1CON1&0x80){BLDC.EMF_now |= 0x01;} //U --BEMF
		C1CON0 = 0x81;
		delay_us(20);
		if(C1CON1&0x80){BLDC.EMF_now |= 0x02;} //V ---BEMF
		C1CON0 = 0x82;
		delay_us(20);
		if(C1CON1&0x80){BLDC.EMF_now |= 0x04;}//W  ---BEMF 

		switch(BLDC.EMF_now)
		{


					case 0x06:{MOS_U_V;break;}  //A+ B- '6'
					case 0x04:{MOS_W_V;break;}    //C+ B-  "4"
					case 0x05:{MOS_W_U;break;}   //C+ A- "5"
					case 0x01:{MOS_V_U;break;}     //B+ A-   "1"
					case 0X03:{MOS_V_W;break;}       //B+ C - "3"
					case 0X02:{MOS_U_W;break;}        // A+ C-  "2"
					default :{MOS_OFF;break;}
		}
	}

	
}
	
void	confirm_phase(void)
{
	uint16_t   i,k,j;
	uint16_t   Iuv,Iuw,Ivu,Ivw,Iwu,Iwv,Ioffset;
	uint8_t    angle;
	
	k = _IPD_GET_TIMES;//采样次数
	j = _IPD_GET_IDLE; //停止间隔
	MOS_OFF;
	out_pwm(_pwm_max);
	Iuv = get_current_now();
	MOS_OFF;
	
	for(i=0;i<j;i++)
	{
		Iuv = get_current_now();
	}
	MOS_U_V;
	Iuv = 0;
	for(i=0;i<k;i++)
	{
		Iuv += get_current_now();
	}
	MOS_OFF;	
	
	for(i=0;i<j;i++)
	{
		Iwv = get_current_now();
	}
	MOS_W_V;
	Iwv = 0;
	for(i=0;i<k;i++)
	{
		Iwv += get_current_now();
	}
	MOS_OFF;
	
	for(i=0;i<j;i++)
	{
		Iwu = get_current_now();
	}
	MOS_W_U;
	Iwu = 0;
	for(i=0;i<k;i++)
	{
		Iwu += get_current_now();
	}
	MOS_OFF;
	
	for(i=0;i<j;i++)
	{
		Ivu = get_current_now();
	}
	MOS_V_U;
	Ivu = 0;
	for(i=0;i<k;i++)
	{
		Ivu += get_current_now();
	}
	MOS_OFF;
	
	for(i=0;i<j;i++)
	{
		Ivw = get_current_now();
	}
	MOS_V_W;
	Ivw = 0;
	for(i=0;i<k;i++)
	{
		Ivw += get_current_now();
	}
	MOS_OFF;
	
	for(i=0;i<j;i++)
	{
		Iuw = get_current_now();
	}
	MOS_U_W;
	Iuw = 0;
	for(i=0;i<k;i++)
	{
		Iuw += get_current_now();
	}
	MOS_OFF;
	Ioffset = 0;
	for(i=0;i<k;i++)
	{
		Ioffset += ADC.current_offset;
	}
	angle = 0;
	if(Iuv>Ivu){angle |= 0x01;}
	if(Ivw>Iwv){angle |= 0x02;}
	if(Iwu>Iuw){angle |= 0x04;}
	
	if(Iuv<(Ioffset+_LINK_CHECK_CURRENT))
	{
		BLDC.error  |= _line_Error;
	}
	if(Ivw<(Ioffset+_LINK_CHECK_CURRENT))
	{
		BLDC.error  |= _line_Error;
	}	
	if(Iwu<(Ioffset+_LINK_CHECK_CURRENT))
	{
		BLDC.error  |= _line_Error;
	}
	
	BLDC.pwm_out = _start_pwm_set;
	out_pwm(BLDC.pwm_out);
	switch(angle)
	{
		case 1:{BLDC.motor_step=1;break;}//0
		case 3:{BLDC.motor_step=0;break;}//5
		case 2:{BLDC.motor_step=5;break;}//4
		case 6:{BLDC.motor_step=4;break;}//3
		case 4:{BLDC.motor_step=3;break;}//2
		case 5:{BLDC.motor_step=2;break;}//1
		default :{BLDC.motor_step=0;break;}
	}
}

/********************************************************************
	*
	*Function Name:void MotorStop(void)
	*Function:
	*Inputr Ref:NO
	*Return Ref:NO
	*
*********************************************************************/				
void  StartMotorRun(void)
{
       switch(BLDC.motor_step)
				{
					case 0:
						{
						PWMD1L   = 0x90;
						PWMD1H   = 0x01;    //Tpwm = 0x190 * 2* 1/8(us) = 400*2 *1/8 = 100us F =10KHz
						PWMD0L   = 0x00;
						PWMD0H   = 0x00;
						PWMLOADEN = 0x03;
						BLDC.motor_step++;
						break;
						}
					case 1:
						{
						PWMD0L   = 0x90;
						PWMD0H   = 0x01;
						PWMD1L   = 0x00;
						PWMD1H   = 0x00;
						PWMLOADEN = 0x03;
						BLDC.motor_step++;
						break;
						}
					case 2:
						{
						PWMD1L   = 0x90;
						PWMD1H   = 0x01;
						PWMD0L   = 0x00;
						PWMD0H   = 0x00;
						PWMLOADEN = 0x03;
						BLDC.motor_step++;
						break;
						}
					case 3:
						{
						PWMD0L   = 0x90;
						PWMD0H   = 0x01;
						PWMD1L   = 0x00;
						PWMD1H   = 0x00;
						PWMLOADEN = 0x03;
						BLDC.motor_step++;
						break;
						}
					case 4:
						{
						PWMD1L   = 0x90;
						PWMD1H   = 0x01;
						PWMD0L   = 0x00;
						PWMD0H   = 0x00;
						PWMLOADEN = 0x03;
						BLDC.motor_step++;
						break;
						}
					case 5:
						{
						PWMD0L   = 0x90;
						PWMD0H   = 0x01;
						PWMD1L   = 0x00;
						PWMD1H   = 0x00;
						PWMLOADEN = 0x03;
						BLDC.motor_step=0; //WT.EDIT
						break;
						}
					default :{MOS_OFF;break;}
				}
				com_charge();


    
}

void	BREAK(void)
{
	if(BLDC.check_over_time)
	{
		BLDC.check_over_time--;
		out_pwm(_break_pwm);
		MOS_BREAK;
		BLDC.duzhuan_time = 0;
	}
	else
	{
		MOS_OFF;
		out_pwm(0);
		BLDC.status = _OPEN;
		BLDC.open_status = OPEN_CONF_COM;
	}
}

void	check_FB(void)
{
	switch(BLDC.status)
	{
		case   _STOP:
			break;
		case  _CHECK:StartTest(); //CHECK();
			break;
		case  _OPEN:StartMotorRun();//OPEN();
			break;
		case  _LOOP:MotorRun();//LOOP();
			break;
		case  _BREAK:BREAK();
			break;
		default:break;
	}
}

/********************************************************************
	*
	*Function Name:void BLDC_start(void)
	*Function:
	*Inputr Ref:NO
	*Return Ref:NO
	*
*********************************************************************/
void	BLDC_start(void)
{
	BLDC.mode = _run;
	bldc_value_init();
	BLDC.status = _CHECK;
	ON_BLDC_INTE;
}
/********************************************************************
	*
	*Function Name:void MotorStop(void)
	*Function:
	*Inputr Ref:NO
	*Return Ref:NO
	*
*********************************************************************/
void	 MotorStop(void)
{
	BLDC.mode = _stop;
	MOS_OFF;
	BLDC.pwm_out = 0;
	out_pwm(BLDC.pwm_out);
	BLDC.status = _STOP;
	OFF_BLDC_INTE;
	BLDC.reset_time = 0;
}

void	set_error(void)
{
	if(++BLDC.duzhuan_time>_en_duzhuan_time)
	{
		BLDC.error  |= _duzhuan_error;
		BLDC.duzhuan_time = 0;
	}
	if(BLDC.pwm_set<(_pwm_min_set))
	{
		BLDC.error  |= _pwm_limit_error;
	}
	if(ADC.current>_I(_set_over_current))
	{
		BLDC.error  |= _current_over_error;
	}
	if((ADC.voltage>_U(_set_over_voltage))||(ADC.voltage<_U(_set_min_voltage)))
	{
		BLDC.error  |= _voltage_over_error;
	}
}

void	reset_error(void)
{
	if(++BLDC.reset_time>(_set_reset_time>>2))
	{
		//BLDC.error  &= ~_cw_ccw_error;
	}
	if(BLDC.reset_time>_set_reset_time)
	{
		//BLDC.error  &= ~_emf_Error;
		//BLDC.error  &= ~_duzhuan_error;
		//BLDC.error  &= ~_hall_error;
		//BLDC.error  &= ~_current_over_error;
	}
	if(BLDC.reset_time>(_set_reset_time<<1))
	{
		
	}
	if(BLDC.pwm_set>(_pwm_min_set+10))
	{
		BLDC.error  &= ~_pwm_limit_error;
	}
	if((ADC.voltage<_U(_set_over_voltage-1))&&(ADC.voltage>_U(_set_min_voltage+1)))
	{
		BLDC.error  &= ~_voltage_over_error;
	}
}

void BLDC_main(void)
{
	if(BLDC.mode)
	{
		if(BLDC.status != _LOOP)
		{
			if(BLDC.pwm_out>_start_max_pwm)
			{
				BLDC.pwm_out = _start_max_pwm;
			}
		}
		else
		{
			BLDC.pwm_out = soft_pwm(BLDC.pwm_set,BLDC.pwm_out);
		}
		out_pwm(BLDC.pwm_out);
		set_error();
		if(BLDC.error != _no_error)
		{
			MotorStop();
		}
	}
	else  
	{
		reset_error();
		if(BLDC.error == _no_error)
		{
			BLDC_start();
		}
	}
}
/********************************************************************
	*
	*Function Name:void MotorStop(void)
	*Function:INTERRUPT
	*Inputr Ref:NO
	*Return Ref:NO
	*
*********************************************************************/

void EPWM_IRQHandler(void)  interrupt 18
{
    static uint8_t temp;
	temp = C1CON1;
	if(temp & 0x80 == 0){
	    delay_us(20);
		if(temp & 0x80 == 0)
			BLDC.EMI_flag = C1CON1;
    }

	if(temp & 0x80 == 1){
	    delay_us(20);
		if(temp & 0x80 == 1)
			BLDC.EMI_flag = C1CON1;
    }
	
	if(BLDC.EMI_flag&0x80)
	{
		_FG_L;
	}
	else
	{
		_FG_H;
	}
	check_FB();
	
	PWMPIF = 0x00;  //中断标志
	PWMZIF = 0x00;
	PWMUIF = 0x00;
	PWMDIF = 0x00;
}




