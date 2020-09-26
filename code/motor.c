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
void BEMF_IOInit(void)
{
    GPIO_SET_MUX_MODE(P01CFG, GPIO_MUX_GPIO);		//设置P01为GPIO模式
	GPIO_ENABLE_INPUT(P0TRIS, GPIO_PIN_1);			//设置为输入模式
	GPIO_ENABLE_RD(P0RD, GPIO_PIN_1);	//开启下拉

	GPIO_SET_MUX_MODE(P36CFG, GPIO_MUX_GPIO);		//设置P36为GPIO模式
	GPIO_ENABLE_INPUT(P3TRIS, GPIO_PIN_6);			//设置为输出模式
	

	
 
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
uint8_t  No_HallSensor_Input(void)
{
     
        static uint8_t state;
		state = 0;
		C1CON2 = 0x00; //比较控制寄存器2 --
		C1CON0 = 0x80; //比较控制寄存器0 --enable compare
		delay_us(20);
		if(C1CON1&0x80){state |= 0x01;} //U --BEMF
		C1CON0 = 0x81;
		delay_us(20);
		if(C1CON1&0x80){state |= 0x02;} //V ---BEMF
		C1CON0 = 0x82;
		delay_us(20);
		if(C1CON1&0x80){state |= 0x04;}//W  ---BEMF 
        
        switch(state)
		{

			case 0x06:{MOS_U_V;break;}  //A+ B- '6'
			case 0x04:{MOS_W_V;break;}    //C+ B-  "4"
			case 0x05:{MOS_W_U;break;}   //C+ A- "5"
			case 0x01:{MOS_V_U;break;}     //B+ A-   "1"
			case 0X03:{MOS_V_W;break;}       //B+ C - "3"
			case 0X02:{MOS_U_W;break;}        // A+ C-  "2"
			default :{MOS_OFF;break;}
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
void NoSensor_Phase(uint8_t state)
{
		switch(state)
				{

					case 0x06:{MOS_U_V;break;}  //A+ B- '6'
					case 0x04:{MOS_W_V;break;}    //C+ B-  "4"
					case 0x05:{MOS_W_U;break;}   //C+ A- "5"
					case 0x01:{MOS_V_U;break;}     //B+ A-   "1"
					case 0X03:{MOS_V_W;break;}       //B+ C - "3"
					case 0X02:{MOS_U_W;break;}        // A+ C-  "2"
					//default :{MOS_OFF;break;}
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
    static uint8_t sense;
	C1CON2 = 0x00; //比较控制寄存器2 --
  //  sense = intBEMF & 0x80;
   
	if(SENSE) sense =1; 
	else sense =0; 

	switch(gPhase){

        case 0:  
              MOS_A_L	; // A+ C- "2"
             // PWMOE = 0x02;
             if(intBEMF){  // BEMF "B"
				
					MOS_C_H;   //下半周
					//PWMOE = 0x12;
			        C1CON0 = 0x82;
		          //  delay_us(20);
					gPhase++;
			  }
			  else{
				  MOS_B_H;
                 // PWMOE = 0x06;
			  }
			 
			  
			  
        break;

		case 1:
				 MOS_B_H; //A+ ,B- '6'
               //  PWMOE = 0x04;
				if(!intBEMF){ //BEMF "C"
				    MOS_A_L; //A+ 
                 //   PWMOE = 0x06;
				    C1CON0 = 0x80;
		        //   delay_us(20);
				    gPhase ++;


				}
				else{
                      MOS_C_L;  //C+ ;
                   //   PWMOE = 0X24;

				}
        break;

		case 2:
				
				 MOS_C_L	; //C+,B- '4'
               //  PWMOE = 0X20;
			    if(intBEMF){  //BEMF "A"

				   MOS_B_H; //B-
                 //   PWMOE = 0X24;
		           C1CON0 = 0x81;
		         //   delay_us(20);
				    gPhase ++;

				}
				else {
                    MOS_A_H ; //A-
                  //  PWMOE =0X21;
				}

		break;

		case 3:
				
				   MOS_A_H	; //C+ A- "5"
                  // PWMOE = 0X01;
			   if(!intBEMF){
                    MOS_C_L;  //C+ ;
                //   PWMOE= 0X21;
                    C1CON0 = 0x21;
		          //  delay_us(20);
					gPhase ++ ;
                }
				else{
                    MOS_B_L ; //B+
                   // PWMOE= 0X09;
                }
				
				

		break;

		case 4:
			     MOS_B_L	;//B+ A- "1"
               //  PWMOE = 0X08;
			    if(intBEMF){
                    MOS_A_H; //A-
                    C1CON0 = 0x09;
		         //  delay_us(20);
                    gPhase ++ ;

				}
				else {
                    MOS_C_H ; //C-
                   // PWMOE = 0X18;
                }
				

		break;

		case 5: 
				
				    MOS_C_H	; //B+,C- "3"
                 //   PWMOE = 0X10;
                    if(!intBEMF){

				      MOS_B_L; //B+ ;
                     // PWMOE = 0X18;
				      C1CON0 = 0x81;
		            //  delay_us(20);
					  gPhase =0;

				    }
					else{
                        MOS_A_L; // A+ ;
                       // PWMOE =0X12;
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


