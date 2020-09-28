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
             // state =1;
	          

	    break;
 
		case 1:
			    MOS_B_H	; //B+ A-  '1'//MOS_U_V	;  //A+ B-  "6"
                MOS_B_L =0;
                MOS_C_L =0;
                MOS_A_L =1;
               delay_us(40);
               //  state =3;
               
		break;

		case 3: 
			   MOS_B_H;//B+ C-   "3"//MOS_W_V	;//C+ B-  "4"
               MOS_B_L =0;
               MOS_A_L =0 ;//?? A 
               MOS_C_L =1;
               delay_us(40);
              //   state =2;
		     

		break ;

		case 2:
			   MOS_A_H;//A+ C- "2"//MOS_W_U	;//C+ A -  '5'
               MOS_A_L=0;
               MOS_B_L=0; //turn off B
               MOS_C_L =1;
                delay_us(40);
             //   state =6;
		    

		break;

		case 6:
			 MOS_A_H	;  //A+ B-  "6"//MOS_V_U	; //B+ A-  '1'
             MOS_C_L =0; //??C
             MOS_A_L = 0; //??A
             MOS_B_L =1;
             delay_us(40);
            //  state =4;
			
			

		break;

		case 4:
			    MOS_C_H	;//C+ B-  "4"// MOS_V_W;//B+ C-   "3"
                MOS_C_L =0; //??C
                MOS_A_L = 0; //??A
			    MOS_B_L=1;
                delay_us(40);
              //   state =5;
				

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
		} //W --BEMF
		C1CON0 = 0x81;
		delay_us(20);
		if(C1CON1&0x80){
			State |= 0x02;
		} //V ---BEMF
		C1CON0 = 0x82;
		delay_us(20);
		if(C1CON1&0x80){
			State |= 0x04;
		}//U  ---BEMF 

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
     
        C1CON2 = 0x00; //比较控制寄存器2 --
        
		C1CON0 = 0x80; //P04 C1P0  数据手册查--BEMF"Ew"
		delay_us(20); 
		if(C1CON1&0x80){motor_step |= 0x01;} 
		
		C1CON0 = 0x81; //C1P1 P03 ="Ev"
		delay_us(20);
		if(C1CON1&0x80){motor_step  |= 0x02;} //P ---BEMF
		
		C1CON0 = 0x82;//C1P2 P02 ="Eu"
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
        
       
		
		if(BEMF_last != state)
		{
			
			  temp = zero_time ;
		   //BLDC.zero_check_time.all -= (BLDC.zero_check_time.all>>3);
			if(state !=0 && state < 7){
				i++;
				temp2=temp;
				temp2 = ++temp2;
				
				zero[0] =temp2 /i; //换向时间
				zero_time =0;
			}
			switch(state)//判断这次电机的相位
			{
				case 1:
					if(BEMF_last == 3)//判断这次电机相位的前一个相位，前一个相位是3，说明是CW方向
					{
						if(cwflg == CW)//通过前后两次的相位，就可以确定电机的转动方向，
						{																	//在这里通过判断当前的方向与电机的实际方向是否一致
							EMI_count++;								//从而就知道电机位置检测是否正确
						}
						else//说明不正确
						{
							EMI_count = 0;//计数清零
							 cwflg = CW;
						}
					}
					if(BEMF_last == 5)//如果前一个相位是5，说明是CCW方向
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
		
		if(EMI_count>=5)//连续5次检测位置正确
		{
			EMI_count = 0;
			if(cwflg == 0)//通过位置检测判断出来的方向与电机实际转动方向一致
			{
				switch(motor_step)//换向
				{
					case 5:
						MOS_C_H	;//C+ A -  '5'  //MOS_U_W	;//A+ C- "2"
					MOS_C_L =0;
					MOS_B_L = 0 ;
					MOS_A_L = 1;
				    delay_us(20);
				
					

				break;
		
				case 1:
						MOS_B_H	; //B+ A-  '1'//MOS_U_V	;  //A+ B-  "6"
						MOS_B_L =0;
						MOS_C_L =0;
						MOS_A_L =1;
					   delay_us(20);
						
					
				break;

				case 3: 
					MOS_B_H;//B+ C-   "3"//MOS_W_V	;//C+ B-  "4"
					MOS_B_L =0;
					MOS_A_L =0 ;//关闭 A 
					MOS_C_L =1;
					  delay_us(20);
						

				break ;

				case 2:
					MOS_A_H;//A+ C- "2"//MOS_W_U	;//C+ A -  '5'
					MOS_A_L=0;
					MOS_B_L=0; //turn off B
					MOS_C_L =1;
					  delay_us(20);
					

				break;

				case 6:
					MOS_A_H	;  //A+ B-  "6"//MOS_V_U	; //B+ A-  '1'
					MOS_C_L =0; //关闭C
					MOS_A_L = 0; //关闭A
					MOS_B_L =1;
				    delay_us(20);
					
					
					

				break;

				case 4:
						MOS_C_H	;//C+ B-  "4"// MOS_V_W;//B+ C-   "3"
						MOS_C_L =0; //关闭C
						MOS_A_L = 0; //关闭A
						MOS_B_L=1;
					     delay_us(20);
						
						

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
	*Function :Dector no hall senseor BEMF 5-1-3-2-6-4 -》
	*Input Ref:NO
	*Return Ref:No
	*
*******************************************************************************/
void NOSense_CCW_DectorPhase(void)
{
 
   if(gPhase >5)gPhase=0;
	
    switch(gPhase){

        case 0:  
                // AB --查看“Ev”  AB ->AC->BC->BA->CA->CB(循环)
              MOS_A_H;
             
             if(intBEMF & 0x80 ==0){  // BEMF "B"  //换向到下一个位置 Ev=C1P1
              
                      MOS_A_L =0;
	                  MOS_B_L =0; 
	                  MOS_C_L =1; // C-  
                     C1CON2 =0x00;
                     C1CON0 = 0x82;
                   
                     gPhase ++ ;  
		       
					
			  }
			  else{
                  MOS_A_L =0;
	               MOS_C_L =0; 
	               MOS_B_L =1; // B-  
                 
			  }
			 
			  
			  
        break;

		case 1:     //AC -> A+ ,B- '6' --查看“Ew”
				      MOS_A_L =0;
	                  MOS_B_L =0; 
	                  MOS_C_L =1; // C-  
               
				if(intBEMF & 0x80 ==1){ //BEMF "C" Ew C1P0
                   
	                   MOS_B_H; //B+  //BC
	                   
	                 C1CON2 =0x00; 
                     C1CON0 = 0x80;
                         gPhase ++ ; 
                    
				}
				else{
                      
                  MOS_A_H;

				}
        break;

		case 2:   // BC
				 MOS_B_H;
			
                if(intBEMF & 0x80 ==0){  //查看 BEMF "Eu" C1P2
                    
                         MOS_C_L =0;
                           MOS_B_L =0; 
                          MOS_A_L =1; // A-
                         C1CON2 =0x00;
                         C1CON0 = 0x81;
                          gPhase ++ ; 
                    
		          }
				else {
                   
                    MOS_A_L =0;
	               MOS_B_L =0; 
	               MOS_C_L =1; // C-
                   
				}

		break;

		case 3:   //BA
				 MOS_B_L =0;
	               MOS_C_L =0; 
	               MOS_A_L =1; // A-   //当前通电C+ B-
                   
			   if(intBEMF & 0x80 ==1){  //查看反向电动势 "Ev"C1P1
                  
                    MOS_C_H; //C+
	                C1CON2 =0x00;
                    C1CON0 = 0x82;  //Eu
                       gPhase ++ ; 
                   
                }
				else{
                   MOS_B_H ;
	              
                }
				
				

		break;

		case 4:   //CA 
             MOS_C_H;  
			     
              if(intBEMF & 0x80 ==0){   //Ev
                   
                        MOS_B_L =0;
                       MOS_A_L =0; 
                       MOS_B_L =1; // B-  
					     C1CON2 =0x00;
                         C1CON0 = 0x80;  //Ew
                          gPhase ++ ; 
                    
                
		        }
				else {
                   
                   MOS_B_L =0;
	               MOS_C_L =0; 
	               MOS_A_L =1; // A-  
                }
				

		break;

          case 5 :  //CB
                    
                      MOS_B_L =0;
	                    MOS_A_L =0; 
	                    MOS_B_L =1; 
                    if(intBEMF & 0x80 ==1){  //查开 “Ew”
                       
                         MOS_A_H;
				         
	                     C1CON2 =0x00;
                         C1CON0 = 0x81;  //Eu

                         gPhase =0 ; 
                        
				  
		            }
					else{
                      MOS_C_H;  
                       
                    }

					
		

		break;






	}


}

/*******************************************************************************
	*
	*Function Name: uint8_t NO_HallSensor_GetPinState(void)
	*Function :Dector no hall senseor BEMF 5-1-3-2-6-4 -》
	*Input Ref:NO
	*Return Ref:No
	*
*******************************************************************************/
void NO_HallSensor_DectorPhase(void)
{

   uint8_t sense =0 ;
  
   

  

    if(intBEMF)sense =1;
      else sense =0;
	
    switch(gPhase){

        case 0:  
                // BC- "2" --查看“Ev”  AB ->CB->CA->BA->BC->AC->AB(循环)
               MOS_A_L=0;
               MOS_B_L=0; //turn off.
			   MOS_C_L =1; //C -  当前是
             
             if(intBEMF & 0x80 ==1){  // BEMF "B"  //换向到下一个位置 Ev=C1P1
              
                     MOS_A_H;    //换向到 位置 6  A+ B-   from BC to AC
                     delay_us(30);
                     C1CON2 =0x00;
                     C1CON0 = 0x81;
                   
                     gPhase ++ ;  
		       
					
			  }
			  else{
                  MOS_B_H;    //当前通电是B+ C- 
                 
			  }
			 
			  
			  
        break;

		case 1:     //AC -> A+ ,B- '6' --查看“Ew”
				    MOS_A_H;
               
				if(intBEMF & 0x80 ==0){ //BEMF "C" Ew C1P0
                   
	                   MOS_A_L =0;
	                   MOS_C_L =0; 
	                   MOS_B_L =1; // B-  
	                     delay_us(30);
	                 C1CON2 =0x00;
                     C1CON0 = 0x80;
                         gPhase ++ ; 
                    
				}
				else{
                      
                   MOS_A_L =0;
                   MOS_B_L =0; 
                   MOS_C_L =1; //当前通电是A+ C- 

				}
        break;

		case 2:   // AB 
				  MOS_A_L =0;
	               MOS_C_L =0; 
	               MOS_B_L =1; // B-  
			
                if(intBEMF & 0x80 ==1){  //查看 BEMF "Eu" C1P2
                    
                         MOS_C_H;  //C+  //换向到CB
                           delay_us(30);
                         C1CON2 =0x00;
                         C1CON0 = 0x82;
                          gPhase ++ ; 
                    
		          }
				else {
                   
                     MOS_A_H;  //当前通电是 A+ B- 
                   
				}

		break;

		case 3:   //CB 
				 MOS_C_H;  
                   
			   if(intBEMF & 0x80 ==0){  //查看反向电动势 "Ev"C1P1
                  
                    MOS_B_L =0;
	               MOS_C_L =0; 
	               MOS_A_L =1; // A- 
	                 delay_us(30);
	                C1CON2 =0x00;
                    C1CON0 = 0x81;  //Eu
                       gPhase ++ ; 
                   
                }
				else{
                   MOS_A_L =0;
	               MOS_C_L =0; 
	               MOS_B_L =1; // B-   //当前通电C+ B-
                  
                }
				
				

		break;

		case 4:   //CA 
			      MOS_B_L =0;
	               MOS_C_L =0; 
	               MOS_A_L =1; // A-   //当前通电C+ B-
                
			    if(intBEMF & 0x80 ==1){   //Ev
                   
                        MOS_B_H ; //BA 
                         delay_us(30);
					     C1CON2 =0x00;
                         C1CON0 = 0x80;  //Ew
                          gPhase ++ ; 
                    
                
		        }
				else {
                    MOS_C_H;  
                    
                }
				

		break;

          case 5 :  //BA
                     MOS_B_H ; //BA 
         
                    if(intBEMF & 0x80 ==0){  //查开 “Ew”
                       

				          MOS_B_L =0;
	                      MOS_A_L =0; 
	                      MOS_C_L =1; // BC
	                        delay_us(30);
	                     C1CON2 =0x00;
                         C1CON0 = 0x82;  //Eu

                         gPhase =0 ; 
                        
				  
		            }
					else{
                        MOS_B_L =0;
	                    MOS_C_L =0; 
	                    MOS_A_L =1; // BA 当前通电是BA
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
 
	
    	C1CON2 = 0x00; //比较控制寄存器2 --
		C1CON0 = 0x80; //比较控制寄存器0 --enable compare
		delay_us(20);//delay_us(20);
		if(C1CON1&0x80){motor_step |= 0x01;} //W --BEMF C
		C1CON0 = 0x81;
		delay_us(20);
		if(C1CON1&0x80){motor_step |= 0x02;} //V ---BEMF B
		C1CON0 = 0x82;
		delay_us(20);
		if(C1CON1&0x80){motor_step |= 0x04;}//U  ---BEMF A
    if(motor_step >6)motor_step =2;
    switch(motor_step){

        case 2:  
              // A+ C- "2"
             
           
				MOS_A_L =0;
                MOS_B_L =0 ;
					
                 MOS_C_L =1;   //下半周
                 MOS_A_H	; // A+ C- "2"
            
			delay_us(20);
			
			 
			  
			  
        break;

		case 6:
				
                  MOS_A_L =0;
                   MOS_C_L =0; //turn off C.
                   MOS_B_L =1; //A+ ,B- '6'
                   MOS_A_H; //A+ 
                 
				  delay_us(20);
               
				
        break;

		case 4:
				
			
               
			   
                   MOS_C_L =0;
                   MOS_A_L =0;
				   MOS_B_L=1; //B-
				   MOS_C_H	; //C+,B- '4'
				   delay_us(20);
                  
		         

		break;

		case 5:
				
                MOS_C_L =0;
                  MOS_B_L =0;
				   MOS_A_L =1	; //C+ A- "5"
				   MOS_C_H;  //C+ ;
                 
			  delay_us(20);
				
				

		break;

		case 1:

                
			   
                    MOS_C_L=0;
                    MOS_B_L=0;
                    MOS_A_L=1; //A-
                    MOS_B_H	;//B+ A- "1"
                 
                delay_us(20);
				

		break;

		case 3: 
				 
                     MOS_A_L=0;
                    MOS_B_L =0;
				    MOS_C_L=1	; //B+,C- "3"
				    MOS_B_H; //B+ ;
                 
                  delay_us(20);

					
		

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
					
                 MOS_C_L =1;   //下半周
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


