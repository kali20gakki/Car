/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : usart2.c
** @auther  : Mrtutu
** @date    : 2019-04-07
** @describe: NodeMcu 通信
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/
#include "usart2.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "oled.h"

/**************************变量定义******************************/
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN];
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN];
u16 USART2_RX_STA = 0; //接收状态标记
/****************************************************************/




/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : usart3_Init  串口2初始化
* @param : bound: 波特率
* @return: None
* @note  : TXD：PA2   RXD：PA3
*
*/
void usart2_Init(u32 bound)
{
    /*结构体初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    /*总线使能*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);     //使能GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);    //使能USART2时钟
    /*引脚复用*/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //GPIOA2复用为USART2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //GPIOA3复用为USART2
    /*USART2 端口初始化*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;              //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;              //上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);                    //初始化
    /*串口中断初始化*/
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);            //开启接收中断
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;         //串口1中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级3 数值越小优先级越高
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //子优先级3,响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化VIC寄存器
    /* USART2初始化 */
    USART_InitStructure.USART_BaudRate = bound;               //波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;     //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;        //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
    USART_Init(USART2, &USART_InitStructure);                  //初始化串口2
    USART_Cmd(USART2, ENABLE);                                 //使能串口2
}




void NodeMCU_Link(void)
{
    static u8 flag = 1;
    while(flag) // 连接wifi
    {
        if(USART2_RX_STA & 0x8000) 
        {
            if(strcmp(USART2_RX_BUF, "1") == 0)
            {
                OLED_ShowString(80,6,"1");
                flag = 0;
            }
        }
    }
    
    while(flag) // 连接IP
    {
        if(USART2_RX_STA & 0x8000) 
        {
            if(strcmp(USART2_RX_BUF, "2") == 0)
            {
                OLED_ShowString(80,6,"OK");
                flag = 0;
            }
        }
    }
    
    usart2_printf("link ok?");
    
    
}

/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : USART3_IRQHandler  串口3接收中断处理
* @param : None
* @return: None
* @note  : None
*
*/
void USART2_IRQHandler(void)
{
    u8 Res;
    /*接收中断 必须以0x0d 0x0a结尾*/
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        Res = USART_ReceiveData(USART2); //(USART2->DR) 读取接收到的数据

        if((USART2_RX_STA & 0x8000) == 0) //接收未完成
        {
            if(USART2_RX_STA & 0x4000) //接收到了0x0d
            {
                if(Res != 0x0a)USART2_RX_STA = 0; //接收错误,重新开始
                else USART2_RX_STA |= 0x8000; //接收完成了
            }
            else
            {
                if(Res == 0x0d)USART2_RX_STA |= 0x4000;
                else
                {
                    USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = Res ;
                    USART2_RX_STA++;
                    if(USART2_RX_STA > (USART2_MAX_RECV_LEN - 1))USART2_RX_STA = 0; //接收数据错误,重新开始接收
                }
            }
        }
    }
}



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : usart3_printf   USART3 printf()实现
* @param : fmt: [输入/出]
**			 ...: [输入/出]
* @return: None
* @note  : None
*
*/
void usart2_printf(char* fmt, ...)
{
    u16 i, j;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char*)USART2_TX_BUF, fmt, ap);
    va_end(ap);
    i = strlen((const char*)USART2_TX_BUF); //此次发送数据的长度
    for(j = 0; j < i; j++) //循环发送数据
    {
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); //等待上次传输完成
        USART_SendData(USART2, (uint8_t)USART2_TX_BUF[j]); 	 //发送数据到串口3
    }

}


/********************************End of File************************************/