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


#define Up GPIO_PIN_3
#define Down GPIO_PIN_4
#define Left GPIO_PIN_5
#define Right GPIO_PIN_6
#define Select GPIO_PIN_7
#define Buttons (Up|Down|Left|Right|Select)

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



static uint32_t Task_MonitorButton_NextExecute = 0;		// Next execution time
static uint32_t Task_MonitorButton_DeltaExecute;			// Number of SysTicks between execution
static bool		Task_MonitorButton_Initialized = false;	// State of task initialization
enum buttonStates{idle, pressed, debounce};
enum buttonStates bstate=idle;
long bSignal;
long keepButton;

extern void Task_MonitorButton() {
	uint32_t Speaker_Data = 0;

	if ( !Task_MonitorButton_Initialized ) {
	    //
	    // Enable the GPIO Port H.
	    //
	    SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOH );//Figure out what needs to be G and what needs to be H

	    //
	    // Configure GPIO_H to drive the Speaker.
	    //
	    GPIOPinTypeGPIOOutput( GPIO_PORTG_BASE, Buttons );
	    GPIOPadConfigSet( GPIO_PORTG_BASE,
	    					Buttons, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD );
	    //
	    //Set PortH<0> ON and PortH<1> OFF.
	    GPIOPinWrite( GPIO_PORTG_BASE, Buttons, 0x01 );
	    //
		//
	    //
	    //The delta between executions is the SysTickFrequency/twice_the_frequency
		//	The initial time to execute is Delta from now.
	    //
	    Task_MonitorButton_DeltaExecute  = (( SysTickFrequency_Nbr ) / (440*2) );
		Task_MonitorButton_NextExecute = SysTickCount_Nbr + Task_MonitorButton_DeltaExecute;


		//
		//	Task is initialized.
		//
		Task_MonitorButton_Initialized = true;

	}

//*****************************************************************************
//
//	Task execution.
//
//*****************************************************************************
	if ( SysTickCount_Nbr >= Task_MonitorButton_NextExecute ) {

	        bSignal=GPIOPinRead(Buttons);
	        switch(bstate){
	        case idle:
	        	if(bSignal != 0XF8){
	        		KeepButtons=bSignal;
	        		bstate=debounce;
	        		Task_MonitorButton_DeltaExecute=(SysTickFrequency_Nbr/100);
	        		Task_MonitorButton_NextExecute=SysTickCount_Nbr + Task_MonitorButton_DeltaExecute;
	        	}
	        	break;
	        case debounce:
	        	if(GPIOinRead(Buttons)==KeepButtons){
	        		bstate=pressed;
							
	        	}
	        }

		}
