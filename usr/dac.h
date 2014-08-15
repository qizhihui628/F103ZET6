/*
 * dac.h
 *
 *  Created on: 2014Äê8ÔÂ15ÈÕ
 *      Author: jeqi
 */

#ifndef DAC_H_
#define DAC_H_

#include "stm32f10x_conf.h"
#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void Dac1_Init(void);
void Dac1_Set_Vol(u16 vol);

#endif /* DAC_H_ */
