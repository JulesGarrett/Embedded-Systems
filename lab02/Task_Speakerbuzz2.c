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



static uint32_t Task_Speakerbuzz2_NextExecute = 0;		// Next execution time
static uint32_t Task_Speakerbuzz2_DeltaExecute;			// Number of SysTicks between execution
static bool		Task_Speakerbuzz2_Initialized = false;	// State of task initialization
static uint32_t Delta_toggle; //The time between toggle the frequency of the tones
static uint32_t Speaker_toggle=0;//Number of SysTicks plus Delta_toggle
extern void Task_Speakerbuzz2() {
	uint32_t Speaker_Data = 0;

	if ( !Task_Speakerbuzz2_Initialized ) {
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
	    Task_Speakerbuzz2_DeltaExecute  = (( SysTickFrequency_Nbr ) / (440*2) );
		Task_Speakerbuzz2_NextExecute = SysTickCount_Nbr + Task_Speakerbuzz2_DeltaExecute;

		
		//
		//	Task is initialized.
		//
		Task_Speakerbuzz2_Initialized = true;
	
	}

//*****************************************************************************
//
//	Task execution.
//
//*****************************************************************************

	//Setting the time between changing the tones to 750 mS.
	Delta_toggle=(( 750 * SysTickFrequency_Nbr ) / 1000 );

	//Alternating the freqency of the speaker from 440Hz to 659Hz
	if(SysTickCount_Nbr >= Speaker_toggle){

			if(Task_Speakerbuzz2_DeltaExecute == ( SysTickFrequency_Nbr  / 880) ){

				Task_Speakerbuzz2_DeltaExecute=( SysTickFrequency_Nbr  / 1318 );
				Speaker_toggle= SysTickCount_Nbr + Delta_toggle;
			}
			else {

				Task_Speakerbuzz2_DeltaExecute=( SysTickFrequency_Nbr  / 880 );
				Speaker_toggle= SysTickCount_Nbr + Delta_toggle;
			}
		}

	if ( SysTickCount_Nbr >= Task_Speakerbuzz2_NextExecute ) {

        //
        // Toggle the Speaker.
        //
		Speaker_Data = GPIOPinRead( GPIO_PORTH_BASE, GPIO_PIN_0 | GPIO_PIN_1 );
		Speaker_Data = Speaker_Data ^ 0x03;
        GPIOPinWrite( GPIO_PORTH_BASE, GPIO_PIN_1 | GPIO_PIN_0, Speaker_Data );

        //
        //	Advance Task_Speakerbuzz2_NextExecute time.
        //

        Task_Speakerbuzz2_NextExecute  = SysTickCount_Nbr + Task_Speakerbuzz2_DeltaExecute;

	}



}
