#include "pwm.h"

/******************************************************************************
** \brief	 EPWM_Config (complementary, no dead time)
** \param [in] 
**            	
** \return  none
** \note  
******************************************************************************/
void EPWM_Config(void)
{
	//����EPWM����ģʽ
	//EPWM_ConfigRunMode(EPWM_WFG_COMPLEMENTARY|EPWM_OC_INDEPENDENT|EPWM_OCU_SYMMETRIC|EPWM_COUNT_UP_DOWN); //����ģʽ+�������ģʽ+�ԳƼ���ģʽ+���¼���(���Ķ���)ģʽ
    PWMCON = 0x02;
	PWMOE  = 0x3F;
	//����EPWM����ʱ��	
	EPWM_ConfigChannelClk(EPWM0, EPWM_CLK_DIV_1);		
	EPWM_ConfigChannelClk(EPWM2, EPWM_CLK_DIV_1);
	EPWM_ConfigChannelClk(EPWM4, EPWM_CLK_DIV_1);
	//����EPWM �����Լ�ռ�ձ�	
	//EPWM_ConfigChannelPeriod(EPWM0, 0x12C0); //Tpmw = PWMDn *2 * 1/8(us)=1.2ms 
   // EPWM_ConfigChannelPeriod(EPWM0, 0x00FA);  //Tpmw = 1/16(KHz) 16KHZ = PWMDn =250 =0xFA
    EPWM_ConfigChannelPeriod(EPWM0, 0x0190);  //Tpmw = 1/10(KHz) 16KHZ = PWMDn = 0x190
	EPWM_ConfigChannelPeriod(EPWM2, 0x0190);
	EPWM_ConfigChannelPeriod(EPWM4, 0x0190);

	#if (1)
	//EPWM_ConfigChannelSymDuty(EPWM0, 0x0960);//
	EPWM_ConfigChannelSymDuty(EPWM0, 0x00c8); // 50%
	EPWM_ConfigChannelSymDuty(EPWM2, 0x00c8);
	EPWM_ConfigChannelSymDuty(EPWM4, 0x00c8);
	#elif (0)
	EPWM_ConfigChannelSymDuty(EPWM0, 0);
	EPWM_ConfigChannelSymDuty(EPWM2, 0);
	EPWM_ConfigChannelSymDuty(EPWM4, 0);
	#else 
	EPWM_ConfigChannelSymDuty(EPWM0, 0xFFFF);
	EPWM_ConfigChannelSymDuty(EPWM2, 0xFFFF);
	EPWM_ConfigChannelSymDuty(EPWM4, 0xFFFF);
	#endif

	//����EPWM�ļ��ط�ʽΪ�Զ�����
	EPWM_EnableAutoLoadMode(EPWM_CH_0_MSK|EPWM_CH_1_MSK|EPWM_CH_2_MSK|EPWM_CH_3_MSK|EPWM_CH_4_MSK|EPWM_CH_5_MSK);
	//����EPWM���������
	EPWM_DisableReverseOutput(EPWM_CH_0_MSK|EPWM_CH_1_MSK|EPWM_CH_2_MSK|EPWM_CH_3_MSK|EPWM_CH_4_MSK|EPWM_CH_5_MSK); //�رշ������
	//����EPWM���
	EPWM_EnableOutput(EPWM_CH_0_MSK|EPWM_CH_1_MSK|EPWM_CH_2_MSK|EPWM_CH_3_MSK|EPWM_CH_4_MSK|EPWM_CH_5_MSK);		//�������
	//����EPWM����
	EPWM_DisableDeadZone(EPWM0);
	EPWM_DisableDeadZone(EPWM2);
	EPWM_DisableDeadZone(EPWM4);
	//����EPWM�ж�
	EPWM_EnableZeroInt(EPWM_CH_0_MSK);				//����EPWM����ж�
	EPWM_AllIntEnable();							//����EPWM���ж�
	IRQ_SET_PRIORITY(IRQ_PWM,IRQ_PRIORITY_HIGH);
	IRQ_ALL_ENABLE();								//�������ж�

	//����EPWM���IO����
	GPIO_SET_MUX_MODE(P16CFG, GPIO_MUX_PG0); //UH
	GPIO_SET_MUX_MODE(P17CFG, GPIO_MUX_PG1); //UL
	
	GPIO_SET_MUX_MODE(P14CFG, GPIO_MUX_PG2); //VH
	GPIO_SET_MUX_MODE(P15CFG, GPIO_MUX_PG3); //VL  ---"VH_O"

	
	GPIO_SET_MUX_MODE(P00CFG, GPIO_MUX_PG4); //WH
	GPIO_SET_MUX_MODE(P13CFG, GPIO_MUX_PG5); //WL
	//����EPWM
	EPWM_Start(EPWM_CH_4_MSK|EPWM_CH_2_MSK|EPWM_CH_0_MSK|EPWM_CH_1_MSK|EPWM_CH_3_MSK|EPWM_CH_5_MSK);
}