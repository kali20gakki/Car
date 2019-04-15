/*
*********************************************************************************************************
*
*	模块名称 : ADC驱动模块
*	文件名称 : bsp_adc.c
*	版    本 : V1.2
*	说    明 : 实现ADC1和ADC3的双通道示波器，ADC2用于简单的电压采集
*	修改记录 :
*		版本号    日期        作者       说明
*		V1.0    2015-01-05   Eric2013   正式发布
*		V1.1    2015-12-29   Eric2013   修改函数TIM1_Config中TIM1的初始化。
*		V1.2    2018-01-06   Eric2013   修改之前的代码用于二代双通道示波器。
*
*	Copyright (C), 2018-2028, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp.h"
#include "MainTask.h"


/*
*********************************************************************************************************
*	                                 ADC寄存器
*********************************************************************************************************
*/
#define ADC_CDR_ADDRESS    ((uint32_t)0x40012308)
#define ADC3_DR_ADDRESS    ((uint32_t)0x4001224C)
#define ADC1_DR_ADDRESS    ((uint32_t)0x4001204C)
#define ADC2_DR_ADDRESS    ((uint32_t)0x4001214C)

/*
*********************************************************************************************************
*	                           ADC1，ADC3的数据缓存，大小均为10240
*********************************************************************************************************
*/
uint16_t ADC1ConvertedValue[10240];
uint16_t ADC3ConvertedValue[10240];

/*
*********************************************************************************************************
*	                                   变量和函数                           
*********************************************************************************************************
*/
static uint16_t g_usFirstTimeIRQFlag;  /* 第一次进入定时器8的中断标志 */
static uint16_t g_usTrigCount = 0;     /* 记录普通触发进入看门狗中断时，DMA传输的位置 */
static uint16_t g_usTrigTempFlag = 0;  /* 临时变量，用于ADC中断 */
static void Time8Recorder(void);

