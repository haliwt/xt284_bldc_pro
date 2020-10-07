#ifndef _INCLUDE_H_
#define _INCLUDE_H_


#include <CMS8S6990.H>
#include <hardware.H>
#include <bldc.H>
#include <pwm.H>
#include <adc.H>
#include <key.H>
#include <flash.H>
#include "intrins.h" 


#define		MOS_U_V			{PWMMASKE = 0x39;C1CON0 = 0x82;C1CON2 = 0x00;} // A+ B -
#define		MOS_U_W			{PWMMASKE = 0x2D;C1CON0 = 0x81;C1CON2 = 0x20;}
#define		MOS_V_W			{PWMMASKE = 0x27;C1CON0 = 0x80;C1CON2 = 0x00;}
#define		MOS_V_U			{PWMMASKE = 0x36;C1CON0 = 0x82;C1CON2 = 0x20;}
#define		MOS_W_U			{PWMMASKE = 0x1E;C1CON0 = 0x81;C1CON2 = 0x00;}
#define		MOS_W_V			{PWMMASKE = 0x1B;C1CON0 = 0x80;C1CON2 = 0x20;}

 
#define		MOS_BREAK		{PWMMASKE = 0x3F;}//PWMFBKC = 0x10;} //1 -掩码数据输出，0-PWM输出
#define		MOS_OFF			{PWMMASKE = 0x3f;}//PWMFBKC = 0x00;}

#define   _FG_H               P0_1 = 0;
#define   _FG_L               P0_1 = 1;


#define   hall_1	 5
#define   hall_2	 4
#define   hall_3	 6
#define   hall_4	 2
#define   hall_5	 3
#define   hall_6	 1

#define   hall_e1	 0
#define   hall_e2	 7



#define   OP_current_channal     0x5F
//#define   current_channal        0x5E
#define   voltage_channal       0x51  //WT.EDIT   0x5C 
#define   pwm_adc_channal        0x5B
#define   change_voltage_channal        0x58	


#define   _set_over_voltage    28
#define   _set_min_voltage      8
#define   _set_over_current     4// WT.EDIT 2020.10.07 //5
#define   _en_duzhuan_time      500
#define   _soft_step_time       5



#define   _set_reset_time       2000
 
#define   _pwm_max							800
#define   _start_pwm_set        50
#define   _start_max_pwm        300
#define   _pwm_min_set          50
#define   _break_pwm						800


#define   _open_min_time					20      //(x/20000)
#define   _open_max_time					2000     //(x/20000)
#define   _break_time							10000    //(x/20000)


#define   _no_error               0x00
#define   _voltage_over_error     0x01
#define   _current_over_error     0x02
#define   _pwm_limit_error        0x04
#define   _duzhuan_error          0x08
#define   _cw_ccw_error           0x10
#define   _emf_Error              0x20
#define   _line_Error             0x40



#define   ON_BLDC_INTE     {PWMPIF = 0x00;PWMZIF = 0x00;PWMUIF = 0x00;PWMDIF = 0x00;EIE2 |= 0x08;}
#define   OFF_BLDC_INTE    {PWMPIF = 0x00;PWMZIF = 0x00;PWMUIF = 0x00;PWMDIF = 0x00;EIE2 &=~0x08;}

#define   ON_PWM_IN_INTE     {T2IF = 0x00;ET2 = 1;}
#define   OFF_PWM_IN_INTE    {T2IF = 0x00;ET2 = 0;}

//#define   ON_HALL_INTE       {P3EXTIF = 0x00;P3EXTIE = 0x46;}
//#define   OFF_HALL_INTE      {P3EXTIF = 0x00;P3EXTIE = 0x00;}


#define   _start_ADC         ADCON0 |= 0x02;

#define   _run               1
#define   _stop              0

#define   _CW               0
#define   _CCW              1

#define		_DIS_RPM				        0
#define		_DIS_POLE				        1


#define   _U(a)      (unsigned int)((float)(a)*1241/10)
#define   _I(a)      (unsigned int)((float)(a)*1325/10)
#define   LIMIT_MAX_CURRENT    245//Ref :2V 6A  WT.EDIT    




#define	_LINK_CHECK_CURRENT_SET			0.5


#define	_IPD_GET_TIMES			15
#define	_IPD_GET_IDLE			  10

#define	_LINK_CHECK_CURRENT			(_I(_LINK_CHECK_CURRENT_SET)*_IPD_GET_TIMES)



#define	FLASH_CODE			(0x00)
#define	FLASH_DATA			(0x10)
#define	FLASH_WRITE			(0x09)
#define	FLASH_READ			(0x01)
#define	FLASH_ERASE			(0x0D)

#endif


