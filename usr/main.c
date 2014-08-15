/* Standard includes. */
#include <stdio.h>
#include <stdint.h>
/* Scheduler includes. */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Library includes. */
#include "stm32f10x_it.h"
#include "stm32f10x_conf.h"
/*usr*/
#include "usart.h"
#include "vPrintString.h"
#include "GPIO.H"
#include "pwm.h"
#include "dac.h"
#include "adc.h"

extern void Pwm_Init(void);
extern xSemaphoreHandle xMutex_vPrintString;
extern __IO uint16_t ADC1ConvertedValue[10];

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 输出高 
}

void delay(uint16_t j)
{
	uint16_t i = 2048;
	while(j--)
	{
		while(i--);
	}
	

}

xSemaphoreHandle xMutex;

void vRed_led_on(void *pvParameters)
{
	
	/* static 所有实例共享这个变量*/
//	int iVariableExample = 0; 
	for(;;)
	{
		if(xSemaphoreTake(xMutex,( portTickType ) 10) == pdTRUE)
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
			GPIO_SetBits(GPIOE,GPIO_Pin_5);
			vPrintString("vRed_led_on task . \r\n");
			vTaskDelay(500/portTICK_RATE_MS);
			xSemaphoreGive(xMutex);
		}
		vTaskDelay(100/portTICK_RATE_MS);

	}
	vTaskDelete(NULL);
}


void vGreen_led_on(void *pvParameters)
{
	
	/* static 所有实例共享这个变量*/
/*	int iVariableExample = 0; */
	for(;;)
	{
		if(xSemaphoreTake(xMutex,( portTickType ) 10) == pdTRUE)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
			GPIO_ResetBits(GPIOE,GPIO_Pin_5);
			vPrintString("vGreen_led_on task . \r\n");
			vTaskDelay(500/portTICK_RATE_MS);
			xSemaphoreGive(xMutex);
		}
		vTaskDelay(100/portTICK_RATE_MS);

	}
	vTaskDelete(NULL);
}

void vTask1(void *pvParameters)
{
	uint16_t i;
	uint16_t temp1,temp2;
	for(;;)
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_All);
		GPIO_ResetBits(GPIOF,GPIO_Pin_All);
		vTaskDelay(500/portTICK_RATE_MS);
		GPIO_ResetBits(GPIOD,GPIO_Pin_All);
		GPIO_SetBits(GPIOF,GPIO_Pin_All);
		vTaskDelay(500/portTICK_RATE_MS);
		temp1 = GPIO_ReadInputData(GPIOE);
		temp2 = GPIO_ReadInputData(GPIOG);
		Dac1_Set_Vol(500);
		xSemaphoreTake(xMutex_vPrintString,portMAX_DELAY);
		{
			printf("GPIOE IS %x, GPIOG IS %x .\r\n",GPIOE_BUFFER,GPIOG_BUFFER);
			for(i=0;i<10;i++)
			{
				printf("adc%d is %d.\r\n",i,ADC1ConvertedValue[i]);
			}
			fflush(stdout);
		}
		xSemaphoreGive(xMutex_vPrintString);


	}
}



int main(void)
{
	IO_Init();
	//LED_Init();
	uart_init(9600);
	Pwm_Init();
	Dac1_Init();
	Adc1_Init();
	
	vSemaphoreCreateBinary(xMutex);
//	vSemaphoreCreateBinary(xMutex_vPrintString);
	vPrint_init();
	if(xMutex != NULL)
	{
		//xTaskCreate(vRed_led_on,"vRed_led_on",100,NULL,1,NULL);
		//xTaskCreate(vGreen_led_on,"vGreen_led_on",100,NULL,2,NULL);
	}
	xTaskCreate(vTask1,"vTask1",100,NULL,1,NULL);
	xTaskCreate(vIO_Refresh,"vIO_Refresh",50,NULL,2,NULL);
	xTaskCreate(vPWM_Refresh,"vPWM_Refresh",50,NULL,2,NULL);
	vTaskStartScheduler();
	for(;;);
	
}




/*
int main(void)
{ 
	LED_Init();		  	//初始化与LED连接的硬件接口
	while(1)
	{

		delay(300);

		delay(300);
	}
	return 0;
}
*/
