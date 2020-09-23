#include "motor.h"

uint8_t gPhase;



void	BLDC_start(void)
{
//	BLDC.mode = _run;
//	bldc_value_init();
//	BLDC.status = _CHECK;
	ON_BLDC_INTE;
}


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
void No_HallSensor_Input(void)
{
     
     gPhase = NO_HallSensor_GetPinState();
	 switch (gPhase)
	{
		 case 5:
		 	  MOS_W_U	; //C+ A- "5"

		 break;

		 case 1:

		   MOS_V_U	;//B+ A- "1"

		 break;

		 case 3:
		 		MOS_V_W	; //B+,C- "3"

		 break;

		 case 2:
		 	 MOS_U_W	; // A+ C- "2"

		 break;

		 case 6:
		 	   MOS_U_V	; //A+ ,B- '6'

		 break;

		 case 4:
               	MOS_W_V	; //C+,B- '4'
		 break;
	}



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
    static uint8_t sense,adj=0;

	if(SENSE_H) sense =1; 
	else sense =0; 
	adj=0;
	switch(gPhase){

        case 0:  
              if(adj==0)
	              MOS_U_W	; // A+ C- "2"
              if(intBEMF==0){
					C1CON2 = 0x00; //比较控制寄存器2 --
					C1CON0 = 0x81; //SENSE_B("V") --输入V 
					gPhase++;
			  }
			  else{
				adj++;
			  }
			  if(adj==1)  MOS_V_W ; //"3"
			  
			  
        break;

		case 1:
				if(adj==0)
				    MOS_U_V	; //A+ ,B- '6'
				if(intBEMF==1){
				C1CON2 = 0x00; //比较控制寄存器2 --
                 C1CON0 = 0x82; //BEMF "C" W 输入
                 gPhase ++;


				}
				else adj++;

				if(adj==1)MOS_U_W	; // "2"
		        

		break;

		case 2:
				if(adj==0)
				MOS_W_V	; //C+,B- '4'
			    if(intBEMF==0){
					C1CON2 = 0x00; //比较控制寄存器2 --
		            C1CON0 = 0x80; //BEMF "A" U
		           gPhase ++ ;

				}
				else adj++;
				if(adj==1)  MOS_U_V	; // '6'

		break;

		case 3:
				if(adj==0)
				   MOS_W_U	; //C+ A- "5"
			    if(intBEMF==1){
                   C1CON2 = 0x00; //比较控制寄存器2 --
				   C1CON0 = 0x81; //BEMF "B" V
					gPhase ++ ;
                }
				else adj++;
				
				if(adj++)MOS_W_V	; //"4"

		break;

		case 4:
				if(adj==0)
				  MOS_V_U	;//B+ A- "1"
			    if(intBEMF==0){

				     C1CON2 = 0x00; //比较控制寄存器2 --
					 C1CON0 = 0x82; //BEMF "C" W
					  gPhase ++ ;

				}
				else adj++;

				if(adj== 0) MOS_W_U	;  //"5"

		break;

		case 5: 
				   if(adj==0)
				   		MOS_V_W	; //B+,C- "3"
                    if(intBEMF==1){

				     C1CON2 = 0x00; //比较控制寄存器2 --
					 C1CON0 = 0x80; //BEMF "A" U
					  gPhase =0;

				    }
					else adj++ ;

					if(adj==1)   MOS_V_U	; //"1"
		

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
			     
	          MOS_W_U	;//C+ A -  '5'  //MOS_U_W	;//A+ C- "2"
              delay_us(500);
	          C1CON2 = 0x00; //比较控制寄存器2 --
			  C1CON0 = 0x81; //SENSE_B("V") --输入V 
			  gPhase=1;
	          

	    break;
        
        
        
        case 5:
			     
	           MOS_W_U	;//C+ A -  '5'  //MOS_U_W	;//A+ C- "2"
              delay_us(500);
	          C1CON2 = 0x00; //比较控制寄存器2 --
			  C1CON0 = 0x81; //SENSE_B("V") --输入V 
			  gPhase=1;
	          

	    break;

		case 1:
			    MOS_V_U	; //B+ A-  '1'//MOS_U_V	;  //A+ B-  "6"
               delay_us(500);
			   C1CON2 = 0x00; //比较控制寄存器2 --
               C1CON0 = 0x82; //BEMF "C" W 输入
                 gPhase =3;
		break;

		case 3: 
			   MOS_V_W;//B+ C-   "3"//MOS_W_V	;//C+ B-  "4"
              delay_us(500);
			  C1CON2 = 0x00; //比较控制寄存器2 --
		      C1CON0 = 0x80; //BEMF "A" U
		      gPhase =2 ;

		break ;

		case 2:
			  MOS_U_W	;//A+ C- "2"//MOS_W_U	;//C+ A -  '5'
              delay_us(500);
			  C1CON2 = 0x00; //比较控制寄存器2 --
		      C1CON0 = 0x81; //BEMF "B" V
		      gPhase =6 ;

		break;

		case 6:
			 MOS_U_V	;  //A+ B-  "6"//MOS_V_U	; //B+ A-  '1'
             delay_us(500);
			 C1CON2 = 0x00; //比较控制寄存器2 --
			 C1CON0 = 0x82; //BEMF "C" W
			 gPhase =4 ;
			

		break;

		case 4:
			    MOS_W_V	;//C+ B-  "4"// MOS_V_W;//B+ C-   "3"
			    delay_us(500);
				C1CON2 = 0x00; //比较控制寄存器2 --
				C1CON0 = 0x80; //BEMF "A" U
				gPhase =0;

		break;
		
		



	}



}

/*******************************************************************************
	*
	*Function Name: void Start_MotorRun(void)
	*Function :Dector no hall senseor BEMF
	*Input Ref:NO
	*Return Ref:No
	*
*******************************************************************************/
void Start_MotorRun(void)
{
	switch(gPhase)
					{
						case 0:
							{
							PWMD1L	 = 0x90;
							PWMD1H	 = 0x01;
							PWMD0L	 = 0x00;
							PWMD0H	 = 0x00;
							PWMLOADEN = 0x03;
                            gPhase ++;
							break;
							}
						case 1:
							{
							PWMD0L	 = 0x90;
							PWMD0H	 = 0x01;
							PWMD1L	 = 0x00;
							PWMD1H	 = 0x00;
							PWMLOADEN = 0x03;
                                 gPhase ++;
							break;
							}
						case 2:
							{
							PWMD1L	 = 0x90;
							PWMD1H	 = 0x01;
							PWMD0L	 = 0x00;
							PWMD0H	 = 0x00;
							PWMLOADEN = 0x03;
                                 gPhase ++;
							break;
							}
						case 3:
							{
							PWMD0L	 = 0x90;
							PWMD0H	 = 0x01;
							PWMD1L	 = 0x00;
							PWMD1H	 = 0x00;
							PWMLOADEN = 0x03;
                                 gPhase ++;
							break;
							}
						case 4:
							{
							PWMD1L	 = 0x90;
							PWMD1H	 = 0x01;
							PWMD0L	 = 0x00;
							PWMD0H	 = 0x00;
							PWMLOADEN = 0x03;
                                 gPhase ++;
							break;
							}
						case 5:
							{
							PWMD0L	 = 0x90;
							PWMD0H	 = 0x01;
							PWMD1L	 = 0x00;
							PWMD1H	 = 0x00;
							PWMLOADEN = 0x03;
                                 gPhase =0;
							break;
							}
						default :{MOS_OFF;break;}
					}
					com_charge();


}
void	com_charge(void)
{
	switch(gPhase)
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


