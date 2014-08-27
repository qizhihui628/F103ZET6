/*
 * sim900a.h
 *
 *  Created on: 2014Äê8ÔÂ22ÈÕ
 *      Author: jeqi
 */

#ifndef SIM900A_H_
#define SIM900A_H_
#include "uart4.h"
extern u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
extern u8 sim900a_gprs_test(void);
extern __IO u16 Sim900a_Timeout;

#endif /* SIM900A_H_ */
