/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : strategy.c
** @auther  : Mrtutu
** @date    : 2019-02-26
** @describe: 小车路径策略
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "strategy.h"
#include "servo.h"
#include "usart3.h"
#include "usart2.h"
#include "uart5.h"
#include "oled.h"
#include "string.h"
#include "beep.h"
#include "math.h"
#include "action.h"

volatile u16 QrcodeNum;
volatile u8 FLAG_QR = 1;
volatile u8 FLAG_WIFI = 1;
/*********************************************************************************************/
/** 物料提取区物料位置 **/
volatile int Position_R;
volatile int Position_G;
volatile int Position_B;

/* Pad 的位置 */
const int Position_Pad = 6;


/** 提取物料顺序 **/
volatile int Take_Material_Order[3];

/** 放置物料到加工区顺序 **/
volatile int Place_Process_Order[3];

/** 从加工区提取物料到成品区 **/
volatile int Take_Process_Order[3];

/** 放置到成品区顺序 **/
volatile int Place_Finish_Order[3];

/** 加工区色环顺序 **/
volatile int Process_R;
volatile int Process_G;
volatile int Process_B;

/** 成品区色环顺序 **/
volatile int Finish_R;
volatile int Finish_G;
volatile int Finish_B;



/** 抓取动作函数指针 **/
void(*pAction1)(void);
void(*pAction2)(void);
void(*pAction3)(void);

/*********************************************************************************************/

extern u8 COUNT_FRONT_L;
extern u8 COUNT_FRONT_R;
extern u8 COUNT_RIGHT_U;
extern u8 COUNT_RIGHT_D;




/*
* @auther: Mrtutu
* @date  ：2019-04-04
*
* @func  : Strategy_ProcessColorInit   加工区色环颜色顺序 从左到右
* @param : num: [输入/出]  场地默认：1
* @return: None
* @note  : None
*
*/
void Strategy_ProcessColorInit(u16 num)
{
    switch(num)  // 颜色顺序
    {
    case 1 :   // RGB
    {
        Process_R = 3;
        Process_G = 4;
        Process_B = 5;
        break;
    }
    case 2 :   // RBG
    {
        Process_R = 3;
        Process_B = 4;
        Process_G = 5;
        break;
    }
    case 3 :   // GRB
    {
        Process_G = 3;
        Process_R = 4;
        Process_B = 5;
        break;
    }
    case 4 :   // GBR
    {
        Process_G = 3;
        Process_B = 4;
        Process_R = 5;
        break;
    }
    case 5 :   // BRG
    {
        Process_B = 3;
        Process_R = 4;
        Process_G = 5;
        break;
    }
    case 6 :   // BGR
    {
        Process_B = 3;
        Process_G = 4;
        Process_R = 5;
        break;
    }
    }
}



/*
* @auther: Mrtutu
* @date  ：2019-04-04
*
* @func  : Strategy_FinishColorInit    成品区色环顺序 从上到下
* @param : num: [输入/出]    默认：1
* @return: None
* @note  : None
*
*/
void Strategy_FinishColorInit(u16 num)
{
    switch(num) // 颜色顺序
    {
    case 1 :  // RGB
    {
        Finish_R = 3;
        Finish_G = 4;
        Finish_B = 5;
        break;
    }
    case 2 :  // RBG
    {
        Finish_R = 3;
        Finish_B = 4;
        Finish_G = 5;
        break;
    }
    case 3 :  // GRB
    {
        Finish_G = 3;
        Finish_R = 4;
        Finish_B = 5;
        break;
    }
    case 4 :  // GBR
    {
        Finish_G = 3;
        Finish_B = 4;
        Finish_R = 5;
        break;
    }
    case 5 :  // BRG
    {
        Finish_B = 3;
        Finish_R = 4;
        Finish_G = 5;
        break;
    }
    case 6 :  // BGR
    {
        Finish_B = 3;
        Finish_G = 4;
        Finish_R = 5;
        break;
    }
    }
}


