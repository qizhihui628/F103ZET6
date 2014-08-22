/*
 * timer4.c
 *
 *  Created on: 2014Äê8ÔÂ22ÈÕ
 *      Author: jeqi
 */

#include "timer4.h"


void TIM4_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;		//72000000/7200 = 10khz 0.1ms
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 99;			//100*0.1 = 10ms timeout
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	/*Enable timer4 interrupt*/
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM4, ENABLE);	// close interrupt

}


void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //
		{
			if(UART4_Timeout < UART4_TIMOUT_MAX)
			{
				UART4_Timeout++;
			}
			else if(UART4_Timeout == UART4_TIMOUT_MAX )
			{
				UART4_RX_STA|=1<<15;
				UART4_Timeout ++;	//close UART4_Timeout.
			}
			else
			{

			}

			if(Sim900a_Timeout)
			{
				Sim900a_Timeout -- ;
			}

		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //clear flag
		//TIM_Cmd(TIM4, DISABLE);	// close interrupt
		}
}
