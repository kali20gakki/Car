#include "timer.h"
#include "usart.h"
#include "led.h"
#include "motor.h"

extern int Encoder_A;
extern int Encoder_B;
extern int Encoder_C;
extern int Encoder_D;

extern int Motor_A;
extern int Motor_B;
extern int Motor_C;
extern int Motor_D;
extern int count1;
extern int testNum;
/*TIM7 初始化*/
void TIM7_Int_Init(u16 arr,u16 psc)
{
	/*结构体初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	/*使能 TIM7 时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
  TIM_TimeBaseInitStructure.TIM_Period = arr; //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	/*初始化定时器TIM7*/
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);
	/*允许定时器 7 更新中断*/
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; //定时器 7 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //响应优先级 3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);//初始化 NVIC
 
	TIM_Cmd(TIM7,ENABLE); //使能定时器 7
}




void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //溢出中断
	{
		Encoder_A = Read_Encoder(1);
		Encoder_B = Read_Encoder(2);
		Encoder_C = Read_Encoder(3);
		Encoder_D = Read_Encoder(4);
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update); //清除中断标志位
}