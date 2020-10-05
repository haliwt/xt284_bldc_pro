#include<include.h>

/*
void  clk_init(void)
{
	TA = 0xAA;
	TA = 0x55;
	CLKDIV = 0x00;
	//CKCON = 0x00;
}
*/
void	gpio_init(void)
{
	P0TRIS = 0x03;  //方向
	P0OD   = 0x00;  //开漏
	P0RD   = 0x00;  //下拉
	P0UP   = 0x00;  //上拉
	P0DR   = 0x00;  //驱动
	P0SR   = 0x00;  //斜率
	P0DS   = 0x00;  //输入模式
	P0     = 0x3C;  //输出
	
	P1TRIS = 0xF8;  //方向
	P1OD   = 0x00;  //开漏
	P1RD   = 0x00;  //下拉
	P1UP   = 0x00;  //上拉
	P1DR   = 0x00;  //驱动
	P1SR   = 0x00;  //斜率
	P1DS   = 0x00;  //输入模式
	P1     = 0x00;  //输出
	
	P2TRIS = 0x0C;  //方向
	P2OD   = 0x04;  //开漏
	P2RD   = 0x00;  //下拉
	P2UP   = 0x08;  //上拉
	P2DR   = 0x00;  //驱动
	P2SR   = 0x00;  //斜率
	P2DS   = 0x0C;  //输入模式
	P2     = 0x7C;  //输出
	
	P3TRIS = 0x00;  //方向
	P3OD   = 0x00;  //开漏
	P3RD   = 0x00;  //下拉
	P3UP   = 0x00;  //上拉
	P3DR   = 0x00;  //驱动
	P3SR   = 0x00;  //斜率
	P3DS   = 0x00;  //输入模式
	P3     = 0x00;  //输出
	
	P31EICFG = 0x00;
	P32EICFG = 0x00;
	P36EICFG = 0x00;

	//复用选择
	P00CFG = 0x12;
	P01CFG = 0x00;
	P02CFG = 0x01;//Ew
	P03CFG = 0x01;//Ev
	P04CFG = 0x01;//Eu
	P05CFG = 0x01;//COM
	
	P13CFG = 0x13;
	P14CFG = 0x14;
	P15CFG = 0x15;//
	P16CFG = 0x16;//
	P17CFG = 0x17;//
	
	P21CFG = 0x00;
	P22CFG = 0x00;//
	P23CFG = 0x00;//
	P24CFG = 0x00;//
	P25CFG = 0x00;//
	P26CFG = 0x01;//Vin
	
	P30CFG = 0x01;//OP_OUT
	P31CFG = 0x01;//OP_N
	P32CFG = 0x01;//OP_P
	P35CFG = 0x00;
	P36CFG = 0x00;
	
	PS_INT0  = 0x3f;
	PS_INT1  = 0x3f;
	PS_T0    = 0x3f;
	PS_T0G   = 0x3f;
	PS_T1    = 0x3f;
	PS_T1G   = 0x3f;
	PS_T2    = 0x3f;
	PS_T2EX  = 0x3f;
	PS_T0    = 0x3f;
	PS_CAP0  = 0x3f;
	PS_CAP1  = 0x3f;
	PS_CAP2  = 0x3f;
	PS_CAP3  = 0x3f;
	PS_ADET  = 0x3f;
	PS_FB    = 0x3f;
}

void	pwm_init(void)
{
	PWMCON = 0x0A;
	PWMOE  = 0x3F;
	
	PWM01PSC = 0x01;//分频
	PWM23PSC = 0x01;
	PWM45PSC = 0x01;
	PWM0DIV  = 0x05;//时钟
	PWM1DIV  = 0x05;
	PWM2DIV  = 0x05;
	PWM3DIV  = 0x05;
	PWM4DIV  = 0x05;
	PWM5DIV  = 0x05;
	
	PWMPINV =   0x00;
	PWMCNTM =   0x3F; //自动加载模式
	PWMCNTE =   0x3F;  //PWM start ,output
	PWMCNTCLR = 0x3F; //PWM计数器清零
	
	PWMP0L = 0x20; //周期值
	PWMP0H = 0x03;
	PWMP1L = 0x20;
	PWMP1H = 0x03;
	
//	PWMP0L = 0x60; //周期值
//	PWMP0H = 0x09;
//	PWMP1L = 0x60;
//	PWMP1H = 0x09;



	
	PWMP2L = 0x00;
	PWMP2H = 0x00;
	PWMP3L = 0x00;
	PWMP3H = 0x00;
	PWMP4L = 0x00;
	PWMP4H = 0x00;
	PWMP5L = 0x00;
	PWMP5H = 0x00;
	
	PWMD0L = 0x00;//比较值
	PWMD0H = 0x00;
	PWMD1L = 0x20;
	PWMD1H = 0x03;

//	PWMD0L = 0x00;//比较值 。WT.EDIT
//	PWMD0H = 0x00;
//	PWMD1L = 0x60;
//	PWMD1H = 0x09;
//	
	PWMD2L = 0x00;
	PWMD2H = 0x00;
	PWMD3L = 0x00;
	PWMD3H = 0x00;
	PWMD4L = 0x00;
	PWMD4H = 0x00;
	PWMD5L = 0x00;
	PWMD5H = 0x00;
	
	PWMDD0L = 0x00;//向下比较值
	PWMDD0H = 0x00;
	PWMDD1L = 0x00;
	PWMDD1H = 0x00;
	PWMDD2L = 0x00;
	PWMDD2H = 0x00;
	PWMDD3L = 0x00;
	PWMDD3H = 0x00;
	PWMDD4L = 0x00;
	PWMDD4H = 0x00;
	PWMDD5L = 0x00;
	PWMDD5H = 0x00;
	
	PWMLOADEN = 0x3F;
	
	
	PWMDTE =  0x00; //死区
	PWM01DT = 0x00;
	PWM23DT = 0x00;
	PWM45DT = 0x00;

//	PWMDTE =  0x03F; //死区
//	PWM01DT = 0xC8;  //200 WT.EDIT
//	PWM23DT = 0x00;
//	PWM45DT = 0x00;

	
	
	PWMMASKE = 0x3F; //掩码
	PWMMASKD = 0x00;
	PWMFBKD = 0x2A;
	PWMFBKC = 0x00; //刹车
	
	PWMPIE = 0x01;  //周期中断
	PWMZIE = 0x00;  //零点中断
	PWMUIE = 0x00;  //向上比较中断
	PWMDIE = 0x00;  //向下比较中断
	
	PWMPIF = 0x00;  //中断标志
	PWMZIF = 0x00;
	PWMUIF = 0x00;
	PWMDIF = 0x00;
}

