#include "motor.h"

uint8_t gPhase;


/*******************************************************************************
	*
	*Function Name: uint8_t NO_HallSensor_GetPinState(void)
	*Function :Dector no hall senseor BEMF
	*Input Ref:NO
	*Return Ref:No
	*
*******************************************************************************/
uint8_t NO_HallSensor_GetPinState(void)
{
   
		static uint16_t State;
		C1CON2 = 0x00; //比较控制寄存器2 --
		C1CON0 = 0x80; //比较控制寄存器0 --enable compare
		delay_us(20);
		if(C1CON1&0x80){
			State |= 0x01;
		} //U --BEMF
		C1CON0 = 0x81;
		delay_us(20);
		if(C1CON1&0x80){
			State |= 0x02;
		} //V ---BEMF
		C1CON0 = 0x82;
		delay_us(20);
		if(C1CON1&0x80){
			State |= 0x04;
		}//W  ---BEMF 

	  return State;
		
   	
}
/*******************************************************************************
	*
	*Function Name: uint8_t NO_HallSensor_GetPinState(void)
	*Function :Dector no hall senseor BEMF
	*Input Ref:NO
	*Return Ref:No
	*
*******************************************************************************/
void NO_HallSensor_DectorPhase(void)
{
    static uint8_t sense;

	if(SENSE_H) sense =1; 
	else sense =0; 
	
	switch(gPhase){

        case 0:  
           
	          MOS_U_W	; // A+ C- "2"
              if(!sense){
					C1CON2 = 0x00; //比较控制寄存器2 --
					C1CON0 = 0x81; //SENSE_B("V") --输入V 
					gPhase++;
					
			 }
			  
        break;

		case 1:
				MOS_U_V	; //A+ ,B- '6'
				if(sense){
				C1CON2 = 0x00; //比较控制寄存器2 --
                 C1CON0 = 0x82; //BEMF "C" W 输入
                 gPhase ++;


				}
		        

		break;

		case 2:
			    MOS_W_V	; //C+,B- '4'
			    if(!sense){
					C1CON2 = 0x00; //比较控制寄存器2 --
		            C1CON0 = 0x80; //BEMF "A" U
		           gPhase ++ ;

				}

		break;

		case 3:
			    MOS_W_U	; //C+ A- "5"
			    if(sense){
                   C1CON2 = 0x00; //比较控制寄存器2 --
				   C1CON0 = 0x81; //BEMF "B" V
					gPhase ++ ;

				}

		break;

		case 4:
			    MOS_V_U	;//B+ A- "1"
			    if(!sense){

				     C1CON2 = 0x00; //比较控制寄存器2 --
					 C1CON0 = 0x82; //BEMF "C" W
					  gPhase ++ ;

				}

		break;

		case 5: 
                   MOS_V_W	; //B+,C- "3"
                    if(sense){

				     C1CON2 = 0x00; //比较控制寄存器2 --
					 C1CON0 = 0x80; //BEMF "A" U
					  gPhase =0;

				}
		

		break;






	}


}
/*******************************************************************************
	*
	*Function Name: void PowerOn_MotorRun(void)
	*Function :Dector no hall senseor BEMF
	*Input Ref:NO
	*Return Ref:No
	*
*******************************************************************************/

void PowerOn_MotorRun(void)
{
    switch(gPhase){

		case 0:
			     
	          MOS_U_W	;//A+ C- "2"
	          C1CON2 = 0x00; //比较控制寄存器2 --
			  C1CON0 = 0x81; //SENSE_B("V") --输入V 
			  gPhase++;
	          

	    break;

		case 1:
			   MOS_U_V	;  //A+ B-  "6"
			   C1CON2 = 0x00; //比较控制寄存器2 --
               C1CON0 = 0x82; //BEMF "C" W 输入
                 gPhase ++;
		break;

		case 2: 
			  MOS_W_V	;//C+ B-  "4"
			  C1CON2 = 0x00; //比较控制寄存器2 --
		      C1CON0 = 0x80; //BEMF "A" U
		      gPhase ++ ;

		break ;

		case 3:
			  MOS_W_U	;//C+ A -  '5'
			  C1CON2 = 0x00; //比较控制寄存器2 --
		      C1CON0 = 0x81; //BEMF "B" V
		      gPhase ++ ;

		break;

		case 4:
			 MOS_V_U	; //B+ A-  '1'
			 C1CON2 = 0x00; //比较控制寄存器2 --
			 C1CON0 = 0x82; //BEMF "C" W
			 gPhase ++ ;
			

		break;

		case 5:
			    MOS_V_W;//B+ C-   "3"
			    
				C1CON2 = 0x00; //比较控制寄存器2 --
				C1CON0 = 0x80; //BEMF "A" U
				gPhase =0;

		break;
		
		



	}



}

	


