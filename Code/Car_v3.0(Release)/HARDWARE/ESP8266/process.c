/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : process.c
** @auther  : Mrtutu
** @date    : 2019-04-01
** @describe: ESP 连接路由开启TCP UDP
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/
#include "process.h"
#include "string.h"
#include "common.h"
#include "oled.h"
/*******************************************************************************
* TCP/UDP Client 透传流程（首次连接）
* 1. AT+CWMODE=1 //设置为 STA 模式      ------   响应 OK
* 2. AT+RST      //重启                 ------   响应 OK
* 3. AT+CWJAP=”ssid”,”password” //连接对象路由器的 ssid 和 password  ------   响应 OK
* 4. AT+CIPSTART="TCP","192.168.1.115",8080  // 连接Server 地址+端口号    ------   响应 CONNECT OK
* 5. AT+CIPMODE=1 // 开启透传           ------   响应 OK
* 6. AT+CIPSEND   // 开始透传
* 7. 互发数据
*******************************************************************************/


/*******************************************************************************
*  TCP/UDP Client 透传流程（快速连接）
* 1. 连接路由器
* 2. AT+SAVETRANSLINK=1,"192.168.1.xxx",8086,"TCP" // 设置模块为上电自动连接到 TCP Server： 192.168.1.xxx， 8086，并进入透传模式
*
*******************************************************************************/

u8 MODE[4] = "TCP";
u8 p[64];


void ESP8266_Set_Link()
{
    while(ESP8266_send_cmd("AT+CWMODE=1", "OK", 500) == 1);		//设置WIFI STA模式
    // OLED DEBUG
    OLED_ShowString(96,6,"1");
    /**连接路由器**/
    delay_ms(100);
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",
                                 wifista_ssid,
                             wifista_password);
	while(ESP8266_send_cmd((char*)p,"WIFI CONNECTED",500) == 1);
    // OLED DEBUG
    OLED_ShowString(96,6,"2");
    delay_ms(100);
    
    /**连接 TCP/UDP 服务器**/
    sprintf((char*)p,"AT+CIPSTART=\"%s\",\"%s\",%s",MODE,IP_Adress,Port);
    while(ESP8266_send_cmd((char*)p,"CONNECT",500) == 1);
    // OLED DEBUG
    OLED_ShowString(96,6,"3");
    /**设置透传模式**/
    while(ESP8266_send_cmd("AT+CIPMODE=1","OK",500) == 1);
    // OLED DEBUG
    OLED_ShowString(96,6,"4");
    delay_ms(100);
    /**开始透传**/
    while(ESP8266_send_cmd("AT+CIPSEND","OK",500) == 1);
    //ESP8266_send_cmd("AT+CIPSEND","OK",100);
    // OLED DEBUG
    OLED_ShowCHinese(80, 6, 12);
    OLED_ShowCHinese(96, 6, 13);
    OLED_ShowCHinese(112, 6, 14);
}


/********************************End of File************************************/