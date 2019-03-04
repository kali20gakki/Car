#include "move.h"
#include "motor.h"
#include "sensor.h"
#include "servo.h"


extern int Motor_A;
extern int Motor_B;
extern int Motor_C;
extern int Motor_D;


/*
* 说明
* AIN1  AIN2  1  0 -- 正  0  1 -- 反
* BIN1  BIN2  1  0 -- 正  0  1 -- 反
*
* CIN1  CIN2  1  0 -- 反  0  1 -- 正
* DIN1  DIN2  1  0 -- 反  0  1 -- 正
*
***/
void firstAction()
{
	Servo1_SetAngle(0);
	Servo2_SetAngle(50);
	Servo3_SetAngle(140);
  Servo4_SetAngle(180);
}

void action2()
{
		Servo2_SetAngle(160);
		Servo3_SetAngle(40);
    Servo4_SetAngle(180);
	  Servo1_SetAngle(60);
}

void action3()
{
	
}
void move_front(u8 pwmValue)
{
	AIN1 = 1;
	AIN2 = 0;
	PWMA = pwmValue;
	
	BIN1 = 1;
	BIN2 = 0;
	PWMB = pwmValue;
	
	CIN1 = 0;
	CIN2 = 1;
	PWMC = pwmValue+25;
	
	DIN1 = 0;
	DIN2 = 1;
	PWMD = pwmValue+2;
}

void move_back(u8 pwmValue)
{
	AIN1 = 0;
	AIN2 = 1;
	PWMA = pwmValue+30;
	
	BIN1 = 0;
	BIN2 = 1;
	PWMB = pwmValue-20;
	
	CIN1 = 1;
	CIN2 = 0;
	PWMC = pwmValue+10;
	
	DIN1 = 1;
	DIN2 = 0;
	PWMD = pwmValue;
}

void move_left(u8 pwmValue)
{
	AIN1 = 0;  //反转
	AIN2 = 1; 
	PWMA = pwmValue+30;
	
	BIN1 = 1;  //正转
	BIN2 = 0;
	PWMB = pwmValue;
	
	CIN1 = 1;  //反转
	CIN2 = 0;
	PWMC = pwmValue+20;
	
	DIN1 = 0;  //正转
	DIN2 = 1;
	PWMD = pwmValue+2;
}

void move_right(u8 pwmValue)
{
	AIN1 = 1;  //反转
	AIN2 = 0; 
	PWMA = pwmValue;
	
	BIN1 = 0;  //正转
	BIN2 = 1;
	PWMB = pwmValue-10;
	
	CIN1 = 0;  //反转
	CIN2 = 1;
	PWMC = pwmValue+20;
	
	DIN1 = 1;  //正转
	DIN2 = 0;
	PWMD = pwmValue;
}

void move_upper_left(u8 pwmValue)
{
	AIN1 = 1;
	AIN2 = 0;
	PWMA = pwmValue;
	
	CIN1 = 0;
	CIN2 = 1;
	PWMC = pwmValue;
	
	BIN1 = 0;
	BIN2 = 0;
	
	DIN1 = 0;
	DIN2 = 0;
}

void move_down_right(u8 pwmValue)
{
	AIN1 = 0;
	AIN2 = 1;
	PWMA = pwmValue+30;
	
	CIN1 = 1;
	CIN2 = 0;
	PWMC = pwmValue-20;
	
	BIN1 = 0;
	BIN2 = 0;
	
	DIN1 = 0;
	DIN2 = 0;
}

void move_stop()
{
	AIN1 = 0;
	AIN2 = 0;
	
	BIN1 = 0;
	BIN2 = 0;
	
	CIN1 = 0;
	CIN2 = 0;
	
	DIN1 = 0;
	DIN2 = 0;
}

/* 循迹传感器：遇到黑线 灯灭 高电平
							 遇到空白 灯亮 低电平
*/
void Trackline_Front(u8 pwmValue)
{
	if(SENSOR_FRONT1 ==1 && SENSOR_FRONT2 ==1)
	{
		move_front(pwmValue);
	}
	else if(SENSOR_FRONT1 ==0 && SENSOR_FRONT2 ==1)  //左偏
	{
		
		move_left(150); 
	}
	else if((SENSOR_FRONT1 ==1 && SENSOR_FRONT2 ==0))  //右偏
	{
		move_right(150);
	}
}


void Trackline_right(u8 pwmValue)
{
	if(SENSOR1 ==0 && SENSOR2 ==0)
	{
		move_right(pwmValue);
	}
	else if(SENSOR1 ==0 && SENSOR2 ==1)  //偏前
	{
		move_back(80);
	}
	else if(SENSOR1 ==1 && SENSOR2 ==0)  //偏后
	{
		move_front(80);
	}
}

void Trackline_Back(u8 pwmValue)
{
	if(SENSOR3 ==1 && SENSOR4 ==1)
	{
			AIN1 = 0;
			AIN2 = 1;
			PWMA = pwmValue+40;
			
			BIN1 = 0;
			BIN2 = 1;
			PWMB = pwmValue-30;
			
			CIN1 = 1;
			CIN2 = 0;
			PWMC = pwmValue+10;
			
			DIN1 = 1;
			DIN2 = 0;
			PWMD = pwmValue-10;
	}
	else if(SENSOR3 ==0 && SENSOR4 ==1)  //遮住4
	{
			AIN1 = 0;  //反转
			AIN2 = 1; 
			PWMA = 255;
			
			BIN1 = 1;  //正转
			BIN2 = 0;
			PWMB = 200;
			
			CIN1 = 1;  //反转
			CIN2 = 0;
			PWMC = 240;
			
			DIN1 = 0;  //正转
			DIN2 = 1;
			PWMD = 215;
		
	}
	else if((SENSOR3 ==1 && SENSOR4 ==0))  //3
	{
		AIN1 = 1;  //反转
		AIN2 = 0; 
		PWMA = 210;
		
		BIN1 = 0;  //正转
		BIN2 = 1;
		PWMB = 200;
		
		CIN1 = 0;  //反转
		CIN2 = 1;
		PWMC = 240;
		
		DIN1 = 1;  //正转
		DIN2 = 0;
		PWMD = 215;
		
	}
}

void move_back_400(u8 pwmValue)
{
				AIN1 = 0;
			AIN2 = 1;
			PWMA = pwmValue+30;
			
			BIN1 = 0;
			BIN2 = 1;
			PWMB = pwmValue-30;
			
			CIN1 = 1;
			CIN2 = 0;
			PWMC = pwmValue+10;
			
			DIN1 = 1;
			DIN2 = 0;
			PWMD = pwmValue+20;
}