#ifndef _KEY_H_
#define _KEY_H_
#include "cms8s6990.h"




#define POWER_KEY  P24

#define POWER_PRES     1

void KEY_Init(void);

void Delay_ms(uint16_t n)  ;
void delay_us(uint8_t n);


uint8_t HDKey_Scan(uint8_t mode);


#endif


