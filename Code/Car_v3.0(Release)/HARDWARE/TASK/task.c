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

u8 BUFF[16];

extern u16 TIME;
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
}




/*
* @auther: Mrtutu
* @date  ：2019-04-11
*
* @func  : Task_Debug  测试任务
* @param : None
* @return: None
* @note  : None
*
*/
void Task_Debug(void)
{
    /*1. 先左移到线上 再寻迹到PAD 识别二维码*******************************************************************/
    Task_MoveLeft();   // 左平移
    
    usart2_printf("开始识别色块顺序?");

    Task_MovePad();    // 向前移动到Pad以及识别物料颜色顺序
    
    usart2_printf("开始识别二维码?");
    
    Servo3_SpeedRegulation(160, 10);
    
    Servo2_SpeedRegulation(75, 10);
    

    Task_Qrcode();     // 识别二维码
    /********************************************************************/

    /*2. 后退到中间位置 第一次抓取放置*******************************************************************/
    
    usart2_printf("抓取第一个物料?");
    Task_MoveMidMaterial();  // 后退到物料中间位置

    /** 第一次抓取动作 **/
    delay_ms(200);
    if(Take_Material_Order[0] == 3)Action_3();
    else if(Take_Material_Order[0] == 4)Action_4();
    else if(Take_Material_Order[0] == 5)Action_5();
    
    /** 先移动到指定色块线上 **/
    Strategy_Xaxis_Move(4, Place_Process_Order[0], 800);
    delay_Ntimes(1);

    /** 再向加工区移动 **/
    Task_MoveProcess();

    usart2_printf("放置第一个物料?");
    /** 放置动作 **/
    delay_ms(200);
    Action_Place_Process();


    /** 返回物料区 **/
    Task_MoveProcessToMaterial();
    delay_Ntimes(2);
    /********************************************************************/

    /*3. 回到物料区 第2次抓取放置*******************************************************************/
     usart2_printf("抓取第二个物料?");
    /** 移动到物料中间位置抓取 **/
    Strategy_Xaxis_Move(Place_Process_Order[0], 4, 800);

    /** 第二次抓取动作 **/
    delay_ms(200);
    if(Take_Material_Order[1] == 3)Action_3();
    else if(Take_Material_Order[1] == 4)Action_4();
    else if(Take_Material_Order[1] == 5)Action_5();

    /** 移动到指定色环线上 **/
    Strategy_Xaxis_Move(4, Place_Process_Order[1], 800);
    delay_Ntimes(1);

    /** 向加工区移动 **/
    Task_MoveProcess();

     usart2_printf("放置第二个物料?");
    /** 放置动作 **/
    delay_ms(200);
    Action_Place_Process();

    /** 返回物料区 **/
    Task_MoveProcessToMaterial();
    delay_Ntimes(2);
    /********************************************************************/

    /*4. 回到物料区 第3次抓取放置*******************************************************************/
     usart2_printf("抓取第三个物料?");
    /** 移动到物料中间位置抓取 **/
    Strategy_Xaxis_Move(Place_Process_Order[1], 4, 800);
    

    /** 第三次抓取动作 **/
    delay_ms(200);
    if(Take_Material_Order[2] == 3)Action_3();
    else if(Take_Material_Order[2] == 4)Action_4();
    else if(Take_Material_Order[2] == 5)Action_5();


    /** 移动到指定色环线上 **/
    Strategy_Xaxis_Move(4, Place_Process_Order[2], 800);
    delay_Ntimes(1);

    /** 向加工区移动 **/
    Task_MoveProcess();

     usart2_printf("放置第三个物料?");
    /** 放置动作 **/
    delay_ms(200);
    Action_Place_Process();


    /** 后退为WIFI 指令做准备 **/
    Task_PrepareForWIFI();
    /********************************************************************/


    /*5. 接收WIFI指令 第一次抓取放置*******************************************************************/
    usart2_printf("等待WIFI指令?");
    Task_WIFI();  // 接收WIFI指令

    /** 移动到指定色环线 **/
    Strategy_Xaxis_Move(Place_Process_Order[2], Take_Process_Order[0], 900);
    
    delay_Ntimes(1);
    usart2_printf("抓取加工区第一个物料?");
    Task_MoveToGrab();
    /** 抓取动作 **/
    delay_ms(200);
    Action_Take_Process();


    /** 后退到相应色块线上 **/
    Strategy_Yaxis_Move(6, Place_Finish_Order[0], 1000);
    delay_Ntimes(1);

    /** 向成品区移动 **/
    Strategy_Xaxis_Move(Take_Process_Order[0], 6, 800);

    /** 延时定位 **/
    Task_PlaceFinish();

    usart2_printf("放置加工区第一个物料?");
    /** 放置动作 **/
    delay_ms(200);
    Action_Place_Finish();


    /** 退回到下一个色块线 **/
    Strategy_Xaxis_Move(7, Take_Process_Order[1], 900);
    delay_ms(200);

    /** 向加工区移动 **/
    Strategy_Yaxis_Move(Place_Finish_Order[0], 5, 900);

    /********************************************************************/


    /*6. WIFI第二次抓取放置*******************************************************************/
    /** 延时定位**/
    Task_MoveToGrab();


    /** 抓取动作 **/
    usart2_printf("抓取加工区第二个物料?");
    delay_ms(200);
    Action_Take_Process();

    /** 退到指定成品区指定色环线 **/
    Strategy_Yaxis_Move(6, Place_Finish_Order[1], 900);
    delay_Ntimes(1);

    /** 向成品区移动 **/
    Strategy_Xaxis_Move(Take_Process_Order[1], 6, 900);

    /** 延时定位 **/
    Task_PlaceFinish();

    /** 放置动作 **/
    usart2_printf("放置加工区第二个物料?");
    delay_ms(200);
    Action_Place_Finish();


    /** 退回到下一个色块线 **/
    Strategy_Xaxis_Move(7, Take_Process_Order[2], 900);
    delay_Ntimes(1);
    /** 向加工区移动 **/
    Strategy_Yaxis_Move(Place_Finish_Order[1], 5, 900);
    /********************************************************************/
    
    /*7. 第三次抓取放置*******************************************************************/
    /** 延时定位**/
    Task_MoveToGrab();
    delay_Ntimes(1);
    /** 抓取动作 **/
    usart2_printf("抓取加工区第三个物料?");
    delay_ms(200);
    Action_Take_Process();
    
    
    /** 退到指定成品区指定色环线 **/
    Strategy_Yaxis_Move(6, Place_Finish_Order[2], 900);
    delay_Ntimes(1);

    /** 向成品区移动 **/
    Strategy_Xaxis_Move(Take_Process_Order[2], 6, 900);

    /** 延时定位 **/
    Task_PlaceFinish();

    /** 放置动作 **/
    usart2_printf("放置加工区第三个物料?");
    delay_ms(200);
    Action_Place_Finish();
    
    Action_FindBlob();
    
    /** 返回出发区 **/
    usart2_printf("任务结束,回到出发点?");
    Task_MoveBackHome();  

    Strategy_Yaxis_Move(Place_Finish_Order[2], 2, 1000); // 向下平移

    Task_Move1x1();
    
    sprintf(BUFF,"耗时%d分%d秒?",TIME/60,TIME %60);
    
    usart2_printf(BUFF);
    /********************************************************************/
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
        Kinematic_Analysis(-900, 0);
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
        Car_TrackFront1(2000);
        Strategy_MaterialColor();
    }
    COUNT_RIGHT_U = 0;
    while(COUNT_RIGHT_U < 1)
    {
        Car_TrackFront1(700);
        Strategy_MaterialColor();
    }

    Kinematic_Analysis(0, 0);
}




