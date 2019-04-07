/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : includes.h
** @auther  : Mrtutu
** @date    : 2019-04-07
** @describe: 
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/
 
#ifndef __INCLUDES_H_
#define __INCLUDES_H_


/*
*********************************************************************************************************
*                                         标准库
*********************************************************************************************************
*/
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>



/*
*********************************************************************************************************
*                                         变量
*********************************************************************************************************
*/
extern u8 COUNT_FRONT_L;
extern u8 COUNT_FRONT_R;
extern u8 COUNT_RIGHT_U;
extern u8 COUNT_RIGHT_D;

extern u8 FLAG_QR;
extern u8 FLAG_WIFI;
extern const int Position_Pad;


extern int Take_Material_Order[3];
extern int Place_Process_Order[3];
extern int Take_Process_Order[3];
extern int Place_Finish_Order[3];

#endif

/********************************End of File************************************/