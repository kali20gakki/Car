/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : oled.c
** @auther  : Mrtutu
** @date    : 2019-02-17
** @describe: 0.96 寸 OLED 底层驱动
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "oled.h"
#include "codetab.h"
#include "delay.h"
#include "myiic.h"



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Write_IIC_Byte  通过I2C总线写一个字节
* @param : IIC_Byte: 待写入字节数据
* @return: None
* @note  : None
*
*/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
    unsigned char i;
    for(i = 0; i < 8; i++)
    {
        if(IIC_Byte & 0x80)
            IIC_SDA = 1;
        else
            IIC_SDA = 0;
        IIC_SCL = high;
        delay_us(4);
        IIC_SCL = 0;
        IIC_Byte <<= 1;
    }
    IIC_SDA = 1; //原程序这里有一个拉高SDA，根据OLED的DATASHEET，此句必须去掉。
    IIC_SCL = 1;
    delay_us(4);
    IIC_SCL = 0;
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : OLED_WrDat  OLED写数据
* @param : IIC_Data:
* @return: None
* @note  : None
*
*/
void OLED_WrDat(unsigned char IIC_Data)
{
    IIC_Start();
    IIC_Send_Byte(0x78);			//D/C#=0; R/W#=0
    IIC_Wait_Ack();
    IIC_Send_Byte(0x40);			//write data
    IIC_Wait_Ack();
    IIC_Send_Byte(IIC_Data);
    IIC_Wait_Ack();
    IIC_Stop();
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : OLED_WrCmd   OLED写命令
* @param : IIC_Command:
* @return: None
* @note  : None
*
*/
void OLED_WrCmd(unsigned char IIC_Command)
{
    IIC_Start();
    IIC_Send_Byte(0x78);            //Slave address,SA0=0
    if(IIC_Wait_Ack())
    {
        while(1);
    }
    IIC_Send_Byte(0x00);			//write command
    IIC_Wait_Ack();
    IIC_Send_Byte(IIC_Command);
    IIC_Wait_Ack();
    IIC_Stop();
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : OLED_Set_Pos  OLED 设置坐标
* @param : x: [输入/出]
**		   y: [输入/出]
* @return: None
* @note  : None
*
*/
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WrCmd(0xb0 + y);
    OLED_WrCmd(((x & 0xf0) >> 4) | 0x10);
    OLED_WrCmd((x & 0x0f) | 0x01);
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : OLED_Fill  OLED全屏
* @param : bmp_dat: [输入/出]
* @return: None
* @note  : None
*
*/
void OLED_Fill(unsigned char bmp_dat)
{
    unsigned char y, x;
    for(y = 0; y < 8; y++)
    {
        OLED_WrCmd(0xb0 + y);
        OLED_WrCmd(0x01);
        OLED_WrCmd(0x10);
        for(x = 0; x < X_WIDTH; x++)
            OLED_WrDat(bmp_dat);
    }
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : OLED_Clear  OLED 清屏
* @param : None
* @return: None
* @note  : None
*
*/
void OLED_Clear(void)
{
    unsigned char y, x;
    for(y = 0; y < 8; y++)
    {
        OLED_WrCmd(0xb0 + y);
        OLED_WrCmd(0x01);
        OLED_WrCmd(0x10);
        for(x = 0; x < X_WIDTH; x++)
            OLED_WrDat(0);
    }
}



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : OLED_Init   OLED初始化
* @param : None
* @return: None
* @note  : None
*
*/
void OLED_Init(void)
{
    delay_ms(500);//初始化之前的延时很重要！
    OLED_WrCmd(0xae);//--turn off oled panel
    OLED_WrCmd(0x00);//---set low column address
    OLED_WrCmd(0x10);//---set high column address
    OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WrCmd(0x81);//--set contrast control register
    OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
    OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_WrCmd(0xa6);//--set normal display
    OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
    OLED_WrCmd(0x3f);//--1/64 duty
    OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WrCmd(0x00);//-not offset
    OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
    OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WrCmd(0xd9);//--set pre-charge period
    OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WrCmd(0xda);//--set com pins hardware configuration
    OLED_WrCmd(0x12);
    OLED_WrCmd(0xdb);//--set vcomh
    OLED_WrCmd(0x40);//Set VCOM Deselect Level
    OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WrCmd(0x02);//
    OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
    OLED_WrCmd(0x14);//--set(0x10) disable
    OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
    OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
    OLED_WrCmd(0xaf);//--turn on oled panel
    OLED_Fill(0x00); //初始清屏
    OLED_Set_Pos(0, 0);
}



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : OLED_P6x8Str
* @param : x: [输入/出]
**		   y: [输入/出]
**		   ch[]: [输入/出]
* @return: None
* @note  : 显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7
*
*/
void OLED_P6x8Str(unsigned char x, unsigned char y, unsigned char ch[])
{
    unsigned char c = 0, i = 0, j = 0;
    while(ch[j] != '\0')
    {
        c = ch[j] - 32;
        if(x > 126)
        {
            x = 0;
            y++;
        }
        OLED_Set_Pos(x, y);
        for(i = 0; i < 6; i++)
            OLED_WrDat(F6x8[c][i]);
        x += 6;
        j++;
    }
}



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : OLED_ShowString
* @param : x: [输入/出]
**		   y: [输入/出]
**		   ch[]: [输入/出]
* @return: None
* @note  : None
*
*/
void OLED_ShowString(unsigned char x, unsigned  char y, unsigned char ch[])
{
    unsigned char c = 0, i = 0, j = 0;
    while(ch[j] != '\0')
    {
        c = ch[j] - 32;
        if(x > 120)
        {
            x = 0;
            y++;
        }
        OLED_Set_Pos(x, y);
        for(i = 0; i < 8; i++)
            OLED_WrDat(F8X16[c * 16 + i]);
        OLED_Set_Pos(x, y + 1);
        for(i = 0; i < 8; i++)
            OLED_WrDat(F8X16[c * 16 + i + 8]);
        x += 8;
        j++;
    }
}



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : OLED_ShowCHinese
* @param : x: [输入/出]
**		   y: [输入/出]
**		   N: [输入/出]
* @return: None
* @note  : None
*
*/
void OLED_ShowCHinese(unsigned char x, unsigned char y, unsigned int N)
{
    unsigned char wm = 0;
    unsigned int adder = 32 * N;
    OLED_Set_Pos(x, y);
    for(wm = 0; wm < 16; wm++)
    {
        OLED_WrDat(F16x16[adder]);
        adder += 1;
    }
    OLED_Set_Pos(x, y + 1);
    for(wm = 0; wm < 16; wm++)
    {
        OLED_WrDat(F16x16[adder]);
        adder += 1;
    }
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Draw_BMP   显示图片
* @param : x0: [输入/出]
**			 y0: [输入/出]
**			 x1: [输入/出]
**			 y1: [输入/出]
**			 BMP[]: [输入/出]
* @return: None
* @note  : None
*
*/
void Draw_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
    unsigned int j = 0;
    unsigned char x, y;

    if(y1 % 8 == 0) y = y1 / 8;
    else y = y1 / 8 + 1;
    for(y = y0; y < y1; y++)
    {
        OLED_Set_Pos(x0, y);
        for(x = x0; x < x1; x++)
        {
            OLED_WrDat(BMP[j++]);
        }
    }
}



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : OLED_ShowNum  显示int数
* @param : x: [输入/出]
**			 y: [输入/出]
**			 Num: [输入/出]
* @return: None
* @note  : None
*
*/
void OLED_ShowNum(unsigned char x, unsigned  char y, int Num)
{
    unsigned char s[8];
    if(Num < 0)
    {
        OLED_ShowString(x, y, "-");
        sprintf(s, "%d", -Num);
        OLED_ShowString(x + 12, y, s);
    }
    else if(Num > 0)
    {
        sprintf(s, "%d", Num);
        OLED_ShowString(x, y, s);
    }
}
/********************************End of File************************************/
