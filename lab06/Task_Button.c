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
#include "Drivers/uartstdio.h"

#include	"FreeRTOS.h"
#include	"task.h"

#define SELECT GPIO_PIN_7

//*****************************************************************************
//
//	Task local static variables.
//
//*****************************************************************************
extern int Select = 0;
static long bSignal;
static long keepButtons;
enum buttonStates {
	idle, pressed, debounce
};
enum buttonStates bState = idle;
uint32_t delay=1;


extern void Task_Button(void *pvParameters) {

	//
	// Enable the GPIO Port G.
	//
	SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOG);

	//
	// Configure GPIO_G to drive the Status LED.
	//
	GPIOPinTypeGPIOInput( GPIO_PORTG_BASE, SELECT);
	GPIOPadConfigSet( GPIO_PORTG_BASE,
	SELECT, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

//*****************************************************************************
//
//	Task execution.
//
//*****************************************************************************
//
//
//
	while(1)
	{
		//UARTStdio_Initialization();
		bSignal = GPIOPinRead(GPIO_PORTG_BASE, SELECT);
		keepButtons = bSignal;
		switch (bState) {
		case idle:

			if (bSignal != 0b10000000) {
				bState = debounce;
				 delay=10;
			}
			break;

		case debounce:
			if (bSignal != 0b10000000 && bSignal == keepButtons) {

					bState = pressed;
			} else {
				bState = idle;
			}
			delay=1;

		break;

		case pressed:

		if (bSignal == 0x80) {
			Select = 1;

			bState = idle;
		}
		break;
	}
		vTaskDelay( (delay*100) );


	}
}
