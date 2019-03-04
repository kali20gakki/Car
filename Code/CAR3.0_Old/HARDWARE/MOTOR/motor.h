#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

#define PWMD   TIM8->CCR1  
#define PWMC   TIM8->CCR2  
#define PWMB   TIM8->CCR4 
#define PWMA   TIM8->CCR3 

#define AIN1  PDout(2)
#define AIN2  PDout(0)

#define BIN1  PDout(1)
#define BIN2  PDout(3)

#define CIN1  PBout(2)
#define CIN2  PEout(8)

#define DIN1  PBout(0)
#define DIN2  PCout(4)

void motor_Pin_Init();
void Motor_Init(u16 arr,u16 psc);
#endif