/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : common.c
** @auther  : Mrtutu
** @date    : 2019-04-01
** @describe: 
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/
#include "common.h"
#include "uart5.h"
#include "string.h"   




//WIFI STA模式,设置要去连接的路由器无线参数,请根据你自己的路由器设置,自行修改.
const u8* wifista_ssid = "Mrtutu";			//路由器SSID号
const u8* wifista_encryption = "wpawpa2_aes";	//wpa/wpa2 aes加密方式
const u8* wifista_password = "zhangweibaba"; 	//连接密码

const u8* IP_Adress="192.168.31.213";
const u8* Port="8888";




/*
* @auther: Mrtutu
* @date  ：2019-04-01
*
* @func  : ESP8266_check_cmd  ESP8266发送命令后,检测接收到的应答
* @param : str: [输入/出]   期待的应答结果
* @return: 0,没有得到期待的应答结果
*         其他,期待应答结果的位置(str的位置)
* @note  : None  
*
*/
u8* ESP8266_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(UART5_RX_STA&0X8000)		//接收到一次数据了
	{ 
		UART5_RX_BUF[UART5_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)UART5_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}




/*
* @auther: Mrtutu
* @date  ：2019-04-01
*
* @func  : ESP8266_send_cmd   向ESP8266发送命令
* @param : cmd: [输入/出]     发送的命令字符串
**		   ack: [输入/出]     期待的应答结果,如果为空,则表示不需要等待应答
**		   waittime: [输入/出]等待时间(单位:10ms)
* @return: 0,发送成功(得到了期待的应答结果)
           1,发送失败
* @note  : None
*
*/
u8 ESP8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	UART5_RX_STA=0;
	uart5_printf("%s\r\n",cmd);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(UART5_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(ESP8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效数据 
				}
					UART5_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 




/*
* @auther: Mrtutu
* @date  ：2019-04-01
*
* @func  : ESP8266_send_data   向ESP8266发送指定数据
* @param : data: [输入/出]     发送的数据(不需要添加回车了)
**		   ack: [输入/出]      期待的应答结果,如果为空,则表示不需要等待应答
**		   waittime: [输入/出] 等待时间(单位:10ms)
* @return: 0,发送成功(得到了期待的应答结果)    
* @note  : None
*
*/
u8 ESP8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	UART5_RX_STA=0;
	uart5_printf("%s",data);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(UART5_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(ESP8266_check_cmd(ack))break;//得到有效数据 
				UART5_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}




/*
* @auther: Mrtutu
* @date  ：2019-04-01
*
* @func  : ESP8266_quit_trans  ESP8266退出透传模式
* @param : None
* @return: 0,退出成功;
           1,退出失败
* @note  : None
*
*/
u8 ESP8266_quit_trans(void)
{
	while((UART5->SR&0X40)==0);	//等待发送空
	UART5->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((UART5->SR&0X40)==0);	//等待发送空
	UART5->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((UART5->SR&0X40)==0);	//等待发送空
	UART5->DR='+';      
	delay_ms(500);					//等待500ms
	return ESP8266_send_cmd("AT","OK",20);//退出透传判断.
}




/********************************End of File************************************/
