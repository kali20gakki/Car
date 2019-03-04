#ifndef __SERVO_H
#define __SERVO_H
#include "sys.h"

void servo_Init(u16 arr,u16 psc);

void Servo1_SetAngle(int angle);
void Servo2_SetAngle(int angle);
void Servo3_SetAngle(int angle);
void Servo4_SetAngle(int angle);
void Servo5_SetAngle(int angle);

void Servo1_SpeedRegulation(int next_angle);
void Servo2_SpeedRegulation(int next_angle);
void Servo3_SpeedRegulation(int next_angle);
void Servo4_SpeedRegulation(int next_angle);
void Servo5_SpeedRegulation(int next_angle);
#endif