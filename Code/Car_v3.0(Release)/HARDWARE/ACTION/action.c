/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : action.c
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
 
#include "action.h"
#include "servo.h"


/*
* @auther: Mrtutu
* @date  ：2019-03-03
*
* @func  : Action_First
* @param : None
* @return: None
* @note  : None  舵机上电后初始动作
*
*/
void Action_First(void)
{
    Servo4_SetAngle(90);
    Servo3_SetAngle(0);
    Servo2_SetAngle(145);
    Servo1_SetAngle(90);
}
/********************************End of File************************************/
