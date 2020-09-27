#include "motor.h"

uint8_t gPhase;
uint16_t zero[1];
uint8_t motor_step;
/*******************************************************************************
	*
	*Function Name: uint8_t NO_HallSensor_GetPinState(void)
	*Function :Dector no hall senseor BEMF
	*Input Ref:NO
	*Return Ref:No
	*
*******************************************************************************/
void OPEN3(void)
{

        static uint8_t state;
		state = 0;
		C1CON2 = 0x00; //???????2 --
		C1CON0 = 0x80; //???????0 --enable compare
		delay_us(20);//delay_us(20);
		if(C1CON1&0x80){state |= 0x01;} //U --BEMF
		C1CON0 = 0x81;
		delay_us(20);
		if(C1CON1&0x80){state |= 0x02;} //V ---BEMF
		C1CON0 = 0x82;
		delay_us(20);
		if(C1CON1&0x80){state |= 0x04;}//W  ---BEMF 
        
        if(state >6) state =4;
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
               MOS_A_L =0 ;//?? A 
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
             MOS_C_L =0; //??C
             MOS_A_L = 0; //??A
             MOS_B_L =1;
             delay_us(40);
              state =4;
			
			

		break;

		case 4:
			    MOS_C_H	;//C+ B-  "4"// MOS_V_W;//B+ C-   "3"
                MOS_C_L =0; //??C
                MOS_A_L = 0; //??A
			    MOS_B_L=1;
                delay_us(40);
                 state =5;
				

		break;
        
        default :
            
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
void BEMF_IOInit(void)
{
    GPIO_SET_MUX_MODE(P01CFG, GPIO_MUX_GPIO);		//����P01ΪGPIOģʽ
	GPIO_ENABLE_INPUT(P0TRIS, GPIO_PIN_1);			//����Ϊ����ģʽ
	GPIO_ENABLE_RD(P0RD, GPIO_PIN_1);	//��������

	GPIO_SET_MUX_MODE(P36CFG, GPIO_MUX_GPIO);		//����P36ΪGPIOģʽ
	GPIO_ENABLE_INPUT(P3TRIS, GPIO_PIN_6);			//����Ϊ���ģʽ
	

	
 
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
		C1CON2 = 0x00; //�ȽϿ��ƼĴ���2 --
		C1CON0 = 0x80; //�ȽϿ��ƼĴ���0 --enable compare
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
     
     
		
		C1CON2 = 0x00; //�ȽϿ��ƼĴ���2 --
		C1CON0 = 0x80; //�ȽϿ��ƼĴ���0 --enable compare
		//delay_us(20);
		if(C1CON1&0x80){motor_step |= 0x01;} //U --BEMF
		C1CON0 = 0x81;
		//delay_us(20);
		if(C1CON1&0x80){motor_step  |= 0x02;} //V ---BEMF
		//C1CON0 = 0x82;
		delay_us(20);
		if(C1CON1&0x80){motor_step  |= 0x04;}//W  ---BEMF 
        
        return motor_step ;
      
    
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
    uint16_t over_time,temp,temp2;
	uint8_t i;
    if(++over_time<5000)
	{
        static uint8_t state,BEMF_last,CW=0,CCW;
		uint8_t cwflg =0,EMI_count,motor_step;
		state = 0;
		
		C1CON2 = 0x00; //�ȽϿ��ƼĴ���2 --
		C1CON0 = 0x80; //�ȽϿ��ƼĴ���0 --enable compare
		delay_us(20);//delay_us(20);
		if(C1CON1&0x80){state |= 0x01;} //U --BEMF
		C1CON0 = 0x81;
		delay_us(20);
		if(C1CON1&0x80){state |= 0x02;} //V ---BEMF
		C1CON0 = 0x82;
		delay_us(20);
		if(C1CON1&0x80){state |= 0x04;}//W  ---BEMF 
        
       
		
		if(BEMF_last != state)
		{
			
			  temp = zero_time ;
		   //BLDC.zero_check_time.all -= (BLDC.zero_check_time.all>>3);
			if(state !=0 && state < 7){
				i++;
				temp2=temp;
				temp2 = ++temp2;
				
				zero[0] =temp2 /i;
				zero_time =0;
			}
			switch(state)//�ж���ε������λ
			{
				case 1:
					if(BEMF_last == 3)//�ж���ε����λ��ǰһ����λ��ǰһ����λ��3��˵����CW����
					{
						if(cwflg == CW)//ͨ��ǰ�����ε���λ���Ϳ���ȷ�������ת������
						{																	//������ͨ���жϵ�ǰ�ķ���������ʵ�ʷ����Ƿ�һ��
							EMI_count++;								//�Ӷ���֪�����λ�ü���Ƿ���ȷ
						}
						else//˵������ȷ
						{
							EMI_count = 0;//��������
							 cwflg = CW;
						}
					}
					if(BEMF_last == 5)//���ǰһ����λ��5��˵����CCW����
					{
						if(cwflg == CCW)
						{
							EMI_count++;
						}
						else
						{
						     EMI_count = 0;
							 cwflg = CCW;
						}
					}
					if(cwflg == CW){motor_step = 3;}
					else{motor_step = 4;}
					
					break;
				case 3:
					if(BEMF_last == 2)
					{
						if(cwflg == CW)
						{
							EMI_count++;
						}
						else
						{
							EMI_count = 0;
							cwflg= CW;
						}
					}
					if(BEMF_last == 1)
					{
						if(cwflg== CCW)
						{
							EMI_count++;
						}
						else
						{
							EMI_count = 0;
							cwflg = CCW;
						}
					}
					if(cwflg == CW){motor_step = 2;}
					else{motor_step = 5;}
					break;
				case 2:
					
					if(BEMF_last == 6)
					{
						if(cwflg == CW)
						{
							EMI_count++;
						}
						else
						{
							EMI_count = 0;
							cwflg = CW;
						}
					}
					if(BEMF_last == 3)
					{
						if(cwflg == CCW)
						{
							EMI_count++;
						}
						else
						{
							EMI_count = 0;
							cwflg = CCW;
						}
					}
					if(cwflg == CW){motor_step = 1;}
					else{motor_step = 0;}
					break;
				case 6:
					if(BEMF_last == 4)
					{
						if(cwflg == CW)
						{
							EMI_count++;
						}
						else
						{
							EMI_count = 0;
							cwflg = CW;
						}
					}
					if(BEMF_last == 2)
					{
						if(cwflg== CCW)
						{
							EMI_count++;
						}
						else
						{
							EMI_count = 0;
							cwflg = CCW;
						}
					}
					if(cwflg== CW){motor_step = 0;}
					else{motor_step = 1;}
					break;
				case 4:
					if(BEMF_last == 5)
					{
						if(cwflg == CW)
						{
							EMI_count++;
						}
						else
						{
							EMI_count = 0;
							cwflg= CW;
						}
					}
					if(BEMF_last == 6)
					{
						if(cwflg == CCW)
						{
							EMI_count++;
						}
						else
						{
							EMI_count = 0;
							cwflg = CCW;
						}
					}
					if(cwflg== CW){motor_step = 5;}
					else{motor_step = 2;}
					break;
				case 5:
					if(BEMF_last == 1)
					{
						if(cwflg== CW)
						{
							EMI_count++;
						}
						else
						{
							EMI_count = 0;
							cwflg= CW;
						}
					}
					if(BEMF_last == 4)
					{
						if(cwflg == CCW)
						{
							EMI_count++;
						}
						else
						{
							EMI_count = 0;
							cwflg = CCW;
						}
					}
					if(cwflg == CW){motor_step = 4;}
					else{motor_step = 3;}
					break;
				default:
					break;
			}
			BEMF_last = state;
		}
		
		if(EMI_count>=5)//����5�μ��λ����ȷ
		{
			EMI_count = 0;
			if(cwflg == 0)//ͨ��λ�ü���жϳ����ķ�������ʵ��ת������һ��
			{
				switch(motor_step)//����
				{
					case 5:
						MOS_C_H	;//C+ A -  '5'  //MOS_U_W	;//A+ C- "2"
					MOS_C_L =0;
					MOS_B_L = 0 ;
					MOS_A_L = 1;
				    delay_us(30);
				
					

				break;
		
				case 1:
						MOS_B_H	; //B+ A-  '1'//MOS_U_V	;  //A+ B-  "6"
						MOS_B_L =0;
						MOS_C_L =0;
						MOS_A_L =1;
					   delay_us(30);
						
					
				break;

				case 3: 
					MOS_B_H;//B+ C-   "3"//MOS_W_V	;//C+ B-  "4"
					MOS_B_L =0;
					MOS_A_L =0 ;//�ر� A 
					MOS_C_L =1;
					  delay_us(30);
						

				break ;

				case 2:
					MOS_A_H;//A+ C- "2"//MOS_W_U	;//C+ A -  '5'
					MOS_A_L=0;
					MOS_B_L=0; //turn off B
					MOS_C_L =1;
					  delay_us(30);
					

				break;

				case 6:
					MOS_A_H	;  //A+ B-  "6"//MOS_V_U	; //B+ A-  '1'
					MOS_C_L =0; //�ر�C
					MOS_A_L = 0; //�ر�A
					MOS_B_L =1;
				    delay_us(30);
					
					
					

				break;

				case 4:
						MOS_C_H	;//C+ B-  "4"// MOS_V_W;//B+ C-   "3"
						MOS_C_L =0; //�ر�C
						MOS_A_L = 0; //�ر�A
						MOS_B_L=1;
					     delay_us(30);
						
						

				break;
        
        default :
           
        break;
		
    
				}
				
			}
			
		}
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
	*Function :Dector no hall senseor BEMF 5-1-3-2-6-4 -��
	*Input Ref:NO
	*Return Ref:No
	*
*******************************************************************************/
void NO_HallSensor_DectorPhase(uint8_t state)
{
 
    static uint8_t phase=0;
	
    
 
	switch(phase){

        case 0:  
              MOS_A_H	; // A+ C- "2"
             
             if(!intBEMF){  // BEMF "B"  //������һ��λ��
                 delay_us(30);
                 if(!intBEMF){
                    MOS_A_L =0;
                    MOS_C_L =0 ;
                        
                     MOS_B_L =1;   //���� λ�� 6  A+ B-
                     phase ++ ; 
                 }   
		       
					
			  }
			  else{
                  MOS_A_L=0;
                  MOS_B_L=0; //turn off.
				  MOS_C_L =1; //C -   ""λ�� 2
                 
			  }
			 
			  
			  
        break;

		case 1:
				   MOS_A_L =0;
                   MOS_C_L =0; //turn off C.
                   MOS_B_L =1; //A+ ,B- '6'
               
				if(intBEMF){ //BEMF "C"
                     delay_us(30);
                    if(intBEMF){
                        MOS_C_H; //A+   // ����λ�� 4 C+ B-
                          phase ++ ; 
                    }
				}
				else{
                      MOS_A_H;  //A+ λ�� 6 A+ B-

				}
        break;

		case 2:
				
				 MOS_C_H	; //C+,B- '4'
               
			    if(!intBEMF){  //BEMF "A"
                    delay_us(30);
                    if(!intBEMF){
                       MOS_C_L =0;
                       MOS_B_L =0;
                       MOS_A_L=1; //B- //���� λ�� 5 C+ A-
                          phase ++ ; 
                    }
		          }
				else {
                   
                    MOS_C_L=0;
                    MOS_A_L =0;
                     MOS_B_L=1 ; //B-
                   
				}

		break;

		case 3:
				 MOS_C_L =0;
                  MOS_B_L =0;//turn off
				   MOS_A_L =1	; //C+ A- "5"
                   
			   if(intBEMF){
                    delay_us(30);
                   if(intBEMF){
                        MOS_B_H;  //B+ ;  //���� 1 B+ A- 
                         phase ++ ; 
                   }
                }
				else{
                    MOS_C_H ; //C+
                  
                }
				
				

		break;

		case 4:
			     MOS_B_H	;//B+ A- "1"
                
			    if(!intBEMF){
                    delay_us(30);
                    if(!intBEMF){
                        MOS_A_L=0;
                        MOS_B_L=0;
                        MOS_C_L=1 ; //C- ������һ��λ�á�1 --3 �� B+ C- 
                          phase ++ ; 
                    }
                
		        }
				else {
                    MOS_C_L=0;
                    MOS_B_L =0 ;
                    MOS_A_L=1 ; //A- //ͣ���ڴ�λ�� ��1��
                    
                }
				

		break;

          case 5	:
                    MOS_A_L=0;
                    MOS_B_L =0;
				    MOS_C_L=1	; //B+,C- "3"
         
                    if(intBEMF){
                        delay_us(30);
                        if(intBEMF){

				           MOS_A_H;//���� ��2��   λ��   A+ C-

                          phase =0 ; 
                        }
				  
		            }
					else{
                        MOS_B_H; // B+ ; λ�� 3 
                        
                    }

					
		

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
void InputValue_DectorPhase(uint8_t state)
{
 
	switch(motor_step){

        case 2:  
              //  MOS_A_H	; // A+ C- "2"
             
           
				MOS_A_L =0;
                MOS_B_L =0 ;
					
                 MOS_C_L =1;   //�°���
                 MOS_A_H	; // A+ C- "2"
            
			
			
			 
			  
			  
        break;

		case 6:
				
                  MOS_A_L =0;
                   MOS_C_L =0; //turn off C.
                   MOS_B_L =1; //A+ ,B- '6'
                   MOS_A_H; //A+ 
                 
				  
               
				
        break;

		case 4:
				
			
               
			   
                   MOS_C_L =0;
                   MOS_A_L =0;
				   MOS_B_L=1; //B-
				   MOS_C_H	; //C+,B- '4'
				   
                  
		         

		break;

		case 5:
				
                MOS_C_L =0;
                  MOS_B_L =0;
				   MOS_A_L =1	; //C+ A- "5"
				   MOS_C_H;  //C+ ;
                 
			  
				
				

		break;

		case 1:

                
			   
                    MOS_C_L=0;
                    MOS_B_L=0;
                    MOS_A_L=1; //A-
                    MOS_B_H	;//B+ A- "1"
                 
                
				

		break;

		case 3: 
				 
                     MOS_A_L=0;
                    MOS_B_L =0;
				    MOS_C_L=1	; //B+,C- "3"
				    MOS_B_H; //B+ ;
                 
                  

					
		

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
void NoSense_InterruptPhase(void)
{
  
    switch(motor_step){
     
        case 1:  
              
             
           
				MOS_A_L =0;
                MOS_B_L =0 ;
					
                 MOS_C_L =1;   //�°���
               MOS_A_H	; // A+ C- "2"
			      
			
			 
			  
			  
        break;

		case 2:
				 
                  MOS_A_L =0;
                   MOS_C_L =0; //turn off C.
                   MOS_B_L =1; //A+ ,B- '6'
                   MOS_A_H; //A+ 
              
				
        break;

		case 3:
				
				  MOS_C_L =0;
                   MOS_A_L =0;
				   MOS_B_L=1; //B-
                  MOS_C_H	; //C+,B- '4'
                
		         

		break;

		case 4:
				
                   MOS_C_L =0;
                  MOS_B_L =0;
				   MOS_A_L =1	; //C+ A- "5"
                  MOS_C_H;  //C+ ;
                   
			  
				
				

		break;

		case 5:
			
                
			   
                    MOS_C_L=0;
                    MOS_B_L=0;
                    MOS_A_L=1; //A-
                     MOS_B_H	;//B+ A- "1"
                
				

		break;

		case 6: 
				   
                  MOS_A_L=0;
                  MOS_B_L =0;
				  MOS_C_L=1	; //B+,C- "3"
                  MOS_B_H; //B+ ;
                    
                  gPhase =0;

					
		

		break;
     }
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


