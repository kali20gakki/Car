#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "servo.h"
#include "oled.h"
#include "usart3.h"
#include "led.h"
#include "motor.h"
#include "encoder.h"
#include "move.h"
#include "sensor.h"
#include "beep.h"
#include "ano.h"
#include "key.h"
/**************************************************************************************

USART1: TXD(P4)	--> RXD (PB11)     通信协议： "check\n" -->  "OK\r\n"     通信建立确认
        RXD(P5) --> TXD (PB10)                 "Q\n"    -->  "123\r\n"    二维码识别
				波特率：9600                           "R\n"    -->  "R(123,321)" Red色块坐标
				                                       "G\n"    -->  "G(123,321)" Green色块坐标
																							 "B\n"    -->  "B(123,321)" Blue色块坐标
																							 "O\n"    -->  "RGB"        色块顺序
				
***************************************************************************************/
/***************************************************************************************
     舵机：               引脚                OLED引脚
		Servo1 :  TIM1CH1 --> PA8                 SCL --> PD10
		Servo2 :  TIM1CH2 --> PA9                 SDA --> PD8
		Servo3 :  TIM1CH3 --> PA10                
		Servo4 :  TIM1CH4 --> PA11                
        Servo5 :  TIM9CH1 --> PA2                 
		
		周期20ms     角度  占空比                 MOTOR
        0.5  ms -->  0      2.5%   249   300        MOTORA --> TIM8CH1 --> PC6
        1    ms -->  45       5%   499              MOTORB --> TIM8CH2 --> PC7
        1.5  ms -->  90     7.5%   749              MOTORC --> TIM8CH3 --> PC8
        2    ms -->  135    10%    999              MOTORD --> TIM8CH4 --> PC9
        2.5  ms -->  180    12.5%  1249  1200       arr = 1000-1    psc =  84-1  PWM频率 = 2KHz   占空比 = N/arr
***************************************************************************************/

/**********************************变量定义****************************************/
int Encoder_A,Encoder_B,Encoder_C,Encoder_D;          //编码器的脉冲计数值
int Motor_A,Motor_B,Motor_C,Motor_D;              //电机PWM变量
int count1 = 0;
int count2 = 0;
int count3 = 0;
/**********************************************************************************/

/**********************************标志位定义**************************************/
u8 flag_KEY1 = 0;   //KEY1 标志位
u8 flag_KEY2 = 0;   //KEY2 标志位
u8 flag_KEY3 = 0;   //KEY3 标志位


/**********************************************************************************/
void delayNs(int N);
 


int main(void)
{
	/*初始化顺序很重要*/
	delay_init(168);	    	                         //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	usart3_Init(9600);
	servo_Init(9999,335);                          //舵机PWM频率为50Hz  占空比 duty = N/arr;
	TIM7_Int_Init(50-1,8400-1);                     //TIM7 定时中断时间为：5ms
	motor_Init(1000-1,84-1);                         //电机PWM频率为2KHz  占空比 duty = N/arr;
	motor_Pin_Init();
	beep_Init();
	KEY_Init();
	IIC_Init();
	OLED_Init();
	OLED_Clear();
	Sensor_Count_Init();
	Sensor_count_Init2();
	Sensor_Track_Init();
	Sensor_TrackB_Init();
	encoder_MotorA_Init();
	encoder_MotorB_Init();
	encoder_MotorC_Init();
	encoder_MotorD_Init();
	
	while(1)
	{
		/***任务一：左移*******/
		 while(1)
		 {
			 	AIN1 = 1;  //反转
				AIN2 = 0; 
				PWMA = 200;
				
				BIN1 = 0;  //正转
				BIN2 = 1;
				PWMB = 200;
				
				CIN1 = 0;  //反转
				CIN2 = 1;
				PWMC = 210;
				
				DIN1 = 1;  //正转
				DIN2 = 0;
				PWMD = 210;
			 if(SENSOR_FRONT1 == 1 && SENSOR_FRONT1 == 1 )break;
		 }
		 while(1)
		 {
			 move_stop();
			 delayNs(3);
			 break;
		 }
		 
		  count1 = 0;
		 
		 /****任务二：循迹直走到PAD***********/
		 while(1)
		 {
			 Trackline_Front(600-100*count1);
			 if(count1 >= 6)
			 {
				 move_stop();
				 break;
			 }
		 }
		 
		 count1 = 0;
		 
		 /***任务三：停下来扫描二维码*******/
		 while(1)
		 {
			 move_stop();
			 beep_ring();
			 delayNs(4);
			 break;
		 }
		 
		 while(1)
		 {
			 firstAction();
			 delayNs(5);
			 break;
		 }
		 
		 /***任务四：返回抓取物料**********/
		 while(1)
		 {
			 move_back_400(200);
			 if(count1 >= 3)
			 {
				 move_stop();
				 break;
			 }
		 }
		 count1 = 0;
		 
		 while(1)
		 {
			 action2();
			 delayNs(8);
			 break;
		 }
		 /***任务五：放置物料************/
		 while(1)
		 {
			 move_stop();
			 beep_ring();
			 count1 = 0;
			 delayNs(4);
			 break;
		 }
		 
		 count1 = 0;		 
		 while(1)
		 {

			 /*左平移*/
			 	AIN1 = 1;  //反转
				AIN2 = 0; 
				PWMA = 190;
				
				BIN1 = 0;  //正转
				BIN2 = 1;
				PWMB = 200;
				
				CIN1 = 0;  //反转
				CIN2 = 1;
				PWMC = 210;
				
				DIN1 = 1;  //正转
				DIN2 = 0;
				PWMD = 240;
			 	 			 
			 if(count1 >= 3)
			 {
				 move_stop();
				 break;
			 }
			 
		 }		 
		 while(1)
		 {
			 move_stop();
			 //beep_ring();
			 delayNs(4);
		 }
		 
		 while(1)
		 {
			 move_stop();
			 //beep_ring();
			 delayNs(4);
		 }
     /**end8************************************************************************************************/

		
	}
}	  


void delayNs(int N)
{
	int i;
	for(i = 0;i< N;i++)
	{
		delay_ms(200);
	}
}