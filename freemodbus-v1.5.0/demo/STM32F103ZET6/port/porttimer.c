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
 * File: $Id: porttimer.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "stm32f10x_conf.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 3599;		//72000000/3600 = 20khz 50uS
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = usTim1Timerout50us;			//100*0.1 = 10ms timeout
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	/*预装载功能使能*/
	TIM_ARRPreloadConfig(TIM2,ENABLE);

	/*2 bits for pre-emption priority  2 bits for subpriority*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//清除溢出中断标志位
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	/*Disable timer4 interrupt*/
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE );

	TIM_Cmd(TIM2, DISABLE);	// close interrupt
    return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
	/* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	//设定定时器4的初始值
	TIM_SetCounter(TIM2, 0x0000);
	//定时器4启动
	TIM_Cmd(TIM2, ENABLE);

}

inline void
vMBPortTimersDisable(  )
{
	/* Disable any pending timers. */
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
	TIM_SetCounter(TIM4, 0x0000);
	//关闭定时器4
	TIM_Cmd(TIM4, DISABLE);
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}


void TIM4_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    //清除定时器T4溢出中断标志位
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    prvvTIMERExpiredISR( );
  }
}

