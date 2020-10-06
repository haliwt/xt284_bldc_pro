#ifndef _BLDC_H_
#define _BLDC_H_
#include "my_type.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define') */
/*****************************************************************************/
/*----------------------------------------------------------------------------
 **ACMP 通道
---------------------------------------------------------------------------*/
#define ACMP0 					(0x00)			/* ACMP channel 0 number */
#define ACMP1					(0x01)			/* ACMP channel 1 number */


#define  _STOP           0
#define  _CHECK          1
#define  _OPEN           2
#define  _LOOP           3
#define  _BREAK          4



void	bldc_value_init(void);
void	BLDC_main(void);
void  blcd_start(void);
void  blcd_stop(void);
void  out_pwm(unsigned int  in);
unsigned int  soft_pwm(unsigned int in,unsigned int out);	
void  check_FB(void);
void  LOOP(void);
void	com_charge(void);
void BLDC_stop(void);


#endif
