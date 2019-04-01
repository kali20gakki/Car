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
#include "uart5.h"
#include "oled.h"
#include "string.h"

volatile u16 QrcodeNum;
volatile u8 FLAG_QR = 1;


/* 实际提取物料区三个转角  根据实际场地修改*/
const u8 TAKE_LEFT  = 117;
const u8 TAKE_MID   = 90;
const u8 TAKE_RIGHT = 70;

/* 实际放置物料区三个转角  根据实际场地修改*/
const u8 PLACE_LEFT = 44;
const u8 PLACE_MID = 90;
const u8 PLACE_RIGHT = 135;

/* 放置物料对应舵机转角 */
u8 PLACE_R;
u8 PLACE_G;
u8 PLACE_B;

/* 提取物料对应的舵机转角 */
u8 TAKE_R;
u8 TAKE_G;
u8 TAKE_B;

/* 物料提取对应舵机4转角 */
volatile u8 angle_Take[3];

/* 物料放置区对应舵机4转角 */
volatile u8 angle_Place[3];




/*
* @auther: Mrtutu
* @date  ：2019-03-07
*
* @func  : PlaceColor_Init
* @param : None
* @return: None
* @note  : None   放置区颜色顺序    从左到右
*
*/
void PlaceColor_Init(u16 Num)
{
    switch(Num)
    {
    case 123: // 红绿蓝
    {
        PLACE_R = PLACE_LEFT;
        PLACE_G = PLACE_MID;
        PLACE_B = PLACE_RIGHT;
    }
    case 132: // 红蓝绿
    {
        PLACE_R = PLACE_LEFT;
        PLACE_B = PLACE_MID;
        PLACE_G = PLACE_RIGHT;
    }
    case 213: // 绿红蓝
    {
        PLACE_G = PLACE_LEFT;
        PLACE_R = PLACE_MID;
        PLACE_B = PLACE_RIGHT;
    }
    case 231: // 绿蓝红
    {
        PLACE_G = PLACE_LEFT;
        PLACE_B = PLACE_MID;
        PLACE_R = PLACE_RIGHT;
    }
    case 312: // 蓝红绿
    {
        PLACE_B = PLACE_LEFT;
        PLACE_R = PLACE_MID;
        PLACE_G = PLACE_RIGHT;
    }
    case 321: // 蓝绿红
    {
        PLACE_B = PLACE_LEFT;
        PLACE_G = PLACE_MID;
        PLACE_R = PLACE_RIGHT;
    }
    }
}


/*
* @auther: Mrtutu
* @date  ：2019-03-03
*
* @func  : Strategy_MaterialColor
* @param : None
* @return: None
* @note  : None  到达物料区颜色
*
*/
void Strategy_MaterialColor(void)
{
    if(USART3_RX_STA & 0x8000)
    {
        if(strcmp(USART3_RX_BUF, "RGB") == 0)
        {
            TAKE_R = TAKE_LEFT;
            TAKE_G = TAKE_MID;
            TAKE_B = TAKE_RIGHT;

            OLED_ShowCHinese(80, 3, 1);
            OLED_ShowCHinese(96, 3, 2);
            OLED_ShowCHinese(112, 3, 3);

            USART_Cmd(USART3, DISABLE);  //失能串口3
        }
        else if(strcmp(USART3_RX_BUF, "RBG") == 0)
        {
            TAKE_R = TAKE_LEFT;
            TAKE_B = TAKE_MID;
            TAKE_G = TAKE_RIGHT;

            OLED_ShowCHinese(80, 3, 1);
            OLED_ShowCHinese(96, 3, 3);
            OLED_ShowCHinese(112, 3, 2);

            USART_Cmd(USART3, DISABLE);  //失能串口3
        }
        else if(strcmp(USART3_RX_BUF, "GRB") == 0)
        {
            TAKE_G = TAKE_LEFT;
            TAKE_R = TAKE_MID;
            TAKE_B = TAKE_RIGHT;

            OLED_ShowCHinese(80, 3, 2);
            OLED_ShowCHinese(96, 3, 1);
            OLED_ShowCHinese(112, 3, 3);

            USART_Cmd(USART3, DISABLE);  //失能串口3
        }
        else if(strcmp(USART3_RX_BUF, "GBR") == 0)
        {
            TAKE_G = TAKE_LEFT;
            TAKE_B = TAKE_MID;
            TAKE_R = TAKE_RIGHT;

            OLED_ShowCHinese(80, 3, 2);
            OLED_ShowCHinese(96, 3, 3);
            OLED_ShowCHinese(112, 3, 1);

            USART_Cmd(USART3, DISABLE);  //失能串口3
        }
        else if(strcmp(USART3_RX_BUF, "BRG") == 0)
        {
            TAKE_B = TAKE_LEFT;
            TAKE_R = TAKE_MID;
            TAKE_G = TAKE_RIGHT;

            OLED_ShowCHinese(80, 3, 3);
            OLED_ShowCHinese(96, 3, 1);
            OLED_ShowCHinese(112, 3, 2);

            USART_Cmd(USART3, DISABLE);  //失能串口3
        }
        else if(strcmp(USART3_RX_BUF, "BGR") == 0)
        {
            TAKE_B = TAKE_LEFT;
            TAKE_G = TAKE_MID;
            TAKE_R = TAKE_RIGHT;

            OLED_ShowCHinese(80, 3, 2);
            OLED_ShowCHinese(96, 3, 3);
            OLED_ShowCHinese(112, 3, 1);

            USART_Cmd(USART3, DISABLE);  //失能串口3
        }

        USART3_RX_STA = 0; //重置接收位
    }
}



