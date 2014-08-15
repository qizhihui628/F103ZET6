/*
 * adc.c
 *
 *  Created on: 2014Äê8ÔÂ15ÈÕ
 *      Author: jeqi
 */

#include "adc.h"


#define ADC1_DR_Address    ((uint32_t)0x4001244C)
//#define ADC3_DR_Address    ((uint32_t)0x40013C4C)
__IO uint16_t ADC1ConvertedValue[10];			//ADC1 buffer.


void Adc1_Init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
/*Enable clock-----------------------------------------------------*/
	/* ADCCLK = PCLK2/4 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div4);

	/* Enable peripheral clocks --------------------------------------*/
	/* Enable DMA1 and DMA2 clocks */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);

	/* Enable ADC1, ADC2, ADC3 and GPIOC clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 |
	RCC_APB2Periph_ADC3 | RCC_APB2Periph_GPIOC, ENABLE);


}


