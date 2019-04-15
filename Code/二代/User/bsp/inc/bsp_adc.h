/*
*********************************************************************************************************
*
*	模块名称 : DAC驱动模块
*	文件名称 : bsp_dac.c
*	版    本 : V1.0
*	说    明 : 实现DAC输出方波
*	修改记录 :
*		版本号    日期        作者     说明
*		V1.0    2015-01-05   Eric2013   正式发布
*
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "stm32f4xx.h"

void bsp_InitADC(void);
void TIM8_MeasureTrigConfig(void);

extern uint16_t count;
extern void ADC_DMA_Open(void);
extern void ADC_DMA_Close(void);
extern uint16_t ADC1ConvertedValue[10240];
extern uint16_t ADC2ConvertedValue[10240];
extern uint16_t ADC3ConvertedValue[10240];

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
