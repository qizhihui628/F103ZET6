/*
 * pwm.h
 *
 *  Created on: 2014Äê8ÔÂ14ÈÕ
 *      Author: jeqi
 */

#ifndef PWM_H_
#define PWM_H_

#include "stm32f10x_conf.h"
#include "stm32f10x.h"


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


void Pwm_Init(void);
void vPWM_Refresh(void *pvParameters);

#endif /* PWM_H_ */
