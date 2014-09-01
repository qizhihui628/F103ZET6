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
#include "usart2.h"
#include "uart4.h"
#include "timer4.h"



extern void Pwm_Init(void);
extern xSemaphoreHandle xMutex_vPrintString;
extern __IO uint16_t ADC1ConvertedValue[10];



xSemaphoreHandle xMutex;



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
			u2_printf("AT+HH\r\n");
			u2_printf("BT+\r\n");

			if(sim900a_send_cmd("AT","OK",300) == 0)
			{
				u1_printf("Success \r\n");
				u1_printf("grprs result is %d\r\n",sim900a_gprs_test());
			}

	//		u4_printf("BT+\r\n");
			u1_printf("GPIOE IS %x, GPIOG IS %x .\r\n",GPIOE_BUFFER,GPIOG_BUFFER);
			for(i=0;i<10;i++)
			{
				u1_printf("adc%d is %d.\r\n",i,ADC1ConvertedValue[i]);
			}
			fflush(stdout);
		}
		xSemaphoreGive(xMutex_vPrintString);


	}
}



int main(void)
{
	//IO_Init();
	//TIM4_Init();
	//LED_Init();
	usart1_config(9600);
//	USART2_Init(9600);
//	UART4_Init(115200);
	//Pwm_Init();
//	Dac1_Init();
//	Adc1_Init();
	u1_printf("init is ok \r\n");
	for(;;)	u1_printf("init is ok \r\n");
	vSemaphoreCreateBinary(xMutex);
//	vSemaphoreCreateBinary(xMutex_vPrintString);
	vPrint_init();
	if(xMutex != NULL)
	{
		//xTaskCreate(vRed_led_on,"vRed_led_on",100,NULL,1,NULL);
		//xTaskCreate(vGreen_led_on,"vGreen_led_on",100,NULL,2,NULL);
	}
	xTaskCreate(vTask1,"vTask1",200,NULL,1,NULL);
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
