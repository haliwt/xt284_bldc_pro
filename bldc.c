#include "include.h"
#include "my_type.h"




#define  OPEN_CONF_COM       0
#define  OPEN_WAIT_CHECK     1
#define  OPEN_CHECK_B        2
#define  OPEN_CHECK_ZERO     3
#define  OPEN_COM_CHARGE     4
#define  OPEN_ERROR          5



#define  _WAIT_CHECK     0
#define  _CHECK_ZERO_B   1
#define  _CHECK_ZERO     2
#define  _WAIT_COM       3
#define  _COM_CHARGE     4
#define  _ERROR          5

#define  FG         P0_1

uint8_t theFirstOn ;

// 用于反电势检测的择多滤波器配置表
/*
code unsigned char cBEMF_FILTER[64] = {  0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,
																					32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,
																					0,2,4,6,8,10,12,14,16,18,1,22,1,26,28,30,
																				  32,34,36,38,1,42,44,46,1,1,1,54,56,58,60,62};		
*/

CHANGE_TYPES  temp2;
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
		case 0:{MOS_U_V;FG=1;break;}
		case 1:{MOS_W_V;break;}
		case 2:{MOS_W_U;break;}
		case 3:{MOS_V_U;FG=0;break;}
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
/*******************************************************************
	*
	*Function Name: void out_pwm(unsigned int in )
	*Function :
	*
	*
	*
*******************************************************************/
void  out_pwm(unsigned int  in)
{
	if(in>_pwm_max){in = _pwm_max;}
	
	temp2.change.math = _pwm_max-in;
	
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

//  		PWMPIE = 0x00;  //周期中断 //WT.EDIT
//		PWMZIE = 0x01;  //零点中断 //WT.EDIT 2020.10.04

		PWMUIE = 0x00;  //向上比较中断
		PWMDIE = 0x00;  //向下比较中断
		PWMPIF = 0x00;  //中断标志
		PWMZIF = 0x00;
		PWMUIF = 0x00;
		PWMDIF = 0x00;
	}
	PWMD1L   = temp2.change.count[1];
	PWMD1H   = temp2.change.count[0];
	PWMLOADEN = 0x02;
}

