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


#define SENSOR_FRONT_L GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)
#define SENSOR_FRONT_M GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)
#define SENSOR_FRONT_R GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)

#define SENSOR_BACK_L GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)
#define SENSOR_BACK_M GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)
#define SENSOR_BACK_R GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)

#define SENSOR_LEFT_U PAin(1)
#define SENSOR_LEFT_D PAin(2)

#define SNESOR_RIGHT_U PAin(3)
#define SNESOR_RIGHT_D PAin(4)


#endif

/********************************End of File************************************/