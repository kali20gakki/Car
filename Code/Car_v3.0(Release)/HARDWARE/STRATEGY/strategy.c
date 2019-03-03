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
#include "oled.h"
#include "string.h"

volatile u16 QrcodeNum;

/* 实际提取物料区三个转角  根据实际场地修改*/
const u8 TAKE_LEFT  = 45;
const u8 TAKE_MID   = 90;
const u8 TAKE_RIGHT = 135;

/* 实际放置物料区三个转角  根据实际场地修改*/
const u8 PLACE_LEFT = 0;
const u8 PLACE_MID = 0;
const u8 PLACE_RIGHT = 0;

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
    /* 物料顺序 */
    OLED_ShowCHinese(0, 3, 7);
    OLED_ShowCHinese(16, 3, 8);
    OLED_ShowCHinese(32, 3, 9);
    OLED_ShowCHinese(48, 3, 10);
    OLED_ShowString(64, 3, ":");

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
        }
        else if(strcmp(USART3_RX_BUF, "RBG") == 0)
        {
            TAKE_R = TAKE_LEFT;
            TAKE_B = TAKE_MID;
            TAKE_G = TAKE_RIGHT;

            OLED_ShowCHinese(80, 3, 1);
            OLED_ShowCHinese(96, 3, 3);
            OLED_ShowCHinese(112, 3, 2);
        }
        else if(strcmp(USART3_RX_BUF, "GRB") == 0)
        {
            TAKE_G = TAKE_LEFT;
            TAKE_R = TAKE_MID;
            TAKE_B = TAKE_RIGHT;

            OLED_ShowCHinese(80, 3, 2);
            OLED_ShowCHinese(96, 3, 1);
            OLED_ShowCHinese(112, 3, 3);
        }
        else if(strcmp(USART3_RX_BUF, "GBR") == 0)
        {
            TAKE_G = TAKE_LEFT;
            TAKE_B = TAKE_MID;
            TAKE_R = TAKE_RIGHT;

            OLED_ShowCHinese(80, 3, 2);
            OLED_ShowCHinese(96, 3, 3);
            OLED_ShowCHinese(112, 3, 1);
        }
        else if(strcmp(USART3_RX_BUF, "BRG") == 0)
        {
            TAKE_B = TAKE_LEFT;
            TAKE_R = TAKE_MID;
            TAKE_G = TAKE_RIGHT;

            OLED_ShowCHinese(80, 3, 3);
            OLED_ShowCHinese(96, 3, 1);
            OLED_ShowCHinese(112, 3, 2);
        }
        else if(strcmp(USART3_RX_BUF, "BGR") == 0)
        {
            TAKE_B = TAKE_LEFT;
            TAKE_G = TAKE_MID;
            TAKE_R = TAKE_RIGHT;

            OLED_ShowCHinese(80, 3, 2);
            OLED_ShowCHinese(96, 3, 3);
            OLED_ShowCHinese(112, 3, 1);
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
    /* 识别二维码 */
    OLED_ShowCHinese(0, 0, 0); 
    OLED_ShowCHinese(16, 0, 4);
    OLED_ShowCHinese(32, 0, 5);
    OLED_ShowCHinese(48, 0, 6);
    OLED_ShowString(64, 0, ":");
    if(USART3_RX_STA & 0x8000)
    {
        QrcodeNum = atoi(USART3_RX_BUF);
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
                break;
            }
            default :   // 错误
            {
                angle_Take[0] = 90;
                angle_Take[1] = 90;
                angle_Take[2] = 90;
                
                OLED_ShowCHinese(80, 0, 11);
                OLED_ShowCHinese(96, 0, 12);
                break;
            }
        }
        
        USART3_RX_STA = 0; //重置接收位
    }

}
/********************************End of File************************************/
