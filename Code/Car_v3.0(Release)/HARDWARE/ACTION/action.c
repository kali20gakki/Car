/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : action.c
** @auther  : Mrtutu
** @date    : 2019-03-06
** @describe:
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

/********************************End of File************************************/
/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : action.c
** @auther  : Mrtutu
** @date    : 2019-03-03
** @describe:
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "action.h"
#include "servo.h"
#include "strategy.h"


void Action_FindBlob(void)
{
    
    Servo3_SpeedRegulation(179, 10);
    delay_ms(100);
    Servo2_SpeedRegulation(50, 10);
    delay_ms(100);
    Servo1_SpeedRegulation(80, 10);
    delay_ms(100);
    Servo4_SpeedRegulation(80, 10);
    delay_ms(100);
    Servo5_SpeedRegulation(160, 10);
    delay_ms(100);
    
    Servo4_SpeedRegulation(100, 10);
    delay_ms(100);
}


void Action_FindQrcode(void)
{
    
}





void Action_3(void)
{
    Servo4_SpeedRegulation(20, 10);
    Servo5_SpeedRegulation(100, 10);
    Servo1_SpeedRegulation(57, 10);
    Servo3_SpeedRegulation(145, 10);
    Servo2_SpeedRegulation(120, 10);
    Servo4_SpeedRegulation(43, 10);
    Servo5_SpeedRegulation(52, 10);
    
    delay_ms(200);
    Servo4_SpeedRegulation(0, 10);
    
    Servo2_SpeedRegulation(90, 10);
    
    Servo1_SpeedRegulation(250, 10);
    
    
//    // 放置
//    Servo2_SpeedRegulation(150, 10);
//    
//    Servo4_SpeedRegulation(17, 10);
//    
//    // 回收
//    Servo5_SpeedRegulation(120, 10);
//    
//    Servo2_SpeedRegulation(90, 10);
//    Servo1_SpeedRegulation(87, 10);
    
}


void Action_4(void)
{
    Servo3_SpeedRegulation(179, 10);
    Servo2_SpeedRegulation(50, 10);
    Servo1_SpeedRegulation(82, 10);
    Servo4_SpeedRegulation(80, 10);
    Servo5_SpeedRegulation(120, 10);
    
    // 抓取
    Servo3_SpeedRegulation(140, 10);
    Servo2_SpeedRegulation(90, 10);
    Servo4_SpeedRegulation(57, 10);
    Servo2_SpeedRegulation(120, 10);
    Servo5_SpeedRegulation(52, 10);
    
    delay_ms(200);
    Servo4_SpeedRegulation(0, 10);
    Servo2_SpeedRegulation(90, 10);
    Servo1_SpeedRegulation(250, 10);
}

void Action_5(void)
{
    Servo4_SpeedRegulation(20, 10);
    Servo5_SpeedRegulation(100, 10);
    Servo1_SpeedRegulation(115, 10);
    Servo3_SpeedRegulation(130, 10);
    Servo2_SpeedRegulation(130, 10);
    Servo4_SpeedRegulation(60, 10);
    Servo5_SpeedRegulation(50, 10);
    
    delay_ms(200);
    Servo4_SpeedRegulation(0, 10);
    
    Servo2_SpeedRegulation(110, 10);
    
    Servo1_SpeedRegulation(250, 10);
    
}



void Action_Place_Process()
{
    Servo2_SpeedRegulation(150, 10);
    
    Servo4_SpeedRegulation(17, 10);
    
    // 回收
    Servo5_SpeedRegulation(120, 10);
    
    Servo2_SpeedRegulation(90, 10);
    Servo1_SpeedRegulation(87, 10);
}


void Action_Take_Process()
{
    Servo5_SpeedRegulation(120, 10);
    Servo4_SpeedRegulation(0, 10);
    Servo2_SpeedRegulation(110, 10); 
    Servo1_SpeedRegulation(250, 10);
    
    Servo2_SpeedRegulation(150, 10);   
    Servo4_SpeedRegulation(17, 10);
    Servo5_SpeedRegulation(52,10);
    
    delay_ms(500);
    Servo2_SpeedRegulation(45, 10); 
    Servo3_SpeedRegulation(180, 10); 
    Servo1_SpeedRegulation(170, 10);    
    
}


void Action_Place_Finish()
{
    Servo3_SpeedRegulation(120, 10); 
    delay_ms(500);
    Servo2_SpeedRegulation(135, 10);
    Servo4_SpeedRegulation(37, 10);
    Servo2_SpeedRegulation(155, 10);
    Servo5_SpeedRegulation(120,10);
    
    delay_ms(500);
    Servo3_SpeedRegulation(100, 10);
    Servo2_SpeedRegulation(90, 10);
    Servo1_SpeedRegulation(250, 10);
    Servo3_SpeedRegulation(180, 10);
    
}
/********************************End of File************************************/
