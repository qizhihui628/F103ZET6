/*
 * usart2.c
 *
 *  Created on: 2014Äê8ÔÂ18ÈÕ
 *      Author: jeqi
 */
#include "usart2.h"

__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN];
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN];
static DMA_InitTypeDef DMA_InitStructure;

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
static void RCC_Configuration(void)
{
  /* DMA clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
  /*Enable usart2 clock*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure USART2 Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Configure USART2 Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/**
  * @brief  Configures the DMA.
  * @param  None
  * @retval None
  */
static void DMA_Configuration(void)
{


  /* USART2 TX DMA1 Channel (triggered by USARTy Tx event) Config */
  DMA_DeInit(DMA1_Channel7);
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART2_TX_BUF;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel7, &DMA_InitStructure);

}

void USART2_Init(uint32_t bound)
{
	USART_InitTypeDef USART_InitStructure;
	/* System Clocks Configuration */
	RCC_Configuration();
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
	  USART_Init(USART2, &USART_InitStructure);


	  /* Enable the USARTy */
	  USART_Cmd(USART2, ENABLE);


	  /*Enable DMA USART2 */
	  USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);


}


/*
 * */
static void USART2_DMA_TX_ENABLE(u16 len)
{

	DMA_Cmd(DMA1_Channel7,DISABLE);
	DMA_InitStructure.DMA_BufferSize = len;
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel7,ENABLE);
}


void u2_printf(char* fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	USART2_DMA_TX_ENABLE(strlen((const char*)USART2_TX_BUF)); 	//
	while(DMA_GetFlagStatus(DMA1_FLAG_TC7) == RESET)
		vTaskDelay(1/portTICK_RATE_MS);	//check if tx finished
	DMA_ClearFlag(DMA1_FLAG_TC7);
}



