#ifndef __SENSOR_H
#define __SENSOR_H
#include "sys.h"

#define SENSOR1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)
#define SENSOR2 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)
#define SENSOR3 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15)
#define SENSOR4 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
#define SENSOR5 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)

#define SENSOR_FRONT1 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)
#define SENSOR_FRONT2 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13)

void Sensor_count_Init2();
#endif