/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "port.h"
#include "stm32f10x_conf.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
static void RCC_Configuration(void)
{
  /* DMA clock enable */
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

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

static void NVIC_conf(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//设定USART1 中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART2_mb_Init(uint32_t bound)
{
	USART_InitTypeDef USART_InitStructure;
	/* System Clocks Configuration */
	RCC_Configuration();
	/* Configure the GPIO ports */
	GPIO_Configuration();
	/* Configure the DMA */
//	DMA_Configuration();
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

	  /*init interrupt*/
	  NVIC_conf();

	  /*Enable DMA USART2 */
	  //USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);


}



/**********************************************************************************/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */

	if (xRxEnable) {
		//使能接收和接收中断
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		//MAX485操作 低电平为接收模式
		//GPIO_ResetBits(GPIOD, GPIO_Pin_8);
	} else {
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
		//MAX485操作 高电平为发送模式
		//GPIO_SetBits(GPIOD, GPIO_Pin_8);
	}

	if (xTxEnable) {
		//使能发送完成中断
		USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	} else {
		//禁止发送完成中断
		USART_ITConfig(USART2, USART_IT_TC, DISABLE);
	}

}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{

	(void)ucPORT; //不修改串口
	(void)ucDataBits; //不修改数据位长度
	(void)eParity; //不修改校验格式

	USART2_mb_Init(ulBaudRate);

    return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */

	USART_SendData(USART2, ucByte);

    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
	*pucByte = USART_ReceiveData(USART2);
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}


void USART2_IRQHandler(void) {
//发生接收中断
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
		prvvUARTRxISR();
//清除中断标志位
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}

//发生完成中断
	if (USART_GetITStatus(USART2, USART_IT_TC) == SET) {
		prvvUARTTxReadyISR();
//清除中断标志
		USART_ClearITPendingBit(USART2, USART_IT_TC);
	}
}


