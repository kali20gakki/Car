/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : main.c
** @auther  : Mrtutu
** @date    : 2019-02-17
** @describe: 物料搬运小车
**
**      注意：更改外部晶振频率（25MHz）
**           1. stm32f4xx.h         #define HSI_VALUE    ((uint32_t)25000000)
**           2. system_stm32f4xx.c  PLL_M  25
**           3. 设置 Xtal：25MHz
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
** 芯片说明：STM32F407VET6   Flash：512Kb  RAM：192Kb
**
**           中断优先级：       抢占优先级         响应优先级
**              TIM7                 0                  2
**             USART3                1                  1
**             UART5                 2                  1
**          SENSOR_FRONT_L
**          SENSOR_FRONT_R
**          SENSOR_LEFT_U
**          SENSOR_LEFT_D
**------------------------------------------------------------------------------
\********************************End of Head************************************/

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
#include "uart5.h"
#include "action.h"
#include "task.h"

/********************************全局变量***************************************/
volatile int Motor_A, Motor_B, Motor_C, Motor_D;      // PI算法返回赋值 临时PWM值
volatile int Encoder_A, Encoder_B, Encoder_C, Encoder_D; // 编码器返回赋值
volatile int Target_A, Target_B, Target_C, Target_D;  // 目标速度

u8 key;

/* 计数器 */
volatile u8 COUNT_FRONT_L;
volatile u8 COUNT_FRONT_R;
volatile u8 COUNT_LEFT_U;
volatile u8 COUNT_RIGHT_U;
volatile u8 COUNT_RIGHT_D;
/*******************************************************************************/

/**********************************标志位***************************************/
/*******************************************************************************/




int main(void)
{
    /********************************驱动初始化*************************************/
    /*初始化顺序很重要*/
    delay_init(168);	    	                     // 延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 // 设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    usart3_Init(9600);                               // OpenMV
    uart5_Init(9600);                                // 串口5
    TIM9_Int_Init(100 - 1, 8400 - 1);	             // 10ms中断一次
    Servo_Init(10000 - 1, 56 - 1);                   // 舵机PWM频率为300Hz
    TIM7_Int_Init(200 - 1, 8400 - 1);                // TIM7 定时中断时间为：10ms
    Motor_Init(1000 - 1, 42 - 1);                    // 电机PWM频率为2KHz
    Motor_Pin_Init();                                // 电机方向控制引脚初始化
    Encoder_MotorA_Init();                           // 电机A编码器初始化
    Encoder_MotorB_Init();                           // 电机B编码器初始化
    Encoder_MotorC_Init();                           // 电机C编码器初始化
    Encoder_MotorD_Init();                           // 电机D编码器初始化
    Sensor_Track_Init();                             // 循迹传感器初始化
    KEY_Init();                                      // 按键初始化
    Beep_Init();                                     // 蜂鸣器初始化
    IIC_Init();                                      // OLED IIC初始化
    OLED_Init();                                     // OLED初始化

    /*******************************************************************************/

    Action_First();
    Task_OLED();
    while(1)
    {
//        key = KEY_Scan(0);
//        if(key == 1)
//        {
//            Task_TestPath();
//            while(1);
//        }
//        else if(key == 2)
//        {
//            Task_TestSensor();
//        }
//        else if(key == 3)
//        {
//            Strategy_QrcodeSquence();
//        }

    }
}


/********************************End of File************************************/