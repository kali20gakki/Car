/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : control.c
** @auther  : Mrtutu
** @date    : 2019-02-17
** @describe: PID 算法
**
**     顺序 ：1. Encoder_A = Read_Encoder(u8 TIMX)
**            2. Motor_A   = Incremental_PI_MotorA(int Encoder, int Target)
**            3. Limit_PWM()
**            4. Set_PWM(int motorA, int motorB, int motorC, int motorD)
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "control.h"
#include "motor.h"
#include "math.h"

extern volatile int Motor_A,Motor_B,Motor_C,Motor_D;         // PI算法返回赋值 临时PWM值

/* PI 速度控制器 参数定义 */
volatile float Kp = 1.0f;
volatile float Ki = 0.1f;



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Set_Pwm  给PWM 寄存器赋值
* @param : motorA: PI控制器 返回的PWM值
**		   motorB: PI控制器 返回的PWM值
**		   motorC: PI控制器 返回的PWM值
**		   motorD: PI控制器 返回的PWM值
* @return: None
* @note  : None
*
*/
void Set_PWM(int motorA, int motorB, int motorC, int motorD)
{
    /****************** MotorA*******************/
    if(motorA >= 0)
    {
        AIN2 = 1;
        AIN1 = 0;
        PWMA = abs(motorA);
    }
    else
    {
        AIN2 = 0;
        AIN1 = 1;
        PWMA = abs(motorA);
    }

    /****************** MotorB*******************/
    if(motorB >= 0)
    {
        BIN2 = 1;
        BIN1 = 0;
        PWMB = abs(motorB);
    }
    else
    {
        BIN2 = 0;
        BIN1 = 1;
        PWMB = abs(motorB);
    }

    /****************** MotorC*******************/
    if(motorC >= 0)
    {
        CIN2 = 1;
        CIN1 = 0;
        PWMC = abs(motorC);
    }
    else
    {
        CIN2 = 0;
        CIN1 = 1;
        PWMC = abs(motorC);
    }

    /****************** MotorD*******************/
    if(motorD >= 0)
    {
        DIN2 = 1;
        DIN1 = 0;
        PWMD = abs(motorD);
    }
    else
    {
        DIN2 = 0;
        DIN1 = 1;
        PWMD = abs(motorD);
    }
}



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Incremental_PI_MotorA  电机A PI速度控制器
* @param : Encoder:  实际速度值
* 		   Target :  目标速度值
* @return: None
* @note  : PWM 占空比
*
*/
int Incremental_PI_MotorA(int Encoder, int Target)
{
    static int Bias, Pwm, Last_bias;
    Bias = Encoder - Target;                    //计算偏差
    Pwm += Kp * (Bias - Last_bias) + Ki * Bias; //增量式PI控制器
    Last_bias = Bias;	                        //保存上一次偏差

    if(Pwm > 1000)Pwm = 1000;                   //限幅
    else if(Pwm < -1000)Pwm = -1000;

    return Pwm;                                 //增量输出
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Incremental_PI_MotorB  电机B PI速度控制器
* @param : Encoder:  实际速度值
* 		   Target :  目标速度值
* @return: None
* @note  : PWM 占空比
*
*/
int Incremental_PI_MotorB(int Encoder, int Target)
{
    static int Bias, Pwm, Last_bias;
    Bias = Encoder - Target;                    //计算偏差
    Pwm += Kp * (Bias - Last_bias) + Ki * Bias; //增量式PI控制器
    Last_bias = Bias;	                        //保存上一次偏差

    if(Pwm > 1000)Pwm = 1000;                   //限幅
    else if(Pwm < -1000)Pwm = -1000;

    return Pwm;                                 //增量输出
}



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Incremental_PI_MotorC  电机C PI速度控制器
* @param : Encoder:  实际速度值
* 		   Target :  目标速度值
* @return: None
* @note  : PWM 占空比
*
*/
int Incremental_PI_MotorC(int Encoder, int Target)
{
    static int Bias, Pwm, Last_bias;
    Bias = Encoder - Target;                    //计算偏差
    Pwm += Kp * (Bias - Last_bias) + Ki * Bias; //增量式PI控制器
    Last_bias = Bias;	                        //保存上一次偏差

    if(Pwm > 1000)Pwm = 1000;                   //限幅
    else if(Pwm < -1000)Pwm = -1000;

    return Pwm;                                 //增量输出
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : Incremental_PI_MotorD  电机D PI速度控制器
* @param : Encoder:  实际速度值
* 		   Target :  目标速度值
* @return: None
* @note  : PWM 占空比
*
*/
int Incremental_PI_MotorD(int Encoder, int Target)
{
    static int Bias, Pwm, Last_bias;
    Bias = Encoder - Target;                    //计算偏差
    Pwm += Kp * (Bias - Last_bias) + Ki * Bias; //增量式PI控制器
    Last_bias = Bias;	                        //保存上一次偏差

    if(Pwm > 1000)Pwm = 1000;                   //限幅
    else if(Pwm < -1000)Pwm = -1000;

    return Pwm;                                 //增量输出
}


/********************************End of File************************************/
