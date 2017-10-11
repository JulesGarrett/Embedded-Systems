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
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"


#include	"FreeRTOS.h"
#include	"task.h"
#include "driverlib/timer.h"

#include	<stdio.h>
#include "semphr.h"

//*****************************************************************************
//
//	External variables and subroutines.
//	(1) SysTick
//	(2) OLED initialization
//
//*****************************************************************************
void TimerISR();
uint32_t TimeCount;
static uint32_t Task_Time_Status = 0;
char timeString[24];
uint32_t Centiseconds = 0;
SemaphoreHandle_t Timer_0_A_Semaphore;

extern void Task_TimeOfDay(void *pvParameters) {

//*****************************************************************************
//
//	Task local static variables.
//
//*****************************************************************************
	uint32_t Hours = 0;
	uint32_t Minutes = 0;
	uint32_t Seconds = 0;

	unsigned long TimerStatus_1;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);


	//
	// Initialize Timer_0_A_Semaphore
	//
	vSemaphoreCreateBinary( Timer_0_A_Semaphore );

	TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
	TimerPrescaleSet(TIMER0_BASE, TIMER_A, 199);
	TimerLoadSet(TIMER0_BASE, TIMER_A, 2500);

	// Register Timer_0_A ISR
	IntRegister( INT_TIMER0A, TimerISR );
	// Enable Timer_0_A interrupt in the peripheral

	TimerIntEnable( TIMER0_BASE, TIMER_TIMA_TIMEOUT );
	// Enable Timer_0_A interrupt in LM3S NVIC
	IntEnable( INT_TIMER0A );
	IntMasterEnable();

	TimerEnable(TIMER0_BASE, TIMER_A);

//*****************************************************************************
//
//	Execution
//
//*****************************************************************************

	while (1) {

		xSemaphoreTake( Timer_0_A_Semaphore, portMAX_DELAY );

		if (Centiseconds > 99) {
			Seconds++;
			Centiseconds = 0;
			if (Seconds == 59) {
				Minutes++;
				Seconds = 0;
				if (Minutes == 59) {
					Hours++;
					Minutes = 0;
				}
			}
		}

		char TimeString[24];

		Task_Time_Status = sprintf(TimeString, " %02d:%02d:%02d:%02d", Hours,
				Minutes, Seconds, Centiseconds);
		Task_Time_Status = Task_Time_Status;

		RIT128x96x4StringDraw(TimeString, 0, 30, 15);

	}
}

void TimerISR() {
	//
	// "Give" the Timer_0_A_Semaphore
	//
	xSemaphoreGive( Timer_0_A_Semaphore );
	//
	// If xHigherPriorityTaskWoken was set to true,
	// we should yield. The actual macro used here is
	// port specific.
	//
	//
	TimerIntClear( TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	TimeCount++;
	Centiseconds++;
}


