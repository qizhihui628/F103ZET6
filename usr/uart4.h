/*
 * uart4.h
 *
 *  Created on: 2014Äê8ÔÂ18ÈÕ
 *      Author: jeqi
 */

#ifndef UART4_H_
#define UART4_H_
#include "stm32f10x_conf.h"
#include "FreeRTOS.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#define UART4_DR_Base           0x40004C04

#define UART4_MAX_RECV_LEN		128				//
#define UART4_MAX_SEND_LEN		128

void u4_printf(char* fmt,...);
void UART4_Init(uint32_t bound);

#endif /* UART4_H_ */
