#include "ano.h"
#include "usart3.h"
#include "math.h"
/******************************************
** 说明：
	1、 发送给上位机的数据帧定义
		@桢头--功能字--长度--数据（一个或多个，具体看协议说明）-校验
		@前2个字节为帧头0xAAAA
		@第3个字节为帧ID，也就是功能字，应设置为0xF1~0xFA中的一个
		@第4个字节为报文数据长度(dlc)
		@第5个字节开始到第5+dlc-1个字节为要传输的数据内容段，每个数据场为高字节在前，地字节在后
		@第5+dlc个字节为CheckSum,为第1个字节到第5+dlc-1个字节所有字节的值相加后，保留结果的低八位作为CheckSum
	2、 外部直接调用这个函数。
	3、 需要在此文件中引用需要发送的其他文件中的数据。
	4、 发送的数据必须是 int_16 型的数据
*****************************************/

void usart3_send_char(u8 c)
{
    while((USART3->SR & 0X40) == 0); //等待上一次发送完毕
    USART3->DR = c;
}


void send_Test()
{
    float count;
    for(count = 0.0; count < 360; count += 0.1)Encoder_send_data(10, 10 * sin(count));
}


void Encoder_send_data(int target, int Encoder_data)
{
    u8 send_buf[50]; //发送数据缓存
    u8 _cnt = 0;
    u8 i;
    u8 sum = 0;	//以下为计算sum校验字节，从0xAA也就是首字节，一直到sum字节前一字节

    send_buf[_cnt++] = 0xAA; //0xAA为帧头
    send_buf[_cnt++] = 0x05; //0x05为数据发送源，具体请参考匿名协议，本字节用户可以随意更改
    send_buf[_cnt++] = 0xAF; //0xAF为数据目的地，AF表示上位机，具体请参考匿名协议
    send_buf[_cnt++] = 0xF1; //0xF1，表示本帧为F1用户自定义帧，对应高级收码的F1功能帧
    send_buf[_cnt++] = 0; //本字节表示数据长度，这里先=0，函数最后再赋值，这样就不用人工计算长度了

    send_buf[_cnt++] = BYTE3(target); //将要发送的数据放至发送缓冲区
    send_buf[_cnt++] = BYTE2(target);
    send_buf[_cnt++] = BYTE1(target);
    send_buf[_cnt++] = BYTE0(target);

    send_buf[_cnt++] = BYTE3(Encoder_data); //将要发送的数据放至发送缓冲区
    send_buf[_cnt++] = BYTE2(Encoder_data);
    send_buf[_cnt++] = BYTE1(Encoder_data);
    send_buf[_cnt++] = BYTE0(Encoder_data);
    
    send_buf[4] = _cnt - 5; //_cnt用来计算数据长度，减5为减去帧开头5个非数据字节

    for(i = 0; i < _cnt; i++)
        sum += send_buf[i];

    send_buf[_cnt++] = sum;	//将sum校验数据放置最后一字节

    for(i = 0; i < _cnt; i++)usart3_send_char(send_buf[i]);
}