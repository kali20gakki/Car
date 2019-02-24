/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : encoder.h
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
 
#ifndef __ENCODER_H_
#define __ENCODER_H_
#include "sys.h"

void Encoder_MotorA_Init(void);
void Encoder_MotorB_Init(void);
void Encoder_MotorC_Init(void);
void Encoder_MotorD_Init(void);

s16 Read_Encoder(u8 TIMX);

#endif

/********************************End of File************************************/