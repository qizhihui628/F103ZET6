/*
 * GPIO.h
 *
 *  Created on: 2014Äê8ÔÂ13ÈÕ
 *      Author: jeqi
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

extern __IO uint16_t GPIOD_BUFFER ;/*OUTPUT BUFFER*/
extern __IO uint16_t GPIOF_BUFFER ;
extern __IO uint16_t GPIOE_BUFFER ;	/*INPUT BUFFER*/
extern __IO uint16_t GPIOG_BUFFER ;

void IO_Init(void);
void vIO_Refresh(void *pvParameters);


#endif /* GPIO_H_ */
