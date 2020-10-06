#ifndef _MY_TYPE_H_
#define _MY_TYPE_H_


union data16
{
	struct
	{
		uint8_t h;
		uint8_t l;
	}one;
	uint16_t all;
};

union data32
{
	struct
	{
		uint8_t xh;
		uint8_t x; 
		uint8_t h; 
		uint8_t l;
	}one;
	unsigned long all;
};
typedef  struct  
{
	uint8_t         pole;
	union data16    pwm_set;
}FLASH_TYPES;


//typedef  struct  
//{
//	uint8_t         power_on_off;
//	uint8_t         power_set;
//	uint8_t         ms_count;
//	uint8_t         ms_rpm_count;
//	uint8_t         state;
//	uint8_t         pole;
//	uint16_t        pwm_hand;
//	uint32_t        rpm_sum;
//	uint8_t         rpm_sum_count;
//	uint16_t        rpm;
//	uint8_t         dis_ram[4];
//	
//	
//	
//}DISPLAY_TYPES;


//typedef  struct  
//{
//	uint8_t         read;
//	uint8_t         buffer;
//	uint8_t         value;
//	uint8_t         inc_delay;
//	uint16_t    		on_time;
//	uint16_t    		off_time;
//	enum{
//		start  = 0,
//		first  = 1,
//		second = 2,
//		cont   = 3,
//		end    = 4,
//		finish = 5,
//	}state;
//}KEY_TYPES;


typedef  struct                     
{  
	uint16_t   current_read[16];   
	
	uint16_t   current_offset;
	uint16_t   current_ram;  
	uint16_t   voltage_sum;    
	uint16_t   current_sum;   
	uint16_t   pwm_adc_sum; 
	uint16_t   change_voltage_sum;   
	
	uint16_t   voltage;    
	uint16_t   current;   
	uint16_t   pwm_adc;
	uint16_t   change_voltage;  
	
	uint8_t    voltage_count;
	uint8_t    current_count;
	uint8_t    pwm_adc_count;
	uint8_t    change_voltage_count;
}ADC_TYPES;

typedef  struct                     
{
	uint16_t   always_count;
	
	uint16_t   T_time;    
	uint16_t   H_time;
	uint16_t   H_time_ram;
	uint16_t   S_time;
	
	uint16_t   T_last;
	uint32_t   temp32;
	
	uint32_t   T_time_sum;
	uint32_t   H_time_sum;
	uint16_t   T_time_out;    
	uint16_t   H_time_out;
	uint8_t    read_count; 
	
	uint16_t   read;
	uint16_t   read_sum;
	uint16_t   out;    
	uint8_t    out_count; 
	uint8_t    flag;
	
}PWM_TYPES;


typedef  struct                     
{
	uint16_t   time_ms;
	
}SYS_TYPES;

typedef  struct                     
{
	uint8_t		 cw_ccw_in;
	uint8_t		 cw_ccw_now;
	uint8_t		 check_cw_ccw_flag;
							
	uint8_t    bemf_filter;
	uint8_t 	 EMI_flag;
	uint8_t    EMI_count;//用来记录电机启动时位置检测的次数
	uint8_t    motor_step;//电机当前相位，也就是当前的霍尔值
	
	uint8_t    start_times;
	uint8_t    EMI_OK_count;
	uint8_t    EMI_NG_count;
	uint8_t    turn_OK_count;
	
	uint16_t   duzhuan_time;
	
	uint16_t   open_period;
	uint16_t   check_over_time;
	
	union data16  zero_now_time;
	union data32  zero_period;
	
	uint8_t    read_EMF;    
	uint8_t    EMF_now;   
	uint8_t    EMF_last;
	
	union data16   output_time;
	union data16   zero_check_time;
	
	uint16_t   step_time;
	uint16_t   step_read;
	uint32_t   step_time_sum;
	uint8_t    step_time_count;
	uint8_t    step_updata_flag;
	
	uint16_t   pwm_set;    
	uint16_t   pwm_out;
	uint16_t   reset_time;
	
	uint8_t    open_status;
	uint8_t    loop_status;
	uint8_t    status; 
	uint8_t    mode; 
	uint8_t    error; 
}BLDC_TYPES;


typedef  struct  
{
	union{
		uint16_t  math;
		uint8_t   count[2];
	}change;
}CHANGE_TYPES;




#endif
