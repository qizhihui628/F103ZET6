#include "usart.h"


char u1_txbuffer[U1_TxBufferSize];
char u1_rxbuffer[U1_RxBufferSize];

u8 u1_txcounter = 0;
u8 u1_rxcounter = 0;

__IO u16 u1_txlength = 0;
__IO u16 u1_rxlength = 0;

static void RCC_Configuration(void);
static void GPIO_Configuration(void);
static void NVIC_Configuration(void);

void usart1_config(uint32_t bound)
{
	USART_InitTypeDef USART_InitStructure;
	  /* System Clocks Configuration */
	  RCC_Configuration();
	  /* NVIC configuration */
	  NVIC_Configuration();
	  /* Configure the GPIO ports */
	  GPIO_Configuration();
	/* USARTy and USARTz configuration ------------------------------------------------------*/
	  /* USARTy and USARTz configured as follow:
	        - BaudRate = 9600 baud
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
	  USART_Init(USART1, &USART_InitStructure);

	  /* Enable USARTy Receive and Transmit interrupts */
	  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

	  /* Enable the USARTy */
	  USART_Cmd(USART1, ENABLE);

}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
static void RCC_Configuration(void)
{
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);


}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

#ifdef USE_STM3210C_EVAL
  /* Enable the USART3 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);

  /* Enable the USART2 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
#elif defined USE_STM3210B_EVAL || defined USE_STM32100B_EVAL
  /* Enable the USART2 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
#endif

  /* Configure USARTy Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USARTy Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  This function handles USARTy global interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    u1_rxbuffer[u1_rxcounter++] = USART_ReceiveData(USART1);

    if(u1_rxcounter == U1_RxBufferSize)
    {
      /* Disable the USARTy Receive interrupt */
      USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    }
  }

  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {
		if (u1_txlength != 0) {
			/* Write one byte to the transmit data register */
			USART_SendData(USART1, u1_txbuffer[u1_txcounter++]);

			if (u1_txcounter == u1_txlength) {
				/* Disable the USARTy Transmit interrupt */
				USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
				
			}
		}
  }
}



void u1_printf(char* fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)u1_txbuffer,fmt,ap);
	va_end(ap);
	u1_txlength = strlen((const char * )u1_txbuffer);
	u1_txcounter = 0;
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//enable tx.
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	//	vTaskDelay(1/portTICK_RATE_MS);	//check if tx finished
	USART_ClearFlag(USART1,USART_FLAG_TC);  //USART_ClearFlag

}
