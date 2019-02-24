/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : hc05.c
** @auther  : Mrtutu
** @date    : 2019-02-21
** @describe: 
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "hc05.h"
#include "uart5.h"




/*
* @auther: Mrtutu
* @date  ：2019-02-21
*
* @func  : HC05_Send_Encoders
* @param : Encoder_A: [输入/出] 
**		     Encoder_B: [输入/出] 
**			 Encoder_C: [输入/出] 
**			 Encoder_D: [输入/出] 
* @return: None
* @note  : 利用HC05来转发来自串口5的消息
*
*/
void HC05_Send_Encoders(int Encoder_A, int Encoder_B, int Encoder_C, int Encoder_D)
{
    uart5_printf("A= %d, B= %d, C= %d, D= %d\r\n",Encoder_A, Encoder_B, Encoder_C, Encoder_D);
}






/********************************End of File************************************/
