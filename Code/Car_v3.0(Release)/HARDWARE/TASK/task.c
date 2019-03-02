/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : task.c
** @auther  : Mrtutu
** @date    : 2019-02-21
** @describe: 小车运行任务
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/
#include "task.h"
#include "sensor.h"

extern u8 COUNT_FRONT_L;
extern u8 COUNT_FRONT_R;
extern u8 COUNT_LEFT_U;
extern u8 COUNT_RIGHT_U;
extern u8 COUNT_RIGHT_D;




void delay_Ntimes(int Num)
{
    int i;
    for(i = 0; i < Num; i++)delay_ms(1000);
}



/*
* @auther: Mrtutu
* @date  ：2019-02-21
*
* @func  : Task_test
* @param : None
* @return: None
* @note  : Test 任务
*
*/
void Task_test(void)
{

}


/*
* @auther: Mrtutu
* @date  ：2019-02-21
*
* @func  : Task_MoveLeft
* @param : None
* @return: None
* @note  : 小车左平移到线上
*
*/
void Task_MoveLeft(void)
{
    while(COUNT_FRONT_L != 2)
    {
        Kinematic_Analysis(-20, 0);
    }
    Kinematic_Analysis(0, 0);
}


/*
* @auther: Mrtutu
* @date  ：2019-02-21
*
* @func  : Task_MovePad
* @param : None
* @return: None
* @note  : 小车移动到平板位置
*
*/
void Task_MovePad(void)
{
    COUNT_RIGHT_U = 0;
    while(COUNT_RIGHT_U < 6)
    {
        Car_TrackFront();
    }

    Kinematic_Analysis(0, 0);
}


void Task_MoveMaterials(void)
{
    COUNT_RIGHT_D = 0;
    while(COUNT_RIGHT_D != 3)Car_TrackBack();

    Kinematic_Analysis(0, 0);
}


void Task_TakeMaterials(void)
{
    
}

void Task_PlaceMaterials(void)
{
    
}

void Task_MovePlaceToMaterials(void)
{
    COUNT_LEFT_U = 0;
    while(COUNT_LEFT_U !=4 )Car_TrackLeft();

    Kinematic_Analysis(0, 0);
}

void Task_MoveFromPlaceToMaterials(void)
{
    COUNT_RIGHT_U = 0;
    while(COUNT_RIGHT_U != 6)Car_TrackRight();
    
     Kinematic_Analysis(0, 0);
}
void Task_MoveStartPoint_4X3(void)
{
    Kinematic_Analysis(30, -40);

    delay_Ntimes(24);
    Kinematic_Analysis(0, 0);
}
/********************************End of File************************************/
