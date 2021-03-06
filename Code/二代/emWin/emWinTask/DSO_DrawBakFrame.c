/*
*********************************************************************************************************
*	                                  
*	模块名称 : 绘制示波器的方格
*	文件名称 : DSO_DrawBakFrame.c
*	版    本 : V1.0
*	说    明 : 绘制示波器的方格
*	修改记录 :
*		版本号    日期          作者         说明
*		V1.0    2015-01-05    Eric2013       首发
*	
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"
#include "MainTask.h"



/*
*********************************************************************************************************
*	函 数 名: CreateWindowAmplitude
*	功能说明: 创建幅值窗口
*	形    参: x0  左上角x坐标
*             y0  左上角y坐标
*             x1  右下角x坐标
*             y1  右下角y坐标
*	返 回 值: 无
*********************************************************************************************************
*/
void DSO_DrawBakFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) 
{
	uint16_t x;
	uint16_t y;
	
	/* 填充背景 */
	GUI_SetBkColor(GUI_BLACK);
	GUI_ClearRect(x0, y0, x1, y1);  
	
	GUI_SetColor(GUI_WHITE);
	
	/* 绘制水平刻度点 */
	for (y = 0; y < 9; y++)								
	{
		for (x = 0; x < 61; x++)
		{
			GUI_DrawPoint(x0 + (x * 10), y0 + (y * 50));
		}
	}
	
	for (x = 0; x < 61; x++)
	{
		GUI_DrawPoint(x0 + (x * 10), y1);
	}
	
	/* 绘制垂直刻度点 */
	for (x = 0; x < 12; x++)							
	{
		for (y = 0; y < 41; y++)
		{
			GUI_DrawPoint(x0 + (x * 50), y0 + (y * 10));
		}
	}
	
	for (y = 0; y < 41; y++)
	{
		GUI_DrawPixel(x1, y0 + (y * 10));
	}
	
	/* 绘制最后脚上的那个点 */
	GUI_DrawPixel(x1 - 1, y1 - 1);
	
	/* 绘制垂直中心刻度点 */
	for (y = 0; y < 41; y++)							
	{	 
		GUI_DrawPixel(x0 - 1 + (300), y0 + (y * 10));
		GUI_DrawPixel(x0 + 1 + (300), y0 + (y * 10));
	}
	
	GUI_DrawPixel(x0 - 1 + (300), y1);
	GUI_DrawPixel(x0 + 1 + (300), y1);
	
	
	/* 绘制水平中心刻度点 */
	for (x = 0; x < 61; x++)								
	{	 
		GUI_DrawPixel(x0 + (x * 10), y0 - 1 + (200));
 		GUI_DrawPixel(x0 + (x * 10), y0 + 1 + (200));
	}
	 
 	GUI_DrawPixel(x1, y0 - 1 + (200));
  	GUI_DrawPixel(x1, y0 + 1 + (200));
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
