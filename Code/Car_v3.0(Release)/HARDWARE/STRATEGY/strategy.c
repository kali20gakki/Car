/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : strategy.c
** @auther  : Mrtutu
** @date    : 2019-02-26
** @describe: 小车路径策略
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "strategy.h"
#include "servo.h"




volatile u16 QrcodeNum;


/* 物料放置对应的舵机转角 */
static u8 angle_R = 20;
static u8 angle_G = 30;
static u8 angle_B = 40;

/* 物料提取对应舵机转角 */
volatile u8 angle_1;
volatile u8 angle_2;
volatile u8 angle_3;


/********************************End of File************************************/