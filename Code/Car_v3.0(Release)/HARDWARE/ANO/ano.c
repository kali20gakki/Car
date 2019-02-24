/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : ano.c
** @auther  : Mrtutu
** @date    : 2019-02-17
** @describe: 匿名上位机通信协议
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "ano.h"
#include "usart3.h"
#include "math.h"


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : usart3_send_char
* @param : c:  发送字符 
* @return: None
* @note  : None
*
*/
void usart3_send_char(u8 c)
{
    while((USART3->SR & 0X40) == 0); //等待上一次发送完毕
    USART3->DR = c;
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : ANO_Send_Data
* @param : target :  目标值
*		   Encoder:  编码器值
* @return: None
* @note  : 小端格式存储
*
*/
void ANO_Send_Data(int target, int Encoder)
{
    u8 send_buf[50]; //发送数据缓存
    u8 _cnt = 0;
    u8 i;
    u8 sum = 0;	//以下为计算sum校验字节，从0xAA也就是首字节，一直到sum字节前一字节

    send_buf[_cnt++] = 0xAA; //0xAA为帧头
    send_buf[_cnt++] = 0x05; //0x05为数据发送源，具体请参考匿名协议，本字节用户可以随意更改
    send_buf[_cnt++] = 0xAF; //0xAF为数据目的地，AF表示上位机，具体请参考匿名协议
    send_buf[_cnt++] = 0xF1; //0xF1，表示本帧为F1用户自定义帧，对应高级收码的F1功能帧
    send_buf[_cnt++] = 0; //本字节表示数据长度，这里先=0，函数最后再赋值，这样就不用人工计算长度了

    send_buf[_cnt++] = BYTE3(target); //将要发送的数据放至发送缓冲区
    send_buf[_cnt++] = BYTE2(target);
    send_buf[_cnt++] = BYTE1(target);
    send_buf[_cnt++] = BYTE0(target);

    send_buf[_cnt++] = BYTE3(Encoder); //将要发送的数据放至发送缓冲区
    send_buf[_cnt++] = BYTE2(Encoder);
    send_buf[_cnt++] = BYTE1(Encoder);
    send_buf[_cnt++] = BYTE0(Encoder);
    
    send_buf[4] = _cnt - 5; //_cnt用来计算数据长度，减5为减去帧开头5个非数据字节

    for(i = 0; i < _cnt; i++)sum += send_buf[i];

    send_buf[_cnt++] = sum;	//将sum校验数据放置最后一字节

    for(i = 0; i < _cnt; i++)usart3_send_char(send_buf[i]);
}
/********************************End of File************************************/