/*
*********************************************************************************************************
*	函 数 名: TIM1_Config
*	功能说明: 配置定时器1，用于触发ADC1和ADC3。
*             当外部触发信号被选为ADC规则或注入转换时，只有它的上升沿可以启动转换。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TIM1_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
	/* 使能定时器1 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	
    
	/* 先禁能再配置 */
    TIM_Cmd(TIM1, DISABLE);	
	
    /*
     ********************************************************************************
    system_stm32f4xx.c 文件中 void SetSysClock(void) 函数对时钟的配置如下：

    HCLK = SYSCLK / 1     (AHB1Periph)
    PCLK2 = HCLK / 2      (APB2Periph)
    PCLK1 = HCLK / 4      (APB1Periph)

    因为APB1 prescaler != 1, 所以 APB1上的TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
    因为APB2 prescaler != 1, 所以 APB2上的TIMxCLK = PCLK2 x 2 = SystemCoreClock;

    APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14
    APB2 定时器有 TIM1, TIM8 ,TIM9, TIM10, TIM11
          
    TIM1 更新周期是 = TIM1CLK / （TIM_Period + 1）/（TIM_Prescaler + 1）
    ********************************************************************************
    */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 	                                   //初始化定时器1的寄存器为复位值
    TIM_TimeBaseStructure.TIM_Period =  168000000/g_SampleFreqTable[TimeBaseId][0] - 1;    //ARR自动重装载寄存器周期的值(定时时间）到设置频率后产生个更新或者中断(也是说定时时间到)
    TIM_TimeBaseStructure.TIM_Prescaler = g_SampleFreqTable[TimeBaseId][1]-1;   		   //PSC时钟预分频数 例如：时钟频率=TIM1CLK/(时钟预分频+1)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    	            //CR1->CKD时间分割值
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //CR1->CMS[1:0]和DIR定时器模式 向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;		    /* TIM1 和 TIM8 必须设置 */	
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /**************ADC1的触发***********************************************/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				   //CCMR2在向上计数时，一旦TIMx_CNT<TIMx_CCR1时通道1为有效电平，否则为无效电平
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;      //CCER 输出使能          
    TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period/2;//CCR3同计数器TIMx_CNT的比较，并在OC1端口上产生输出信号 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;    	   //CCER输出极性设置
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;	   /* only for TIM1 and TIM8. 此处和正相引脚不同 */	
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;		   /* only for TIM1 and TIM8. */		 
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;	   /* only for TIM1 and TIM8. */
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;	   /* only for TIM1 and TIM8. */	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	
	/**************ADC3的触发***********************************************/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;     
    TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period-1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;	   /* only for TIM1 and TIM8. 此处和正相引脚不同 */	
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;		   /* only for TIM1 and TIM8. */		 
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;	   /* only for TIM1 and TIM8. */
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;	   /* only for TIM1 and TIM8. */		
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    //TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);		//CMR2 设置预装载使能  更新事件产生时写入有效
    //TIM_ARRPreloadConfig(TIM1, ENABLE);		   		 	//CR1  设置ARR自动重装 更新事件产生时写入有效
    TIM_Cmd(TIM1, ENABLE);	
	
    /* 使能PWM输出 */	
    TIM_CtrlPWMOutputs(TIM1, ENABLE);  		
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitADC
*	功能说明: ADC初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitADC(void)
{  
	ADC_InitTypeDef       ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    DMA_InitTypeDef       DMA_InitStructure;
    GPIO_InitTypeDef      GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
    /* 配置模拟看门狗中断NVIC */
    NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    /* 使能 ADC1, ADC2, DMA2 和 GPIO 时钟 ****************************************/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_ADC3, ENABLE);
	
	/* DMA2 Stream1 channel1 配置用于ADC3 **************************************/
	//DMA_DeInit(DMA2_Stream1); 
    DMA_InitStructure.DMA_Channel = DMA_Channel_2;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC3ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1024*10;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream1, ENABLE);
	
	/* DMA2 Stream0 channel0 配置用于ADC1 **************************************/
	//DMA_DeInit(DMA2_Stream0); 
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC1ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1024*10;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream0, ENABLE);

    /* 配置ADC引脚为模拟输入模式******************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
		
    /*
	***************************************************************************   
	  PCLK2 = HCLK / 2 
	  下面选择的是2分频
	  ADCCLK = PCLK2 /2 = HCLK / 4 = 168 / 4 = 42M
      ADC采样频率： Sampling Time + Conversion Time = 3 + 12 cycles = 15cyc
                    Conversion Time = 42MHz / 15cyc = 2.8Mbps.
	****************************************************************************
	*/
    
    /* ADC公共部分初始化**********************************************************/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_8Cycles;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInit(&ADC_CommonInitStructure); 
	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	 /*ADC3的配置*****************************************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC3;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
    
    /* ADC3 规则通道配置 */
	ADC_Init(ADC3, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC3, ADC_Channel_10, 1, ADC_SampleTime_3Cycles);

    /* 使能 ADC3 DMA */
	ADC_DMACmd(ADC3, ENABLE);
	
	/* 配置模拟看门狗的阀值 注意别配置反了，要不一直进入中断 */
    ADC_AnalogWatchdogThresholdsConfig(ADC3, 4095, 0);
    
    /* 配置模拟看门狗监测ADC3的通道10 */
    ADC_AnalogWatchdogSingleChannelConfig(ADC3, ADC_Channel_10);
    
    /* 使能一个规则通道的看门狗 */
    ADC_AnalogWatchdogCmd(ADC3, ADC_AnalogWatchdog_SingleRegEnable);

    /* 使能模拟看门狗中断 */
    ADC_ITConfig(ADC3, ADC_IT_AWD, ENABLE);
    
	/* 使能DMA请求 (多ADC模式) --------------------------------------------------*/
	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

	/* 使能 ADC3 --------------------------------------------------------------*/
	ADC_Cmd(ADC3, ENABLE);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	
	/*ADC1的配置******************************************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
    
    /* ADC1 规则通道配置 -------------------------------------------------------*/
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_3Cycles);

    /* 使能 ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* 使能DMA请求 (多ADC模式) --------------------------------------------------*/
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	/* 使能 ADC1 --------------------------------------------------------------*/
	ADC_Cmd(ADC1, ENABLE);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

    /*ADC2的配置*****************************************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC2;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
    
    /* ADC2 规则通道配置 */
	ADC_Init(ADC2, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_480Cycles);

	/* 使能 ADC2 */
	ADC_Cmd(ADC2, ENABLE);
	
	/**定时器配置********************************************************************/
	TIM1_Config();
}

