/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : uart5.h
** @auther  : Mrtutu
** @date    : 2019-02-17
** @describe: 
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/
 
#ifndef __UART5_H_
#define __UART5_H_
#include "sys.h"
#include "stdio.h"
#define UART5_MAX_RECV_LEN 255
#define UART5_MAX_SEND_LEN 255

extern u8 UART5_RX_BUF[UART5_MAX_RECV_LEN];
extern u8 UART5_TX_BUF[UART5_MAX_SEND_LEN];
extern u16 UART5_RX_STA;

void TIM6_Int_Init(u16 arr,u16 psc);
void uart5_Init(u32 bound);
void uart5_send_char(u8 c);
void uart5_send_string(u8* str);
void UART5_IRQHandler(void);
void uart5_printf(char* fmt, ...);

#endif

/********************************End of File************************************/