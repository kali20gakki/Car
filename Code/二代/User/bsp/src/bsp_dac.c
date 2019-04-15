/*
*********************************************************************************************************
*
*	模块名称 : DAC驱动模块
*	文件名称 : bsp_dac.c
*	版    本 : V1.0
*	说    明 : 实现DAC输出方波
*	修改记录 :
*		版本号    日期        作者     说明
*		V1.0    2015-01-05   Eric2013   正式发布
*		V1.1    2015-12-29   Eric2013   修改DAC的初始化，稍初始了一个参数
*                                       DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
*
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp.h"


#define DAC_DHR12R1_ADDRESS    0x40007408  
uint16_t g_usWaveBuff[64];     /* 波形缓存 */
/*
*********************************************************************************************************
*	函 数 名: bsp_InitDAC
*	功能说明: DAC初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitDAC(void)
{   
    uint8_t i;

	/* 用缓存放一个周期的方波 */
	for(i =0; i < 32; i++)
	{
		g_usWaveBuff[i] = 0;
	}
	
	for(i =0; i < 32; i++)
	{
		g_usWaveBuff[i+32] = 3000;
	}
	
	/* MDK优化等级为1的时候，DAC初始化顺序，不同的优化等级，
	   不同的输出顺序容易操作DAC无法输出波形。
	   这个问题要引起大家的特别注意！！。
	*/
	DAC_Ch1_WaveConfig();
	DAC_GPIOConfig(); 
    TIM6_Config(); 
}

/*
*********************************************************************************************************
*	函 数 名: TIM6_Config
*	功能说明: 配置定时器6，用于触发DAC。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM6_Config(void)
{
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
    
     /* 使能TIM6时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    
    /* --------------------------------------------------------
    system_stm32f4xx.c 文件中 void SetSysClock(void) 函数对时钟的配置如下：

    HCLK = SYSCLK / 1     (AHB1Periph)
    PCLK2 = HCLK / 2      (APB2Periph)
    PCLK1 = HCLK / 4      (APB1Periph)

    因为APB1 prescaler != 1, 所以 APB1上的TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
    因为APB2 prescaler != 1, 所以 APB2上的TIMxCLK = PCLK2 x 2 = SystemCoreClock;

    APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14
    APB2 定时器有 TIM1, TIM8 ,TIM9, TIM10, TIM11
          
    TIM6 更新周期是 = TIM6CLK / （TIM_Period + 1）/（TIM_Prescaler + 1）
	根据如下的配置，更新周期是：
	TIM6CLK / （TIM_Period + 1）/（TIM_Prescaler + 1）
	= 84MHz /（55+1）/（0+1）
	= 1.5MHz
    ----------------------------------------------------------- */
    
    /* 配置TIM6 */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
    TIM_TimeBaseStructure.TIM_Period = 100;          
    TIM_TimeBaseStructure.TIM_Prescaler = 0;       
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    /* TIM6 TRGO 选择 */
    TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

    /* 使能TIM6 */
    TIM_Cmd(TIM6, ENABLE);   
}

/*
*********************************************************************************************************
*	函 数 名: DAC_GPIOConfig
*	功能说明: DAC通道一的PA4引脚配置
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DAC_GPIOConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
    /*  使能引脚时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    /* DAC通道1输出引脚配置（PA.4） */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
}

/*
*********************************************************************************************************
*	函 数 名: DAC_Ch1_WaveConfig
*	功能说明: DAC通道1方波输出
*             上面函数TIM6_Config已将将TIM6的输出配置成1.5MHz，下面DMA的缓存大小是64,那么方波的输出频率就是
*             方波周期 = TIM6更新周期/64 = 1.5MHz / 64 = 23437.5Hz
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DAC_Ch1_WaveConfig(void)
{
    DAC_InitTypeDef  DAC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
      
    /* DAC 外设时钟使能 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    
    /* DMA1 时钟使能 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
     
    /* DAC通道1配置 */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;/* 此参数也需要初始化 */
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    
    /* DMA1_Stream5 通道7配置 **************************************/  
    DMA_DeInit(DMA1_Stream5);
    DMA_InitStructure.DMA_Channel = DMA_Channel_7;  
    DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1_ADDRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&g_usWaveBuff;
    DMA_InitStructure.DMA_BufferSize = 64;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream5, &DMA_InitStructure);    

    /* 使能 DMA1_Stream5 */
    DMA_Cmd(DMA1_Stream5, ENABLE);

    /* 使能DAC通道 */
    DAC_Cmd(DAC_Channel_1, ENABLE);

    /* 使能DAC的DMA通道 */
    DAC_DMACmd(DAC_Channel_1, ENABLE);    
}


/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
