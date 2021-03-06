/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : sensor.h
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

#ifndef __SENSOR_H_
#define __SENSOR_H_
#include "sys.h"

/* 前 */
#define SENSOR_FRONT_L             PAin(5)   //PA5
#define SENSOR_FRONT_R             PCin(3)   //PC3
/* 后 */
#define SENSOR_BACK_L              PEin(5)   //PE5
#define SENSOR_BACK_R              PCin(1)   //PC1
/* 左 */
#define SENSOR_LEFT_U              PEin(6)   //PE6
#define SENSOR_LEFT_D              PCin(15)   //PC15
/* 右 */
#define SNESOR_RIGHT_U             PCin(13)   //PA3
#define SNESOR_RIGHT_D             PCin(0)    //PC0



#endif

/********************************End of File************************************/