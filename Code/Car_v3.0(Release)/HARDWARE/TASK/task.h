/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : task.h
** @auther  : Mrtutu
** @date    : 2019-02-21
** @describe: 
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/
 
#ifndef __TASK_H_
#define __TASK_H_
#include "sys.h"

void delay_Ntimes(int Num);
void Task_Full(void);
void Task_TestSensor(void);
void Task_TestPath(void);
void Task_MoveLeft(void);
void Task_MovePad(void);
void Task_MoveBackward(void);
void Task_MoveMaterials(void);
void Task_TakeMaterials(void);
void Task_PlaceMaterials(void);
void Task_MovePlaceToMaterials(void);
void Task_MoveFromPlaceToMaterials(void);
void Task_MoveStartPoint_4X3(void);
void Task_OLED(void);

#endif

/********************************End of File************************************/