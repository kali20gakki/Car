#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

#define KEY1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10)
#define KEY2 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)
#define KEY3 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)
#endif