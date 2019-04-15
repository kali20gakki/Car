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
#include "usart2.h"


/********************************全局变量***************************************/
volatile int Motor_A, Motor_B, Motor_C, Motor_D;      // PI算法返回赋值 临时PWM值
volatile int Encoder_A, Encoder_B, Encoder_C, Encoder_D; // 编码器返回赋值
volatile int Target_A, Target_B, Target_C, Target_D;  // 目标速度


/******************************* 计数器 ***************************************/
volatile u8 COUNT_FRONT_L;
volatile u8 COUNT_FRONT_R;
volatile u8 COUNT_LEFT_U;
volatile u8 COUNT_RIGHT_U;
volatile u8 COUNT_RIGHT_D;

volatile u16 counter;
volatile u16 TIME;
u8 key;


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
    usart2_Init(9600);                               // 串口2  NodeMCU
    //uart5_Init(115200);                              // 串口5
    TIM9_Int_Init(100 - 1, 8400 - 1);	             // 10ms中断一次
    Servo_Init(10000 - 1, 56 - 1);                   // 舵机PWM频率为300Hz
    TIM12_PWM_Init(10000 - 1, 28 - 1);               //
    TIM7_Int_Init(200 - 1, 8400 - 1);                // TIM7 定时中断时间为：10ms ---- PID
    TIM13_Int_Init(16800 - 1, 5000 - 1);             // TIM13 溢出时间： 1s ---- 计时3分钟
    TIM11_Int_Init(16800 - 1, 1000 - 1);             // TIM11 溢出时间： 100ms ---- 延时定位
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
    OLED_Clear();

    /****************************初始化动作***************************************************/

    Strategy_ProcessColorInit(1);
    Strategy_FinishColorInit(1);
    Task_OLED();
    Action_FindBlob();
    /*****************************************************************************************/
    usart2_Clear();
    while(1)
    {
        key = KEY_Scan(0);
        if(key == 1)
        {
            //Task_Release();
            Task_MoveProcess();
            while(1);
        }
        else if(key == 2)
        {
            TIME = 0;
            TIM_Cmd(TIM13, ENABLE); //使能定时器 7
            Task_Debug();
            while(1);
        }
        else if(key == 3)
        {
            //Task_MoveProcessToMaterial();
            //while(1);
            //while(1)Task_TestSensor();
        }
        //Car_TrackBack1(1500);
        //Car_TrackRight();
        
//        Servo4_SpeedRegulation(0, 20);
//        delay_ms(500);
//        delay_ms(500);
//        Servo4_SpeedRegulation(180, 20);
//        delay_ms(500);
//        delay_ms(500);


//          OLED_ShowNum(80,6,counter);
//          if(counter > 20)
//          {
//              Beep_ring();
//              counter = 0;
//          }

//        key = KEY_Scan(0);
//        if(key == 1)
//        {
//            OLED_ShowString(80, 6, "1");
//        }
//        else if(key == 2)
//        {
//            OLED_ShowString(80, 6, "2");
//        }
//        else if(key == 3)
//        {
//            OLED_ShowString(80, 6, "3");
//        }


//        Servo3_SpeedRegulation(0, 20);
//        delay_Ntimes(2);
//        Servo3_SpeedRegulation(180, 20);
//        delay_Ntimes(2);

    }
}


/********************************End of File************************************/