/*
*********************************************************************************************************
*	函 数 名: DMA_Open
*	功能说明: 使能ADC1，ADC2，ADC3的DMA
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void ADC_DMA_Open(void)
{
    DMA_InitTypeDef       DMA_InitStructure;

	/* DMA2 Stream1 channel1 配置用于ADC3 **************************************/
    DMA_DeInit(DMA2_Stream1);  /* 在DMA的DMA_Mode_Normal模式，一定要使用这个函数，循环模式可以不用 */
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC3ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1024*10;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;          
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream1, ENABLE);
	
	/* DMA2 Stream0 channel0 配置用于ADC1 **************************************/
    DMA_DeInit(DMA2_Stream0);
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC1ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1024*10;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream0, ENABLE);

	ADC_RegularChannelConfig(ADC3, ADC_Channel_10, 1, ADC_SampleTime_3Cycles);

	/* 使能 ADC3 --------------------------------------------------------------*/
	ADC_Cmd(ADC3, ENABLE);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_3Cycles);

	/* 使能 ADC1 --------------------------------------------------------------*/
	ADC_Cmd(ADC1, ENABLE);

	/**定时器配置,一定要重新的初始化从而保证同时触发 **/
	TIM1_Config();

	/* 只有在普通触发模式在启动TIM8的计时功能 */
	if(TriggerFlag != 0)
	{
		Time8Recorder();
	}
}

/*
*********************************************************************************************************
*	函 数 名: DMA_Close
*	功能说明: 关闭ADC1，ADC2，ADC3及其DMA
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void ADC_DMA_Close()
{
	TIM_Cmd(TIM1, DISABLE);
	
	DMA_Cmd(DMA2_Stream1, DISABLE);
	
	DMA_Cmd(DMA2_Stream0, DISABLE);
	
	/* 禁止 ADC1 ---------------------------------------------------------------------*/
	ADC_Cmd(ADC1, DISABLE);
	
	/* 禁止 ADC3 ---------------------------------------------------------------------*/
	ADC_Cmd(ADC3, DISABLE);
}

/*
*********************************************************************************************************
*	函 数 名: ADC_IRQHandler
*	功能说明: 模拟看门狗中断服务程序。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void ADC_IRQHandler()
{ 
	/* 读取DMA剩余要传输的数目 */
	g_usTrigCount = DMA2_Stream1->NDTR;

	/* 确认是否是ADC3的看门狗中断 */
	if((ADC3->SR)&0x01)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
    {
		/* 取触发值的前一个点，查看是否是在阀值范围内，从而判断是上升沿还是下降沿 */
		if(g_usTrigCount == 10240)
		{
			/* 剩余10240表示触发值是ADC3ConvertedValue[10239]是触发值，那么上一个点就是10238 */
			g_usTrigTempFlag = ADC3ConvertedValue[10238]; 
		}
		else if(g_usTrigCount == 10239)
		{
			/* 剩余10239表示触发值是ADC3ConvertedValue[0]是触发值，那么上一个点就是10239 */
			g_usTrigTempFlag = ADC3ConvertedValue[10239]; 
		}
		else
		{
			/* 剩余10239表示触发值是ADC3ConvertedValue[0]是触发值，那么上一个点就是10239 */
			g_usTrigTempFlag = ADC3ConvertedValue[10238 - g_usTrigCount]; 
		}
		
		/* 判断是否是上升沿，是的话开启定时器记录ADC数据 */
		if(g_usTrigTempFlag <= g_TrigVol->usTrigValue)
		{
			/* 关闭ADC3的看门狗中断 */
			ADC3->CR1 &= 0xffffffbf;
			TriggerFlag = 1;
			
			/* 启动定时器计数 */
			TIM8->CR1 |= 0x01;
		}
        /* 清除挂起标志 */
		ADC3->SR &= 0xfe;
    }
}

