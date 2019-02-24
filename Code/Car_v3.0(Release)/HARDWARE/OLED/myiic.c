/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : myiic.c
** @auther  : Mrtutu
** @date    : 2019-02-17
** @describe:
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

/********************************End of File************************************//********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, Main, China, Mrtutu.
**                           All Rights Reserved
**
**                                By Mrtutu
**                           Blog: www.mrtutu.cn
**
**----------------------------------文件信息------------------------------------
** @flie    : myiic.c
** @auther  : Mrtutu
** @date    : 2019-02-17
** @describe: IO 模拟 IIC --> OLED
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "myiic.h"
#include "delay.h"


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : IIC_Init
* @param : None
* @return: None
* @note  : None
*
*/
void IIC_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

    IIC_SCL = 1;
    IIC_SDA = 1;
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : IIC_Start  产生IIC起始信号
* @param : None
* @return: None
* @note  : None
*
*/
void IIC_Start(void)
{
    SDA_OUT();     //sda线输出
    IIC_SDA = 1;
    IIC_SCL = 1;
    delay_us(2);
    IIC_SDA = 0;   //START:when CLK is high,DATA change form high to low
    delay_us(2);
    IIC_SCL = 0;   //钳住I2C总线，准备发送或接收数据
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : IIC_Stop  产生IIC停止信号
* @param : None
* @return: None
* @note  : None
*
*/
void IIC_Stop(void)
{
    SDA_OUT();//sda线输出
    IIC_SCL = 0;
    IIC_SDA = 0; //STOP:when CLK is high DATA change form low to high
    delay_us(2);
    IIC_SCL = 1;
    IIC_SDA = 1; //发送I2C总线结束信号
    delay_us(2);
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : IIC_Wait_Ack
* @param : None
* @return: None
* @note  : 1，接收应答失败
*          0，接收应答成功
*
*/
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    SDA_IN();      //SDA设置为输入
    IIC_SDA = 1;
    delay_us(1);
    IIC_SCL = 1;
    delay_us(1);

    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL = 0; //时钟输出0
    return 0;
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : IIC_Ack  产生ACK应答
* @param : None
* @return: None
* @note  : None
*
*/
void IIC_Ack(void)
{
    IIC_SCL = 0;
    SDA_OUT();
    IIC_SDA = 0;
    delay_us(4);
    IIC_SCL = 1;
    delay_us(4);
    IIC_SCL = 0;
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : IIC_NAck  不产生ACK应答
* @param : None
* @return: None
* @note  : None
*
*/
void IIC_NAck(void)
{
    IIC_SCL = 0;
    SDA_OUT();
    IIC_SDA = 1;
    delay_us(4);
    IIC_SCL = 1;
    delay_us(4);
    IIC_SCL = 0;
}



/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : IIC_Send_Byte   IIC发送1字节数据
* @param : txd: 待发送的字节数据
* @return: None
* @note  : None
*
*/
u8 test=0;
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    IIC_SCL = 0; //拉低时钟开始数据传输

    for(t = 0; t < 8; t++)
    {
        if((txd & 0x80) >> 7)
            IIC_SDA = 1;
        else
            IIC_SDA = 0;

        txd <<= 1;
        delay_us(5);
        IIC_SCL = 1;
        delay_us(5);
        IIC_SCL = 0;
        delay_us(5);
    }
}


/*
* @auther: Mrtutu
* @date  ：2019-02-17
*
* @func  : IIC_Read_Byte  IIC 读取 1 Byte
* @param : ack:  ack=1时，发送ACK，ack=0，发送nACK 
* @return: None
* @note  : None
*
*/
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	 SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(5);
		    IIC_SCL=1;
        receive<<=1;
		     
        if(READ_SDA)receive++;   
		   delay_us(5); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}
/********************************End of File************************************/
