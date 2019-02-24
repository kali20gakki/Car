/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : servo.h
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
 
#ifndef __SERVO_H_
#define __SERVO_H_
#include "sys.h"

void Servo_Init(u16 arr,u16 psc);

void Servo1_SetAngle(int angle);
void Servo2_SetAngle(int angle);
void Servo3_SetAngle(int angle);
void Servo4_SetAngle(int angle);
void Servo5_SetAngle(int angle);

//void Servo1_SpeedRegulation(int next_angle);
//void Servo2_SpeedRegulation(int next_angle);
//void Servo3_SpeedRegulation(int next_angle);
//void Servo4_SpeedRegulation(int next_angle);
//void Servo5_SpeedRegulation(int next_angle);

#endif

/********************************End of File************************************/