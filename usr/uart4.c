/*
 * uart4.c
 *
 *  Created on: 2014Äê8ÔÂ18ÈÕ
 *      Author: jeqi
 */
#include "uart4.h"

__align(8) u8 UART4_TX_BUF[UART4_MAX_SEND_LEN];
u8 UART4_RX_BUF[UART4_MAX_RECV_LEN];
static DMA_InitTypeDef DMA_InitStructure;
__IO u16 UART4_Timeout = UART4_TIMOUT_MAX + 1;
__IO u16 UART4_RX_STA=0;

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
static void RCC_Configuration(void)
{
  /* DMA2 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
  /*Enable uart4 clock*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure UART4 Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  /* Configure UART4 Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

}

/**
  * @brief  Configures the DMA.
  * @param  None
  * @retval None
  */
static void DMA_Configuration(void)
{

  /* UART4 TX DMA2 Channel (triggered by USARTy Tx event) Config */
  DMA_DeInit(DMA2_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = UART4_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)UART4_TX_BUF;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel5, &DMA_InitStructure);

}


/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
static void NVIC_Config_UART4(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}





void UART4_Init(uint32_t bound)
{
	USART_InitTypeDef USART_InitStructure;
	/*Init timer4*/
//	TIM4_Init();
	/* System Clocks Configuration */
	RCC_Configuration();
	/* NVIC configuration */
	NVIC_Config_UART4();
	/* Configure the GPIO ports */
	GPIO_Configuration();
	/* Configure the DMA */
	DMA_Configuration();
	/* USARTy and USARTz configuration ------------------------------------------------------*/
	  /* USARTy and USARTz configured as follow:
	        - BaudRate = bound baud
	        - Word Length = 8 Bits
	        - One Stop Bit
	        - No parity
	        - Hardware flow control disabled (RTS and CTS signals)
	        - Receive and transmit enabled
	  */
	  USART_InitStructure.USART_BaudRate = bound;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	  USART_InitStructure.USART_Parity = USART_Parity_No;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	  /* Configure USARTy */
	  USART_Init(UART4, &USART_InitStructure);
	  /*ENABLE USART_IT_RXNE INTERRRUPT*/
	  USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	  /* Enable the USARTy */
	  USART_Cmd(UART4, ENABLE);
	  /*Enable DMA USART2 */
	  USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);


}


/*
 * */
static void UART4_DMA_TX_ENABLE(u16 len)
{

	DMA_Cmd(DMA2_Channel5,DISABLE);
	DMA_InitStructure.DMA_BufferSize = len;
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);
	DMA_Cmd(DMA2_Channel5,ENABLE);
}


void u4_printf(char* fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)UART4_TX_BUF,fmt,ap);
	va_end(ap);
	UART4_DMA_TX_ENABLE(strlen((const char*)UART4_TX_BUF)); 	//
	while(DMA_GetFlagStatus(DMA2_FLAG_TC5) == RESET)
		vTaskDelay(1/portTICK_RATE_MS);	//check if tx finished
	DMA_ClearFlag(DMA2_FLAG_TC5);
}




void UART4_IRQHandler(void)
{
	u8 res;
	if(USART_GetFlagStatus(UART4,USART_FLAG_RXNE) != RESET)
	{
		res = USART_ReceiveData(UART4);
		if(UART4_RX_STA < UART4_MAX_RECV_LEN )
		{
			UART4_Timeout = 0;		//reset timeout
			UART4_RX_BUF[UART4_RX_STA++]=res;	//receive data
		}
		else
		{
			UART4_RX_STA|=1<<15;
		}

		//USART_SendData(UART4,res);
	}
}