/*
*********************************************************************************************************
*	函 数 名: TIM8_MeasureTrigConfig
*	功能说明: 使用TIM8为普通触发模式下数据采集计时，定时采集触发值前后的1024个ADC数据
*	形    参：无
*	返 回 值: 无		        
*********************************************************************************************************
*/
/* 
	每次捕获采集触发值前后的1024个ADC数据(单通道)。
*/
const uint32_t g_TrigFreqTable[][2] =
{
	{60,   1024},    //2.8Msps  168000000/2800000 = 60  => 60 * 1024 
	{84,   1024},    //2Msps    168000000/2000000 = 84  => 84 * 1024 
	{168,  1024},    //1Msps    168000000/1000000 = 168 => 168 * 1024
	{336,  1024},    //500Ksps  168000000/500000  = 336 => 336 * 1024
	{840,  1024},    //200Ksps  168000000/200000  = 840 => 840 * 1024
	
	{1680,  1024},   //100Ksps 168000000/100000  = 1680  => 1680 * 1024
	{3360,  1024},   //50Ksps  168000000/50000   = 3360  => 3360 * 1024 
	{8400,  1024},   //20Ksps  168000000/20000   = 8400  => 8400 * 1024
	{16800, 1024},   //10Ksps  168000000/10000   = 16800 => 16800 * 1024 
	{33600, 1024},   //5Ksps   168000000/5000    = 33600 => 33600 * 1024 

	/* 下面5种采样频率下刷新较慢，因为采集前后1024个ADC的时间较长 */
	{42000,    2048},    //2Ksps 168000000/2000  = 84000 => 84000 * 1024
	{42000,    4096},    //1Ksps 168000000/1000  = 168000 => 168000 * 1024 
	{42000,    8192},    //500sps 168000000/500  = 336000 => 336000 * 1024 
	{42000,    20480},   //200sps 168000000/200  = 840000 => 840000 * 1024 
	{42000,    40960},   //100sps 168000000/100  = 1680000 => 1680000 * 1024
	
	/* 下面这几种采样率不做触发支持 */
	{42000,    40960},   //50sps 
	{42000,    40960},   //20sps
	{42000,    40960},   //10sps 
	{42000,    40960},   //5sps 
	{42000,    40960},   //2sps 
	{42000,    40960},   //1sps 	
};

void TIM8_MeasureTrigConfig(void)
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 开启时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	
	/* 加上第一次进入中断的标志，进入中断后将其置1 */
	g_usFirstTimeIRQFlag = 0;

	/* 使能定时器8中断  */
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* 定时器配置 */
	TimeBaseId = 1;        /* 开机后按照ADC单通道1Msps进行配置 */
	TIM_DeInit(TIM8);  
	TIM_BaseInitStructure.TIM_Period = g_TrigFreqTable[TimeBaseId][0] - 1;
	TIM_BaseInitStructure.TIM_Prescaler = g_TrigFreqTable[TimeBaseId][1] - 1;
	TIM_BaseInitStructure.TIM_ClockDivision = 0;
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM8, &TIM_BaseInitStructure); 
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM8, DISABLE);
}

