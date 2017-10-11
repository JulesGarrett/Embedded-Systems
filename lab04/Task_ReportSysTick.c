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
extern uint32_t 	SysTickCount_Nbr;


//*****************************************************************************
//
//	Task local static variables.
//
//*****************************************************************************


static uint32_t Task_ReportSysTick_Status = 0;			// Status of string formatting
														//	and writing to OLED.

extern void Task_ReportSysTick(void *pvParameters) {

	    RIT128x96x4Init(1000000);


		//This was changed from LED_blinky toy first and last name
		RIT128x96x4StringDraw ("Julia Garrett", 36, 0, 8 );





//*****************************************************************************
//
//	Task execution.
//
//*****************************************************************************

	while(1){

		char	TimeString[24];

				Task_ReportSysTick_Status = sprintf( TimeString, "Task_Blinky: %d", SysTickCount_Nbr );
				Task_ReportSysTick_Status = Task_ReportSysTick_Status;

			    RIT128x96x4StringDraw( TimeString, 0, 12, 15);
			    vTaskDelay( 2500 );

	}
}
