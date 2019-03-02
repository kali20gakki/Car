/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : message.c
** @auther  : Mrtutu
** @date    : 2019-02-17
** @describe: OLED显示信息
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "message.h"
#include "oled.h"
#include "string.h"
#include "usart3.h"
#include "strategy.h"
#include "string.h"


extern int Encoder_A;
extern int Encoder_B;
extern int Encoder_C;
extern int Encoder_D;

extern u16 QrcodeNum;

unsigned char i;          //计数变量
unsigned char Send_Count; //串口需要发送的数据个数



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : OLED_show_Qcode OLED显示二维码识别结果
* @param : None
* @return: None
* @note  : None
*
*/
void OLED_Show_Qcode(void)
{
    u8 flag_Qc = 1; //二维码信息比较结果 标志位
    //OLED_Clear();

    OLED_ShowCHinese(0, 0, 0); //识别结果：
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
                OLED_ShowCHinese(80, 0, 1);
                OLED_ShowCHinese(96, 0, 2);
                OLED_ShowCHinese(112, 0, 3);
            }
            case 132 :  // 红蓝绿
            {
                OLED_ShowCHinese(80, 0, 1);
                OLED_ShowCHinese(96, 0, 3);
                OLED_ShowCHinese(112, 0, 2);
            } 
            case 213 :  // 绿红蓝
            {
                OLED_ShowCHinese(80, 0, 2);
                OLED_ShowCHinese(96, 0, 1);
                OLED_ShowCHinese(112, 0, 3);
            }
            case 231 :  // 绿蓝红
            {
                OLED_ShowCHinese(80, 0, 2);
                OLED_ShowCHinese(96, 0, 3);
                OLED_ShowCHinese(112, 0, 1);
            }
            case 312 :  // 蓝红绿
            {
                OLED_ShowCHinese(80, 0, 3);
                OLED_ShowCHinese(96, 0, 1);
                OLED_ShowCHinese(112, 0, 2);
            }
            case 321 :  // 蓝绿红
            {
                OLED_ShowCHinese(80, 0, 3);
                OLED_ShowCHinese(96, 0, 2);
                OLED_ShowCHinese(112, 0, 1);
            }
            default :   // 错误
            {
                
            }

        }

        USART3_RX_STA = 0; //重置接收位
        Beep_ring();       //通信成功一次 蜂鸣器响一次
    }
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : OLED_show_colorSort OLED 显示 物料识别顺序
* @param : None
* @return: None
* @note  : None
*
*/
void OLED_Show_ColorSort(void)
{
    u8 flag_sort = 1; //二维码信息比较结果 标志位

    //OLED_Clear();
    OLED_ShowCHinese(0, 3, 7); //物料顺序：
    OLED_ShowCHinese(16, 3, 8);
    OLED_ShowCHinese(32, 3, 9);
    OLED_ShowCHinese(48, 3, 10);
    OLED_ShowString(64, 3, ":");

    if(USART3_RX_STA & 0x8000)
    {
        flag_sort = strcmp(USART3_RX_BUF, "RGB"); // RGB
        if(flag_sort == 0)
        {
            OLED_ShowCHinese(80, 3, 1);
            OLED_ShowCHinese(96, 3, 2);
            OLED_ShowCHinese(112, 3, 3);
        }

        flag_sort = strcmp(USART3_RX_BUF, "RBG"); // RBG
        if(flag_sort == 0)
        {
            OLED_ShowCHinese(80, 3, 1);
            OLED_ShowCHinese(96, 3, 3);
            OLED_ShowCHinese(112, 3, 2);
        }

        flag_sort = strcmp(USART3_RX_BUF, "GRB"); // GRB
        if(flag_sort == 0)
        {
            OLED_ShowCHinese(80, 3, 2);
            OLED_ShowCHinese(96, 3, 1);
            OLED_ShowCHinese(112, 3, 3);
        }

        flag_sort = strcmp(USART3_RX_BUF, "GBR"); // GBR
        if(flag_sort == 0)
        {
            OLED_ShowCHinese(80, 3, 2);
            OLED_ShowCHinese(96, 3, 3);
            OLED_ShowCHinese(112, 3, 1);
        }

        flag_sort = strcmp(USART3_RX_BUF, "BRG"); // BRG
        if(flag_sort == 0)
        {
            OLED_ShowCHinese(80, 3, 3);
            OLED_ShowCHinese(96, 3, 1);
            OLED_ShowCHinese(112, 3, 2);
        }

        flag_sort = strcmp(USART3_RX_BUF, "BGR"); // BGR
        if(flag_sort == 0)
        {
            OLED_ShowCHinese(80, 3, 2);
            OLED_ShowCHinese(96, 3, 3);
            OLED_ShowCHinese(112, 3, 1);
        }

        USART3_RX_STA = 0; //重置接收位
        Beep_ring();       //通信成功一次 蜂鸣器响一次
    }

}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : OLED_Show_Encoders   OLED 显示编码器转速
* @param : None
* @return: None
* @note  : None
*
*/
void OLED_Show_Encoders(void)
{
    OLED_ShowString(0, 0, "A:");
    OLED_ShowString(0, 2, "B:");
    OLED_ShowString(0, 4, "C:");
    OLED_ShowString(0, 6, "D:");
    /* 显示目标转速 */
    OLED_ShowNum(40, 0, Encoder_A);
    OLED_ShowNum(40, 2, Encoder_B);
    OLED_ShowNum(40, 4, Encoder_C);
    OLED_ShowNum(40, 6, Encoder_D);
}


/********************************End of File************************************/