/*
*********************************************************************************************************
*	函 数 名: Time8Recorder
*	功能说明: 使用TIM8为普通触发模式下数据采集计时，定时采集触发值前后的1024个ADC数据
*	形    参：无
*	返 回 值: 无		        
*********************************************************************************************************
*/
static void Time8Recorder(void)
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure; 
	
	/* 加上第一次进入中断的标志 */
	g_usFirstTimeIRQFlag = 0;
	
	TIM_DeInit(TIM8);  
	TIM_BaseInitStructure.TIM_Period = g_TrigFreqTable[TimeBaseId][0] - 1;
	TIM_BaseInitStructure.TIM_Prescaler = g_TrigFreqTable[TimeBaseId][1] - 1;
	TIM_BaseInitStructure.TIM_ClockDivision = 0;
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM8, &TIM_BaseInitStructure); 
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM8, DISABLE);
}

/*
*********************************************************************************************************
*	函 数 名: TIM8_UP_TIM13_IRQHandler
*	功能说明: 定时器中断函数
*	形    参: 无
*	返 回 值: 无		        
*********************************************************************************************************
*/
void TIM8_UP_TIM13_IRQHandler(void)
{
	uint16_t i, k, usCurPos;
	
	/* 第一次进入TIM8中断执行退出操作，因为定时器更新中断有个小bug，
	   一旦初始化了定时器并且使能中断就会有立即进入一次中断的情况，为了防止这种情况的
	   出现，作如下操作：
	*/
	if(g_usFirstTimeIRQFlag == 0)
	{
		g_usFirstTimeIRQFlag = 1;
		
		TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
		
		/* 如果是第一次进入就退出 */
		return;
	}
	
	/* 定时器时间中断 */
	if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
	{
		/* 先关闭定时器 */
		TIM_Cmd(TIM8, DISABLE);
	
		/* 确保开启的定时器计时采集ADC数据 */
		if(TriggerFlag == 1)
		{
			/* 定时器采集ADC数据结束 */
			TriggerFlag = 2;
			
			/* 开启定时器计数前DMA传输数据的位置 */
			usCurPos = 10240 - g_usTrigCount;
			
			/*
				下面分为三种情况获取6K的数据(三通道，单通道就是前后1024个ADC数据)：
			     1. 当前的位置 < 1024.
			     2. 1024 <= 当前的位置 <= 10240 - 1024(9216)
				 3. 当前位置 > 10240 - 1024(9216)
			*/
			/* 第一种情况：当前的位置 < 1024. */
			if(usCurPos < 1024)
			{
				k = 1024 - usCurPos;
				usCurPos = 10240 - k;
				
				/* 前部分数据 */
				for(i = 0; i < k; i++)
				{
					g_DSO1->usWaveBufTemp[i] = ADC3ConvertedValue[i+usCurPos];
				}
				
				usCurPos = i;
				k = 10240 - g_usTrigCount + 1024;
				
				/* 后部分数据 */
				for(i = 0; i < k; i++)
				{
					g_DSO1->usWaveBufTemp[i + usCurPos] = ADC3ConvertedValue[i];
				}
			}
			/* 第三种情况：当前位置 > 10240 - 1024(9216) */
			else if(usCurPos > 9216)
			{
				usCurPos = usCurPos - 1024;
				
				/* 采集前1024+g_usTrigCount的数据 */
				for(i = 0; i < (g_usTrigCount + 1024); i++)
				{
					g_DSO1->usWaveBufTemp[i] = ADC3ConvertedValue[i+usCurPos];
				}
				
				k = i;
				usCurPos = (1024 - g_usTrigCount);
				
				/* 剩余数据的采集 */
				for(i = 0; i < usCurPos; i++)
				{
					g_DSO1->usWaveBufTemp[i + k] = ADC3ConvertedValue[i]; 
				}
			}
			/* 第二种情况：1024 <= 当前的位置 <= 10240 - 1024(9216)  */
			else
			{
				usCurPos = usCurPos - 1024;
				for(i = 0; i < 2048; i++)
				{
					g_DSO1->usWaveBufTemp[i] = ADC3ConvertedValue[i+usCurPos];
				}
			}
			
		}
	
		TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
	}
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
