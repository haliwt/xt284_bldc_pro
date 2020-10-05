//#include<include.h>
//#include "my_type.h"

//#define		_KEY_SP_EN				      0X01
//#define		_KEY_SP_PL				      0X02
//#define		_KEY_SP_INC				      0X04
//#define		_KEY_SP_DEC				      0X08
//#define		_KEY_P_ON				        0X10
//#define		_KEY_P_OFF				      0X20

//#define		_KEY_SP_INC_CONT				0X84
//#define		_KEY_SP_DEC_CONT				0X88



//#define		_KEY_ALL_OFF				    0X3f



//extern  BLDC_TYPES    BLDC;

//uint8_t  eeprom_updata_flag;
////idata    KEY_TYPES       key;
////extern   idata   DISPLAY_TYPES   led;
//FLASH_TYPES  FLASH;


//void	key_value_init(void)
//{
//	key.read = 0;
//	key.buffer = 0;
//	key.value = 0;
//	key.inc_delay = 0;
//	key.on_time = 0;
//	key.off_time = 0;
//	key.state = 0;
//}

//unsigned char	key_scan(void)
//{
//	unsigned char  reval = 0;
//	unsigned char  read_ram = 0;
//	read_ram = IIC_L_read(0x4f);
//	key.read = _KEY_ALL_OFF;
//	switch(read_ram)
//	{
//		case 0X4F:
//			key.read &= ~_KEY_SP_EN;
//			break;
//		case 0x57:
//			key.read &= ~_KEY_SP_PL;
//			break;
//		case 0x5f:
//			key.read &= ~_KEY_SP_INC;
//			break;
//		case 0x47:
//			key.read &= ~_KEY_SP_DEC;
//			break;
//		case 0x67:
//			key.read &= ~_KEY_P_OFF;
//			break;
//		default:
//			break;
//	}
//	if(!P2_5)
//	{
//		key.read &= ~_KEY_P_ON;
//	}
//	switch(key.state )
//	{
//		case start:
//		{
//			if(key.read != _KEY_ALL_OFF)
//			{
//				key.buffer   = key.read;
//				key.state    = first;
//				key.on_time  = 0;
//				key.off_time = 0;
//			}
//			break;
//		}
//		case first:
//		{
//			if(key.read == key.buffer)
//			{
//				if(++key.on_time>10)
//				{
//					key.value = key.buffer^_KEY_ALL_OFF;
//					key.on_time = 0;
//					key.state   = second;
//				}
//			}
//			else
//			{
//				key.state   = start;
//			}
//			break;
//		}
//		case second:
//		{
//			if(key.read == key.buffer)
//			{
//				if(++key.on_time>500)
//				{
//					key.value = key.value|0x80;
//					key.on_time = 0;
//					key.state   = finish;
//				}
//			}
//			else if(key.read == _KEY_ALL_OFF)
//			{
//				if(++key.off_time>10)
//				{
//					key.state   = finish;
//				}
//			}
//			break;
//		}
//		case finish:
//		{
//			reval = key.value;
//			key.state   = end;
//			break;
//		}
//		case end:
//		{
//			if(key.read == _KEY_ALL_OFF)
//			{
//				if(++key.off_time>10)
//				{
//					key.state   = start;
//				}
//			}
//			else
//			{
//				if(key.value&0x80)
//				{
//					if(++key.inc_delay>10)
//					{
//						key.inc_delay = 0;
//						reval = key.value;
//					}
//				}
//			}
//			break;
//		}
//		default:
//		{
//			break;
//		}
//	}
//	
//	return  reval;
//}



//void	key_handing(void)
//{
//	unsigned char  temp;
//	
//	temp = key_scan();
//	switch(temp)
//	{
//		case _KEY_P_ON:
//			if(led.power_set == 0){led.power_set = 1;}
//			break;
//		case _KEY_P_OFF:
//			if(led.power_set){led.power_set = 0;}
//			break;
//		case _KEY_SP_INC_CONT:
//		case _KEY_SP_INC:
//			if(led.state ==  _DIS_RPM)
//			{
//				if(led.power_on_off == 0){break;}
//				if(led.pwm_hand<_pwm_max)
//				{
//					led.pwm_hand++;
//				}
//				else{led.pwm_hand=_pwm_max;}
//			}
//			else
//			{
//				if(led.pole<99)
//				{
//					led.pole++;
//				}
//			}
//			break;
//		case _KEY_SP_DEC_CONT:
//		case _KEY_SP_DEC:
//			if(led.state ==  _DIS_RPM)
//			{
//				if(led.power_on_off == 0){break;}
//				if(led.pwm_hand>(_pwm_min_set+20))
//				{
//					led.pwm_hand--;
//				}
//				else{led.pwm_hand=(_pwm_min_set+20);}
//			}
//			else
//			{
//				if(led.pole>1)
//				{
//					led.pole--;
//				}
//			}
//			break;
//		case _KEY_SP_EN:
//			if(led.state == _DIS_POLE)
//			{
//				eeprom_updata_flag = 1;
//				led.state =  _DIS_RPM;
//			}
//			break;
//		case _KEY_SP_PL:
//			if(led.power_on_off){break;}
//			led.state = _DIS_POLE;
//			break;
//		default:
//			break;
//	}
//	if(P3_6==0)
//	{
//		led.power_set = 0;
//	}
//	if(led.state == _DIS_POLE)
//	{
//		led.power_set = 0;
//		led.power_on_off = 0;
//	}
//	else
//	{
//		if((P3_6==0)||(led.power_set))
//		{
//			led.power_on_off = 1;
//		}
//		else
//		{
//			led.power_on_off = 0;
//			if(FLASH.pwm_set.all != led.pwm_hand)
//			{
//				eeprom_updata_flag = 1;
//			}
//		}
//	}
//	if(led.power_on_off)
//	{
//		BLDC.pwm_set = led.pwm_hand;
//	}
//	else
//	{
//		BLDC.pwm_set = 0;
//		BLDC.error  &= ~_cw_ccw_error;
//		BLDC.error  &= ~_emf_Error;
//		BLDC.error  &= ~_duzhuan_error;
//		BLDC.error  &= ~_current_over_error;
//		BLDC.error  &= ~_line_Error;
//	}
//	if(eeprom_updata_flag)
//	{
//		eeprom_updata_flag = 0;
//		FLASH.pole = led.pole;
//		FLASH.pwm_set.all = led.pwm_hand;
//		eeprom_write(&FLASH);
//	}
//}




