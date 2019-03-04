#include "servo.h"
#include "delay.h"

/*
*  Servo1 :  TIM1CH1 --> PA8
*  Servo2 :  TIM1CH2 --> PA9
*  Servo3 :  TIM1CH3 --> PA10
*  Servo4 :  TIM1CH4 --> PA11
*  Servo5 :  TIM9CH1 --> PA2
*/

/********************************参数定义*************************************/
static int servo1_last_angle = 0;
static int servo2_last_angle = 0;
static int servo3_last_angle = 0;
static int servo4_last_angle = 0;
static int servo5_last_angle = 0;
/*****************************************************************************/

/*
* 函数功能：舵机PWM初始化
* 函数说明：arr = 2000-1    psc = 840-1        PWM频率为50Hz
*/
void servo_Init(u16 arr,u16 psc)
{
	/* 结构体初始化 */
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	/* 总线使能 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//TIM1 时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);//TIM9 时钟使能
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能 PORTA 时钟
	/*IO 复用*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8,  GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_TIM1);   
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);   	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_TIM9);  
	/* IO 初始化 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11; //PA2 --> PA11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2 --> PA11
	/* 定时器初始化 */
	TIM_TimeBaseStructure.TIM_Prescaler=psc; //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr; //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器TIM1
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure);//初始化定时器TIM9
	
	TIM_OCStructInit(& TIM_OCInitStructure);                       //恢复初始
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //定时器模式为pwm模式1
  TIM_OCInitStructure.TIM_Pulse =0;                              //脉冲值，即输出都是低电平
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      //极性为高
	/*初始化定时器各个通道*/
	/*TIM1CH1*/
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);     
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	/*TIM1CH2*/
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	/*TIM1CH3*/
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	/*TIM1CH4*/
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	/*TIM9CH1*/
	TIM_OC1Init(TIM9, &TIM_OCInitStructure);     
  TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);
	
	/*使能定时器*/
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	/*使能定时器PWM输出模式*/
	TIM_Cmd(TIM9, ENABLE);
  TIM_CtrlPWMOutputs(TIM9, ENABLE);
}

/*
* 函数功能：1号舵机角度输出
* 函数说明：角度范围： 100打开最大角度 --- 160 完全闭合 
*/
void Servo1_SetAngle(int angle)
{
	int pwm_value;
	//if(angle<100||angle>160)angle=100;
	pwm_value = 5*angle+300;
	TIM_SetCompare1(TIM1,pwm_value);
}

/*
* 函数功能：2号舵机角度输出
* 函数说明：角度范围：0 -- 180
*/
void Servo2_SetAngle(int angle)
{
	int pwm_value;
	//if(angle<0||angle>180)angle=20;
	pwm_value = 5*angle+300;
	TIM_SetCompare2(TIM1,pwm_value);
}

/*
* 函数功能：3号舵机角度输出
* 函数说明：角度范围：70 -- 180
*/
void Servo3_SetAngle(int angle)
{
	int pwm_value;
	//if(angle<70||angle>180)angle=80;
	pwm_value = 5*angle+300;
	TIM_SetCompare3(TIM1,pwm_value);
}

/*
* 函数功能：4号舵机角度输出
* 函数说明：角度范围：0 -- 150
*/
void Servo4_SetAngle(int angle)
{
	int pwm_value;
	//if(angle<0||angle>150)angle=0;
	pwm_value = 5*angle+300;
	TIM_SetCompare4(TIM1,pwm_value);
}

/*
* 函数功能：5号舵机角度输出
* 函数说明：角度范围：0 -- 180
*/
void Servo5_SetAngle(int angle)
{
	int pwm_value;
	//if(angle<40||angle>120)angle=80;
	pwm_value = 5*angle+300;
	TIM_SetCompare1(TIM9,pwm_value);
}



/*
* 函数功能：一号舵机 角度输出 插值细分
* 函数说明：有效降低舵机转速，增加稳定性
*/
void Servo1_SpeedRegulation(int next_angle)
{
	int N;
	int i;
	N  = abs(next_angle - servo1_last_angle);
	if (servo1_last_angle < next_angle)
	{
		for(i = 0; i < N; i++)
		{
			servo1_last_angle += 1;
			Servo1_SetAngle(servo1_last_angle);
			delay_ms(20);
		}
	}
	if(servo1_last_angle > next_angle)
	{
		for(i = 0; i < N; i++)
		{
			servo1_last_angle -= 1;
			Servo1_SetAngle(servo1_last_angle);
			delay_ms(20);
		}
	}
}


/*
* 函数功能：二号舵机 角度输出 插值细分
* 函数说明：有效降低舵机转速，增加稳定性
*/
void Servo2_SpeedRegulation(int next_angle)
{
	int N;
	int i;
	N  = abs(next_angle - servo2_last_angle);
	if (servo2_last_angle < next_angle)
	{
		for(i = 0; i < N; i++)
		{
			servo2_last_angle += 1;
			Servo2_SetAngle(servo2_last_angle);
			delay_ms(20);
		}
	}
	if(servo2_last_angle > next_angle)
	{
		for(i = 0; i < N; i++)
		{
			servo2_last_angle -= 1;
			Servo2_SetAngle(servo2_last_angle);
			delay_ms(20);
		}
	}
}


/*
* 函数功能：三号舵机 角度输出 插值细分
* 函数说明：有效降低舵机转速，增加稳定性
*/
void Servo3_SpeedRegulation(int next_angle)
{
	int N;
	int i;
	N  = abs(next_angle - servo3_last_angle);
	if (servo3_last_angle < next_angle)
	{
		for(i = 0; i < N; i++)
		{
			servo3_last_angle += 1;
			Servo3_SetAngle(servo3_last_angle);
			delay_ms(20);
		}
	}
	if(servo3_last_angle > next_angle)
	{
		for(i = 0; i < N; i++)
		{
			servo3_last_angle -= 1;
			Servo3_SetAngle(servo3_last_angle);
			delay_ms(20);
		}
	}
}


/*
* 函数功能：四号舵机 角度输出 插值细分
* 函数说明：有效降低舵机转速，增加稳定性
*/
void Servo4_SpeedRegulation(int next_angle)
{
	int N;
	int i;
	N  = abs(next_angle - servo4_last_angle);
	if (servo4_last_angle < next_angle)
	{
		for(i = 0; i < N; i++)
		{
			servo4_last_angle += 1;
			Servo4_SetAngle(servo4_last_angle);
			delay_ms(20);
		}
	}
	if(servo4_last_angle > next_angle)
	{
		for(i = 0; i < N; i++)
		{
			servo4_last_angle -= 1;
			Servo4_SetAngle(servo4_last_angle);
			delay_ms(20);
		}
	}
}


/*
* 函数功能：五号舵机 角度输出 插值细分
* 函数说明：有效降低舵机转速，增加稳定性
*/
void Servo5_SpeedRegulation(int next_angle)
{
	int N;
	int i;
	N  = abs(next_angle - servo5_last_angle);
	if (servo5_last_angle < next_angle)
	{
		for(i = 0; i < N; i++)
		{
			servo5_last_angle += 1;
			Servo5_SetAngle(servo5_last_angle);
			delay_ms(20);
		}
	}
	if(servo5_last_angle > next_angle)
	{
		for(i = 0; i < N; i++)
		{
			servo5_last_angle -= 1;
			Servo5_SetAngle(servo5_last_angle);
			delay_ms(20);
		}
	}
}
