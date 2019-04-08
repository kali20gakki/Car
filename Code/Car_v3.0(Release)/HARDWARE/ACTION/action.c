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
** @date    : 2019-03-06
** @describe:
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

/********************************End of File************************************/
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
#include "strategy.h"


void Action_FindBlob(void)
{
    Servo1_SpeedRegulation(60,10);
    Servo2_SpeedRegulation(60, 10);
    Servo3_SpeedRegulation(180, 10);
    Servo4_SpeedRegulation(40, 10);
}


void Action_FindQrcode(void)
{
    Servo1_SpeedRegulation(60,10);
    Servo3_SpeedRegulation(145, 10);
    Servo2_SpeedRegulation(115, 10);
    Servo4_SpeedRegulation(40, 10);
}


void Action_AfterQrcode(void)
{
    Servo1_SpeedRegulation(60,10);
    Servo2_SpeedRegulation(60, 10);//60
    Servo3_SpeedRegulation(180, 10);
    Servo4_SpeedRegulation(40, 10);
}



void Action_3(void)
{
    
}


void Action_4(void)
{
    
}

void Action_5(void)
{
    
}


/********************************End of File************************************/