static void ACMP_Dealy(void)
{
	volatile uint8_t i;
	for(i=0;i<50;i++)
		_nop_();
}

void	vc_init(void)
{
	uint8_t  temp,out,Adjvalue;
	
	
	C0CON0 = 0x00;//过流保护
	C0CON1 = 0x00;
	C0CON2 = 0x00;
	C0ADJE = 0x00;
	C0HYS  = 0x00;//迟滞
	
	
	C1CON0 = 0x60;
	C1ADJE = 0xAA;     //使用C0ADJE中的数据
	C1CON1 &= ~0x5f;
	
	C1CON0 |= 0x80;
	ACMP_Dealy();
	out = C1CON1&0x80;
	for(temp=0; temp<0x20;temp++)
	{
		C1CON1 = temp;
		ACMP_Dealy();
		if(out!= (C1CON1&0x80))
		{
			Adjvalue = temp;
			break; 
		}
	}
	
	C1CON0 =  0x00;//反电动势
	C1CON1 =  Adjvalue;
	C1CON2 =  0x00;
	C1HYS  =  0x00;//迟滞
	C1ADJE  = 0xAA;
	//Adjvalue = C1CON1;
	CNIE    = 0x00;
	CNIF    = 0x00;
	CNVRCON = 0x00;
	CNFBCON = 0x00;//刹车控制
	
	C0CON0  = 0x00;//过流保护
	
	C1CON0 |= 0x80;//反电动势
}
	
void	op_init(void)
{
	OP0CON0 = 0xB0;//电流
	OP0CON1 = 0x00;
	OP0ADJE = 0x00;
	
	OP1CON0 = 0x00;//未使用
	OP1CON1 = 0x00;
	OP1ADJE = 0x00;
}


void	adc_init(void)
{
	ADCON0 = 0x48; //0B0100 1000  //OP0_O
	ADCON1 = 0x5B; //
	ADCON2 = 0x00;
	ADCMPC = 0x00;
	ADDLYL = 0x00;
	ADRESH = 0x00;
	ADRESL = 0x00;
	ADCMPH = 0x00;
	ADCMPL = 0x00;
	ADCLDO = 0x00;//参考电压1.2V采样电流
	ADCON1 |= 0x80;
}


void	time01_init(void)
{
	TMOD  = 0x11;
	TCON  = 0x50;
//	TL0   = 0x00;
//	TH0   = 0x00;
	CKCON = 0x00; //Fsys =48MHz  T0 = 1/（48/12） =1/4MHZ = 0.25us 65536 *0.25 =16ms

	TL0 = 0x5f;  //1ms //WT.EDIT
	TH0 = 0xf0;
}

void	time2_init(void)  //pwm采样，中断最高优先级
{
	T2CON  = 0x00;
	TL2    = 0x00;
	TH2    = 0x00;
	RLDL   = 0x00;
	RLDH   = 0x00;
	CCL1   = 0x00;
	CCH1   = 0x00;
	CCL2   = 0x00;
	CCH2   = 0x00;
	CCL3   = 0x00;
	CCH3   = 0x00;
	CCEN   = 0x00;
}

void	time34_init(void)
{
	T34MOD = 0x99;
	TL3    = 0x5F;
	TH3    = 0xF0;
	TL4    = 0x00;
	TH4    = 0x00;
}

void	interrupt_init(void)
{
	IE      = 0x00;
	T2IE    = 0x00;
	P0EXTIE = 0x00;
	P1EXTIE = 0x00;
	P2EXTIE = 0x00;
	P3EXTIE = 0x00;
	EIE2    = 0x00;
	IP      = 0x00;
	EIP1    = 0x00;
	EIP2    = 0x00;
}


void	hardware_init(void)
{
	EA = 0;
	//clk_init();
	gpio_init();
	pwm_init();
	vc_init();
	op_init();
	adc_init();
	time01_init();
	time2_init();
	time34_init();
	interrupt_init();
	EA = 1;
}