/*
* @auther: Mrtutu
* @date  ：2019-04-04
*
* @func  : Strategy_MaterialColor   识别物料放置区颜色顺序  从左到右
* @param : None
* @return: None
* @note  : None
*
*/
void Strategy_MaterialColor(void)
{
    if(USART3_RX_STA & 0x8000)
    {
        if(strcmp(USART3_RX_BUF, "RGB") == 0)
        {
            Position_R = 3;
            Position_G = 4;
            Position_B = 5;

            OLED_ShowCHinese(80, 3, 1);
            OLED_ShowCHinese(96, 3, 2);
            OLED_ShowCHinese(112, 3, 3);
            
            usart2_printf("识别到的物料颜色 : RGB?");
            Beep_ring();
        }
        else if(strcmp(USART3_RX_BUF, "RBG") == 0)
        {
            Position_R = 3;
            Position_G = 5;
            Position_B = 4;

            OLED_ShowCHinese(80, 3, 1);
            OLED_ShowCHinese(96, 3, 3);
            OLED_ShowCHinese(112, 3, 2);
            
            usart2_printf("识别到的物料颜色 : RBG?");
            Beep_ring();
        }
        else if(strcmp(USART3_RX_BUF, "BGR") == 0)
        {
            Position_R = 5;
            Position_G = 4;
            Position_B = 3;

            OLED_ShowCHinese(80, 3, 3);
            OLED_ShowCHinese(96, 3, 2);
            OLED_ShowCHinese(112, 3, 1);
            
            usart2_printf("识别到的物料颜色 : BGR?");
            Beep_ring();
        }
        else if(strcmp(USART3_RX_BUF, "BRG") == 0)
        {
            Position_R = 4;
            Position_G = 5;
            Position_B = 3;

            OLED_ShowCHinese(80, 3, 2);
            OLED_ShowCHinese(96, 3, 3);
            OLED_ShowCHinese(112, 3, 1);
            
            usart2_printf("识别到的物料颜色 : BRG?");
            Beep_ring();
        }
        else if(strcmp(USART3_RX_BUF, "GBR") == 0)
        {
            Position_R = 5;
            Position_G = 3;
            Position_B = 4;

            OLED_ShowCHinese(80, 3, 3);
            OLED_ShowCHinese(96, 3, 1);
            OLED_ShowCHinese(112, 3, 2);
            
            usart2_printf("识别到的物料颜色 : GBR?");
            Beep_ring();
        }
        else if(strcmp(USART3_RX_BUF, "GRB") == 0)
        {
            Position_R = 4;
            Position_G = 3;
            Position_B = 5;

            OLED_ShowCHinese(80, 3, 2);
            OLED_ShowCHinese(96, 3, 1);
            OLED_ShowCHinese(112, 3, 3);
            
            usart2_printf("识别到的物料颜色 : GRB?");
            Beep_ring();
        }

        USART3_RX_STA = 0; //重置接收位
    }
}




