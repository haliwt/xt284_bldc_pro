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
	//设置EPWM运行模式
	//EPWM_ConfigRunMode(EPWM_WFG_COMPLEMENTARY|EPWM_OC_INDEPENDENT|EPWM_OCU_SYMMETRIC|EPWM_COUNT_UP_DOWN); //互补模式+输出独立模式+对称计数模式+上下计数(中心对齐)模式
    PWMCON = 0x02;
	PWMOE  = 0x3F;
	//设置EPWM运行时钟	
	EPWM_ConfigChannelClk(EPWM0, EPWM_CLK_DIV_1);		
	EPWM_ConfigChannelClk(EPWM2, EPWM_CLK_DIV_1);
	EPWM_ConfigChannelClk(EPWM4, EPWM_CLK_DIV_1);
	//设置EPWM 周期以及占空比	
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

	//设置EPWM的加载方式为自动加载
	EPWM_EnableAutoLoadMode(EPWM_CH_0_MSK|EPWM_CH_1_MSK|EPWM_CH_2_MSK|EPWM_CH_3_MSK|EPWM_CH_4_MSK|EPWM_CH_5_MSK);
	//设置EPWM的输出极性
	EPWM_DisableReverseOutput(EPWM_CH_0_MSK|EPWM_CH_1_MSK|EPWM_CH_2_MSK|EPWM_CH_3_MSK|EPWM_CH_4_MSK|EPWM_CH_5_MSK); //关闭反向输出
	//设置EPWM输出
	EPWM_EnableOutput(EPWM_CH_0_MSK|EPWM_CH_1_MSK|EPWM_CH_2_MSK|EPWM_CH_3_MSK|EPWM_CH_4_MSK|EPWM_CH_5_MSK);		//开启输出
	//设置EPWM死区
	EPWM_DisableDeadZone(EPWM0);
	EPWM_DisableDeadZone(EPWM2);
	EPWM_DisableDeadZone(EPWM4);
	//设置EPWM中断
	EPWM_EnableZeroInt(EPWM_CH_0_MSK);				//开启EPWM零点中断
	EPWM_AllIntEnable();							//开启EPWM总中断
	IRQ_SET_PRIORITY(IRQ_PWM,IRQ_PRIORITY_HIGH);
	IRQ_ALL_ENABLE();								//开启总中断

	//设置EPWM输出IO复用
	GPIO_SET_MUX_MODE(P16CFG, GPIO_MUX_PG0); //UH
	GPIO_SET_MUX_MODE(P17CFG, GPIO_MUX_PG1); //UL
	
	GPIO_SET_MUX_MODE(P14CFG, GPIO_MUX_PG2); //VH
	GPIO_SET_MUX_MODE(P15CFG, GPIO_MUX_PG3); //VL  ---"VH_O"

	
	GPIO_SET_MUX_MODE(P00CFG, GPIO_MUX_PG4); //WH
	GPIO_SET_MUX_MODE(P13CFG, GPIO_MUX_PG5); //WL
	//开启EPWM
	EPWM_Start(EPWM_CH_4_MSK|EPWM_CH_2_MSK|EPWM_CH_0_MSK|EPWM_CH_1_MSK|EPWM_CH_3_MSK|EPWM_CH_5_MSK);
}