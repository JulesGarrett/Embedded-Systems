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

#include	"FreeRTOS.h"
#include	"task.h"

#include	<stdio.h>
#include  "driverlib/adc.h"

//*****************************************************************************
//
//	External variables and subroutines.
//	(1) SysTick
//	(2) OLED initialization
//
//*****************************************************************************
extern uint32_t SysTickCount_Nbr;
unsigned long int ulValue;	    //ADC Initialized
extern int Select;
int doit = 0;
int arr[100];
//*****************************************************************************
//
//	Task local static variables.
//
//*****************************************************************************

extern void Task_ADC_Sample(void *pvParameters) {
	//
	// Enable port D
	//
	SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOD);

	GPIOPinTypeGPIOOutput( GPIO_PORTD_BASE, GPIO_PIN_2);
	GPIOPadConfigSet( GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_STRENGTH_4MA,
	GPIO_PIN_TYPE_OD);
	GPIOPinWrite( GPIO_PORTD_BASE, GPIO_PIN_2, 0x04);

	//
	//
	//
	// Initialization ADC
	SysCtlPeripheralEnable( SYSCTL_PERIPH_ADC0);		//enable ADC0


	ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 0,
	ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
	ADCSequenceEnable(ADC0_BASE, 1);
	ADCIntClear(ADC0_BASE, 1);



//*****************************************************************************
//
//	Task execution.
//
//*****************************************************************************

	while (1) {
		if(Select==1){
			GPIOPinWrite( GPIO_PORTD_BASE, GPIO_PIN_2, 0x00);
			vTaskDelay(5);
			GPIOPinWrite( GPIO_PORTD_BASE, GPIO_PIN_2, 0x04);



//
// Read the value from the ADC.
//
		uint8_t i;
		for (i = 0; i < 100; i++) {
			ADCProcessorTrigger(ADC0_BASE, 1);
			while(!ADCIntStatus(ADC0_BASE, 1, false)){
			}
			//
			// Read the value from the ADC.
			//
			ADCSequenceDataGet(ADC0_BASE, 1, &ulValue);
			arr[i] = ulValue;
			ADCIntClear(ADC0_BASE, 1);  //clear previous value;
			SysCtlDelay(16667);
		}
		Select=0;
		doit = 1;

	}
		vTaskDelay(1000);
	}
}
