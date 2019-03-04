#include "usart3.h"

/**************************变量定义******************************/
u8 USART3_RX_buf[USART3_MAX_RECV_LEN];
u8 USART3_TX_buf[USART3_MAX_SEND_LEN];
u16 USART3_RX_STA=0;//接收状态标记
/****************************************************************/

/*
* 函数功能：串口3初始化
* 函数说明：TXD：PB10   RXD：PB11
*/

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
	USART3->DR = (u8) ch;      
	return ch;
}
#endif


void usart3_Init(u32 bound)
{
	/*结构体初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	/*总线使能*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);      //使能GPIOB时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);     //使能USART3时钟 
	/*引脚复用*/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);  //GPIOB10复用为USART3
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);  //GPIOB11复用为USART3
	/*USART3 端口初始化*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;              //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;              //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);                     //初始化
	/*串口中断初始化*/
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);            //开启接收中断
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;         //串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;   //抢占优先级3 数值越小优先级越高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;         //子优先级3,响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化VIC寄存器
  /* USART3初始化 */
	USART_InitStructure.USART_BaudRate = bound;               //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;     //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;        //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	USART_Init(USART3, &USART_InitStructure);                  //初始化串口3
	USART_Cmd(USART3, ENABLE);                                 //使能串口3 
}


/*
* 函数功能：字符串转U32整型
* 入口参数： 字符串
* 返回值  ;  u32整型
*/
unsigned long strtou32(char *str)
{
	unsigned long temp=0;
	unsigned long fact=1;
	unsigned char len=strlen(str);
	unsigned char i;
	for(i=len;i>0;i--)
	{
		temp+=((str[i-1]-0x30)*fact);
		fact*=10;
	}
	return temp;
}

/*
* 函数功能： 将一个32位的变量dat转为字符串，比如把1234转为"1234"
* 入口参数： dat:待转换的long型的变量   str:指向字符数组的指针，转换后的字节串放在其中 
* 返回值  ;  无
*/
void u32tostr(unsigned long dat,char *str)
{
	char temp[20];
	unsigned char i=0,j=0;
	i=0;
	while(dat)
	{
		temp[i]=dat%10+0x30;
		i++;
		dat/=10;
	}
	j=i;
	for(i=0;i<j;i++)
	{
		str[i]=temp[j-i-1];
	}
	if(!i) {str[i++]='0';}
	str[i]=0;
}


/*
* 函数功能：串口3中断函数
* 函数说明：
*
*/
void USART3_IRQHandler(void)
{
	u8 Res;
	/*接收中断 必须以0x0d 0x0a结尾*/
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		Res =USART_ReceiveData(USART3);//(USART3->DR) 读取接收到的数据
		
		if((USART3_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART3_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART3_RX_STA=0;//接收错误,重新开始
        else USART3_RX_STA|=0x8000; //接收完成了 
			}
			else
			{
				if(Res==0x0d)USART3_RX_STA|=0x4000;
        else
				{
					USART3_RX_buf[USART3_RX_STA&0X3FFF]=Res ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART3_MAX_RECV_LEN-1))USART3_RX_STA=0;//接收数据错误,重新开始接收  
        }  
			}
		}
	}
}