/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : servo.c
** @auther  : Mrtutu
** @date    : 2019-02-17
** @describe: 舵机底层驱动
**
**           *  Servo1 :  TIM1CH2 --> PA9
**           *  Servo2 :  TIM1CH1 --> PA8
**           *  Servo3 :  TIM1CH4 --> PA11
**           *  Servo4 :  TIM1CH3 --> PA10
**           *  Servo5 :  TIM9CH1 --> PA2  （free）
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "servo.h"
#include "delay.h"


const float K = 33.333333f;  // PWM公式系数  

/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : servo_Init 舵机PWM初始化
* @param : arr: [重装载值]
**		   psc: [预分频系数]
* @return: None
* @note  : PWM频率为300Hz
*          PWM = K * angle + 1500；
*
*/
void Servo_Init(u16 arr, u16 psc)
{
    /* 结构体初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    /* 总线使能 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //TIM1 时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能 PORTA 时钟
    /*IO 复用*/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8,  GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);
    //GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,  GPIO_AF_TIM9);
    /* IO 初始化 */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11; //PA2 --> PA11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度 50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
    //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2 --> PA11
    /* 定时器初始化 */
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseStructure.TIM_Period = arr; //自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //初始化定时器TIM1
    TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure); //初始化定时器TIM9

    TIM_OCStructInit(& TIM_OCInitStructure);                       //恢复初始
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              //定时器模式为pwm模式1
    TIM_OCInitStructure.TIM_Pulse = 0;                             //脉冲值，即输出都是低电平
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
//    TIM_OC1Init(TIM9, &TIM_OCInitStructure);
//    TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);

    /*使能定时器*/
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    /*使能定时器PWM输出模式*/
//    TIM_Cmd(TIM9, ENABLE);
//    TIM_CtrlPWMOutputs(TIM9, ENABLE);
}



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Servo1_SetAngle  1号舵机角度输出
* @param : angle: 输入角度
* @return: None
* @note  : 角度范围：
*
*/
void Servo1_SetAngle(int angle)
{
    int pwm_value;
    if(angle<0||angle>180)return; 
    pwm_value = K * angle + 1500;
    TIM_SetCompare2(TIM1, pwm_value);
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Servo2_SetAngle 2号舵机角度输出
* @param : angle: 输入角度
* @return: None
* @note  : 角度范围：
*
*/
void Servo2_SetAngle(int angle)
{
    int pwm_value;
    if(angle<0||angle>180)return; 
    pwm_value = K * angle + 1500;
    TIM_SetCompare1(TIM1, pwm_value);
}



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Servo3_SetAngle  3号舵机角度输出
* @param : angle:  输入角度
* @return: None
* @note  : 角度范围：
*
*/
void Servo3_SetAngle(int angle)
{
    int pwm_value;
   if(angle<0||angle>180)return; 
    pwm_value = K * angle + 1500;
    TIM_SetCompare4(TIM1, pwm_value);
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Servo4_SetAngle  4号舵机角度输出
* @param : angle: 输出角度
* @return: None
* @note  : 角度范围：
*
*/
void Servo4_SetAngle(int angle)
{
    int pwm_value;
    if(angle<0||angle>180)return; 
    pwm_value = K * angle + 1500;
    TIM_SetCompare3(TIM1, pwm_value);
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Servo5_SetAngle 4号舵机角度输出
* @param : angle: 输出角度
* @return: None
* @note  : 角度范围：
*
*/
void Servo5_SetAngle(int angle)
{
    int pwm_value;
    if(angle<0||angle>180)return; 
    pwm_value = K * angle + 1500;
    TIM_SetCompare1(TIM9, pwm_value);
}


/********************************End of File************************************/
