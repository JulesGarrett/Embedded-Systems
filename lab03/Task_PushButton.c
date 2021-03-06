//*****************************************************************************
//
//	Task_LEDBlink.c - Blink the LED.
//
//		Author: 		Gary J. Minden
//		Organization:	KU/EECS/EECS 388
//		Date:			2016-08-16 (B60816)
//		Version:		1.0
//
//		Purpose:		Example task that demonstrates:
//							(1) setting up a GPIO pin (LEC)
//							(2) blinking (toggling) a LED
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
#include "driverlib/gpio.h"

//*****************************************************************************
//
//	External variables.
//	(1) SysTick
//
//*****************************************************************************
extern uint32_t		SysTickFrequency_Nbr;
extern uint32_t 	SysTickCount_Nbr;

//*****************************************************************************
//
//	Task local static variables.
//
//*****************************************************************************



static uint32_t Task_PushButton_NextExecute = 0;		// Next execution time
static uint32_t Task_PushButton_DeltaExecute;			// Number of SysTicks between execution
static bool		Task_PushButton_Initialized = false;	// State of task initialization

extern void Task_PushButton() {
	uint32_t Speaker_Data = 0;

	if ( !Task_PushButton_Initialized ) {
	    //
	    // Enable the GPIO Port H.
	    //
	    SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOH );

	    //
	    // Configure GPIO_H to drive the Speaker.
	    //
	    GPIOPinTypeGPIOOutput( GPIO_PORTH_BASE, GPIO_PIN_1 | GPIO_PIN_0 );
	    GPIOPadConfigSet( GPIO_PORTH_BASE,
	    					GPIO_PIN_1 | GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD );
	    //
	    //Set PortH<0> ON and PortH<1> OFF.
	    GPIOPinWrite( GPIO_PORTH_BASE, GPIO_PIN_1 | GPIO_PIN_0, 0x01 );
	    //
		//
	    //
	    //The delta between executions is the SysTickFrequency/twice_the_frequency
		//	The initial time to execute is Delta from now.
	    //
	    Task_PushButton_DeltaExecute  = (( SysTickFrequency_Nbr ) / (440*2) );
		Task_PushButton_NextExecute = SysTickCount_Nbr + Task_PushButton_DeltaExecute;


		//
		//	Task is initialized.
		//
		Task_PushButton_Initialized = true;

	}

//*****************************************************************************
//
//	Task execution.
//
//*****************************************************************************



	if ( SysTickCount_Nbr >= Task_PushButton_NextExecute ) {

        //
        // Toggle the Speaker.
        //
		Speaker_Data = GPIOPinRead( GPIO_PORTH_BASE, GPIO_PIN_0 | GPIO_PIN_1 );
		Speaker_Data = Speaker_Data ^ 0x03;
        GPIOPinWrite( GPIO_PORTH_BASE, GPIO_PIN_1 | GPIO_PIN_0, Speaker_Data );

        //
        //	Advance Task_PushButton_NextExecute time.
        //

        Task_PushButton_NextExecute  = SysTickCount_Nbr + Task_PushButton_DeltaExecute;

	}



}
