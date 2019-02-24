/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : uart5.c
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

#include "uart5.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "timer.h"


u8 UART5_RX_BUF[UART5_MAX_RECV_LEN];
__align(8) u8 UART5_TX_BUF[UART5_MAX_SEND_LEN];
u16 UART5_RX_STA = 0; //接收状态标记



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : UART5_Init
* @param : bound: 波特率
* @return: None
* @note  : None
*
*/
void uart5_Init(u32 bound)
{
    /*结构体初始化*/
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /*总线使能*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);

    /*引脚复用*/
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);

    /* GPIO初始化 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;              //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;              //上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);                    //初始化

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;              //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;              //上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);                    //初始化

    /*串口中断初始化*/
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);             //开启接收中断
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;          //串口中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级2 数值越小优先级越高
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //子优先级2,响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化VIC寄存器

    /* UART5初始化 */
    USART_InitStructure.USART_BaudRate = bound;               //波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;     //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;   //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
    USART_Init(UART5, &USART_InitStructure);                  //初始化串口5
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
    USART_Cmd(UART5, ENABLE);                                 //使能串口5
   
}



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : uart5_send_char
* @param : c: 待发送字符
* @return: None
* @note  : None
*
*/
void uart5_send_char(u8 c)
{
    while((UART5->SR & 0X40) == 0); //等待上一次发送完毕
    UART5->DR = c;
}



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : uart5_send_string
* @param : str: 发送字符串
* @return: None
* @note  : None
*
*/
void uart5_send_string(u8* str)
{
    while(*str != '\0')
    {
        uart5_send_char(*str);
        str++;
    }
}



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : UART5_IRQHandler
* @param : None
* @return: None
* @note  : None
*
*/
void UART5_IRQHandler(void)
{
    u8 res;
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {

        res = USART_ReceiveData(UART5);
        if((UART5_RX_STA & (1 << 15)) == 0) //接收完的一批数据,还没有被处理,则不再接收其他数据
        {
            if(UART5_RX_STA < UART5_MAX_RECV_LEN)		//还可以接收数据
            {
                TIM_SetCounter(TIM7, 0); //计数器清空
                if(UART5_RX_STA == 0)
                    TIM_Cmd(TIM7, ENABLE);  //使能定时器7
                UART5_RX_BUF[UART5_RX_STA++] = res;		//记录接收到的值
            }
            else
            {
                UART5_RX_STA |= 1 << 15;					//强制标记接收完成
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
void uart5_printf(char* fmt, ...)
{
    u16 i, j;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char*)UART5_TX_BUF, fmt, ap);
    va_end(ap);
    i = strlen((const char*)UART5_TX_BUF); //此次发送数据的长度
    for(j = 0; j < i; j++) //循环发送数据
    {
        while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET); //等待上次传输完成
        USART_SendData(UART5, (uint8_t)UART5_TX_BUF[j]); 	 //发送数据到串口5
    }

}

/********************************End of File************************************/
