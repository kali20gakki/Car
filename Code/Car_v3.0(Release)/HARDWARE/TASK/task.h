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
void Task_MoveIncline(void);
void Task_TestPath(void);
void Task_MoveLeft(void);
void Task_MovePad(void);
void Task_Qrcode(void);
void Task_MoveBackward(void);
void Task_MoveMaterials(void);
void Task_TakeMaterials(void);
void Task_PlaceMaterials(void);
void Task_MoveToPlace(void);
void Task_MoveFromPlaceToMaterials(void);
void Task_MoveStartPoint_4X3(void);
void Task_OLED(void);
void Task_WIFI(void);
void Task_MoveProcess(void);
void Task_MoveMidMaterial(void);
void Task_PlaceFinish(void);
void Task_MoveProcessToMaterial(void);
void Task_PrepareForWIFI(void);
void Task_MoveToGrab(void);
void Task_MoveBackHome(void);
void Task_Move1x1(void);
void Task_MoveToPlace2();
#endif

/********************************End of File************************************/