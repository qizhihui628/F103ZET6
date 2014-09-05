#ifndef __USART_H
#define __USART_H
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "stm32f10x_conf.h"

#define U1_TxBufferSize 128
#define U1_RxBufferSize 128

extern void usart1_config(uint32_t bound);/*usart1 initialization*/
extern void u1_printf(char* fmt,...);

void uart_init(uint32_t bound);
#endif


