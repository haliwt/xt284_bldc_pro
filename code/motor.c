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
uint8_t NoHall_PhaseValue(void) 
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
        
        return state;
      
    
}
/*******************************************************************************
	*
	*Function Name: uint8_t NO_HallSensor_GetPinState(void)
	*Function :Dector no hall senseor BEMF
	*Input Ref:NO
	*Return Ref:No
	*
*******************************************************************************/
void OPEN2(void)
{
     
        static uint8_t state;
		state = 0;
		C1CON2 = 0x00; //比较控制寄存器2 --
		C1CON0 = 0x80; //比较控制寄存器0 --enable compare
		delay_us(20);//delay_us(20);
		if(C1CON1&0x80){state |= 0x01;} //U --BEMF
		C1CON0 = 0x81;
		delay_us(20);
		if(C1CON1&0x80){state |= 0x02;} //V ---BEMF
		C1CON0 = 0x82;
		delay_us(20);
		if(C1CON1&0x80){state |= 0x04;}//W  ---BEMF 
        
        switch(state){

		  
     
        case 5:
			     MOS_C_H	;//C+ A -  '5'  //MOS_U_W	;//A+ C- "2"
              MOS_C_L =0;
              MOS_B_L = 0 ;
              MOS_A_L = 1;
              delay_us(40);
              state =1;
	          

	    break;
 
		case 1:
			    MOS_B_H	; //B+ A-  '1'//MOS_U_V	;  //A+ B-  "6"
                MOS_B_L =0;
                MOS_C_L =0;
                MOS_A_L =1;
               delay_us(40);
                 state =3;
               
		break;

		case 3: 
			   MOS_B_H;//B+ C-   "3"//MOS_W_V	;//C+ B-  "4"
               MOS_B_L =0;
               MOS_A_L =0 ;//关闭 A 
               MOS_C_L =1;
               delay_us(40);
                 state =2;
		     

		break ;

		case 2:
			   MOS_A_H;//A+ C- "2"//MOS_W_U	;//C+ A -  '5'
               MOS_A_L=0;
               MOS_B_L=0; //turn off B
               MOS_C_L =1;
                delay_us(40);
                state =6;
		    

		break;

		case 6:
			 MOS_A_H	;  //A+ B-  "6"//MOS_V_U	; //B+ A-  '1'
             MOS_C_L =0; //关闭C
             MOS_A_L = 0; //关闭A
             MOS_B_L =1;
             delay_us(40);
              state =4;
			
			

		break;

		case 4:
			    MOS_C_H	;//C+ B-  "4"// MOS_V_W;//B+ C-   "3"
                MOS_C_L =0; //关闭C
                MOS_A_L = 0; //关闭A
			    MOS_B_L=1;
                delay_us(40);
                 state =5;
				

		break;
        
        default :
            state = 5;
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
void NO_HallSensor_DectorPhase(uint8_t state)
{
 
    static uint8_t sense;
	C1CON2 = 0x00; //比较控制寄存器2 --
  //  sense = intBEMF & 0x80;
   
	if(SENSE) sense =1; 
	else sense =0; 
    gPhase = state;
	switch(gPhase){

        case 0:  
              MOS_A_H	; // A+ C- "2"
             
             if(!intBEMF){  // BEMF "B"
				MOS_A_L =0;
                MOS_B_L =0 ;
					
                 MOS_C_L =1;   //下半周
                    
			        C1CON0 = 0x82;
		       
					gPhase++;
			  }
			  else{
                  MOS_A_L=0;
                  MOS_C_L=0; //turn off.
				  MOS_B_L =1; //B -
                 
			  }
			 
			  
			  
        break;

		case 1:
				   MOS_A_L =0;
                   MOS_C_L =0; //turn off C.
                   MOS_B_L =1; //A+ ,B- '6'
               
				if(intBEMF){ //BEMF "C"
				    MOS_A_H; //A+ 
                   
				    C1CON0 = 0x80;
		       
				    gPhase ++;


				}
				else{
                      MOS_C_H;  //C+ ;
                     

				}
        break;

		case 2:
				
				 MOS_C_H	; //C+,B- '4'
               
			    if(!intBEMF){  //BEMF "A"
                   MOS_C_L =0;
                   MOS_A_L =0;
				   MOS_B_L=1; //B-
                  
		           C1CON0 = 0x81;
		        
				    gPhase ++;

				}
				else {
                   
                    MOS_B_L=0;
                    MOS_C_L =0;
                     MOS_A_L=1 ; //A-
                   
				}

		break;

		case 3:
				 MOS_C_L =0;
                  MOS_B_L =0;
				   MOS_A_L =1	; //C+ A- "5"
                   
			   if(intBEMF){
                    MOS_C_H;  //C+ ;
                 
                    C1CON0 = 0x21;
		         
					gPhase ++ ;
                }
				else{
                    MOS_B_H ; //B+
                  
                }
				
				

		break;

		case 4:
			     MOS_B_H	;//B+ A- "1"
                
			    if(!intBEMF){
                    MOS_C_L=0;
                    MOS_B_L=0;
                    MOS_A_L=1; //A-
                    C1CON0 = 0x09;
		          
                    gPhase ++ ;

				}
				else {
                    MOS_A_L=0;
                    MOS_B_L =0 ;
                    MOS_C_L=1 ; //C-
                    
                }
				

		break;

		case 5: 
				    MOS_A_L=0;
                    MOS_B_L =0;
				    MOS_C_L=1	; //B+,C- "3"
         
                    if(intBEMF){

				      MOS_B_H; //B+ ;
              
				      C1CON0 = 0x81;
		            
					  gPhase =0;

				    }
					else{
                        MOS_A_H; // A+ ;
                       
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
			     
	          MOS_C_H	;//C+ A -  '5'  //MOS_U_W	;//A+ C- "2"
              MOS_C_L =0;
              MOS_B_L = 0 ;
              MOS_A_L = 1;
              delay_us(40);
	         
			  gPhase=1;
	          

	    break;
        
        
      #if 0  
        case 5:
			     
	           MOS_C_H	;//C+ A -  '5'  //MOS_U_W	;//A+ C- "2"
               MOS_C_L=0;
               MOS_B_L =0; 
               MOS_A_L=1;
                delay_us(40);
	        
			  gPhase=1;
	          

	    break;
#endif 
		case 1:
			    MOS_B_H	; //B+ A-  '1'//MOS_U_V	;  //A+ B-  "6"
                MOS_B_L =0;
                MOS_C_L =0;
                MOS_A_L =1;
                 delay_us(40);
			 
                 gPhase =3;
		break;

		case 3: 
			   MOS_B_H;//B+ C-   "3"//MOS_W_V	;//C+ B-  "4"
               MOS_B_L =0;
               MOS_A_L =0 ;//关闭 A 
               MOS_C_L =1;
               delay_us(40);
			  
		      gPhase =2 ;

		break ;

		case 2:
			   MOS_A_H;//A+ C- "2"//MOS_W_U	;//C+ A -  '5'
               MOS_A_L=0;
               MOS_B_L=0; //turn off B
               MOS_C_L =1;
               delay_us(40);
		      gPhase =6 ;

		break;

		case 6:
			 MOS_A_H	;  //A+ B-  "6"//MOS_V_U	; //B+ A-  '1'
             MOS_C_L =0; //关闭C
             MOS_A_L = 0; //关闭A
             MOS_B_L =1;
              delay_us(40);
		
			 gPhase =4 ;
			

		break;

		case 4:
			    MOS_C_H	;//C+ B-  "4"// MOS_V_W;//B+ C-   "3"
                MOS_C_L =0; //关闭C
                MOS_A_L = 0; //关闭A
			    MOS_B_L=1;
                  delay_us(40);
				
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
void OPEN(void)
{
	 PWMCON = 0X0A;
     PWMOE  = 0x07;
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


