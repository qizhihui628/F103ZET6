#include <stdio.h>
#include "vPrintString.h"


xSemaphoreHandle xMutex_vPrintString = NULL;
void vPrint_init(void)
{
	xMutex_vPrintString = xSemaphoreCreateMutex();
}

void vPrintString(const portCHAR *pcString)
{
	xSemaphoreTake(xMutex_vPrintString,portMAX_DELAY);
	{
		printf("%s",pcString);
		fflush(stdout);
	}
	xSemaphoreGive(xMutex_vPrintString);

#if 0
	if( kbhit() )
	{
		vTaskEndScheduler();
	}
#endif
}
