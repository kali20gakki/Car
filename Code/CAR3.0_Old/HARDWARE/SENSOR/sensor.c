#include "sensor.h"
#include "delay.h"
#include "oled.h"
#include "key.h"

extern int count1;
extern int count2;
extern int count3;

extern u8 flag_KEY1;
extern u8 flag_KEY2;
extern u8 flag_KEY3;

/* 循迹传感器：遇到黑线 灯亮 低电平
							 遇到空白 灯灭 高电平
*/


/*红外循迹传感器初始化*/
void Sensor_Track_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//使能 GPIOD,GPIOE 时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化 GPIOE1,2,3,4
	 
}

void Sensor_TrackB_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//使能 GPIOD,GPIOE 时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化  
}

/* 红外传感器外部中断计数初始化 */
void Sensor_Count_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);//使能 GPIOD,GPIOE 时钟
	
	/* IO初始化 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化  
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能 SYSCFG 时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource11);//PE11 连接线 11
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource13);//PE11 连接线 13
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource15);//PE11 连接线 15
	
	/* 配置 EXTI_Line11 13 15 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line11|EXTI_Line13|EXTI_Line15;
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


void Sensor_count_Init2()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//使能 GPIOD,GPIOE 时钟
	
	/* IO初始化 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化  
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能 SYSCFG 时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15);//PB15 连接线 15
	
	/* 配置 EXTI_Line15 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
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

/* 外部中断处理函数 */
void EXTI15_10_IRQHandler(void)
{
	delay_ms(10);      //适当延时防止毛刺干扰
	/* LINE11中断判断 */
	if(EXTI_GetITStatus(EXTI_Line11)!=RESET)
	{ 
		/* 中断逻辑 */
		if (SENSOR1 == 0)count1++;
		EXTI_ClearITPendingBit(EXTI_Line11); //清除 LINE 上的中断标志位
	}
	
		/* LINE13中断判断 */
	if(EXTI_GetITStatus(EXTI_Line13)!=RESET)
	{ 
		/* 中断逻辑 */
		if (SENSOR2 == 0)count2++;
		EXTI_ClearITPendingBit(EXTI_Line13); //清除 LINE 上的中断标志位
	}
	
		/* LINE15中断判断 */
	if(EXTI_GetITStatus(EXTI_Line15)!=RESET)
	{ 
		/* 中断逻辑 */
		if (SENSOR5 == 0)count3++;
		EXTI_ClearITPendingBit(EXTI_Line15); //清除 LINE 上的中断标志位
	}
}
