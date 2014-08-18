/*
 * usart2.h
 *
 *  Created on: 2014Äê8ÔÂ18ÈÕ
 *      Author: jeqi
 */

#ifndef USART2_H_
#define USART2_H_
#include "stm32f10x_conf.h"
#include "FreeRTOS.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#define USART2_DR_Base           0x40004404

#define USART2_MAX_RECV_LEN		1024				//
#define USART2_MAX_SEND_LEN		1024

void u2_printf(char* fmt,...);
void USART2_Init(uint32_t bound);

#endif /* USART2_H_ */
