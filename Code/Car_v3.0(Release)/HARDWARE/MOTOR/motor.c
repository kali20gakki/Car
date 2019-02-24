/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : motor.c
** @auther  : Mrtutu
** @date    : 2019-02-17
** @describe: 直流电机底层驱动
**
**           具体引脚分配在头文件中修改
**           MOTORC --> TIM8CH1 --> PC6
**           MOTORD --> TIM8CH2 --> PC7
**           MOTORA --> TIM8CH3 --> PC8
**           MOTORB --> TIM8CH4 --> PC9
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**           不使用PA13 PA14 和SW口冲突
**          
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "motor.h"


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : motor_Init  电机PWM初始化
* @param : arr: 重装载值
**		   psc: 预分频系数
* @return: None
* @note  : arr = 1000-1    psc =  84-1  PWM频率 = 2KHz   占空比 = N/arr
*
*/
void Motor_Init(u16 arr, u16 psc)
{
    /* 结构体初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    /* 总线使能 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //TIM8 时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能 PORTC 时钟
    /*IO 复用*/
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6,  GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7,  GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8,  GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9,  GPIO_AF_TIM8);
    /* IO 初始化 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /* 定时器初始化 */
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseStructure.TIM_Period = arr; //自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //初始化定时器TIM8

    TIM_OCStructInit(& TIM_OCInitStructure);                       //恢复初始
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //定时器模式为pwm模式1
    TIM_OCInitStructure.TIM_Pulse = 0;                             //脉冲值，即输出都是低电平
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

/*
* @auther: Mrtutu
* @date  ：2019-02-17
* 
* @func  : Motor_Pin_Init  电机方向控制引脚初始化
* @param : 
* @return: None
* @note  : None
*
*/
void Motor_Pin_Init(void)
{
	GPIO_InitTypeDef GPIO_AInitStructure; // GPIOA
	GPIO_InitTypeDef GPIO_CInitStructure; // GPIOC
	GPIO_InitTypeDef GPIO_EInitStructure; // GPIOE
    GPIO_InitTypeDef GPIO_DInitStructure; // GPIOD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOD, ENABLE);//使能 PORT 时钟
	
	/* IO 初始化 A12 */
	GPIO_AInitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_AInitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_AInitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度 50MHz
	GPIO_AInitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_AInitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉			
	GPIO_Init(GPIOA,&GPIO_AInitStructure); 
    /* IO 初始化 D0 */
	GPIO_DInitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_DInitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_DInitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度 50MHz
	GPIO_DInitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_DInitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉			
	GPIO_Init(GPIOD,&GPIO_DInitStructure); 
	/* IO 初始化 C10 C11*/
	GPIO_CInitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_CInitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_CInitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度 50MHz
	GPIO_CInitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_CInitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉			
	GPIO_Init(GPIOC,&GPIO_CInitStructure);
	/* IO 初始化 E8 E9 E11 E13*/
	GPIO_EInitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13;
	GPIO_EInitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_EInitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度 50MHz
	GPIO_EInitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_EInitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉			
	GPIO_Init(GPIOE,&GPIO_EInitStructure);
}
/********************************End of File************************************/