/*
* @auther: Mrtutu
* @date  ：2019-04-11
*
* @func  : Task_MoveMidMaterial  移动到物料中间
* @param : None
* @return: None
* @note  : None
*
*/
void Task_MoveMidMaterial(void)
{
    COUNT_RIGHT_D = 0;
    while(COUNT_RIGHT_D < 2)
    {
        Car_TrackBack1(800);
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
        Car_TrackLeft1(1000);
    }

    COUNT_FRONT_R = 0;
    while(COUNT_FRONT_R < 10) // 12根线到加工区
    {
        Car_TrackLeft1(1700);
    }

    /** 延时定位 **/
    counter = 0;
    TIM_Cmd(TIM11, ENABLE);
    while(counter < 4)
    {
        Car_TrackLeft1(800);
    }
    TIM_Cmd(TIM11, DISABLE);
    Kinematic_Analysis(0, 0);

}




/*
* @auther: Mrtutu
* @date  ：2019-04-11
*
* @func  : Task_PlaceFinish
* @param : None
* @return: None
* @note  : None
*
*/
void Task_PlaceFinish(void)
{
    /** 延时定位 **/
    counter = 0;
    TIM_Cmd(TIM11, ENABLE);
    while(counter < 9)
    {
        Car_TrackFront1(700);
    }
    TIM_Cmd(TIM11, DISABLE);
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
    while(COUNT_FRONT_R < 2) // 12根线到原料区
    {
        Car_TrackRight1(800);
    }
    COUNT_FRONT_R = 0;
    while(COUNT_FRONT_R < 8) // 12根线到原料区
    {
        Car_TrackRight1(1200);
    }
    COUNT_FRONT_R = 0;
    while(COUNT_FRONT_R < 2) // 12根线到原料区
    {
        Car_TrackRight1(800);
    }
    Kinematic_Analysis(0, 0);

}



void Task_PrepareForWIFI(void)
{
    COUNT_FRONT_R = 0;
    while(COUNT_FRONT_R < 2) // 向右移动两根线
    {
        Car_TrackRight1(700);
    }

    Kinematic_Analysis(0, 0);

}



void Task_MoveToGrab(void)
{
    COUNT_FRONT_R = 0;
    while(COUNT_FRONT_R < 2) // 向左移动两根线
    {
        Car_TrackLeft1(800);
    }

    /** 延时定位 **/
    counter = 0;
    TIM_Cmd(TIM11, ENABLE);
    while(counter < 6)
    {
        Car_TrackLeft1(700);
    }
    TIM_Cmd(TIM11, DISABLE);
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
    while(FLAG_QR)
    {
        Strategy_QrcodeSquence();
        Servo4_SpeedRegulation(100, 5);
        delay_ms(200);
        Servo4_SpeedRegulation(105, 5);
        delay_ms(200);
    }
        
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
        Car_TrackBack1(2000);
    }
    COUNT_RIGHT_D = 0;
    while(COUNT_RIGHT_D < 1)
    {
        Car_TrackBack1(800);
    }
    Kinematic_Analysis(0, 0);

}



void Task_Move1x1(void)
{
    /** 延时定位 **/
    counter = 0;
    TIM_Cmd(TIM11, ENABLE);
    while(counter < 4)
    {
        Kinematic_Analysis(1500, -1500);  // 斜走回来
    }
    TIM_Cmd(TIM11, DISABLE);
    
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
