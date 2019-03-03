/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : key.c
** @auther  : Mrtutu
** @date    : 2019-02-17
** @describe: 按键底层驱动
**
**           KEY1 ---- PE0
**           KEY2 ---- PE2
**           KEY3 ---- PE4
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "key.h"
#include "delay.h"

/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : KEY_Init   按键初始化
* @param : None
* @return: None
* @note  : None
*
*/
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    /* IO初始化 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}



/*
* @auther: Mrtutu
* @date  ：2019-03-03
*
* @func  : KEY_Scan
* @param : mode: [输入/出] 
* @return: None
* @note  : None
*
*/
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1; //按键按松开标志
    if(mode)key_up = 1; //支持连按
    if(key_up && (KEY1 == 0 || KEY2 == 0 || KEY3 == 0 ))
    {
        delay_ms(10);//去抖动
        key_up = 0;
        if(KEY1 == 0)return 1;
        else if(KEY2 == 0)return 2;
        else if(KEY3 == 0)return 3;
    }
    else if(KEY1 == 1 && KEY2 == 1 && KEY3 == 0)key_up = 1;
    
    return 0;
}

/********************************End of File************************************/
