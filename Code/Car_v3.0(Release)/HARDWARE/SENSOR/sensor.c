/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : sensor.c
** @auther  : Mrtutu
** @date    : 2019-02-17
** @describe: 寻迹传感器 底层驱动
**
**           根据实际安装需求更改以及头文件
**
**           遇到黑线 灯灭 高电平 （已检查）
**			 遇到空白 灯亮 低电平
**
**
**
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "sensor.h"
#include "delay.h"

extern u8 COUNT_FRONT_L;
extern u8 COUNT_FRONT_R;
extern u8 COUNT_LEFT_U;
extern u8 COUNT_RIGHT_U;
extern u8 COUNT_RIGHT_D;



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Sensor_Track_Init
* @param : None
* @return: None
* @note  : 红外寻迹传感器初始化   
*
*/
void Sensor_Track_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    /* 使能时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE,ENABLE);
    /* GPIO_C  0 1 3 13 15*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_13|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   //普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
    /* GPIO_E 6*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
    /* GPIO_A 3 5*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
    
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能 SYSCFG 时钟
    
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0); //PC0  连接线 0
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource3); //PC3  连接线 3
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);//PC13 连接线 13
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5); //PA5 连接线 5
    
    /* 配置 中断线 */
    EXTI_InitStructure.EXTI_Line =  EXTI_Line0|EXTI_Line3|EXTI_Line5| EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
    //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下升沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能 LINE11
    EXTI_Init(&EXTI_InitStructure);
    
    /* 中断初始化 LINE0 */
    NVIC_InitStructure.NVIC_IRQChannel =  EXTI0_IRQn; //外部中断 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级 1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//响应优先级 2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);//配置 NVIC
    
    /* 中断初始化 LINE3 */
    NVIC_InitStructure.NVIC_IRQChannel =  EXTI3_IRQn; //外部中断 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级 1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//响应优先级 2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);//配置 NVIC
   
    
    /* 中断初始化 LINE6  5 */
    NVIC_InitStructure.NVIC_IRQChannel =  EXTI9_5_IRQn; //外部中断 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级 1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//响应优先级 2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);//配置 NVIC
    
    /* 中断初始化 LINE13 15 */
    NVIC_InitStructure.NVIC_IRQChannel =  EXTI15_10_IRQn; //外部中断 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级 1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04;//响应优先级 2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);//配置 NVIC
}


/*
* @auther: Mrtutu
* @date  ：2019-03-03
*
* @func  : EXTI0_IRQHandler
* @param : None
* @return: None
* @note  : None
*
*/
void EXTI0_IRQHandler(void)
{
    delay_ms(5);      //适当延时防止毛刺干扰    
     /* LINE0中断判断 --- Right Down*/
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        /* 中断逻辑 */
        if(SNESOR_RIGHT_D == 0)COUNT_RIGHT_D++;
        EXTI_ClearITPendingBit(EXTI_Line0); //清除 LINE 上的中断标志位
    }
}


/*
* @auther: Mrtutu
* @date  ：2019-03-02
*
* @func  : EXTI3_IRQHandler
* @param : None
* @return: None
* @note  : None
*
*/
void EXTI3_IRQHandler(void)
{
    delay_ms(5);      //适当延时防止毛刺干扰
    
    /* LINE3 FRONT_L  FRONT_R */
    if(EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        if(SENSOR_FRONT_R == 0)COUNT_FRONT_R++;
        EXTI_ClearITPendingBit(EXTI_Line3); //清除 LINE 上的中断标志位
    }
}



/*
* @auther: Mrtutu
* @date  ：2019-03-03
*
* @func  : EXTI9_5_IRQHandler
* @param : None
* @return: None
* @note  : None
*
*/
void EXTI9_5_IRQHandler(void)
{
    delay_ms(5);      //适当延时防止毛刺干扰
    
     /* LINE5 */
    if(EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        if(SENSOR_FRONT_L == 0)COUNT_FRONT_L++;
        EXTI_ClearITPendingBit(EXTI_Line5); //清除 LINE 上的中断标志位
    }
    
}

/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : EXTI15_10_IRQHandler    中断线 10 - 15 处理函数
* @param : None
* @return: None
* @note  : 中断线 0-4 每个中断线对应一个中断函数，
*          中断线 5-9 共用中断函数 EXTI9_5_IRQHandler，
*          中断线 10-15 共用中断函数 EXTI15_10_IRQHandler
*
*/
void EXTI15_10_IRQHandler(void)
{
    delay_ms(5);      //适当延时防止毛刺干扰
    
    /* LINE13中断判断 --- Right UP*/
    if(EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        /* 中断逻辑 */
        if(SNESOR_RIGHT_U == 0)COUNT_RIGHT_U++;
        EXTI_ClearITPendingBit(EXTI_Line13); //清除 LINE 上的中断标志位
    }
    
}
/********************************End of File************************************/
