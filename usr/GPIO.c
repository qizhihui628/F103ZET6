/*
 * GPIO.c
 *
 *  Created on: 2014��8��13��
 *      Author: jeqi
 */

#include "GPIO.h"



void IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIOD Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG, ENABLE);	//GPIOF GPIOD OUTPUT,GPIOE GPIOG INPUT

/* OUTPUT GPIOD GPIOF */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOF, &GPIO_InitStructure);

/* INPUT GPIOE GPIOG */
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_All;//PE2~4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��GPIOG

#if 0
	//��ʼ�� WK_UP-->GPIOA.0	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0
#endif


}

__IO uint16_t GPIOD_BUFFER = 0;/*OUTPUT BUFFER*/
__IO uint16_t GPIOF_BUFFER = 0;
__IO uint16_t GPIOE_BUFFER = 0;	/*INPUT BUFFER*/
__IO uint16_t GPIOG_BUFFER = 0;

void vIO_Refresh(void *pvParameters)
{
	for(;;)
	{
		taskENTER_CRITICAL();
		GPIO_Write(GPIOD,GPIOD_BUFFER);				/*output*/
		GPIO_Write(GPIOD,GPIOF_BUFFER);
		GPIOE_BUFFER = GPIO_ReadInputData(GPIOE);	/*input*/
		GPIOG_BUFFER = GPIO_ReadInputData(GPIOG);
		taskEXIT_CRITICAL();
		vTaskDelay(10/portTICK_RATE_MS);

	}
}






