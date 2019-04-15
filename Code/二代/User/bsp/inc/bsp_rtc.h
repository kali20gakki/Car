/*
*********************************************************************************************************
*
*	模块名称 : RTC
*	文件名称 : bsp_rtc.c
*	版    本 : V1.0
*	说    明 : RTC底层驱动
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_RTC_H
#define __BSP_RTC_H

void RTC_TimeShow(void);
void RTC_AlarmShow(void);
void RTC_TimeRegulate(void);
void RTC_DateShow(void);
void bsp_InitRTC(void);
void RTC_Config(void);
uint8_t RTC_CalcWeek(uint16_t _year, uint8_t _mon, uint8_t _day);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

