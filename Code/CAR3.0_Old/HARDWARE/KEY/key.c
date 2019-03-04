#include "key.h"

/* 中断按键初始化 */
void KEY_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	/* IO初始化 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能 SYSCFG 时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource10);//PE11 连接线 11
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource12);//PE11 连接线 13
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource14);//PE11 连接线 15
	
	/* 配置 EXTI_Line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line10|EXTI_Line14|EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能 LINE11
	EXTI_Init(&EXTI_InitStructure);
	
	/* 中断初始化 LINE10 - 15 */
	NVIC_InitStructure.NVIC_IRQChannel =  EXTI15_10_IRQn;//外部中断 11
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//响应优先级 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置 NVIC
}

