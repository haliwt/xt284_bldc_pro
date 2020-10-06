#ifndef _KEY_H_
#define _KEY_H_


#define  POWER_KEY    P2_4    //input IO POWER KEY
#define  POWER_PRES   1

#define    LED_POWER_RED     P2_6  //电源按键指示灯

#define    MOTOR_RUN    1
#define    MOTOR_STOP   0

extern uint8_t MOTOR_RUN_FLG;






void KEY_Handing(void);


uint8_t HDKey_Scan(uint8_t mode);


#endif


