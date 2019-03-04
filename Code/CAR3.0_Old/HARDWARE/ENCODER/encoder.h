#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"

void encoder_MotorA_Init();
void encoder_MotorB_Init();
void encoder_MotorC_Init();
void encoder_MotorD_Init();

s16 Read_Encoder(u8 TIMX);
#endif