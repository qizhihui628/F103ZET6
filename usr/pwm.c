/*
 * pwm.c
 *
 *  Created on: 2014Äê8ÔÂ14ÈÕ
 *      Author: jeqi
 */
#include "pwm.h"


__IO uint16_t CCR1_Val = 333;
__IO uint16_t CCR2_Val = 249;
__IO uint16_t CCR3_Val = 166;
__IO uint16_t CCR4_Val = 83;
__IO uint16_t PrescalerValue = 0;


void Pwm_Init(void)
{


	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;



	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	 /* GPIOB Configuration:TIM4 Channel1, 2, 3 and 4 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* -----------------------------------------------------------------------
	 TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
	 The TIM3CLK frequency is set to SystemCoreClock (Hz), to get TIM3 counter
	 clock at 24 MHz the Prescaler is computed as following:
	 - Prescaler = (TIM3CLK / TIM3 counter clock) - 1
	 SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
	 and Connectivity line devices and to 24 MHz for Low-Density Value line and
	 Medium-Density Value line devices

	 The TIM3 is running at 36 KHz: TIM3 Frequency = TIM3 counter clock/(ARR + 1)
	 = 24 MHz / 666 = 36 KHz
	 TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
	 TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
	 TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
	 TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
	 ----------------------------------------------------------------------- */
	/*----------------------------------------------------------------------- */
	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 665;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

	TIM_OC2Init(TIM4, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;

	TIM_OC3Init(TIM4, &TIM_OCInitStructure);

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

	TIM_OC4Init(TIM4, &TIM_OCInitStructure);

	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);

	/* TIM3 enable counter */
	TIM_Cmd(TIM4, ENABLE);


}

void vPWM_Refresh(void *pvParameters)
{
	for (;;)
	{
		taskENTER_CRITICAL();
		TIM_SetCompare1(TIM4, CCR1_Val);
		TIM_SetCompare2(TIM4, CCR2_Val);
		TIM_SetCompare3(TIM4, CCR3_Val);
		TIM_SetCompare4(TIM4, CCR4_Val);
		taskEXIT_CRITICAL();
		vTaskDelay(500 / portTICK_RATE_MS);
	}
}



