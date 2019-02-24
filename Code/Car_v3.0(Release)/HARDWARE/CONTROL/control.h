/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : control.h
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

#ifndef __CONTROL_H_
#define __CONTROL_H_
#include "sys.h"

void Set_PWM(int motorA, int motorB, int motorC, int motorD);
void limit_PWM(void);
int myabs(int a);
int Incremental_PI_MotorA(int Encoder, int Target);
int Incremental_PI_MotorB(int Encoder, int Target);
int Incremental_PI_MotorC(int Encoder, int Target);
int Incremental_PI_MotorD(int Encoder, int Target);

#endif

/********************************End of File************************************/