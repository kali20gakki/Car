/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : move.c
** @auther  : Mrtutu
** @date    : 2019-02-17
** @describe: 小车运动姿态控制
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "move.h"
#include "math.h"
#include "sensor.h"


/* 单精度浮点 7位有效 */
const float PI_180 = 0.0174533f;         // PI/180 减少计算量

extern volatile int Target_A, Target_B, Target_C, Target_D;




/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Kinematic_Analysis
* @param : Vx: X轴速度
**		   Vy: Y轴速度
**		   Vz: Z轴速度
* @return: None
* @note  : None
*
*/
void Kinematic_Analysis(int Vx, int Vy)
{
    Target_A = +Vx - Vy;
    Target_B = -Vx - Vy;
    Target_C = -Vx + Vy;
    Target_D = +Vx + Vy;
}



/*
* @auther: Mrtutu
* @date  ：2019-02-18
*
* @func  : Car_SetMove
* @param : velocity: 小车速率
**		   angle   : 速度角度
* @return: None
* @note  : None
*
*/
void Car_SetMove(u8 velocity, float angle)
{
    float Vx, Vy, delta;

    if(velocity < 0 || angle < 0 || angle > 360)return;

    if(angle >= 0 && angle <= 90)       // 速度矢量在 第一象限
    {
        Vx = velocity * cosf(angle * PI_180);
        Vy = velocity * sinf(angle * PI_180);
        Kinematic_Analysis(Vx, Vy);
    }
    else if(angle > 90 && angle <= 180)  // 速度矢量在 第二象限
    {
        delta = 180 - angle;
        Vx = velocity * cosf(angle * PI_180);
        Vy = velocity * sinf(angle * PI_180);
        Kinematic_Analysis(-Vx, Vy);
    }
    else if(angle > 180 && angle <= 270) // 速度矢量在 第三象限
    {
        delta = 270 - angle;
        Vx = velocity * cosf(angle * PI_180);
        Vy = velocity * sinf(angle * PI_180);
        Kinematic_Analysis(-Vx, -Vy);
    }
    else if(angle > 270 && angle < 360)  // 速度矢量在 第四象限
    {
        delta = 360 - angle;
        Vx = velocity * cosf(angle * PI_180);
        Vy = velocity * sinf(angle * PI_180);
        Kinematic_Analysis(Vx, -Vy);
    }
}



/*
* @auther: Mrtutu
* @date  ：2019-02-21
*
* @func  : Car_TrackFront
* @param : None
* @return: None
* @note  : 循迹直走  平移效果不佳，采用
*
*/
void Car_TrackFront(void)
{
    if((SENSOR_FRONT_L == 0 && SENSOR_FRONT_R == 0)) //没有偏离
    {
        Kinematic_Analysis(0, 40); // 直走
    }
    else if(SENSOR_FRONT_L == 0 && SENSOR_FRONT_R == 1) // 左偏
    {
        /* 速度差5 */
        Target_A = -40;
        Target_B = -40;
        Target_C =  35;
        Target_D =  35;

    }
    else if(SENSOR_FRONT_L == 1 && SENSOR_FRONT_R == 0) // 右偏
    {
        Target_A = -35;
        Target_B = -35;
        Target_C =  40;
        Target_D =  40;
    }
}



/*
* @auther: Mrtutu
* @date  ：2019-02-21
*
* @func  : Car_TrackBack
* @param : None
* @return: None
* @note  : 循迹后退
*
*/
void Car_TrackBack(void)
{
    if(SENSOR_BACK_L == 0 && SENSOR_BACK_R == 0) // 没有偏离
    {
        Kinematic_Analysis(0, -30); // 后退
    }
    else if(SENSOR_BACK_L == 1 && SENSOR_BACK_R == 0) // 左偏
    {
        /* 速度差5 */
        Target_A =  30;
        Target_B =  30;
        Target_C = -35;
        Target_D = -35;
    }
    else if(SENSOR_BACK_L == 0 && SENSOR_BACK_R == 1) // 右偏
    {
        /* 速度差5 */
        Target_A =  35;
        Target_B =  35;
        Target_C = -30;
        Target_D = -30;
    }
}



/*
* @auther: Mrtutu
* @date  ：2019-02-28
*
* @func  : Car_TrackLeft
* @param : None
* @return: None
* @note  : 循迹左平移
*
*/
void Car_TrackLeft(void)
{
    if(SENSOR_LEFT_U == 0 && SENSOR_LEFT_D == 0) // 没有偏离
    {
        Kinematic_Analysis(-20, 0);
    }
    else if(SENSOR_LEFT_U == 1 && SENSOR_LEFT_D == 0) // 上偏
    {
        /* 速度差5 */
        Target_A = -25;
        Target_B =  20;
        Target_C =  20;
        Target_D = -25;
    }
    else if(SENSOR_LEFT_U == 0 && SENSOR_LEFT_D == 1) // 下偏
    {
        /* 速度差5 */
        Target_A = -20;
        Target_B =  25;
        Target_C =  25;
        Target_D = -20;
    }
}



/*
* @auther: Mrtutu
* @date  ：2019-02-28
*
* @func  : Car_TrackRight
* @param : None
* @return: None
* @note  : None
*
*/
void Car_TrackRight(void)
{
    if(SNESOR_RIGHT_U == 0 && SNESOR_RIGHT_D == 0) // 没有偏离
    {
        Kinematic_Analysis(20, 0);

    }
    else if(SNESOR_RIGHT_U == 0 && SNESOR_RIGHT_D == 1) // 上偏
    {
        /* 速度差5 */
        Target_A =  20;
        Target_B = -25;
        Target_C = -25;
        Target_D =  20;


    }
    else if(SNESOR_RIGHT_U == 1 && SNESOR_RIGHT_D == 0) // 下偏
    {
        /* 速度差5 */
        Target_A =  25;
        Target_B = -20;
        Target_C = -20;
        Target_D =  25;

    }

}
/********************************End of File************************************/
