/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : beep.c
** @auther  : Mrtutu
** @date    : 2019-02-17
** @describe: 蜂鸣器底层驱动
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "beep.h"
#include "delay.h"

/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Beep_Init  蜂鸣器初始化
* @param : None
* @return: None
* @note  : BEEP --> E1
*
*/
void Beep_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能 GPIOB 时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //BEEP 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
    GPIO_ResetBits(GPIOE, GPIO_Pin_1); //输出0，关闭蜂鸣器输出
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Beep_ring 蜂鸣器响声
* @param : 
* @return: None
* @note  : 滴 延时100ms 滴
*
*/
void Beep_ring(void)
{
	BEEP=0;
	delay_ms(100);
	BEEP=1;
	delay_ms(100);
	BEEP=0;
	delay_ms(100);
	BEEP=1;
	delay_ms(100);
	BEEP=0;
}
/********************************End of File************************************/
