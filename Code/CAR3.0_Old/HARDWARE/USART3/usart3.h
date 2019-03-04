#ifndef __USART3_H
#define __USART3_H
#include "sys.h"
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#define USART3_MAX_RECV_LEN 255
#define USART3_MAX_SEND_LEN 255

extern u8 USART3_RX_buf[USART3_MAX_RECV_LEN];
extern u8 USART3_TX_buf[USART3_MAX_SEND_LEN];
extern u16 USART3_RX_STA; //接收状态标记 
void uart3_init(u32 bound);

#endif