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
#include "oled.h"
#include "action.h"
#include "strategy.h"
#include "servo.h"

extern u8 COUNT_FRONT_L;
extern u8 COUNT_FRONT_R;
extern u8 COUNT_LEFT_U;
extern u8 COUNT_RIGHT_U;
extern u8 COUNT_RIGHT_D;

extern u8 FLAG_QR;

extern u8 angle_Place[3];

/*
* @auther: Mrtutu
* @date  ：2019-03-05
*
* @func  : delay_Ntimes
* @param : Num: [输入/出]
* @return: None
* @note  : None   延时次数
*
*/
void delay_Ntimes(int Num)
{
    int i;
    for(i = 0; i < Num; i++)delay_ms(1000);
}



/*
* @auther: Mrtutu
* @date  ：2019-03-05
*
* @func  : Task_Full
* @param : None
* @return: None
* @note  : None  完整任务
*
*/
void Task_Full(void)
{
    Task_MoveLeft();         // 左移到线上
    Task_MovePad();          // 寻迹到Pad
    Task_Qrcode();           // 识别Qrcode
    Task_MoveMaterials();    // 后退到物料

    delay_Ntimes(1);
    /* 第一次搬运 */
    Action_Take_1(7);
    Action_AfterTake(7);
    if(angle_Place[0] == 90)Task_MoveToPlace2();
    else Task_MoveToPlace();
    Action_Place_1(7);
    delay_Ntimes(1);
    Action_AfterPlace(7);
    delay_Ntimes(1);
    Task_MoveFromPlaceToMaterials();

    /* 第二次搬运 */
    //Action_Take_2(7);
    delay_Ntimes(1);
    //Action_AfterTake(7);
    if(angle_Place[1] == 90)Task_MoveToPlace2();
    else Task_MoveToPlace();
   // Action_Place_2(7);
    delay_Ntimes(1);
    //Action_AfterPlace(7);
    delay_Ntimes(1);
    Task_MoveFromPlaceToMaterials();


    /* 第三次搬运 */
    //Action_Take_3(7);
    delay_Ntimes(1);
    //Action_AfterTake(7);
    if(angle_Place[2] == 90)Task_MoveToPlace2();
    else Task_MoveToPlace();
    //Action_Place_3(7);
    delay_Ntimes(1);
   // Action_AfterPlace(7);

    delay_Ntimes(1);

    Task_MoveStartPoint_4X3();
}


/*
* @auther: Mrtutu
* @date  ：2019-03-05
*
* @func  : Task_TestSensor
* @param : None
* @return: None
* @note  : None
*
*/
void Task_TestSensor(void)
{
    OLED_Clear();   // OLED复位

    while(1)
    {
        OLED_ShowString(0, 0, "F_L = ");
        OLED_ShowNum(50, 0, COUNT_FRONT_L);
        OLED_ShowNum(80, 0, SENSOR_FRONT_L);

        OLED_ShowString(0, 2, "F_R = ");
        OLED_ShowNum(50, 2, COUNT_FRONT_R);
        OLED_ShowNum(80, 2, SENSOR_FRONT_R);

        OLED_ShowString(0, 4, "R_U = ");
        OLED_ShowNum(50, 4, COUNT_RIGHT_U);
        OLED_ShowNum(80, 4, SNESOR_RIGHT_U);

        OLED_ShowString(0, 6, "R_D = ");
        OLED_ShowNum(50, 6, COUNT_RIGHT_D);
        OLED_ShowNum(80, 6, SNESOR_RIGHT_D);
    }
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
void Task_TestPath(void)
{
    Task_MoveLeft();
    delay_ms(100);
    Task_MovePad();
    delay_Ntimes(2);
    Task_MoveMaterials();
    delay_Ntimes(2);
    Task_MoveMaterials();
    delay_Ntimes(2);
    Task_MoveFromPlaceToMaterials();
    delay_Ntimes(2);
    Task_MoveMaterials();
    delay_Ntimes(2);
    Task_MoveFromPlaceToMaterials();
    delay_Ntimes(2);
    Task_MoveMaterials();
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
* @date  ：2019-03-05
*
* @func  : Task_MoveIncline
* @param : None
* @return: None
* @note  : None
*
*/
void Task_MoveIncline(void)
{
    COUNT_FRONT_L = 0;
    while(COUNT_FRONT_L != 3)
    {
        Kinematic_Analysis(-20, 20);
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
        Strategy_MaterialColor();
    }

    Kinematic_Analysis(0, 0);
}



/*
* @auther: Mrtutu
* @date  ：2019-03-06
*
* @func  : Task_Qrcode
* @param : None
* @return: None
* @note  : None  识别Qrcode
*
*/
void Task_Qrcode(void)
{
    Kinematic_Analysis(0, 0);
    while(FLAG_QR)Strategy_QrcodeSquence();
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
    while(COUNT_FRONT_R != 2)Kinematic_Analysis(-15, 0);

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
void Task_MoveToPlace(void)
{
    COUNT_FRONT_R = 0;
    while(COUNT_FRONT_R != 4)Car_TrackLeft();

    Kinematic_Analysis(0, 0);
}



/*
* @auther: Mrtutu
* @date  ：2019-03-08
*
* @func  : Task_MoveToPlace2
* @param : None
* @return: None
* @note  : None
*
*/
void Task_MoveToPlace2(void)
{
    COUNT_FRONT_R = 0;
    while(COUNT_FRONT_R != 3)Car_TrackLeft();

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
    while(COUNT_FRONT_R != 3)Car_TrackRight();

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
    Kinematic_Analysis(30, -40);

    delay_Ntimes(15);
    Kinematic_Analysis(0, 0);
}




void Task_OLED(void)
{
    OLED_Clear();   // OLED复位

    /* 物料顺序 */
    OLED_ShowCHinese(0, 3, 7);
    OLED_ShowCHinese(16, 3, 8);
    OLED_ShowCHinese(32, 3, 9);
    OLED_ShowCHinese(48, 3, 10);
    OLED_ShowString(64, 3, ":");

    /* 识别二维码 */
    OLED_ShowCHinese(0, 0, 0);
    OLED_ShowCHinese(16, 0, 4);
    OLED_ShowCHinese(32, 0, 5);
    OLED_ShowCHinese(48, 0, 6);
    OLED_ShowString(64, 0, ":");
}


/********************************End of File************************************/
