/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : motor.h
** @auther  : Mrtutu
** @date    : 2019-02-17
** @describe: 
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/
 
#ifndef __MOTOR_H_
#define __MOTOR_H_
#include "sys.h"

#define PWMA   TIM8->CCR3  // 
#define PWMB   TIM8->CCR4 
#define PWMC   TIM8->CCR1 
#define PWMD   TIM8->CCR2  


#define AIN1  PDout(0)   // PWMA -- > PC8
#define AIN2  PAout(12)

#define BIN1  PCout(11)  // PWMB -- > PC9
#define BIN2  PCout(10)

#define CIN1  PEout(11)  // PWMC -- > PC6
#define CIN2  PEout(13)

#define DIN1  PEout(9)   // PWMD -- > PC7
#define DIN2  PEout(8)

void Motor_Pin_Init();
void Motor_Init(u16 arr,u16 psc);

#endif

/********************************End of File************************************/