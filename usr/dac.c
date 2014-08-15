/*
 * dac.c
 *
 *  Created on: 2014Äê8ÔÂ15ÈÕ
 *      Author: jeqi
 */

#include "dac.h"

void Dac1_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //Enable porta clock
   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //Enable dac clock

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // config port
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //Analog input
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_SetBits(GPIOA,GPIO_Pin_4)	;//PA.4 output "1"

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	// no trigger
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//no wave generation
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//output buffer disable
    DAC_Init(DAC_Channel_1,&DAC_InitType);	 //init

	DAC_Cmd(DAC_Channel_1, ENABLE);  //enable

    DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12bit right align

}

//output voltage
//vol:0~3300,voltage:0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	float temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//
}

