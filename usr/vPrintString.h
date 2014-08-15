#ifndef __VPRINTSTRING_H
#define __VPRINTSTRING_H

#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "semphr.h"

void vPrint_init(void);
void vPrintString(const portCHAR *pcString);



#endif