/*
* @auther: Mrtutu
* @date  ：2019-03-03
*
* @func  : Strategy_QrcodeSquence
* @param : None
* @return: None
* @note  : None   二维码识别后 物料顺序
*
*/
void Strategy_QrcodeSquence(void)
{
    if(UART5_RX_STA & 0x8000)
    {
        QrcodeNum = atoi(UART5_RX_BUF);
        switch(QrcodeNum)
        {
        case 123 :  // 红绿蓝
        {
            angle_Take[0] = TAKE_R;
            angle_Take[1] = TAKE_G;
            angle_Take[2] = TAKE_B;

            angle_Place[0] = PLACE_R;
            angle_Place[1] = PLACE_G;
            angle_Place[2] = PLACE_B;

            OLED_ShowCHinese(80, 0, 1);
            OLED_ShowCHinese(96, 0, 2);
            OLED_ShowCHinese(112, 0, 3);

            USART_Cmd(UART5, DISABLE);  //失能串口5
            FLAG_QR = 0;
            break;
        }
        case 132 :  // 红蓝绿
        {
            angle_Take[0] = TAKE_R;
            angle_Take[1] = TAKE_B;
            angle_Take[2] = TAKE_G;

            angle_Place[0] = PLACE_R;
            angle_Place[1] = PLACE_B;
            angle_Place[2] = PLACE_G;

            OLED_ShowCHinese(80, 0, 1);
            OLED_ShowCHinese(96, 0, 3);
            OLED_ShowCHinese(112, 0, 2);

            USART_Cmd(UART5, DISABLE);  //失能串口5
            FLAG_QR = 0;
            break;
        }
        case 213 :  // 绿红蓝
        {
            angle_Take[0] = TAKE_G;
            angle_Take[1] = TAKE_R;
            angle_Take[2] = TAKE_B;

            angle_Place[0] = PLACE_G;
            angle_Place[1] = PLACE_R;
            angle_Place[2] = PLACE_B;

            OLED_ShowCHinese(80, 0, 2);
            OLED_ShowCHinese(96, 0, 1);
            OLED_ShowCHinese(112, 0, 3);

            USART_Cmd(UART5, DISABLE);  //失能串口5
            FLAG_QR = 0;
            break;
        }
        case 231 :  // 绿蓝红
        {
            angle_Take[0] = TAKE_G;
            angle_Take[1] = TAKE_B;
            angle_Take[2] = TAKE_R;

            angle_Place[0] = PLACE_G;
            angle_Place[1] = PLACE_B;
            angle_Place[2] = PLACE_R;

            OLED_ShowCHinese(80, 0, 2);
            OLED_ShowCHinese(96, 0, 3);
            OLED_ShowCHinese(112, 0, 1);

            USART_Cmd(UART5, DISABLE);  //失能串口5
            FLAG_QR = 0;
            break;
        }
        case 312 :  // 蓝红绿
        {
            angle_Take[0] = TAKE_B;
            angle_Take[1] = TAKE_R;
            angle_Take[2] = TAKE_G;

            angle_Place[0] = PLACE_B;
            angle_Place[1] = PLACE_R;
            angle_Place[2] = PLACE_G;

            OLED_ShowCHinese(80, 0, 3);
            OLED_ShowCHinese(96, 0, 1);
            OLED_ShowCHinese(112, 0, 2);

            USART_Cmd(UART5, DISABLE);  //失能串口5
            FLAG_QR = 0;
            break;
        }
        case 321 :  // 蓝绿红
        {
            angle_Take[0] = TAKE_B;
            angle_Take[1] = TAKE_G;
            angle_Take[2] = TAKE_R;

            angle_Place[0] = PLACE_B;
            angle_Place[1] = PLACE_G;
            angle_Place[2] = PLACE_R;

            OLED_ShowCHinese(80, 0, 3);
            OLED_ShowCHinese(96, 0, 2);
            OLED_ShowCHinese(112, 0, 1);

            USART_Cmd(UART5, DISABLE);  //失能串口5
            FLAG_QR = 0;
            break;
        }
        default :   // 错误
        {
            angle_Take[0] = TAKE_R;
            angle_Take[1] = TAKE_G;
            angle_Take[2] = TAKE_B;

            angle_Place[0] = PLACE_R;
            angle_Place[1] = PLACE_G;
            angle_Place[2] = PLACE_B;

            OLED_ShowCHinese(80, 0, 11);
            OLED_ShowCHinese(96, 0, 0);
            OLED_ShowCHinese(112, 0, 4);
            break;
        }
        }

        UART5_RX_STA = 0; //重置接收位
    }

}
/********************************End of File************************************/
