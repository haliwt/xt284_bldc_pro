#ifndef _ADC_H_
#define _ADC_H_

#define MOTOR_LED    P2_4 
#define POWER_LED    P2_6

void	adc_value_init(void);
void	scan_adc_channal(void);
uint16_t	get_current_now(void);
void	check_current_offset(void);
void read_change_voltage(void);
#endif
