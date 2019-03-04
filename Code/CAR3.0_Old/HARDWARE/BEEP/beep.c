#include "beep.h"

/*蜂鸣器初始化 */
void beep_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能 GPIOB 时钟
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //BEEP 端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
  GPIO_ResetBits(GPIOE,GPIO_Pin_1);//输出0，关闭蜂鸣器输出
}

/*
* 函数功能：蜂鸣器响声 1
* 函数说明：滴 延时100ms 滴
**/
void beep_ring()
{
	BEEP=0;
	delay_ms(100);
	BEEP=1;
	delay_ms(100);
	BEEP=0;
	delay_ms(100);
	BEEP=1;
	delay_ms(100);
	BEEP=0;
}