/*
* @auther: Mrtutu
* @date  ：2019-04-04
*
* @func  : Strategy_QrcodeSquence
* @param : None
* @return: None
* @note  : None
*
*/
void Strategy_QrcodeSquence(void)
{
    if(USART3_RX_STA & 0x8000)
    {
        QrcodeNum = atoi(USART3_RX_BUF);
        switch(QrcodeNum)
        {
        case 123 :  // 红绿蓝
        {
            Take_Material_Order[0] = Position_R;
            Take_Material_Order[1] = Position_G;
            Take_Material_Order[2] = Position_B;
            
            Place_Process_Order[0] = Process_R;
            Place_Process_Order[1] = Process_G;
            Place_Process_Order[2] = Process_B;
            
            pAction1 = Action_3;
            pAction2 = Action_4;
            pAction3 = Action_5;

            OLED_ShowCHinese(80, 0, 1);
            OLED_ShowCHinese(96, 0, 2);
            OLED_ShowCHinese(112, 0, 3);
            
            usart2_printf("识别到二维码 : 123?");
            FLAG_QR = 0;
            Beep_ring();

            break;
        }
        case 132 :  // 红蓝绿
        {
            Take_Material_Order[0] = Position_R;
            Take_Material_Order[1] = Position_B;
            Take_Material_Order[2] = Position_G;
            
            Place_Process_Order[0] = Process_R;
            Place_Process_Order[1] = Process_B;
            Place_Process_Order[2] = Process_G;
            
            pAction1 = Action_3;
            pAction2 = Action_4;
            pAction3 = Action_5;

            OLED_ShowCHinese(80, 0, 1);
            OLED_ShowCHinese(96, 0, 3);
            OLED_ShowCHinese(112, 0, 2);
            
            usart2_printf("识别到二维码 : 132?");
            
            FLAG_QR = 0;
            Beep_ring();

            break;
        }
        case 213 :  // 绿红蓝
        {
            Take_Material_Order[0] = Position_G;
            Take_Material_Order[1] = Position_R;
            Take_Material_Order[2] = Position_B;
            
            Place_Process_Order[0] = Process_G;
            Place_Process_Order[1] = Process_R;
            Place_Process_Order[2] = Process_B;

            OLED_ShowCHinese(80, 0, 2);
            OLED_ShowCHinese(96, 0, 1);
            OLED_ShowCHinese(112, 0, 3);
            
            usart2_printf("识别到二维码 : 213?");
            
            FLAG_QR = 0;
            Beep_ring();

            break;
        }
        case 231 :  // 绿蓝红
        {
            Take_Material_Order[0] = Position_G;
            Take_Material_Order[1] = Position_B;
            Take_Material_Order[2] = Position_R;
            
            Place_Process_Order[0] = Process_G;
            Place_Process_Order[1] = Process_B;
            Place_Process_Order[2] = Process_R;

            OLED_ShowCHinese(80, 0, 2);
            OLED_ShowCHinese(96, 0, 3);
            OLED_ShowCHinese(112, 0, 1);
            
            usart2_printf("识别到二维码 : 231?");
            
            FLAG_QR = 0;
            Beep_ring();

            break;
        }
        case 312 :  // 蓝红绿
        {
            Take_Material_Order[0] = Position_B;
            Take_Material_Order[1] = Position_R;
            Take_Material_Order[2] = Position_G;
            
            Place_Process_Order[0] = Process_B;
            Place_Process_Order[1] = Process_R;
            Place_Process_Order[2] = Process_G;

            OLED_ShowCHinese(80, 0, 3);
            OLED_ShowCHinese(96, 0, 1);
            OLED_ShowCHinese(112, 0, 2);
            
            usart2_printf("识别到二维码 : 312?");

            FLAG_QR = 0;
            Beep_ring();
            
            break;
        }
        case 321 :  // 蓝绿红
        {
            Take_Material_Order[0] = Position_B;
            Take_Material_Order[1] = Position_G;
            Take_Material_Order[2] = Position_R;
            
            Place_Process_Order[0] = Process_B;
            Place_Process_Order[1] = Process_G;
            Place_Process_Order[2] = Process_R;

            OLED_ShowCHinese(80, 0, 3);
            OLED_ShowCHinese(96, 0, 2);
            OLED_ShowCHinese(112, 0, 1);
            
            usart2_printf("识别到二维码 : 321?");
            
            FLAG_QR = 0;
            Beep_ring();

            break;
        }
        }
        USART3_RX_STA = 0; //重置接收位
    }
}



