/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : action.h
** @auther  : Mrtutu
** @date    : 2019-03-03
** @describe: 
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/
 
#ifndef __ACTION_H_
#define __ACTION_H_
#include "sys.h"

void Action_First(void);

void action1(void);
void action2(void);
void action3(void);

void actionA(void);
void actionB(void);
void actionC(void);
void actionG(void);

void Action_Take_1(u8 time);
void Action_Take_2(u8 time);
void Action_Take_3(u8 time);

void Action_AfterTake(u8 time);

void Action_Place_1(u8 time);
void Action_Place_2(u8 time);
void Action_Place_3(u8 time);

void Action_AfterPlace(u8 time);

void action_Test(void);
#endif

/********************************End of File************************************/