//*****************************************************************************
//
//	Task_ReportSysTick.c - Display SysTickCount_Nbr.
//
//		Author: 		Gary J. Minden
//		Organization:	KU/EECS/EECS 388
//		Date:			2016-08-16 (B60816)
//		Version:		1.0
//
//		Purpose:		Example task that demonstrates:
//							(1) Formatting a string
//							(2) Write string to OLED
//
//		Notes:			Updated at KU and adapted from the TI LM3S1968 blinky
//						and other examples.
//
//		Date:			2016-08-16 (B60816)
//						Moved task code to separate files.
//						Changed structure of task code. See individual tasks.
//						Updated basic types.
//
//*****************************************************************************
//
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

#include "driverlib/sysctl.h"
#include "Drivers/rit128x96x4.h"

#include "Drivers/uartstdio.h"
#include	"Drivers/UARTStdio_Initialization.h"

#include	"FreeRTOS.h"
#include	"task.h"

#include	<stdio.h>

//*****************************************************************************
//
//	External variables and subroutines.
//	(1) SysTick
//	(2) OLED initialization
//
//*****************************************************************************
//extern uint32_t		SysTickFrequency_Nbr;
extern uint32_t SysTickCount_Nbr;
extern int arr[100];
uint8_t i;

//*****************************************************************************
//
//	Task local static variables.
//
//*****************************************************************************

extern unsigned long int ulValue;
extern int doit;

extern void Task_UART(void *pvParameters) {
while(1){
	UARTStdio_Initialization(); //	UARTStdio is initialized.

	if (doit == 1) {
		for (i = 0; i < 100; i++) {
			UARTprintf("%03d\n", arr[i]);
		}
		doit=0;
	}
	vTaskDelay(1000);
}

}