/*******************************************************************
	*
	*Function Name: void LOOP(void)
	*Function :
	*
	*
	*
*******************************************************************/
void LOOP(void)
{
	//unsigned char i;
	//if(BLDC.pwm_set >720 ){
		{
			
       #if 1
		switch(BLDC.loop_status)
		{
			case   _WAIT_CHECK:
				if(TF1)//这里其实才是等待消磁,定时器溢出中断标志位
				{
					TF1 = 0;
					BLDC.loop_status = _CHECK_ZERO;
				}
				break;
			case   _CHECK_ZERO_B:
				if(!(BLDC.EMI_flag&0x80))
				{
				
                    BLDC.loop_status = _CHECK_ZERO;
				}
				/*if(TF1)
				{
					TF1 = 0;
					BLDC.error  |= _emf_Error;
				}*/
				break;
			case  _CHECK_ZERO://消磁完了之后等待过零点
				BLDC.zero_now_time.one.h  = TH0;
				BLDC.zero_now_time.one.l  = TL0;//这个值不停的跟新，直到比较器发生跳变
				if(BLDC.EMI_flag&0x80)			//此时记录的就是上次过零点和下次过零点之间的时间，也就是60度的时间
				{
					TH0 = 0;
					TL0 = 0;
					TF0 = 0;
					BLDC.zero_period.all = BLDC.zero_period.all - (BLDC.zero_period.all >>2) + BLDC.zero_now_time.all;//240   这一句的作用是将60度的
					BLDC.output_time.all = BLDC.zero_period.all >>3;//30	除以8，也就是三十度的时间					              	时间扩大4倍，也就是240度的时间
					
					BLDC.step_read = BLDC.output_time.all;
					
					BLDC.output_time.all -= (BLDC.output_time.all>>2);//24度的时间
					BLDC.zero_check_time.all = BLDC.output_time.all >>1;//12度的时间
				  BLDC.zero_check_time.all += (BLDC.zero_check_time.all>>3);//12度的时间在加上一个值，这个值作为微调用
					BLDC.duzhuan_time = 0;
					TL1 = ~BLDC.output_time.one.l;					
					TH1 = ~BLDC.output_time.one.h; 
					TF1 = 0;
					
					BLDC.step_time_sum += BLDC.step_read;
				
							if(++BLDC.step_time_count>=64)//if(++BLDC.step_time_count>=64) //WT.EDIT
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
			case  _WAIT_COM://这里是等待延时30时间，但是这个延时时间并不一定是30度，根据实际情况调整
			    //if(TF1)//
				{
					TF1 = 0;
                    //delay_us(20);//WT.EDIT 
                    // delay_us(30);//WT.EDIT 
                    //delay_us(10);//WT.EDIT--BETTER
                    delay_us(6); //WT.EDIT 
					BLDC.loop_status = _COM_CHARGE;
				}
				break;
			case  _COM_CHARGE:
				TL1 = ~BLDC.zero_check_time.one.l;	//这里是将消磁的时间加入定时器，随后立马换向				
				TH1 = ~BLDC.zero_check_time.one.h; 
				TF1 = 0;
				if(++BLDC.motor_step>=6){BLDC.motor_step = 0;}
				com_charge();
				BLDC.loop_status = _WAIT_CHECK;
				break;
			case  _ERROR:
				break;
			default:BLDC.loop_status = _WAIT_CHECK;
				break;
		}

      #endif 
	}
	#if 0
	else{
	for(i=0;i<8;i++)
	{
		switch(BLDC.loop_status)
		{
			case   _WAIT_CHECK:
				if(TF1)//这里其实才是等待消磁,定时器溢出中断标志位
				{
					TF1 = 0;
					BLDC.loop_status = _CHECK_ZERO;
				}
				break;
			case   _CHECK_ZERO_B:
				if(!(BLDC.EMI_flag&0x80))
				{
					BLDC.loop_status = _CHECK_ZERO;
				}
				/*if(TF1)
				{
					TF1 = 0;
					BLDC.error  |= _emf_Error;
				}*/
				break;
			case  _CHECK_ZERO://消磁完了之后等待过零点
				BLDC.zero_now_time.one.h  = TH0;
				BLDC.zero_now_time.one.l  = TL0;//这个值不停的跟新，直到比较器发生跳变
				if(BLDC.EMI_flag&0x80)					//此时记录的就是上次过零点和下次过零点之间的时间，也就是60度的时间
				{
					TH0 = 0;
					TL0 = 0;
					TF0 = 0;
					BLDC.zero_period.all = BLDC.zero_period.all - (BLDC.zero_period.all >>2) + BLDC.zero_now_time.all;//240   这一句的作用是将60度的
					BLDC.output_time.all = BLDC.zero_period.all >>3;//30	除以8，也就是三十度的时间					              	时间扩大4倍，也就是240度的时间
					
					BLDC.step_read = BLDC.output_time.all;
					
					BLDC.output_time.all -= (BLDC.output_time.all>>2);//24度的时间
					BLDC.zero_check_time.all = BLDC.output_time.all >>1;//12度的时间
				  BLDC.zero_check_time.all += (BLDC.zero_check_time.all>>3);//12度的时间在加上一个值，这个值作为微调用
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
			case  _WAIT_COM://这里是等待延时30时间，但是这个延时时间并不一定是30度，根据实际情况调整
				if(TF1)
				{
					TF1 = 0;
					BLDC.loop_status = _COM_CHARGE;
				}
				break;
			case  _COM_CHARGE:
				TL1 = ~BLDC.zero_check_time.one.l;	//这里是将消磁的时间加入定时器，随后立马换向				
				TH1 = ~BLDC.zero_check_time.one.h; 
				TF1 = 0;
				if(++BLDC.motor_step>=6){BLDC.motor_step = 0;}
				com_charge();
				BLDC.loop_status = _WAIT_CHECK;
				break;
			case  _ERROR:
				break;
			default:BLDC.loop_status = _WAIT_CHECK;
				break;
		}
	}
		}
	#endif 
}
/*******************************************************************
	*
	*Function Name: void CHECK(void)
	*Function :
	*
	*
	*
*******************************************************************/
void  CHECK(void)
{
	if(++BLDC.check_over_time<5000)
	{

		BLDC.duzhuan_time = 0;
		BLDC.EMF_now = 0;
		C1CON2 = 0x00;//配置比较器1
		C1CON0 = 0x80;//比较器1使能--输入是C1P0 
		delay_us(20);
		if(C1CON1&0x80){BLDC.EMF_now |= 0x01;}//如果比较器1的输出位为高，则
		C1CON0 = 0x81;//这句不知干嘛的，猜测是配置比较器，去采集三个过零点信号
		delay_us(20);
		if(C1CON1&0x80){BLDC.EMF_now |= 0x02;}
		C1CON0 = 0x82;
		delay_us(20);
		if(C1CON1&0x80){BLDC.EMF_now |= 0x04;}
		
		if(BLDC.EMF_last != BLDC.EMF_now)
		{
			BLDC.duzhuan_time = 0;
			BLDC.zero_now_time.one.h  = TH0;
			BLDC.zero_now_time.one.l  = TL0;
			TH0 = 0;
			TL0 = 0;
			TF0 = 0;
			BLDC.zero_period.all = BLDC.zero_period.all - (BLDC.zero_period.all >>2) + BLDC.zero_now_time.all;//240
			BLDC.output_time.all = BLDC.zero_period.all >>3;//30
			BLDC.output_time.all -= (BLDC.output_time.all>>3);
			BLDC.zero_check_time.all = BLDC.output_time.all >>2;
			//BLDC.zero_check_time.all -= (BLDC.zero_check_time.all>>3);
			
			switch(BLDC.EMF_now)//判断这次电机的相位
			{
				case 1:
					if(BLDC.EMF_last == 3)//判断这次电机相位的前一个相位，前一个相位是3，说明是CW方向
					{
						if(BLDC.check_cw_ccw_flag == _CW)//通过前后两次的相位，就可以确定电机的转动方向，
						{																	//在这里通过判断当前的方向与电机的实际方向是否一致
							BLDC.EMI_count++;								//从而就知道电机位置检测是否正确
						}
						else//说明不正确
						{
							BLDC.EMI_count = 0;//计数清零
							BLDC.check_cw_ccw_flag = _CW;
						}
					}
					if(BLDC.EMF_last == 5)//如果前一个相位是5，说明是CCW方向
					{
						if(BLDC.check_cw_ccw_flag == _CCW)
						{
							BLDC.EMI_count++;
						}
						else
						{
							BLDC.EMI_count = 0;
							BLDC.check_cw_ccw_flag = _CCW;
						}
					}
					if(BLDC.check_cw_ccw_flag == _CW){BLDC.motor_step = 3;}
					else{BLDC.motor_step = 4;}
					
					break;
				case 3:
					if(BLDC.EMF_last == 2)
					{
						if(BLDC.check_cw_ccw_flag == _CW)
						{
							BLDC.EMI_count++;
						}
						else
						{
							BLDC.EMI_count = 0;
							BLDC.check_cw_ccw_flag = _CW;
						}
					}
					if(BLDC.EMF_last == 1)
					{
						if(BLDC.check_cw_ccw_flag == _CCW)
						{
							BLDC.EMI_count++;
						}
						else
						{
							BLDC.EMI_count = 0;
							BLDC.check_cw_ccw_flag = _CCW;
						}
					}
					if(BLDC.check_cw_ccw_flag == _CW){BLDC.motor_step = 2;}
					else{BLDC.motor_step = 5;}
					break;
				case 2:
					
					if(BLDC.EMF_last == 6)
					{
						if(BLDC.check_cw_ccw_flag == _CW)
						{
							BLDC.EMI_count++;
						}
						else
						{
							BLDC.EMI_count = 0;
							BLDC.check_cw_ccw_flag = _CW;
						}
					}
					if(BLDC.EMF_last == 3)
					{
						if(BLDC.check_cw_ccw_flag == _CCW)
						{
							BLDC.EMI_count++;
						}
						else
						{
							BLDC.EMI_count = 0;
							BLDC.check_cw_ccw_flag = _CCW;
						}
					}
					if(BLDC.check_cw_ccw_flag == _CW){BLDC.motor_step = 1;}
					else{BLDC.motor_step = 0;}
					break;
				case 6:
					if(BLDC.EMF_last == 4)
					{
						if(BLDC.check_cw_ccw_flag == _CW)
						{
							BLDC.EMI_count++;
						}
						else
						{
							BLDC.EMI_count = 0;
							BLDC.check_cw_ccw_flag = _CW;
						}
					}
					if(BLDC.EMF_last == 2)
					{
						if(BLDC.check_cw_ccw_flag == _CCW)
						{
							BLDC.EMI_count++;
						}
						else
						{
							BLDC.EMI_count = 0;
							BLDC.check_cw_ccw_flag = _CCW;
						}
					}
					if(BLDC.check_cw_ccw_flag == _CW){BLDC.motor_step = 0;}
					else{BLDC.motor_step = 1;}
					break;
				case 4:
					if(BLDC.EMF_last == 5)
					{
						if(BLDC.check_cw_ccw_flag == _CW)
						{
							BLDC.EMI_count++;
						}
						else
						{
							BLDC.EMI_count = 0;
							BLDC.check_cw_ccw_flag = _CW;
						}
					}
					if(BLDC.EMF_last == 6)
					{
						if(BLDC.check_cw_ccw_flag == _CCW)
						{
							BLDC.EMI_count++;
						}
						else
						{
							BLDC.EMI_count = 0;
							BLDC.check_cw_ccw_flag = _CCW;
						}
					}
					if(BLDC.check_cw_ccw_flag == _CW){BLDC.motor_step = 5;}
					else{BLDC.motor_step = 2;}
					break;
				case 5:
					if(BLDC.EMF_last == 1)
					{
						if(BLDC.check_cw_ccw_flag == _CW)
						{
							BLDC.EMI_count++;
						}
						else
						{
							BLDC.EMI_count = 0;
							BLDC.check_cw_ccw_flag = _CW;
						}
					}
					if(BLDC.EMF_last == 4)
					{
						if(BLDC.check_cw_ccw_flag == _CCW)
						{
							BLDC.EMI_count++;
						}
						else
						{
							BLDC.EMI_count = 0;
							BLDC.check_cw_ccw_flag = _CCW;
						}
					}
					if(BLDC.check_cw_ccw_flag == _CW){BLDC.motor_step = 4;}
					else{BLDC.motor_step = 3;}
					break;
				default:
					break;
			}
			BLDC.EMF_last = BLDC.EMF_now;
		}
		
		if(BLDC.EMI_count>=5)//连续5次检测位置正确
		{
			BLDC.EMI_count = 0;
			if(BLDC.check_cw_ccw_flag == BLDC.cw_ccw_now)//通过位置检测判断出来的方向与电机实际转动方向一致
			{
				switch(BLDC.motor_step)//换向
				{
					case 0:{MOS_U_V;break;}
					case 1:{MOS_W_V;break;}
					case 2:{MOS_W_U;break;}
					case 3:{MOS_V_U;break;}
					case 4:{MOS_V_W;break;}
					case 5:{MOS_U_W;break;}
					default :{MOS_OFF;break;}
				}
				TL1 = ~BLDC.zero_check_time.one.l;					
				TH1 = ~BLDC.zero_check_time.one.h; 
				TF1 = 0;
				BLDC.loop_status = _WAIT_CHECK;
				BLDC.status = _LOOP;//切入闭环
				BLDC.EMI_NG_count = 0;
			}
			else
			{
				BLDC.check_over_time = 0;
				BLDC.check_over_time = _break_time;
				MOS_OFF;
				BLDC.status = _BREAK;
			}
		}
	}
	
	
	else
	{
		BLDC.status = _OPEN;
		BLDC.open_status = OPEN_CONF_COM;
	}
}
/*******************************************************************
	*
	*Function Name: void confirm_phase(void)
	*Function :
	*
	*
	*
*******************************************************************/	
void	confirm_phase(void)
{
	uint16_t   i,k,j;
	uint16_t   Iuv,Iuw,Ivu,Ivw,Iwu,Iwv,Ioffset;
	uint8_t    angle;
	
	k = _IPD_GET_TIMES;//采样次数
	j = _IPD_GET_IDLE; //停止间隔
	MOS_OFF;  //输出0000 
	out_pwm(_pwm_max);
	Iuv = get_current_now();  //采样电阻0.02R 上半桥道通，Pmos管  
	MOS_OFF;
	
	for(i=0;i<j;i++)
	{
		Iuv = get_current_now();
	}
	MOS_U_V; //A+ B- "6"
	Iuv = 0;
	for(i=0;i<k;i++)
	{
		Iuv += get_current_now(); //Iuv > Ivu
	}
	MOS_OFF;	
	
	for(i=0;i<j;i++)
	{
		Iwv = get_current_now();
	}
	MOS_W_V; //C+ B- '4'
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
	MOS_W_U; //C+ A- '5'
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
	MOS_V_U; //B+ A- '1'
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
	MOS_V_W; //B+ C- '3'
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
	MOS_U_W; //A+ C- '2'
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
	if(Iuv>Ivu){angle |= 0x01;} //顺序通电，采样电压值比较
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
/*******************************************************************
	*
	*Function Name: void OPEN(void)
	*Function :
	*Input Ref:
	*Return Ref:
	*
*******************************************************************/
void  OPEN(void)
{
		switch(BLDC.open_status)
		{
			case   OPEN_CONF_COM:
				BLDC.zero_check_time.all =  BLDC.open_period;
				confirm_phase();
				com_charge();
				BLDC.duzhuan_time = 0;
				BLDC.check_over_time = 0;
			  BLDC.open_period = _open_max_time;
				BLDC.zero_check_time.all =  BLDC.open_period;
				TH1 = 0; //定时器 1
				TL1 = 0;
				TF1 = 0;
				BLDC.open_status = OPEN_WAIT_CHECK;
				break;
			case   OPEN_WAIT_CHECK:
				if(TF1) //计数器 1溢出中断 0.25us*65535 =16.384ms 如何判断比较好？
				{
					TF1 = 0;
					BLDC.open_status = OPEN_CHECK_ZERO;
				}
				break;
			case   OPEN_CHECK_B:
				if((BLDC.EMI_flag&0x80)==0x00)
				{
					BLDC.open_status = OPEN_CHECK_ZERO;
				}
				if((--BLDC.zero_check_time.all) == 0)
				{
					BLDC.open_status = OPEN_COM_CHARGE;
					BLDC.EMI_OK_count = 0;
					BLDC.turn_OK_count = 0;
				}
				break;
			case  OPEN_CHECK_ZERO: //过零点
				BLDC.zero_now_time.one.h  = TH0; //定时器值--实际的时间值 计时开始 定时器0
				BLDC.zero_now_time.one.l  = TL0;
				if(BLDC.EMI_flag&0x80) //检查电压比较器-输出变化，BEMF反向电动势
				{
					if(++BLDC.EMI_OK_count>=10) //经过暂短延时 判断
					{
						BLDC.EMI_OK_count = 0;
						TH0 = 0; //定时器 0
						TL0 = 0;
						TF0 = 0;
						BLDC.zero_period.all =  (BLDC.zero_now_time.all<<2);//240
						if(++BLDC.turn_OK_count>5)
						{
							BLDC.turn_OK_count = 0;
							MOS_OFF;
							BLDC.output_time.all = BLDC.zero_now_time.all >>1;//30
							BLDC.output_time.all -= (BLDC.zero_now_time.all >>2);//30度 
							BLDC.zero_check_time.all = BLDC.output_time.all >>1;
							BLDC.status = _LOOP; //进入循环
							TL1 = ~BLDC.zero_check_time.one.l;					
							TH1 = ~BLDC.zero_check_time.one.h; 
							TF1 = 0;
							BLDC.loop_status = _WAIT_CHECK; //进入循环的状态中的，WAIT
						}
						else
						{
							BLDC.open_status = OPEN_COM_CHARGE;
						}
					}
				}
				else
				{
					BLDC.EMI_OK_count = 0;
				}
				if((--BLDC.zero_check_time.all) == 0)
				{
					BLDC.open_status = OPEN_COM_CHARGE;
					BLDC.EMI_OK_count = 0;
					BLDC.turn_OK_count = 0;
				}
				break;
			case  OPEN_COM_CHARGE:
				BLDC.open_period -= (BLDC.open_period/15+1);
				if(BLDC.open_period<_open_min_time)
				{
					BLDC.open_period = _open_max_time;
					if(++BLDC.start_times>3)
					{
						BLDC.error  |= _emf_Error; //反向电动势错误
						BLDC.start_times = 0;
					}
				}
				BLDC.zero_check_time.all =  BLDC.open_period;
				//confirm_phase();
				if(++BLDC.motor_step>=6){BLDC.motor_step = 0;}
				
				com_charge();
				BLDC.duzhuan_time = 0;
				BLDC.check_over_time = 0;
				TH1 = 0x7f;
				TL1 = 0;
				TF1 = 0;
				BLDC.open_status = OPEN_WAIT_CHECK;
				break;
			case  OPEN_ERROR:
				break;
			default:BLDC.loop_status = OPEN_CONF_COM;
				break;
		}
}

/*******************************************************************
	*
	*Function Name: void BREAK(void)
	*Function :
	*
	*
	*
*******************************************************************/
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
/*******************************************************************
	*
	*Function Name: void check_FB(void)
	*Function :
	*
	*
	*
*******************************************************************/
void	check_FB(void)
{
	switch(BLDC.status)
	{
		case   _STOP:
			break;
		case  _CHECK:CHECK();//预定位，预定位成功后将状态切换到open
			break;
		case  _OPEN:OPEN();
			break;
		case  _LOOP:LOOP();
			break;
		case  _BREAK:BREAK();
			break;
		default:break;
	}
}

/*******************************************************************
	*
	*Function Name: void BLDC_start(void)
	*Function :
	*
	*
	*
*******************************************************************/
void	BLDC_start(void)
{
	    BLDC.mode = _run;
		bldc_value_init();//电机的各个状态值初始化
		BLDC.status = _CHECK;
		ON_BLDC_INTE;
	
}
/*******************************************************************
	*
	*Function Name: void BLDC_stop(void)
	*Function :
    *Input Ref:
    *Return Ref:
	*
*******************************************************************/
void	BLDC_stop(void)
{
	BLDC.mode = _stop;
	MOS_OFF;
	BLDC.pwm_out = 0;
	out_pwm(BLDC.pwm_out);
	BLDC.status = _STOP;
	OFF_BLDC_INTE;
	BLDC.reset_time = 0;
}
/*******************************************************************
	*
	*Function Name: void set_error(void)
	*Function :
	*Input Ref:
	*Return Ref:
	*
*******************************************************************/

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
 #if 1
	if(ADC.current > LIMIT_MAX_CURRENT)//if(ADC.current>_I(_set_over_current)) //
	{
		BLDC.error  |= _current_over_error;
	}
	#endif 
	#if 0  //硬件没有这部分电路支持此功能
	if((ADC.voltage>_U(_set_over_voltage))||(ADC.voltage<_U(_set_min_voltage)))
	{
		BLDC.error  |= _voltage_over_error;
	}
	#endif 
}

void	reset_error(void)
{
	#if 0
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
	#endif 
	BLDC.error = _no_error;// WT.EDIT 
	#if 0
	if((ADC.voltage<_U(_set_over_voltage-1))&&(ADC.voltage>_U(_set_min_voltage+1)))
	{
		BLDC.error  &= ~_voltage_over_error;
	}
	#endif 
}

void	BLDC_main(void)
{
	if(BLDC.mode)//上电执行完启动后执行这个，此时电机状态已经切换到run状态
	{
		if(BLDC.status != _LOOP)//开环启动
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
		set_error(); //WT.EDIT cancel
		if(BLDC.error != _no_error)
		{
			BLDC_stop();
		}
	}
	else  //上电默认先执行这个
	{
		reset_error();
		if(BLDC.error == _no_error)
		{
			BLDC_start();
		}
	}
}

void EPWM_IRQHandler(void)  interrupt 18//PWM中断
{
	BLDC.EMI_flag = C1CON1; //read  ACMP1 interrupt 

	check_FB();
	
	PWMPIF = 0x00;  //中断标志
	PWMZIF = 0x00;
	PWMUIF = 0x00;
	PWMDIF = 0x00;
}


/********************************************************************************
 ** \brief	 ACMP interrupt service function
 **			
 ** \param [in]  none   
 **
 ** \return none
 ******************************************************************************/
 #if 0
void ACMP_IRQHandler(void)  interrupt 14
{
   if (CNIF & (1<< ACMP1)){
	  BLDC.EMI_flag = C1CON1; //read  ACMP1 interrupt 
	  check_FB();

       
  
	  CNIF &= ~(1<< ACMP1);

   }
	
}
#endif 

