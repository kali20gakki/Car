#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

void Kinematic_Velocity(float Vx,float Vy,float Vz);
void Kinematic_Position(float Vx,float Vy,float Vz);
void setTargetMotors(long int a,long int b,long int c, long int d);
#endif

