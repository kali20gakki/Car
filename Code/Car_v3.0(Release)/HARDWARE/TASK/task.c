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
#include "math.h"
#include "beep.h"
#include "includes.h"

int num;
extern u16 counter;


extern volatile int Process_R;
extern volatile int Process_G;
extern volatile int Process_B;

extern volatile int Position_R;
extern volatile int Position_G;
extern volatile int Position_B;

extern volatile int Finish_R;
extern volatile int Finish_G;
extern volatile int Finish_B;



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
void Task_Release(void)
{
    /** 1. 向左平移到线上 **/
    Task_MoveLeft();

    /** 2. 向前移动到Pad以及识别物料颜色顺序 **/
    Task_MovePad();


    /** 3. 识别二维码 **/
    Servo1_SpeedRegulation(60,30);
    Servo3_SpeedRegulation(145, 10);
    Servo2_SpeedRegulation(115, 10);//60
    Servo4_SpeedRegulation(40, 10);
    Task_Qrcode();
    Servo1_SpeedRegulation(60,30);
    Servo2_SpeedRegulation(60, 10);//60
    Servo3_SpeedRegulation(180, 10);
    Servo4_SpeedRegulation(40, 10);


    delay_Ntimes(2);

    /** 4. 向后移动到第一个抓取位置以及抓取 **/
    Strategy_Xaxis_Move(Position_Pad, Take_Material_Order[0], 20);

    // 第一次抓取动作
    delay_Ntimes(2);

    /** 5. 先移动到指定色环线上 再向加工区左移**/
    Strategy_Xaxis_Move(Take_Material_Order[0], Place_Process_Order[0], 20); // 先移动到指定色环线上

    Task_MoveProcess();  // 向加工区移动

    // 第一次放置动作
    delay_Ntimes(2);

    /** 6. 先原路返回再移动到抓取位置 **/
    Task_MoveProcessToMaterial();  // 原路返回

    Strategy_Xaxis_Move(Place_Process_Order[0], Take_Material_Order[1], 20); // 移动到指定色环线上

    // 第二次抓取动作
    delay_Ntimes(2);

    /** 7. 先移动到指定色环线上 再向加工区左移**/
    Strategy_Xaxis_Move(Take_Material_Order[1], Place_Process_Order[1], 20); // 先移动到指定色环线上

    Task_MoveProcess();  // 向加工区移动

    // 第二次放置动作
    delay_Ntimes(2);


    /** 8. 先原路返回再移动到抓取位置 **/
    Task_MoveProcessToMaterial();  // 原路返回

    Strategy_Xaxis_Move(Place_Process_Order[1], Take_Material_Order[2], 20); // 移动到指定色环线上

    // 第三次抓取动作
    delay_Ntimes(2);


    /** 9. 先移动到指定色环线上 再向加工区左移**/
    Strategy_Xaxis_Move(Take_Material_Order[2], Place_Process_Order[2], 20); // 先移动到指定色环线上

    Task_MoveProcess();  // 向加工区移动

    // 第三次放置动作
    delay_Ntimes(2);


    /** 10. 向外移动两根线为WIFI指令做准备 **/
    Task_PrepareForWIFI();


    /** 11. 等待WIFI指令 **/
    Task_WIFI();


    /** 12. 先移动到对应色块线上再向左平移 **/
    Strategy_Xaxis_Move(Place_Process_Order[2], Take_Process_Order[0], 25); // 先移动到指定色环线上

    Task_MoveToGrab();  // 再左平移

    // 第一次抓取动作
    delay_Ntimes(2);


    /** 13. 先移动到指定色块线上再向前移动 **/
    Strategy_Yaxis_Move(6, Place_Finish_Order[0], 20); // 移动到指定色块线

    Strategy_Xaxis_Move(Take_Process_Order[0], 6, 25); // 向前移动

    Car_TrackFront1(10);  // 速度应该较慢 后面加了延时无寻迹功能
    delay_ms(1000);   // 适当延时
    delay_ms(1000);   // 适当延时

    Kinematic_Analysis(0, 0);  // 停止

    // 第一次放置动作
    delay_Ntimes(2);

    /** 14. 先回到指定色块线上再向左平移**/
    Strategy_Xaxis_Move(Position_Pad, Take_Process_Order[0], 20); // 移动到指定色块线

    Strategy_Yaxis_Move(Place_Finish_Order[0], 5, 20); // 向左平移到加工区

    Task_MoveToGrab();  // 再左平移

    // 第二次抓取动作
    delay_Ntimes(2);


    /** 15. 先后退到指定色环线上再右移 **/
    Strategy_Yaxis_Move(6, Place_Finish_Order[1], 20); // 先后退到指定色环线上

    Strategy_Xaxis_Move(Take_Process_Order[1], Position_Pad, 25); // 再右移


    Car_TrackFront1(10);  // 速度应该较慢 后面加了延时无寻迹功能
    delay_ms(1000);   // 适当延时
    delay_ms(1000);   // 适当延时

    Kinematic_Analysis(0, 0);  // 停止
    // 第二次放置动作
    delay_Ntimes(2);


    /** 16. 先回到指定色块线上再向左平移 **/
    Strategy_Xaxis_Move(7, Take_Process_Order[2], 20); // 移动到指定色块线

    Strategy_Yaxis_Move(Place_Finish_Order[1], 5, 20); // 向左平移到加工区

    Task_MoveToGrab();  // 再左平移

    // 第三次抓取动作
    delay_Ntimes(2);

    /** 17. 先后退到指定色环线上再右移**/
    Strategy_Yaxis_Move(6, Place_Finish_Order[2], 20); // 先后退到指定色环线上

    Strategy_Xaxis_Move(Take_Process_Order[2], Position_Pad, 25); // 再右移


    Car_TrackFront1(15);  // 速度应该较慢 后面加了延时无寻迹功能
    delay_ms(1000);   // 适当延时
    delay_ms(1000);   // 适当延时

    Kinematic_Analysis(0, 0);  // 停止

    // 第三次放置动作
    delay_Ntimes(2);


    /** 18. 回到坐标1的线再向下平移 **/
    Task_MoveBackHome();

    Strategy_Yaxis_Move(Place_Finish_Order[2], 2, 20); // 向下平移

    Task_Move1x1();
}



