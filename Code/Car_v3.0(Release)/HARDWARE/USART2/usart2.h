/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : usart2.h
** @auther  : Mrtutu
** @date    : 2019-04-07
** @describe: 
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/
 
#ifndef __USART2_H_
#define __USART2_H_
#include "sys.h"
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#define USART2_MAX_RECV_LEN 255
#define USART2_MAX_SEND_LEN 255

extern u8 USART2_RX_BUF[USART2_MAX_RECV_LEN];
extern u8 USART2_TX_BUF[USART2_MAX_SEND_LEN];
extern u16 USART2_RX_STA; //接收状态标记 
void usart2_init(u32 bound);
void usart2_printf(char* fmt, ...);

#endif

/********************************End of File************************************/