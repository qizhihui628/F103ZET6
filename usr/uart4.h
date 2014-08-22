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
#define UART4_TIMOUT_MAX		30

void u4_printf(char* fmt,...);
void UART4_Init(uint32_t bound);
extern __IO u16 UART4_RX_STA;
extern __IO u16 UART4_Timeout;

extern __align(8) u8 UART4_TX_BUF[UART4_MAX_SEND_LEN];
extern u8 UART4_RX_BUF[UART4_MAX_RECV_LEN];


#endif /* UART4_H_ */
