/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : encoder.c
** @auther  : Mrtutu
** @date    : 2019-02-17
** @describe: 编码器底层初始化
**
**            MotorA：TIM2 A相 --> PA15   B相 --> PB3
**            MotorB：TIM4 A相 --> PB6    B相 --> PB7
**            MotorC：TIM5 A相 --> PA0    B相 --> PA1
**            MotorD：TIM3 A相 --> PA6    B相 --> PA7
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/
#include "encoder.h"
#include "motor.h"


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Encoder_MotorA_Init  电机A 编码器初始化
* @param : None
* @return: None
* @note  : MotorA：TIM2 A相 --> PA15   B相 --> PB3
*
*/
void Encoder_MotorA_Init(void)
{
    /* 结构体初始化 */
    GPIO_InitTypeDef GPIOA_InitStructure;
    GPIO_InitTypeDef GPIOB_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    /*总线使能*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能TIM2
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能PORTA
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能PORTB

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3,  GPIO_AF_TIM2);
    /* PA15初始化 */
    GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIOA_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIOA_InitStructure);
    /* PB3初始化 */
    GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIOB_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIOB_InitStructure);
    /* TIM2 复位 */
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    /* 设置TIM2 为编码器接口模式 */
    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
    TIM_ICStructInit(&TIM_ICInitStructure);    // 初始化为默认值
    TIM_ICInitStructure.TIM_ICFilter = 6;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    /* 清零 TIM2->CNT */
    TIM2->CNT = 0;
    /* 使能TIM2 */
    TIM_Cmd(TIM2, ENABLE);
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Encoder_MotorB_Init 电机B编码器初始化
* @param : None
* @return: None
* @note  : MotorB：TIM3 A相 --> PA6    B相 --> PA7
*
*/
void Encoder_MotorB_Init(void)
{
    /* 结构体初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    /*总线使能*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能 PORTA 时钟

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);
    /*IO使能*/
    //GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /*TIM3 复位*/
    TIM_DeInit(TIM3);
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    /* 设置TIM3 为编码器接口模式 */
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
    TIM_ICStructInit(&TIM_ICInitStructure);    // 初始化为默认值
    TIM_ICInitStructure.TIM_ICFilter = 6;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    /* 清零 TIM2->CNT */
    TIM3->CNT = 0;
    /* 使能TIM2 */
    TIM_Cmd(TIM3, ENABLE);
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Encoder_MotorC_Init 电机C编码器初始化
* @param : None
* @return: None
* @note  : MotorC：TIM4 A相 --> PB6    B相 --> PB7
*
*/
void Encoder_MotorC_Init(void)
{
    /* 结构体初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    /*总线使能*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能 PORTB 时钟

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
    /*IO使能*/
    //GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /*TIM4 复位*/
    TIM_DeInit(TIM4);
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    /* 设置TIM4 为编码器接口模式 */
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
    TIM_ICStructInit(&TIM_ICInitStructure);    // 初始化为默认值
    TIM_ICInitStructure.TIM_ICFilter = 6;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
    /* 清零 TIM4->CNT */
    TIM4->CNT = 0;
    /* 使能TIM2 */
    TIM_Cmd(TIM4, ENABLE);
}



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Encoder_MotorD_Init  电机D 编码器初始化
* @param : None
* @return: None
* @note  : MotorD：TIM5 A相 --> PA0    B相 --> PA1
*
*/
void Encoder_MotorD_Init(void)
{
    /* 结构体初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    /*总线使能*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能 PORTA 时钟

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM4);
    /*IO使能*/
    //GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /*TIM4 复位*/
    TIM_DeInit(TIM5);
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    /* 设置TIM4 为编码器接口模式 */
    TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
    TIM_ICStructInit(&TIM_ICInitStructure);    // 初始化为默认值
    TIM_ICInitStructure.TIM_ICFilter = 6;
    TIM_ICInit(TIM5, &TIM_ICInitStructure);
    /* 清零 TIM4->CNT */
    TIM5->CNT = 0;
    /* 使能TIM2 */
    TIM_Cmd(TIM5, ENABLE);
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Read_Encoder   读取编码器值
* @param : TIMX: 定时器编号
* @return: s16 ： short int 速度值
* @note  : None
*
*/
s16 Read_Encoder(u8 TIMX)
{
   s16 Encoder_TIM;    
   switch(TIMX)
	 {
	     case 1:  Encoder_TIM = TIM2->CNT;TIM2->CNT=0; break; // MotorA
		 case 2:  Encoder_TIM = TIM4->CNT;TIM4->CNT=0; break; // MotorB
		 case 3:  Encoder_TIM = TIM5->CNT;TIM5->CNT=0; break; // MotorC
		 case 4:  Encoder_TIM = TIM3->CNT;TIM3->CNT=0; break; // MotorD
		 default: Encoder_TIM = 0;
	 }
		return Encoder_TIM;
}

/********************************End of File************************************/
