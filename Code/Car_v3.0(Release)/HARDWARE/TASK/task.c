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
void Task_Test(void)
{
    Task_MoveLeft();
    delay_ms(100);
    Task_MovePad();
    delay_Ntimes(2);
    Task_MoveMaterials();
    delay_Ntimes(2);
//    Task_MoveBackward();
//    delay_Ntimes(2);
    Task_MovePlaceToMaterials();
    delay_Ntimes(2);
    Task_MoveFromPlaceToMaterials();
    delay_Ntimes(2);
    Task_MovePlaceToMaterials();
    delay_Ntimes(2);
    Task_MoveFromPlaceToMaterials();
    delay_Ntimes(2);
    Task_MovePlaceToMaterials();
    delay_Ntimes(2);
    Task_MoveStartPoint_4X3();
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
    COUNT_FRONT_L = 0;
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
    while(COUNT_RIGHT_U != 6)
    {
        Car_TrackFront();
    }

    Kinematic_Analysis(0, 0);
}


/*
* @auther: Mrtutu
* @date  ：2019-03-03
*
* @func  : Task_MoveMaterials
* @param : None
* @return: None
* @note  : None
*
*/
void Task_MoveMaterials(void)
{
    COUNT_RIGHT_D = 0;
    while(COUNT_RIGHT_D != 3)Car_TrackBack();

    Kinematic_Analysis(0, 0);
}



/*
* @auther: Mrtutu
* @date  ：2019-03-04
*
* @func  : Task_MoveBackward
* @param : None
* @return: None
* @note  : None
*
*/
void Task_MoveBackward(void)
{
    COUNT_FRONT_R = 0;
    while(COUNT_FRONT_R != 2)Kinematic_Analysis(-20, 0);
    
    Kinematic_Analysis(0, 0);
}


/*
* @auther: Mrtutu
* @date  ：2019-03-03
*
* @func  : Task_TakeMaterials
* @param : None
* @return: None
* @note  : None 提取物料
*
*/
void Task_TakeMaterials(void)
{

}



/*
* @auther: Mrtutu
* @date  ：2019-03-03
*
* @func  : Task_PlaceMaterials
* @param : None
* @return: None
* @note  : None  放置物料
*
*/
void Task_PlaceMaterials(void)
{

}



/*
* @auther: Mrtutu
* @date  ：2019-03-03
*
* @func  : Task_MovePlaceToMaterials
* @param : None
* @return: None
* @note  : None  平移到放置区
*
*/
void Task_MovePlaceToMaterials(void)
{
    COUNT_FRONT_R = 0;
    while(COUNT_FRONT_R != 4)Car_TrackLeft();

    Kinematic_Analysis(0, 0);
}



/*
* @auther: Mrtutu
* @date  ：2019-03-03
*
* @func  : Task_MoveFromPlaceToMaterials
* @param : None
* @return: None
* @note  : None   从放置区平移到提取区
*
*/
void Task_MoveFromPlaceToMaterials(void)
{
    COUNT_FRONT_R = 0;
    while(COUNT_FRONT_R != 4)Car_TrackRight();

    Kinematic_Analysis(0, 0);
}



/*
* @auther: Mrtutu
* @date  ：2019-03-03
*
* @func  : Task_MoveStartPoint_4X3
* @param : None
* @return: None
* @note  : None   -20 / 11
*
*/
void Task_MoveStartPoint_4X3(void)
{
    Kinematic_Analysis(24, -40);

    delay_Ntimes(19);
    Kinematic_Analysis(0, 0);
}
/********************************End of File************************************/