void Strategy_WIFISquence(void)
{
    if(USART2_RX_STA & 0x8000)
    {
        if(strcmp(USART2_RX_BUF, "123") == 0)  // RGB
        {
            Take_Process_Order[0] = Process_R;
            Take_Process_Order[1] = Process_G;
            Take_Process_Order[2] = Process_B;
            
            Place_Finish_Order[0] = Finish_R;
            Place_Finish_Order[1] = Finish_G;
            Place_Finish_Order[2] = Finish_B;
            
            
            FLAG_WIFI = 0;
            
            OLED_ShowString(96,6,"123");
            usart2_printf("接收到WIFI : 123?");
            Beep_ring();
            
        }
        else if(strcmp(USART2_RX_BUF, "132") == 0) // RBG
        {
            Take_Process_Order[0] = Process_R;
            Take_Process_Order[1] = Process_B;
            Take_Process_Order[2] = Process_G;
            
            Place_Finish_Order[0] = Finish_R;
            Place_Finish_Order[1] = Finish_B;
            Place_Finish_Order[2] = Finish_G;
            
            FLAG_WIFI = 0;
            OLED_ShowString(96,6,"132");
            usart2_printf("接收到WIFI : 132?");
            Beep_ring();
        }
        else if(strcmp(USART2_RX_BUF, "213") == 0) // GRB
        {
            Take_Process_Order[0] = Process_G;
            Take_Process_Order[1] = Process_R;
            Take_Process_Order[2] = Process_B;
            
            Place_Finish_Order[0] = Finish_G;
            Place_Finish_Order[1] = Finish_R;
            Place_Finish_Order[2] = Finish_B;
            
            FLAG_WIFI = 0;
            
            OLED_ShowString(96,6,"213");
            usart2_printf("接收到WIFI : 213?");
            Beep_ring();
        }
        else if(strcmp(USART2_RX_BUF, "231") == 0) // GBR
        {
            Take_Process_Order[0] = Process_G;
            Take_Process_Order[1] = Process_B;
            Take_Process_Order[2] = Process_R;
            
            Place_Finish_Order[0] = Finish_G;
            Place_Finish_Order[1] = Finish_B;
            Place_Finish_Order[2] = Finish_R;
            
            FLAG_WIFI = 0;
            
            OLED_ShowString(96,6,"231");
            usart2_printf("接收到WIFI : 231?");
            Beep_ring();
        }
        else if(strcmp(USART2_RX_BUF, "312") == 0) // BRG
        {
            Take_Process_Order[0] = Process_B;
            Take_Process_Order[1] = Process_R;
            Take_Process_Order[2] = Process_G;
            
            Place_Finish_Order[0] = Finish_B;
            Place_Finish_Order[1] = Finish_R;
            Place_Finish_Order[2] = Finish_G;
            
            FLAG_WIFI = 0;
            OLED_ShowString(96,6,"312");
            usart2_printf("接收到WIFI : 312?");
            Beep_ring();
        }
        else if(strcmp(USART2_RX_BUF, "321") == 0) // BGR
        {
            Take_Process_Order[0] = Process_B;
            Take_Process_Order[1] = Process_G;
            Take_Process_Order[2] = Process_R;
            
            Place_Finish_Order[0] = Finish_B;
            Place_Finish_Order[1] = Finish_G;
            Place_Finish_Order[2] = Finish_R;
            
            FLAG_WIFI = 0;
            OLED_ShowString(96,6,"321");
            usart2_printf("接收到WIFI : 321?");
            Beep_ring();
        }
        
        USART2_RX_STA = 0; //重置接收位
    }
}





void Strategy_Xaxis_Move(int x0, int x1, int V)
{
    int TEMP;
    TEMP = x1 - x0;
    if(TEMP > 0)  // 向x轴正向移动(向前寻迹)  ---  COUNT_RIGHT_U
    {
        COUNT_RIGHT_U = 0;
        while(COUNT_RIGHT_U < TEMP)
        {
            Car_TrackFront1(V);
        }
        
        Kinematic_Analysis(0, 0);  // 停止
    }
    else if(TEMP < 0) // 向x轴负向移动(向后寻迹)  ---  COUNT_RIGHT_D
    {
        COUNT_RIGHT_D = 0;
        while(COUNT_RIGHT_D < abs(TEMP))
        {
            Car_TrackBack1(V);
        }
        
        Kinematic_Analysis(0, 0);  // 停止
    }
    else if(TEMP == 0) // 不移动
    {
        // pass
        Kinematic_Analysis(0, 0);  // 停止
    }
}



void Strategy_Yaxis_Move(int y0, int y1, int V)
{
    int TEMP;
    TEMP = y1 - y0;
    if(TEMP > 0)  // 向y轴的正向移动（向左寻迹）  ---  COUNT_FRONT_L
    {
        COUNT_FRONT_L = 0;
        while(COUNT_FRONT_L < 2*(TEMP+1))
        {
            Car_TrackLeft1(V);
        }
        
        Kinematic_Analysis(0, 0);  // 停止
    }
    else if(TEMP < 0)  // 向y轴的负向移动（向右寻迹）  ---  COUNT_FRONT_R
    {
        COUNT_FRONT_R = 0;
        while(COUNT_FRONT_R < 2*(abs(TEMP)+1))
        {
            Car_TrackRight1(V);
        }
        Kinematic_Analysis(0, 0);  // 停止
    }
    else if(TEMP == 0)
    {
        // Pass
        Kinematic_Analysis(0, 0);  // 停止
    }
    
}

/********************************End of File************************************/