void Task_Test()
{
    Task_MoveLeft();         // 左移到线上
    Task_MovePad();          // 寻迹到Pad

    Task_Qrcode();           // 识别二维码
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
        Kinematic_Analysis(-18, 0);
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
    while(COUNT_RIGHT_U < 5)
    {
        Car_TrackFront1(40);
        Strategy_MaterialColor();
    }
    COUNT_RIGHT_U = 0;
    while(COUNT_RIGHT_U < 1)
    {
        Car_TrackFront1(20);
        Strategy_MaterialColor();
    }

    Kinematic_Analysis(0, 0);
}




/*
* @auther: Mrtutu
* @date  ：2019-04-06
*
* @func  : Task_MoveProcess
* @param : None
* @return: None
* @note  : None   向左平移到加工区
*
*/
void Task_MoveProcess(void)
{
    COUNT_FRONT_R = 0;
    while(COUNT_FRONT_R < 2) // 12根线到加工区
    {
        Car_TrackLeft1(20);
    }
    
    COUNT_FRONT_R = 0;
    while(COUNT_FRONT_R < 10) // 12根线到加工区
    {
        Car_TrackLeft1(40);
    }

    COUNT_FRONT_L = 0;
    while(COUNT_FRONT_L < 1) // 12根线到加工区
    {
        Car_TrackLeft1(20);
    }
    Kinematic_Analysis(0, 0);

}



/*
* @auther: Mrtutu
* @date  ：2019-04-06
*
* @func  : Task_MoveProcessToMaterial
* @param : None
* @return: None
* @note  : None   从加工区向右平移到物料区
*
*/
void Task_MoveProcessToMaterial(void)
{
    COUNT_FRONT_R = 0;
    while(COUNT_FRONT_R < 10) // 12根线到原料区
    {
        Car_TrackRight1(40);
    }
    COUNT_FRONT_R = 0;
    while(COUNT_FRONT_R < 2) // 12根线到原料区
    {
        Car_TrackRight1(15);
    }
    Kinematic_Analysis(0, 0);

}



void Task_PrepareForWIFI(void)
{
    COUNT_FRONT_R = 0;
    while(COUNT_FRONT_R < 2) // 向右移动两根线
    {
        Car_TrackRight1(20);
    }

    Kinematic_Analysis(0, 0);

}



void Task_MoveToGrab(void)
{
    COUNT_FRONT_R = 0;
    while(COUNT_FRONT_R < 2) // 向左移动两根线
    {
        Car_TrackLeft1(20);
    }

    COUNT_FRONT_L = 0;
    while(COUNT_FRONT_L < 1) // 向左移动两根线
    {
        Car_TrackLeft1(20);
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


void Task_WIFI(void)
{
    Kinematic_Analysis(0, 0);
    while(FLAG_WIFI)Strategy_WIFISquence();
}




void Task_MoveBackHome(void)
{
    COUNT_RIGHT_D = 0;
    while(COUNT_RIGHT_D < 5)
    {
        Car_TrackBack1(40);
    }
    COUNT_RIGHT_D = 0;
    while(COUNT_RIGHT_D < 1)
    {
        Car_TrackBack1(20);
    }
    Kinematic_Analysis(0, 0);
    
}



void Task_Move1x1(void)
{
    Kinematic_Analysis(15, -15);  // 斜走回来

    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    Kinematic_Analysis(0, 0); // 停止
}



void Task_SetAlarm(u16 s)
{
    counter = 0;
    while(1)
    {
        if(counter > s)
        {
            Beep_ring();
            break;
        }
    }
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

    /* WIFI */
    OLED_ShowString(0, 6, "WIFI");
    OLED_ShowCHinese(32, 6, 13);
    OLED_ShowCHinese(48, 6, 14);
    OLED_ShowString(64, 6, ":");

}


/********************************End of File************************************/
