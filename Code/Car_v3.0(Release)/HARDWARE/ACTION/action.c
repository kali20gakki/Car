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


extern u8 angle_Take[3];
extern u8 angle_Place[3];

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

    Servo4_SpeedRegulation(90, 5);
    Servo3_SpeedRegulation(1, 5);
    Servo2_SpeedRegulation(145, 5);
    Servo1_SpeedRegulation(60, 5);
    Servo3_SpeedRegulation(0, 5);
}


/*
* @auther: Mrtutu
* @date  ：2019-03-06
*
* @func  : Action_Take_1
* @param : time: [输入/出]
* @return: None
* @note  : None   抓取物料动作一
*
*/
void Action_Take_1(u8 time)
{
    Servo4_SpeedRegulation(angle_Take[0], time);   // 底部先转到指定位置
    Servo3_SpeedRegulation(90, 7);               // 抬平
    Servo2_SpeedRegulation(0, 7);              // 前臂下去
    delay_ms(200);                                 // 适当延时
    Servo3_SpeedRegulation(50, time);               // 抬平
    Servo1_SpeedRegulation(0, 4);               // 爪子闭合
}



/*
* @auther: Mrtutu
* @date  ：2019-03-06
*
* @func  : Action_Take_2
* @param : time: [输入/出]
* @return: None
* @note  : None  抓取物料动作二
*
*/
void Action_Take_2(u8 time)
{
    Servo4_SpeedRegulation(angle_Take[1], time);   // 底部先转到指定位置
    Servo3_SpeedRegulation(90, 7);               // 抬平
    Servo2_SpeedRegulation(0, 7);              // 前臂下去
    delay_ms(200);                                 // 适当延时
    Servo3_SpeedRegulation(50, time);               // 抬平
    Servo1_SpeedRegulation(0, 4);               // 爪子闭合
}



/*
* @auther: Mrtutu
* @date  ：2019-03-06
*
* @func  : Action_Take_3
* @param : time: [输入/出]
* @return: None
* @note  : None   抓取物料动作三
*
*/
void Action_Take_3(u8 time)
{
    Servo4_SpeedRegulation(angle_Take[2], time);   // 底部先转到指定位置
    Servo3_SpeedRegulation(90, 7);               // 抬平
    Servo2_SpeedRegulation(0, 7);              // 前臂下去
    delay_ms(200);                                 // 适当延时
    Servo3_SpeedRegulation(50, time);               // 抬平
    Servo1_SpeedRegulation(0, 4);               // 爪子闭合
}



void Action_AfterTake(u8 time)
{
    Servo1_SpeedRegulation(0, time);               // 爪子一直闭合
    Servo2_SpeedRegulation(145, time);              // 前臂直立起来
    Servo3_SpeedRegulation(0, time);               // 抬平

    Servo4_SpeedRegulation(90, time);              // 底部舵机复位
}

void Action_Place_1(u8 time)
{
    Servo4_SpeedRegulation(angle_Place[0], 10);   // 底部先转到指定位置
    Servo3_SpeedRegulation(90, 1);                 // 3先转一个合适的角度
    Servo2_SpeedRegulation(50, 10);               //
    Servo3_SpeedRegulation(180, time + 5);               // 3适中速度放下

    delay_ms(1000);
    Servo1_SpeedRegulation(30, 1);                  // 松手 快速
}

void Action_Place_2(u8 time)
{
    Servo4_SpeedRegulation(angle_Place[1], time);   // 底部先转到指定位置
    Servo3_SpeedRegulation(90, 10);                 // 3先转一个合适的角度
    Servo2_SpeedRegulation(50, time);               // 落地
    Servo3_SpeedRegulation(180, time + 5);            // 3适中速度放下
    delay_ms(1000);
    Servo1_SpeedRegulation(30, 1);                  // 松手 快速
}

void Action_Place_3(u8 time)
{
    Servo4_SpeedRegulation(angle_Place[2], time);   // 底部先转到指定位置
    Servo3_SpeedRegulation(90, 10);                 // 3先转一个合适的角度
    Servo2_SpeedRegulation(50, time);               // 落地
    Servo3_SpeedRegulation(180, time + 5);               // 3适中速度放下
    delay_ms(1000);
    Servo1_SpeedRegulation(30, 1);                  // 松手 快速
}


void Action_AfterPlace(u8 time)
{
    Servo1_SpeedRegulation(30, time);               // 爪子打开

    Servo3_SpeedRegulation(90, 10);               // 先抬起来
    //delay_ms(100);
    Servo2_SpeedRegulation(145, time);                // 先抬起来

    Servo3_SpeedRegulation(10, time);

    Servo4_SpeedRegulation(90, time);               // 复位
}


void Action_Test()
{
    Servo1_SpeedRegulation(60, 15);
    Servo2_SpeedRegulation(0, 15);
    Servo3_SpeedRegulation(60, 15);
    Servo4_SpeedRegulation(90, 15);
}
/********************************End of File************************************/
