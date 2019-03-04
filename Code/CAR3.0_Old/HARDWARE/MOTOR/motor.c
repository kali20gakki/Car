#include "motor.h"

/*
* 函数功能：电机PWM调速初始化 
* 函数说明：MOTORA --> TIM8CH1 --> PC6
*           MOTORB --> TIM8CH2 --> PC7
*           MOTORC --> TIM8CH3 --> PC8
*           MOTORD --> TIM8CH4 --> PC9
*           
            MOTORA --> TIM8CH3 --> PC8
						MOTORB --> TIM8CH4 --> PC9
						MOTORC --> TIM8CH2 --> PC7
						MOTORD --> TIM8CH1 --> PC6
*           arr = 1000-1    psc =  84-1  PWM频率 = 2KHz   占空比 = N/arr
*/
void motor_Init(u16 arr,u16 psc)
{
	/* 结构体初始化 */
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	/* 总线使能 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);//TIM8 时钟使能
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能 PORTC 时钟
	/*IO 复用*/
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6,  GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7,  GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8,  GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9,  GPIO_AF_TIM8);
	/* IO 初始化 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); 
	/* 定时器初始化 */
	TIM_TimeBaseStructure.TIM_Prescaler=psc; //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr; //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);//初始化定时器TIM8
	
	TIM_OCStructInit(& TIM_OCInitStructure);                       //恢复初始
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //定时器模式为pwm模式1
  TIM_OCInitStructure.TIM_Pulse =0;                              //脉冲值，即输出都是低电平
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      //极性为高
	/*初始化定时器各个通道*/
	/*TIM8CH1*/
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);     
  TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
	/*TIM8CH2*/
  TIM_OC2Init(TIM8, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
	/*TIM8CH3*/
  TIM_OC3Init(TIM8, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
	/*TIM8CH4*/
  TIM_OC4Init(TIM8, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);
	/*使能定时器*/
	TIM_Cmd(TIM8, ENABLE);
	/*使能定时器PWM输出模式*/
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
}

/*电机控制引脚初始化*/
void motor_Pin_Init()
{
	GPIO_InitTypeDef GPIO_DInitStructure; // GPIOD
	GPIO_InitTypeDef GPIO_BInitStructure; // GPIOB
	GPIO_InitTypeDef GPIO_CInitStructure; // GPIOC
	GPIO_InitTypeDef GPIO_EInitStructure; // GPIOE
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);//使能 PORT 时钟
	
	/* IO 初始化 D0 D1 D2 D3*/
	GPIO_DInitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_DInitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_DInitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz
	GPIO_DInitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_DInitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉			
	GPIO_Init(GPIOD,&GPIO_DInitStructure); 
	/* IO 初始化 B0 B2*/
	GPIO_BInitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2;
	GPIO_BInitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_BInitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz
	GPIO_BInitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_BInitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉			
	GPIO_Init(GPIOB,&GPIO_BInitStructure);
	/* IO 初始化 C4*/
	GPIO_CInitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_CInitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_CInitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz
	GPIO_CInitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_CInitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉			
	GPIO_Init(GPIOC,&GPIO_CInitStructure);
	/* IO 初始化 E8*/
	GPIO_EInitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_EInitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_EInitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz
	GPIO_EInitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_EInitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉			
	GPIO_Init(GPIOE,&GPIO_EInitStructure);
}
